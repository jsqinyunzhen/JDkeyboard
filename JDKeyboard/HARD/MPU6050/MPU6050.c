#include "MPU6050.h"
#include "delay.h"
#include "sys.h"
#include "USART.h"

void MPU_IIC_Init(void)            //初始化IIC的IO口	
{
     GPIO_InitTypeDef GPIO_InitStructure;
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	   
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   GPIO_Init(GPIOB, &GPIO_InitStructure);
	   GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_10); 	//PB8,PB9 输出高
}

void MPU_IIC_Start(void)//产生IIC起始信号
{
		 MPU_SDA_OUT();     //sda线输出
		 MPU_IIC_SDA=1;	  	  
		 MPU_IIC_SCL=1;
		 delay_us(4);
		 MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
		 delay_us(4);
		 MPU_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

void MPU_IIC_Stop(void)//产生IIC停止信号
{
		 MPU_SDA_OUT();//sda线输出
		 MPU_IIC_SCL=0;
		 MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	   delay_us(4);
		 MPU_IIC_SCL=1;
		 delay_us(4); 
		 MPU_IIC_SDA=1;//发送I2C总线结束信号						   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MPU_IIC_Wait_Ack(void)
{
	   u8 waitTime=0;
	   MPU_SDA_IN();      //SDA设置为输入  
	   //IIC_SDA=1;delay_us(1);	   
	   MPU_IIC_SCL=1;
	   delay_us(1);	 
	   while(MPU_READ_SDA)
	   {
		       waitTime++;
		       if(waitTime>250)
		       {
			             MPU_IIC_Stop();
			             return 1;
		       }
	   }
	   MPU_IIC_SCL=0;//时钟输出0 	   
	   return 0;  
} 

//产生ACK应答
void MPU_IIC_Ack(void)
{
	   MPU_IIC_SCL=0;
	   MPU_SDA_OUT();
	   MPU_IIC_SDA=0;
	   delay_us(2);
	   MPU_IIC_SCL=1;
	   delay_us(2);
	   MPU_IIC_SCL=0;
}

//不产生ACK应答		    
void MPU_IIC_NAck(void)
{
	   MPU_IIC_SCL=0;
	   MPU_SDA_OUT();
	   MPU_IIC_SDA=1;
	   delay_us(2);
	   MPU_IIC_SCL=1;
	   delay_us(2);
	   MPU_IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
     u8 t;   
	   MPU_SDA_OUT(); 	    
     MPU_IIC_SCL=0;//拉低时钟开始数据传输
     for(t=0;t<8;t++)
     {              
            MPU_IIC_SDA=(txd&0x80)>>7;
            txd<<=1; 	  
		        delay_us(2);   //对TEA5767这三个延时都是必须的
		        MPU_IIC_SCL=1;
		        delay_us(2); 
		        MPU_IIC_SCL=0;	
		        delay_us(2);
     }	 
}

u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	   unsigned char i,read=0;
	   MPU_SDA_IN();//SDA设置为输入
     for(i=0;i<8;i++ )
	   {
            MPU_IIC_SCL=0; 
            delay_us(2);
		        MPU_IIC_SCL=1;
            read<<=1;
            if(MPU_READ_SDA)read++;   
            delay_us(1); 
     }					 
     if (!ack)
            MPU_IIC_NAck();//发送nACK
     else
            MPU_IIC_Ack(); //发送ACK   
            return read;
}

//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 addr,u8 data) 				 
{ 
      MPU_IIC_Start(); 
	    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	    if(MPU_IIC_Wait_Ack())	//等待应答
	    {
		        MPU_IIC_Stop();		 
		        return 1;		
	    }
      MPU_IIC_Send_Byte(addr);	//写寄存器地址
      MPU_IIC_Wait_Ack();		//等待应答 
	    MPU_IIC_Send_Byte(data);//发送数据
	    if(MPU_IIC_Wait_Ack())	//等待ACK
	    {
		        MPU_IIC_Stop();	 
		        return 1;		 
	    }		 
      MPU_IIC_Stop();	 
	    return 0;
}

//IIC读一个字节 
//addr:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 addr)
{
	     u8 data;
       MPU_IIC_Start(); 
	     MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	     MPU_IIC_Wait_Ack();		//等待应答 
       MPU_IIC_Send_Byte(addr);	//写寄存器地址
       MPU_IIC_Wait_Ack();		//等待应答
       MPU_IIC_Start();
	     MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
       MPU_IIC_Wait_Ack();		//等待应答 
	     data=MPU_IIC_Read_Byte(0);//读取数据,发送nACK 
       MPU_IIC_Stop();			//产生一个停止条件 
	     return data;		
}

//设置MPU6050陀螺仪传感器满量程范围
//num:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_TLY_LCH(u8 num)
{
	     return MPU_Write_Byte(MPU_TLY,num<<3);//设置陀螺仪满量程范围  
}

//设置MPU6050加速度传感器满量程范围
//num:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_SENSOR_LCH(u8 num)
{
	     return MPU_Write_Byte(MPU_SENSOR,num<<3);//设置加速度传感器满量程范围  
}

//设置MPU6050的数字低通滤波器
//dtlb:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_DTLB(u16 dtlb)
{
	    u8 data=0;
	    if(dtlb>=188)      data=1;
	    else if(dtlb>=98)  data=2;
	    else if(dtlb>=42)  data=3;
	    else if(dtlb>=20)  data=4;
	    else if(dtlb>=10)  data=5;
	    else data=6; 
	    return MPU_Write_Byte(MPU_DIRVE,data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//cyl:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 cyl)
{
	    u8 data;
	    if(cyl>1000)cyl=1000;
	    if(cyl<4)cyl=4;
	    data=1000/cyl-1;
	    data=MPU_Write_Byte(MPU_CYFBL,data);	//设置数字低通滤波器
 	    return MPU_Set_DTLB(cyl/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temp(void)
{
      u8 buf[2]; 
      short raw;
	    float temp;
	    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH,2,buf); 
      raw=((u16)buf[0]<<8)|buf[1];  
      temp=36.53+((double)raw)/340;  
      return temp*100;;
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_TLY(short *gx,short *gy,short *gz)
{
      u8 buf[6],res;  
	    res=MPU_Read_Len(MPU_ADDR,MPU_TLY_XOUTH,6,buf);
	    if(res==0)
	    {
		  *gx=((u16)buf[0]<<8)|buf[1];  
		  *gy=((u16)buf[2]<<8)|buf[3];  
		  *gz=((u16)buf[4]<<8)|buf[5];
	    } 	
      return res;;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Sensor(short *ax,short *ay,short *az)
{
      u8 buf[6],res;  
	    res=MPU_Read_Len(MPU_ADDR,MPU_SENSOR_XOUTH,6,buf);
	    if(res==0)
	    {
		  *ax=((u16)buf[0]<<8)|buf[1];  
		  *ay=((u16)buf[2]<<8)|buf[3];  
		  *az=((u16)buf[4]<<8)|buf[5];
	    } 	
      return res;;
}

//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	    u8 i; 
      MPU_IIC_Start(); 
	    MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	    if(MPU_IIC_Wait_Ack())	//等待应答
	    {
		         MPU_IIC_Stop();		 
		         return 1;		
	    }
      MPU_IIC_Send_Byte(reg);	//写寄存器地址
      MPU_IIC_Wait_Ack();		//等待应答
	    for(i=0;i<len;i++)
	    {
		         MPU_IIC_Send_Byte(buf[i]);	//发送数据
		         if(MPU_IIC_Wait_Ack())		//等待ACK
		         {
			              MPU_IIC_Stop();	 
			              return 1;		 
		         }		
	    }    
      MPU_IIC_Stop();	 
	    return 0;	
} 

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	     MPU_IIC_Start(); 
	     MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	     if(MPU_IIC_Wait_Ack())	//等待应答
	     {
		           MPU_IIC_Stop();		 
		           return 1;		
	     }
       MPU_IIC_Send_Byte(reg);	//写寄存器地址
       MPU_IIC_Wait_Ack();		//等待应答
       MPU_IIC_Start();
	     MPU_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令	
       MPU_IIC_Wait_Ack();		//等待应答 
	     while(len)
	     {
		          if(len==1)*buf=MPU_IIC_Read_Byte(0);//读数据,发送nACK 
		          else *buf=MPU_IIC_Read_Byte(1);		//读数据,发送ACK  
		          len--;
		          buf++; 
	     }    
       MPU_IIC_Stop();	//产生一个停止条件 
	     return 0;	
}

u8 MPU_INIT(void)
{
	     u8 res;
	     MPU_IIC_Init();
	     MPU_Write_Byte(MPU_POWER1,0X80);	//复位MPU6050
	     delay_ms(100);
	     MPU_Write_Byte(MPU_POWER1,0X00);	//唤醒MPU6050
	     MPU_Set_TLY_LCH(3);					//陀螺仪传感器,±2000dps
	     MPU_Set_SENSOR_LCH(0);					//加速度传感器,±2g
	     MPU_Set_Rate(50);						//设置采样率50Hz
	     MPU_Write_Byte(MPU_INT_EN,0X00);	//关闭所有中断
	     MPU_Write_Byte(MPU_USER_CTRL,0X00);	//I2C主模式关闭
	     MPU_Write_Byte(MPU_FIFO_EN,0X00);	//关闭FIFO
	     MPU_Write_Byte(MPU_INTBP_CFG,0X80);	//INT引脚低电平有效
	     res=MPU_Read_Byte(MPU_DEVICE_ID);
	     if(res==MPU_ADDR)  //器件ID正确
	     {
				      MPU_Write_Byte(MPU_POWER1,0X01);	//设置CLKSEL,PLL X轴为参考
				      MPU_Write_Byte(MPU_POWER2,0X00);	//加速度与陀螺仪都工作
				      MPU_Set_Rate(50);						//设置采样率为50Hz
			 }
	     else return 1;
	     return 0;
}

//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void MPU6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	     u8 tbuf[12]; 
	     tbuf[0]=(aacx>>8)&0XFF;
	     tbuf[1]=aacx&0XFF;
	     tbuf[2]=(aacy>>8)&0XFF;
	     tbuf[3]=aacy&0XFF;
	     tbuf[4]=(aacz>>8)&0XFF;
	     tbuf[5]=aacz&0XFF; 
	     tbuf[6]=(gyrox>>8)&0XFF;
	     tbuf[7]=gyrox&0XFF;
	     tbuf[8]=(gyroy>>8)&0XFF;
	     tbuf[9]=gyroy&0XFF;
	     tbuf[10]=(gyroz>>8)&0XFF;
	     tbuf[11]=gyroz&0XFF;
	     usart1_niming_send(0XA1,tbuf,12);//自定义帧,0XA1
}







