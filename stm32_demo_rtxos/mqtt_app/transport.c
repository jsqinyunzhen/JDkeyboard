/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/


/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/

#include "stm32f10x.h"
#include "st_printf.h"
#include "stm32f10x_it.h"
#include "app4g.h"
#include "project_config.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "transport.h"
#include "mqtt_app.h"

#define INVALID_SOCKET 0xffffffff
static int mysock = INVALID_SOCKET;

int get_mqtt_transport_sock(void)
{
    return mysock;
}
#define STATE_CHECK 0
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
//	int rc = 0;
    //rc = write(sock, buf, buflen);
    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
	char str_ref[20] = {0};
    int ret = -1;
    uint8_t result_len = 0;
    int i = 0;
    if(get_mqtt_transport_sock() < 0)
    {
        printf("socket have closed,mysock=%d..\r\n",mysock);
        return -1;
    }
    printf("\r\n transport_sendPacketBuffer =%d:\r\n",buflen);
#if 0
    for(i=0; i<buflen; i++)
    {
        if((i)%8 == 0)
            printf("%03d: ",i);

        printf("0x%02x,",buf[i]);
        if((i+1)%8 == 0)
            printf("\r\n");
    }
    printf("\r\n");
#endif
#if(STATE_CHECK ==1)

    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QIACT);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = at_cmd_ack(cmd, strlen((const char*)cmd), result, &result_len,"OK", 2000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        printf("%s fail,ret=%d\r\n",cmd,ret);
        return -1;
    }
#endif
    //QISEND
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //send  data  with changeable length
    sprintf((char*)at_tx_buffer,"AT+QISEND=%d,%d\r\n",sock,buflen);
    //sprintf((char*)at_tx_buffer,"AT+QISEND=%d\r\n",sock);
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,">", 2000);
    //+QIOPEN: 0,0
    if (ret ==0 )
    {
        printf("ready send data...\r\n");
    }
    else
    {
        printf("%s fail,ret=%d\r\n",cmd,ret);
        return -1;
    }

    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    if(buflen>UART2_TX_BUFFER_LEN)
    {
        printf("uart tx buffer not engouth\r\n");
        return -1;

    }
    else
    {
        memcpy(at_tx_buffer,buf,buflen);//note******************
    }

    ret =at_cmd_ack(cmd, buflen,result,&result_len,"SEND OK", 2000);
    /*
    if (ret ==0 )
    {
        printf("send data ok...\r\n");

    }
    else
    {
        printf("%s fail,ret=%d\r\n",cmd,ret);
        return -2;
    }
    */
    printf("send data end,ret=%d,sock=%d\r\n",ret,sock);
	if(ret == 1)//"+QIURC: \"recv\",0,"
	{
		memset(str_ref,0,sizeof(str_ref));
		sprintf(str_ref,"+QIURC: \"recv\",%d,",sock);
		//if(strstr((const char*)result,"+QIURC: \"recv\",0,") != NULL)
		MQTT_PRINT_DEBUG("QIURC recv check...str_ref=%s\r\n",str_ref);
		if(strstr((const char*)result,str_ref) != NULL)
		{
			result_len = 0;
			memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
			memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
			sprintf(( char*)at_tx_buffer,"AT+QIRD=%d\r\n",sock);
			ret =at_cmd_ack(cmd, strlen((const char *)cmd),result,&result_len,"+QIRD:", 2000);
			MQTT_PRINT_DEBUG("at_cmd_ack ret=%d,result_len %d is:\r\n",ret,result_len);
			//is SEND OK???s
		}

	}
	memset(str_ref,0,sizeof(str_ref));
	sprintf(str_ref,"+QIURC: \"closed\",%d",sock);
	//if(strstr((const char*)result,"+QIURC: \"recv\",0,") != NULL)
	printf("QIURC closed check...str_ref=%s\r\n",str_ref);

   // if(strstr((const char*)result,"+QIURC: \"closed\",0") != NULL)
	if(strstr((const char*)result,str_ref) != NULL)
    {
        //printf("socket closed ,ret=%d rec_times=%d\r\n",ret,rec_times);
        //QISTATE

        mysock = INVALID_SOCKET;
        printf("check socket closed,mysock=%d..\r\n",mysock);
        result_len = 0;

        memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
        memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
        snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QIACT);
        //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
        ret = at_cmd_ack(cmd, strlen((const char*)cmd), result, &result_len,"OK",2000);
        if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
        {
            // snprintf(datacap, datacap_len, "4G");
            return -3;
        }

        return -4;
    }

