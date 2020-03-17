#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <locale.h>	//本地化设置
#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include "ec20.h"

int serial_fd;
void serial_init(int fd)
{
    struct termios options;
    tcgetattr(fd, &options);
    options.c_cflag |= ( CLOCAL | CREAD );
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CSTOPB; 
    options.c_iflag |= IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    tcsetattr(fd,TCSANOW,&options);
}

/**
  * @brief	向 AIR 模块发送命令
  * @param	cmd			发送的命令
  * @param	ack			期待的应答结果
  * @param	waittime 	等待时间(单位:10ms)
  * @retval	0 发送成功
			1 发送失败
  */
unsigned char ec20_send_cmd(char *cmd, char *ack, int trytimes,int waittime)
{
	unsigned char res = 1;
	char reply[200] = {'\0'};
	int wait = waittime;
	int ret = 0;
	while((res)&&(trytimes--))
	{
		write(serial_fd,cmd,strlen(cmd));
		
		if (ack && waittime)
		{
			while(--waittime)
			{
				usleep(10);
				ret = read(serial_fd,reply,sizeof(reply));
				if(ret > 0)
				{
					if(strstr(reply,ack)!= NULL){
						res = 0;
					    break;
					}
					memset(reply,'0',200);
				}
			}
		}
		waittime = wait;
	}
	return res;
}

unsigned char ec20_send_data(char *buf,int buflen,char *ack, int trytimes,int waittime)
{
	unsigned char res = 1;
	char reply[200] = {0};
	int wait = waittime;
	int ret = 0;
	char recvchar;
	int count = 0;
	write(serial_fd,buf,buflen);
	while((res)&&(trytimes--))
	{
		memset(reply,'0',200);
		count = 0;
		if (ack && waittime)
		{
			while(--waittime)
			{
				usleep(10);
				ret = read(serial_fd,&recvchar,1);
			    if(ret < 0)
			        continue;
			    if((recvchar == 0x0d)||(recvchar == 0x0a)){
			        read(serial_fd,&recvchar,1);
			    	if((recvchar == 0x0d)||(recvchar == 0x0a))
			             break;
			     }
				reply[count++] = recvchar;
				if(strstr(reply,ack)!= NULL){
					res = 0;
					break;
				}
			}
		}
		waittime = wait;
	}
	return res;
}
int transport_getdata(unsigned char *buf,int count)
{
	int ret = 0;
	int i;
	char cmd[30] = {'\0'};
	char readbuf[1024] = {'\0'};
	char *p;
	sprintf(cmd,"AT+QIRD=0,%d",count);
	while(1)
	{
	     write(serial_fd,cmd,strlen(cmd));
	     write(serial_fd,"\r\n",2);
	     usleep(10);
	     ret = read(serial_fd,readbuf,1024);
		 readbuf[1023] = '\0';
	     if((ret <=0)||(strstr(readbuf,"+QIRD: ") == NULL)||(strstr(readbuf,"+QIRD: 0") != NULL))
	         continue;
		 p = readbuf;
		while(strncmp(p,"+QIRD: ",7) != 0)
			p++;
		p += 7;
		ret = 0;
		while(*p != 0x0d)
		{
			ret *= 10;
			ret += *p - '0';
			p++;
		}
		p+= 2;
		for(i = 0;i < ret;i++)
			buf[i] = *p++;

         break;
   }
   return ret;
}

void ec20_poweron(void)
{
	ec20_send_cmd("AT\r\n", "OK",1,100);
}


int transport_close(void)
{
	ec20_send_cmd("AT+CIPCLOSE\r\n", "CLOSE OK",1,200);
	close(serial_fd);
	return 0;
}

/**
  * @brief	AIR TCP连接测试
  */
