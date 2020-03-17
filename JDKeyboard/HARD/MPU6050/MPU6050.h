#ifndef _MPU6050_H
#define _MPU6050_H

#include "stm32f10x.h"

#define MPU_SDA_IN()   {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=(u32)8<<8;}
#define MPU_SDA_OUT()  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=(u32)3<<8;}

#define MPU_IIC_SCL    PBout(9) //SCL
#define MPU_IIC_SDA    PBout(10) //SDA	 
#define MPU_READ_SDA   PBin(10)  //输入SDA 

#define  MPU_ADDR			 0X68

#define  MPU_POWER1    0X6B    //电源管理寄存器1
#define  MPU_POWER2    0X6C    //电源管理寄存器2
#define  MPU_TLY       0X1B    //陀螺仪
#define  MPU_SENSOR    0X1C    //加速度传感器
#define  MPU_FIFO_EN   0X23
#define  MPU_CYFBL     0X19    //采样分辨率
#define  MPU_DIRVE     0X1A

#define MPU_TLY_XOUTH		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_TLY_XOUTL		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_TLY_YOUTH		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_TLY_YOUTL		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_TLY_ZOUTH		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_TLY_ZOUTL		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_SENSOR_XOUTH		0X3B	//加速度值,X轴高8位寄存器
#define MPU_SENSOR_XOUTL		0X3C	//加速度值,X轴低8位寄存器
#define MPU_SENSOR_YOUTH		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_SENSOR_YOUTL		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_SENSOR_ZOUTH		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_SENSOR_ZOUTL		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL		0X42	//温度值低8位寄存器

#define MPU_INT_EN			0X38	//中断使能寄存器
#define MPU_USER_CTRL		0X6A	//用户控制寄存器
#define MPU_INTBP_CFG		0X37	//中断/旁路设置寄存器
#define MPU_DEVICE_ID		0X75	//器件ID寄存器

void MPU_IIC_Init(void);
void MPU_IIC_Start(void);
void MPU_IIC_Stop(void);
u8 MPU_IIC_Wait_Ack(void);
void MPU_IIC_Ack(void);
void MPU_IIC_NAck(void);
void MPU_IIC_Send_Byte(u8 txd);
u8 MPU_IIC_Read_Byte(unsigned char ack);
u8 MPU_INIT(void);
u8 MPU_Write_Byte(u8 addr,u8 data);
u8 MPU_Read_Byte(u8 addr);
u8 MPU_Set_TLY_LCH(u8 num);
u8 MPU_Set_SENSOR_LCH(u8 num);
u8 MPU_Set_Rate(u16 cyl);
short MPU_Get_Temp(void);
u8 MPU_Get_TLY(short *gx,short *gy,short *gz);
u8 MPU_Get_Sensor(short *ax,short *ay,short *az);
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
void MPU6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);





#endif