#if(STATE_CHECK ==1)

    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //sprintf(( char*)at_tx_buffer,"AT+QISEND=0,0\r\n");
    sprintf((char*)at_tx_buffer,"AT+QISEND=%d,0\r\n",sock);
    ret =at_cmd_ack(cmd, strlen((const char *)cmd),result,&result_len,"OK", 2000);
    if (ret == 0 )
    {
        printf("check send data ok...\r\n");
    }
    else
    {
        printf("%s fail,ret=%d\r\n",cmd,ret);
        return -5;
    }
#endif
    return 0;
}


int transport_getdata(unsigned char* buf, int count)
{
#if 1
    return transport_getdatanb((void *)&mysock,buf,  count);
#else

    int rc = 0;
    //rc = write(sock, buf, buflen);
    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    int ret = -1;
    uint8_t result_len = 0;


    //QISTATE
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
//	snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    sprintf(( char*)at_tx_buffer,"AT+QIRD=0,%d",count);
    ret =at_cmd_ack(cmd, strlen((const char *)cmd),result,&result_len,"+QIRD:", 2000);
    if (ret ==0 )
    {

    }
    else
        return -1;

    //printf("received %d bytes count %d\n", rc, (int)count);
    return rc;
#endif
}


int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
    int sock = *((int *)sck); 	/* sck: pointer to whatever the system may use to identify the transport */
    /* this call will return after the timeout set on initialization if no bytes;
       in your system you will use whatever you use to get whichever outstanding
       bytes your socket equivalent has ready to be extracted right now, if any,
       or return immediately */
    //int rc = 0;

    //rc = write(sock, buf, buflen);
    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    int ret = -1;
    int data_len = -1;
    int bit_num;
    uint8_t result_len = 0;
    uint8_t rec_times = 0;
    int i = 0;
    uint8_t*p =NULL;
	uint8_t context_id = 0;//context_id 1----16
//[10-10-12:09:20]+QIRD: 0
//[10-10-12:09:20]
//[10-10-12:09:20]OK
	char str_ref[20] = {0x0d,0x0a,0x2b,0x51,0x49,0x52,0x44,0x3a,
						0x20,0x30,0x0d,0x0a,0x0d,0x0a,0x4f,0x4b,
						0x0d,0x0a,};
    //QISTATE
    MQTT_PRINT_DEBUG("\r\n transport_getdatanb:count=%d\r\n",count);

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    sprintf(( char*)at_tx_buffer,"AT+QIRD=%d,%d\r\n",sock,count);
//+QIURC: "recv",0
rec:
    rec_times++;
    MQTT_PRINT_DEBUG("\r\n try cmd(%s),rec_times=%d\r\n",cmd,rec_times);
    ret =at_cmd_ack(cmd, strlen((const char *)cmd),result,&result_len,"+QIRD:", 5000);
    MQTT_PRINT_DEBUG("at_cmd_ack ret=%d,result_len %d is:\r\n",ret,result_len);
    /*
    set uart2 rx complete:21=AT+QIRD=0,1
    ERROR

    at_cmd_ack ret=-3,result_len 21 is:
    0x41,0x54,0x2b,0x51,0x49,0x52,0x44,0x3d,
    0x30,0x2c,0x31,0x0d,0x0d,0x0a,0x45,0x52,
    0x52,0x4f,0x52,0x0d,0x0a,
    */
    #if 0
    for(i=0; i<result_len; i++)
    {
        printf("0x%02x,",result[i]);
        if((i+1)%8 == 0)
            printf("\r\n");
    }
    printf("\r\n");
	#endif
    //if(rec_times > 30)
    if(rec_times > 5)
    {
        printf("no rec data\r\n");
        return -1;
    }
#if 0
[09-10-20:09:19]+QIURC: "recv"
    ,0
    [09-10-20:09:19]
    [09-10-20:09:19]+QIRD: 1
    [09-10-20:09:19]
    [09-10-20:09:19]
    [09-10-20:09:19]OK

    [09-11-16:35:58]+QIRD: 0
    [09-11-16:35:58]
    [09-11-16:35:58]OK
    [09-11-16:35:58]
[09-11-16:35:59]+QIURC: "recv"
    ,0


#endif
	#if 0
	if(result_len ==18)
	{
		for(i=0; i<result_len; i++)
		{
			printf("0x%02x,",result[i]);
			str_ref[i] = result[i];
			if((i+1)%8 == 0)
				printf("\r\n");
		}
		

	}
	#endif
	if(result_len ==18)
	{
		//+QIRD: 0 OK
		if(strncmp((const char*)result,str_ref,18) == 0)
		{
			printf("there is no data \r\n");
	        return -1;
		}
	}
    printf("check 1\r\n");
//[09-24-14:16:25]+QIURC: "pdpdeact",1	
#if 0
[09-24-14:16:26]set uart2 rx complete:36=
[09-24-14:16:26]+QIURC: "pdpdeact",1
[09-24-14:16:26]AT+QIRD=0,1
[09-24-14:16:26]ERROR