int check_state(void)
{
	int ret;
	/* 确保模块工作正常 */
	ret = ec20_send_cmd("AT+GSN\r\n", "OK",3,20);					/*  */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+CPIN?\r\n", "OK",3,20);					/* 查询SIM卡状态 */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+CIMI\r\n", "OK",3,20);					/*  */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+QCCID\r\n", "OK",3,20);					/*  */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+CSQ\r\n", "OK",2,20);						/* 信号质量 */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+CREG?\r\n", "OK",2,20);					/* 网络注册信息 */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+CGREG?\r\n", "OK",2,20);					/* GPRS网络注册状态 */
	if(ret)
		goto error;
	ret = ec20_send_cmd("AT+CEREG?\r\n", "OK",2,20);
	if(ret)
		goto error;
	return 0;
error:
	return 1;
}

int set_tcp_config(void)
{
	int ret;
	/* TCP设置 */
	ret = ec20_send_cmd("AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1\r\n", "OK",2,20);  //配置为快发模式
	if(ret)
		return 1;
	return 0;
}
int transport_open(void)
{
	int ret;
	serial_fd = open(DEV_NAME,O_RDWR | O_NOCTTY | O_NDELAY);
	if(serial_fd < 0)
	{
		printf("open error!\n");
		return -1;
	}
	serial_init(serial_fd);

	ec20_poweron();
	ec20_send_cmd("ATV1\r\n", "OK",2,100);						/* 关闭回显 */
	ec20_send_cmd("ATE1\r\n", "OK",2,100);						/* 关闭回显 */
	ec20_send_cmd("AT+CMEE=2\r\n", "OK",2,100);						/* 关闭回显 */

	ret = check_state();
	if(ret){
		printf("the 4G modules state error!\n");
		return -1;
	}
	ret = set_tcp_config();
	if(ret){
		printf("config tcp error!\n");
		return -1;
	}		
	return 0;
}

int connect_and_send(const char *server_ip_and_port,unsigned  char *buf,int buflen)
{
	char cmd[30] = {0};
	int ret;
	char connect_server_ip_port_cmd[56];
	sprintf(cmd,"AT+QISEND=0,%d\r\n",buflen);
	memset(connect_server_ip_port_cmd,'\0',56);
	strcpy(connect_server_ip_port_cmd,"AT+QIOPEN=1,0,\"UDP\",");
	//strcpy(connect_server_ip_port_cmd,"AT+QIOPEN=1,0,\"TCP\",");
	strcat(connect_server_ip_port_cmd,server_ip_and_port);
	strcat(connect_server_ip_port_cmd,",0,0\r\n");
act:
	ret = ec20_send_cmd("AT+QIDEACT=1\r\n", "OK",1,40);
	if(ret)
	{
		goto act;
	}
	ret = ec20_send_cmd("AT+QIACT=1\r\n", "OK",1,40);
	if(ret)
	{
		goto act;
	}
open:
	ret = ec20_send_cmd(connect_server_ip_port_cmd, "OK",2,20);
    if(ret){
        goto act;
	}
	ret = ec20_send_cmd("AT+QISTATE=1,0\r\n","2,1,0,0",8,20);
	if(ret)
	{
		ret = ec20_send_cmd("AT+QICLOSE=0\r\n","OK",1,100);
		if(ret)
		{
			printf("open error!\n");
			goto error;
		}
		goto open;
	}

	ret = ec20_send_cmd(cmd, ">",1,100);
	if(ret){
		goto open;
	}
	ret = ec20_send_data(buf,buflen, "SEND OK",4,100);
	if(ret){
		goto error;
	}
	return buflen;
error:
	return 0;

}
int transport_sendPacketBuffer(const char *server_ip_and_port,unsigned  char *buf,int buflen)
{
	int ret;
	ret = connect_and_send(server_ip_and_port,buf,buflen);
	if(!ret){
		printf("connect server error!\n");
		goto close;
	}
	return ret;
close:
	/* 关闭连接 */
	ec20_send_cmd("AT+CIPCLOSE\r\n", "CLOSE OK",1,20);
	return -1;
}