#endif


    if(strstr((const char*)result,"+QIURC: \"pdpdeact\",") != NULL)
    {
        printf("must execute AT+QIDEACT to deactivate the context and reset all connections\r\n");
        p = (uint8_t*)strstr((const char*)result,",");
        p += strlen(",");
        context_id = 0;
        bit_num = 0;
        while(*p != 0x0d)
        {
            context_id *= 10;
            context_id += *p - '0';

            p++;
            bit_num++;
        }
        MQTT_PRINT_DEBUG("deactivate contextid is=%d(1-16)\r\n",context_id);
    }
    else 
    {
		memset(str_ref,0,sizeof(str_ref));
		sprintf(str_ref,"+QIURC: \"closed\",%d,",sock);
		//if(strstr((const char*)result,"+QIURC: \"recv\",0,") != NULL)
		MQTT_PRINT_DEBUG("getdata...str_ref=%s\r\n",str_ref);

		if(strstr((const char*)result,str_ref) != NULL)
	// else if(strstr((const char*)result,"+QIURC: \"closed\",0") != NULL)
		{
		    MQTT_PRINT_DEBUG("socket closed ,ret=%d rec_times=%d\r\n",ret,rec_times);
		    //QISTATE
		    result_len = 0;

		    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
		    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
		    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN,"AT+QICLOSE=%d\r\n",sock);
		    // snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QICLOSE);
		    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
		    //ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
		    ret =at_cmd_ack(cmd, strlen((const char *)cmd),result,&result_len,"OK", 5000);
		    MQTT_PRINT_DEBUG("at_cmd_ack ret=%d,result_len %d is:\r\n",ret,result_len);
		    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
		    {
		        // snprintf(datacap, datacap_len, "4G");
		        printf("AT+QICLOSE=%d fail\r\n",sock);
		        return -2;
		    }
		    else
		    {

		        mysock = INVALID_SOCKET;
		        sock = transport_open("\"mqtt.jindoo.jopool.net\"",MQTT_PORT);
		        MQTT_PRINT_DEBUG("mqtt_tcp_socket re-transport_open %d\r\n",sock);
		        if(sock < 0)
		        {
		            return -4;
		        }

		    }
		    return -3;
		}
	}
    MQTT_PRINT_DEBUG("check 2\r\n");
    if(strstr((const char*)result,"+QIRD: 0") != NULL)
    {
		memset(str_ref,0,sizeof(str_ref));
		sprintf(str_ref,"+QIURC: \"recv\",%d,",sock);
		//if(strstr((const char*)result,"+QIURC: \"recv\",0,") != NULL)
		printf("getdata2...str_ref=%s\r\n",str_ref);

        //p = (uint8_t*)strstr((const char*)result,"+QIURC: \"recv\",0,");
		p = (uint8_t*)strstr((const char*)result,str_ref);
        if(p != NULL)
        {
            printf("data have rec and get\r\n");
            for(i=0; i<result_len; i++)
            {
                printf("0x%02x,",result[i]);
                if((i+1)%8 == 0)
                    printf("\r\n");
            }
            printf("\r\n QIURC: recv\r\n");
            goto out;
        }
        else
        {
#if 0
            +QIRD: 0

                OK

    +QIURC: "recv"
                ,1

                rx 1  ok
    at_cmd_ack ret=0,result_len 38 is:
                               0x0d,0x0a,0x2b,0x51,0x49,0x52,0x44,0x3a,
                               0x20,0x30,0x0d,0x0a,0x0d,0x0a,0x4f,0x4b,
                               0x0d,0x0a,0x0d,0x0a,0x2b,0x51,0x49,0x55,
                               0x52,0x43,0x3a,0x20,0x22,0x72,0x65,0x63,
                               0x76,0x22,0x2c,0x31,0x0d,0x0a,

#endif
            //p = (uint8_t*)strstr((const char*)result,"+QIURC: \"recv\",0");
			//p = (uint8_t*)strstr((const char*)result,str_ref);
            //if(p != NULL)
           //     printf("data have rec ,but need  re-get\r\n");
            //else
                printf("data have not rec,send read check\r\n");
            printf("goto rec1=%d**\r\n",rec_times);
            goto rec;
        }


    }
    else
    {
        printf("$$$$$$$$$$$$$\r\n");
    }
    {
        printf("check 3\r\n");
        p = (uint8_t*)strstr((const char*)result,"+QIRD: ");
        if(p ==NULL)
        {
            printf("goto rec2=%d,result=%s**\r\n",rec_times,result);
            p = (uint8_t*)strstr((const char*)result,"ERROR");
            if(p )
            {
                p = (uint8_t*)strstr((const char*)result,"ERROR");
                printf("If the connection does not exist\r\n");
                goto out;
            }
            goto rec;
        }

        data_len = 0;
        //p += 7;
        p += strlen("+QIRD: ");
        //printf("*p=%c,*p=0x%02x,ret=%d\r\n",*p,*p,ret);
        //printf("*p=%c,*p=0x%02x,ret=%d\r\n",*(p+1),*(p+1),ret);
        //printf("*p=%c,*p=0x%02x,ret=%d\r\n",*(p+2),*(p+2),ret);
        //printf("*p=%c,*p=0x%02x,ret=%d\r\n",*(p+3),*(p+3),ret);
        data_len= 0;
        bit_num = 0;
        while(*p != 0x0d)
        {
            printf("*p=%c *p=0x%02x,ret=%d\r\n",*p,*p,ret);
            data_len *= 10;
            data_len += *p - '0';
            bit_num++;
            p++;
        }
        printf("autual data len2 is :data_len=%d bit_num=%d\r\n",data_len,bit_num);
        p+= 2;
        for(i = 0; i < data_len; i++)
        {
            buf[i] = *p++;
            //printf("buf[%d]=0x%02x \r\n",i,buf[i]);
        }
        if(count == data_len)
        {
            printf("rec end\r\n");
            goto out;
        }

    }
    printf("check 4\r\n");
	memset(str_ref,0,sizeof(str_ref));
	sprintf(str_ref,"+QIURC: \"recv\",%d,",sock);
	//if(strstr((const char*)result,"+QIURC: \"recv\",0,") != NULL)
	printf("getdata4...str_ref=%s\r\n",str_ref);

    //if(strstr((const char*)result,"+QIURC: \"recv\",0") != NULL)
	if(strstr((const char*)result,str_ref) != NULL)
    {
       // p = (uint8_t*)strstr((const char*)result,"+QIURC: \"recv\",0,");
		p = (uint8_t*)strstr((const char*)result,str_ref);
        if(p == NULL)
        {
            printf("need re-send read data\r\n");
        }
        else
        {
            printf("data have rec\r\n");
            for(i=0; i<result_len; i++)
            {
                printf("0x%02x,",result[i]);
                if((i+1)%8 == 0)
                    printf("\r\n");
            }
            printf("\r\n QIURC: recv\r\n");
        }


        goto rec;
    }


//+QIURC: "closed",0
#if 0

    p = (uint8_t*)strstr((const char*)result,"+QIRD: ");
    if(p ==NULL)
    {
        return -1;
    }
    //printf("autual data len is :p=%s\r\n",p);
    ret = 0;
    p += 7;
    while(*p != 0x0d)
    {
        printf("*p=%c *p=0x%02x,ret=%d\r\n",*p,*p,ret);
        ret *= 10;
        ret += *p - '0';

        p++;
    }
    printf("autual data len is :ret=%d\r\n",ret);
    p+= 2;

    for(i = 0; i < ret; i++)
        buf[i] = *p++;

    //ret =at_cmd_ack(cmd, strlen(cmd),result,&result_len,"+QIURC:", 2000);
    ret = 0;
    if (ret ==0 )
    {
        result_len = 0;
        //memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
        //memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
        //sprintf(( char*)at_tx_buffer,"AT+QIRD=%d,%d\r\n",sock,count);

        ret =at_cmd_ack(cmd, strlen(cmd),result,&result_len,"+QIRD:", 5000);
        if (ret ==0 )
        {
            printf("result QIRD:%d\r\n",result_len);
            memcpy(buf,result,result_len);

        }
        else
            return -1;
    }
    else
    {
        return -2;
    }
#endif

out:
    //printf("received %d bytes count %d\n", rc, (int)count);
    for(i=0; i<data_len; i++)
    {
        printf("0x%02x,",buf[i]);
        if((i+1)%8 == 0)
            printf("\r\n");
    }
    printf("\r\n transport_getdatanb %d:%d end\r\n",count,data_len);
    return data_len;



}

/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
*/
int transport_open(char* addr, int port)
{
    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
	char str_ref[15] = {0};
    int ret = -1;
    uint8_t result_len = 0;
    int contextid= 1;
	int connectid= 0;
    int error= 0;
    int bit_num= 0;
    //QISTATE
#if(STATE_CHECK ==1)
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"OK", 3000);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
#endif
    printf("AT_QIOPEN start\r\n");





//transport_sendPacketBuffer("\"182.92.12.11\",123",buf,nbytes);

#if 1
    //AT+QIOPEN=1,2,"UDP SERVICE","127.0.0.1",0,3030,0
open:
    result_len = 0;
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    if(port == NTP_PORT)
    {
        //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN,"AT+QIOPEN=1,0,\"UDP\",%s,%d,0,0\r\n",addr,  port);
        //connectid =1;
        contextid = NTP_UDP_CONTEXT_ID;
        connectid = NTP_UDP_CONNECT_ID;
        snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN,"AT+QIOPEN=%d,%d,\"UDP\",%s,%d,0,0\r\n",contextid,connectid,addr,  port);
    }
    else if(port == MQTT_PORT)
		{
        //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QIOPEN);
        contextid = MQTT_TCP_CONTEXT_ID;
        connectid = MQTT_TCP_CONNECT_ID;
        snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN,"AT+QIOPEN=%d,%d,\"TCP\",%s,%d,0,0\r\n",contextid,connectid,addr,  port);
		}
    else
    {
        printf("invalid port=%d\r\n",port);
        return -1;
    }
    printf("cmd=%s\r\n",cmd);
#else
    char connect_server_ip_port_cmd[56];
    memset(connect_server_ip_port_cmd,'\0',56);
    strcpy(connect_server_ip_port_cmd,"AT+QIOPEN=1,0,\"UDP\",");
    //strcpy(connect_server_ip_port_cmd,"AT+QIOPEN=1,0,\"TCP\",");
    //strcat(connect_server_ip_port_cmd,server_ip_and_port);
    strcat(connect_server_ip_port_cmd,addr, port);
    strcat(connect_server_ip_port_cmd,",0,0\r\n");
#endif
    ret =at_cmd_ack(cmd, strlen((const char*)cmd),result,&result_len,"+QIOPEN:", 7000);
    //+QIOPEN: 0,0
    if (ret ==0 )
    {
        char i = 0;
        //+QIOPEN: 0,561 result_len=24 strlen(pstr)=9
        //get connectid
        printf("result=%s result_len=%d\r\n",result,result_len);
        char* pstr= strstr((const char*)result, ":");
        printf("pstr=%s strlen(pstr)=%d\r\n",pstr,strlen(pstr));
        for(i=0; i<strlen(pstr); i++)
            printf("(*%d:%c=0x%02x*), ",i,*(pstr+i),*(pstr+i));
        printf("\r\n");

        connectid= *(pstr+2)-'0';
        //get error
        //pstr= strstr((const char*)result, ",");
        //connectid=0 error=178004728
        
       // pstr= strstr((const char*)result, "+QIOPEN: 0,");
		sprintf(str_ref,"+QIOPEN: %d,",connectid);
		pstr= strstr((const char*)result, str_ref);
        printf("str_ref=%s  pstr=%s strlen(pstr)=%d\r\n",str_ref,pstr,strlen(pstr));
        for(i=0; i<strlen(pstr); i++)
            printf("(*%d:%c=0x%02x*), ",i,*(pstr+i),*(pstr+i));

        printf("\r\n");
#if 1
        //pstr += strlen("+QIOPEN: 0,");
		pstr += strlen(str_ref);
        error = 0;
        bit_num = 0;
        while(*pstr != 0x0d)
        {
            error *= 10;
            error += *pstr - '0';

            pstr++;
            bit_num++;
        }

#else
        if(strlen(pstr) ==6)
        {
            error= *(pstr+1)-'0';
            error *= 10;

            error += *(pstr+2)-'0';
            error *= 10;

            error += *(pstr+3)-'0';
        }
        else
            error= *(pstr+1)-'0';
#endif
        //error= *(pstr+4)-'0';
        printf("connectid=%d error=%d bit_num=%d\r\n",connectid,error,bit_num);
        if(error == 561)
        {
            //connectid ++;
            //Open PDP context failed
            //goto open;
            return -2;
        }
        if(error == 563)
        {
            connectid ++;
            //563  Socket identity has been used
            goto open;
        }

        else if(error != 0)
            return -2;

		if(connectid != MQTT_TCP_CONNECT_ID )
			printf("####MQTT_TCP_CONNECT_ID != connectid= %d\r\n",connectid);
        mysock = connectid;
		
    }
    else
    {
        printf("QIOPEN FAIL %d\r\n",ret);
        return -3;
    }
#if(STATE_CHECK ==1)

    printf("QISTATE start\r\n");
    //QISTATE
    result_len = 0;

    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    //snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -4;
    }
#endif

    return mysock;
}


int transport_close(int sock)
{
    uint8_t  *cmd = at_tx_buffer;
    uint8_t *result = at_rx_buffer;
    int ret = -1;
    uint8_t result_len = 0;
    int connectid= sock;

    result_len = 0;

    //AT_QICLOSE
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, "AT+QICLOSE=%d\r\n",connectid);
    printf("at_tx_buffer=%s\r\n",at_tx_buffer);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }

    result_len = 0;
    //QISTATE
    memset(at_tx_buffer,0,UART2_TX_BUFFER_LEN);
    memset(at_rx_buffer,0,UART2_RX_BUFFER_LEN);
    snprintf(( char*)at_tx_buffer,UART2_TX_BUFFER_LEN, AT_QISTATE);
    ret = AT_cmd(cmd, strlen((const char*)cmd), result, &result_len);
    if (ret || !strlen((const char*)result) || !strstr((const char*)result, "OK"))
    {
        // snprintf(datacap, datacap_len, "4G");
        return -1;
    }
    return 0;
}

// 短整型大小端互换

#define BigLittleSwap16(A)  ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))

// 长整型大小端互换

#define BigLittleSwap32(A)  ( (((uint32_t)(A)&0xff000000) >> 24) | (((uint32_t)(A)&0x00ff0000) >> 8) |(((uint32_t)(A)&0x0000ff00) << 8)|(((uint32_t)(A)&0x000000ff) << 24))

// 本机大端返回1，小端返回0

int checkCPUendian()

{

    union {

        unsigned long int i;

        unsigned char s[4];

    } c;



    c.i = 0x12345678;
//--1--BIG , 0 LITTLE
    return (0x12 == c.s[0]);

}

// 模拟htonl函数，本机字节序转网络字节序

unsigned long int htonl(unsigned long int h)

{

    // 若本机为大端，与网络字节序同，直接返回

    // 若本机为小端，转换成大端再返回

    return checkCPUendian() ? h : BigLittleSwap32(h);

}

// 模拟ntohl函数，网络字节序转本机字节序

unsigned long int ntohl(unsigned long int n)

{

    // 若本机为大端，与网络字节序同，直接返回

    // 若本机为小端，网络数据转换成小端再返回

    return checkCPUendian() ? n : BigLittleSwap32(n);

}

// 模拟htons函数，本机字节序转网络字节序

unsigned short int htons(unsigned short int h)

{

    // 若本机为大端，与网络字节序同，直接返回

    // 若本机为小端，转换成大端再返回

    return checkCPUendian() ? h : BigLittleSwap16(h);

}

// 模拟ntohs函数，网络字节序转本机字节序

unsigned short int ntohs(unsigned short int n)

{

    // 若本机为大端，与网络字节序同，直接返回

    // 若本机为小端，网络数据转换成小端再返回

    return checkCPUendian() ? n : BigLittleSwap16(n);

}



#define VERSION_3           3
#define VERSION_4           4

#define MODE_CLIENT         3
#define MODE_SERVER         4


#define NTP_LI              0
#define NTP_VN              VERSION_3
#define NTP_MODE            MODE_CLIENT
#define NTP_STRATUM         0
#define NTP_POLL            4
#define NTP_PRECISION       -6

#define NTP_HLEN            48

#define NTP_PORT            123
#define NTP_SERVER          "182.92.12.11"

#define TIMEOUT             10

#define BUFSIZE            100// 1500

#define JAN_1970            0x83aa7e80

#define NTP_CONV_FRAC32(x)  (uint64_t) ((x) * ((uint64_t)1<<32))
#define NTP_REVE_FRAC32(x)  ((double) ((double) (x) / ((uint64_t)1<<32)))

#define NTP_CONV_FRAC16(x)  (uint32_t) ((x) * ((uint32_t)1<<16))
#define NTP_REVE_FRAC16(x)  ((double)((double) (x) / ((uint32_t)1<<16)))


#define USEC2FRAC(x)        ((uint32_t) NTP_CONV_FRAC32( (x) / 1000000.0 ))
#define FRAC2USEC(x)        ((uint32_t) NTP_REVE_FRAC32( (x) * 1000000.0 ))


#define NTP_LFIXED2DOUBLE(x)    ((double) ( ntohl(((struct l_fixedpt *) (x))->intpart) - JAN_1970 + FRAC2USEC(ntohl(((struct l_fixedpt *) (x))->fracpart)) / 1000000.0 ))


struct s_fixedpt {
    uint16_t    intpart;
    uint16_t    fracpart;
};

struct l_fixedpt {
    uint32_t    intpart;
    uint32_t    fracpart;
};


struct ntphdr {
#if 0//__BYTE_ORDER == __BID_ENDIAN
    unsigned int    ntp_li:2;
    unsigned int    ntp_vn:3;
    unsigned int    ntp_mode:3;
#endif
//#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int    ntp_mode:3;
    unsigned int    ntp_vn:3;
    unsigned int    ntp_li:2;
//#endif
    uint8_t         ntp_stratum;
    uint8_t         ntp_poll;
    int8_t          ntp_precision;
    struct s_fixedpt    ntp_rtdelay;
    struct s_fixedpt    ntp_rtdispersion;
    uint32_t            ntp_refid;
    struct l_fixedpt    ntp_refts;
    struct l_fixedpt    ntp_orits;
    struct l_fixedpt    ntp_recvts;
    struct l_fixedpt    ntp_transts;
};


struct timeval {
    time_t tv_sec; // seconds
    long tv_usec; // microseconds
};
int get_ntp_packet(void *buf, size_t *size)
{
    struct ntphdr *ntp;
    struct timeval tv= {0};


    if (!size || *size<NTP_HLEN)
        return -1;

    memset(buf, 0, *size);

    ntp = (struct ntphdr *) buf;
    ntp->ntp_li = NTP_LI;
    ntp->ntp_vn = NTP_VN;
    ntp->ntp_mode = NTP_MODE;
    ntp->ntp_stratum = NTP_STRATUM;
    ntp->ntp_poll = NTP_POLL;
    ntp->ntp_precision = NTP_PRECISION;

    //gettimeofday(&tv, NULL);
    printf("tv.tv_sec:%d\r\n",tv.tv_sec);
    printf("tv.tv_usec:%d\r\n",tv.tv_usec);

    ntp->ntp_transts.intpart = htonl(tv.tv_sec + JAN_1970);
    ntp->ntp_transts.fracpart = htonl(USEC2FRAC(tv.tv_usec));

    *size = NTP_HLEN;

    return 0;
}


void print_ntp(struct ntphdr *ntp)
{
    time_t time;

    printf("LI:\t%d \r\n", ntp->ntp_li);
    printf("VN:\t%d \r\n", ntp->ntp_vn);
    printf("Mode:\t%d \r\n", ntp->ntp_mode);
    printf("Stratum:\t%d \r\n", ntp->ntp_stratum);
    printf("Poll:\t%d \r\n", ntp->ntp_poll);
    printf("precision:\t%d \r\n", ntp->ntp_precision);

    printf("Route delay:\t %lf \r\n",
           ntohs(ntp->ntp_rtdelay.intpart) + NTP_REVE_FRAC16(ntohs(ntp->ntp_rtdelay.fracpart)));
    printf("Route Dispersion:\t%lf \r\n",
           ntohs(ntp->ntp_rtdispersion.intpart) + NTP_REVE_FRAC16(ntohs(ntp->ntp_rtdispersion.fracpart)));
    printf("Referencd ID:\t %d \r\n", ntohl(ntp->ntp_refid));


    time = ntohl(ntp->ntp_refts.intpart) - JAN_1970;
    printf("Reference:\t%d %ld %s \r\n",
           ntohl(ntp->ntp_refts.intpart) - JAN_1970,
           FRAC2USEC(ntohl(ntp->ntp_refts.fracpart)),
           ctime(&time));

    time = ntohl(ntp->ntp_orits.intpart) - JAN_1970;
    printf("Originate:\t%d %d frac=%ld (%s) \r\n",
           ntohl(ntp->ntp_orits.intpart) - JAN_1970,
           FRAC2USEC(ntohl(ntp->ntp_orits.fracpart)),
           ntohl(ntp->ntp_orits.fracpart),
           ctime(&time) );

    time = ntohl(ntp->ntp_recvts.intpart) - JAN_1970;
    printf("Receive:\t%d %d (%s) \r\n",
           ntohl(ntp->ntp_recvts.intpart) - JAN_1970,
           FRAC2USEC(ntohl(ntp->ntp_recvts.fracpart)),
           ctime(&time) );

    time = ntohl(ntp->ntp_transts.intpart) - JAN_1970;
    printf("Transmit:\t%d %d (%s) \r\n",
           ntohl(ntp->ntp_transts.intpart) - JAN_1970,
           FRAC2USEC(ntohl(ntp->ntp_transts.fracpart)),
           ctime(&time) );
}

#if 1
double get_rrt(const struct ntphdr *ntp, const struct timeval *recvtv)
{
    double t1, t2, t3, t4;

    t1 = NTP_LFIXED2DOUBLE(&ntp->ntp_orits);
    t2 = NTP_LFIXED2DOUBLE(&ntp->ntp_recvts);
    t3 = NTP_LFIXED2DOUBLE(&ntp->ntp_transts);
    t4 = recvtv->tv_sec + recvtv->tv_usec / 1000000.0;

    return (t4 - t1) - (t3 - t2);
}

double get_offset(const struct ntphdr *ntp, const struct timeval *recvtv)
{
    double t1, t2, t3, t4;

    t1 = NTP_LFIXED2DOUBLE(&ntp->ntp_orits);
    t2 = NTP_LFIXED2DOUBLE(&ntp->ntp_recvts);
    t3 = NTP_LFIXED2DOUBLE(&ntp->ntp_transts);
    t4 = recvtv->tv_sec + recvtv->tv_usec / 1000000.0;

    return ((t2 - t1) + (t3 - t4)) / 2;
}
#endif



int at_udp_ntp_test(void)
{
    int sock = 0;
    size_t nbytes;
    unsigned char buf[BUFSIZE]= {0};
    char i =0;
    int ret = -1;

    sock  = transport_open("\"182.92.12.11\"",NTP_PORT);
    nbytes = BUFSIZE;
    if (get_ntp_packet(buf, &nbytes) != 0)
    {
        printf("construct ntp request error \r\n");

    }
    printf("get_ntp_packet:%d\r\n",nbytes);
    for(i=0; i<nbytes; i++)
        printf("buf[%d]=0x%x,",i,buf[i]);
    printf("get_ntp_packet:\r\n");
    ret = transport_sendPacketBuffer(sock,buf,nbytes);
    printf("transport_sendPacketBuffer ret=%d\r\n",ret);

    memset(buf,0,sizeof(buf));
    ret = transport_getdatanb(&sock,buf,BUFSIZE);
    printf("transport_getdatanb:%d\r\n",ret);
    if(ret >0)
    {
        for(i=0; i<ret; i++)
        {
            printf("buf[%02d]=0x%02x,",i,buf[i]);
            if((i+1)%8 == 0)
                printf("\r\n");
        }
        printf("\r\ntransport_getdatanb:\r\n");
    }
    print_ntp((struct ntphdr *)buf);

    //struct timeval recvtv;
//	double offset;
//	offset = get_offset((struct ntphdr *)buf, &recvtv);

    //获取NTP服务器返回的时间
//#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

    struct ntphdr *pbuf=(struct ntphdr *)buf;

//	recvtv.tv_sec	= pbuf->ntp_transts.intpart - JAN_1970;
//	recvtv.tv_usec	= USEC(pbuf->ntp_transts.fracpart);

    time_t time;
    time = ntohl(pbuf->ntp_transts.intpart) - JAN_1970;


    printf("获取NTP服务器返回的时间 %s\r\n",ctime(&time) );
    struct tm *tm_now;



    tm_now = localtime(&time);
    printf("cur tm_now =%d-%d-%d %d:%d:%d \r\n",       \
           tm_now->tm_year+1900, tm_now->tm_mon, tm_now->tm_mday,  \
           tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);


    transport_close(sock );

    return 0;
}

int at_tcp_mqtt_test(void)
{
    int sock = 0;
    size_t nbytes;
//	unsigned char buf[BUFSIZE]= {0};
    char i =0;
    int ret = -1;

    sock  = transport_open("\"mqtt.fluux.io\"",1883);

    //nbytes = BUFSIZE;
#if 0
    [09-10-18:34:24]0x10,0x26,0x00,0x04,0x4d,0x51,0x54,0x54,
    [09-10-18:34:24]0x04,0xc2,0x00,0x14,0x00,0x02,0x6d,0x65,
    [09-10-18:34:24]0x00,0x08,0x74,0x65,0x73,0x74,0x75,0x73,
    [09-10-18:34:24]0x65,0x72,0x00,0x0c,0x74,0x65,0x73,0x74,
    [09-10-18:34:24]0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,


    [09-10-18:47:27]0x10,0x26,0x00,0x04,0x4d,0x51,0x54,0x54,
    [09-10-18:47:27]0x04,0xc2,0x00,0x14,0x00,0x02,0x6d,0x65,
    [09-10-18:47:27]0x00,0x08,0x74,0x65,0x73,0x74,0x75,0x73,
    [09-10-18:47:27]0x65,0x72,0x00,0x0c,0x74,0x65,0x73,0x74,
    [09-10-18:47:27]0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,

#endif
    unsigned char buf[]= {0x10,0x26,0x00,0x04,0x4d,0x51,0x54,0x54,
                          0x04,0xc2,0x00,0x14,0x00,0x02,0x6d,0x65,
                          0x00,0x08,0x74,0x65,0x73,0x74,0x75,0x73,
                          0x65,0x72,0x00,0x0c,0x74,0x65,0x73,0x74,
                          0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64
                         };
    nbytes = sizeof( buf);

    ret = transport_sendPacketBuffer(sock,buf,nbytes);
    printf("transport_sendPacketBuffer ret=%d\r\n",ret);

    memset(buf,0,sizeof(buf));
    //connack
    ret = transport_getdatanb(&sock,buf,4);
    printf("transport_getdatanb:%d\r\n",ret);
    if(ret >0)
    {
        for(i=0; i<ret; i++)
        {
            printf("buf[%02d]=0x%02x,",i,buf[i]);
            if((i+1)%8 == 0)
                printf("\r\n");
        }
        printf("\r\ntransport_getdatanb:\r\n");
    }


    transport_close(sock );

    return 0;
}

