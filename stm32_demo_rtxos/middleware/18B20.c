#include "stm32f10x.h"
#include "stm32f10x_it.h"
//extern void DS18B20_GPIO_Input(void); 
//extern void DS18B20_GPIO_Output(void); 
//uint8_t  DS18B20_Reset(void);  

unsigned char RH_H,RH_L,T_H,T_L,Sumcheck,TEMP,succ,count,t_f;


#if 0
#define DS18B20_CLK    RCC_APB2Periph_GPIOA
#define DS18B20_PIN    GPIO_Pin_8
#define DS18B20_PORT   GPIOA
//总体代表DS18B20的GPIO口为PB9

#else
#define DS18B20_CLK    RCC_APB2Periph_GPIOB
#define DS18B20_PIN    GPIO_Pin_9
#define DS18B20_PORT   GPIOB
//总体代表DS18B20的GPIO口为PB9
#endif



/****************************************************************************
* 名    称：void LED_Config(void)
* 功    能：LED 控制初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void DS18B20_GPIO_Input(void){
	GPIO_InitTypeDef GPIO_InitStructure;	  
  	
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;				     //PA8配置为通用推挽输出  
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);	
	
} 

/****************************************************************************
* 名    称：void LED_Config(void)
* 功    能：LED 控制初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void DS18B20_GPIO_Output(void){
	GPIO_InitTypeDef GPIO_InitStructure;	  
  	
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;				     //PA8配置为通用推挽输出  
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);	
	
}


#define Set_DS18B20() GPIO_SetBits(DS18B20_PORT,DS18B20_PIN); 

#define Reset_DS18B20() GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN); 


unsigned char DS18B20_Reset(void)
{
	uint8_t result;  			 
	DS18B20_GPIO_Output(); 
	Set_DS18B20(); 
	Delay_us(2); 
	Reset_DS18B20();  
	Delay_us(960);//490us--960us 
	Set_DS18B20();
	Delay_us(60);//15-60us 
	DS18B20_GPIO_Input();  
	if(GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)==1) 
		result=1; 
	else 
		result=0;  
	Delay_us(200);  
	DS18B20_GPIO_Output(); 
	Set_DS18B20(); 
	Delay_us(280); 
	return result; 
} 
/////////////////////////////////////////////////
void DS18B20_WriteByte(uint8_t cmd){
	uint8_t i=0;  
	DS18B20_GPIO_Output(); 
	for(i=0;i<8;i++){  
		Reset_DS18B20(); 
		Delay_us(2);//<15us 4us 
		if(cmd&0x01){
			Set_DS18B20(); 
		} 
		else {
			Reset_DS18B20(); 
		} 
		cmd>>=1; 
		Delay_us(80);//>60us 
		Set_DS18B20(); 
		Delay_us(2);//>1us 
	} 
} 

  

unsigned char DS18B20_ReadByte(void){  
	unsigned char  temper_read=0,i=0; 
	for(i=0;i<8;i++){ 
		temper_read>>=1; 
		DS18B20_GPIO_Output(); 
		Reset_DS18B20(); 
		Delay_us(2);//>1us 
		Set_DS18B20(); 
		Delay_us(2); 
		DS18B20_GPIO_Input(); 
		if(GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)==1){ 
			temper_read|=0x80;
		} 
		Delay_us(70);//>60us 
		DS18B20_GPIO_Output(); 
		Set_DS18B20();
		Delay_us(2); 
	} 
	return temper_read;
} 

  

void start_temp_convet(void) {  
	DS18B20_Reset();
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x44);
} 



unsigned short Read_temp(void){
	uint8_t ret = 0;
	uint8_t temp[2];
	unsigned short temper;
	start_temp_convet(); 
	ret = DS18B20_Reset();
	printf("DS18B20_Reset ret=%d\r\n",ret);
	DS18B20_WriteByte(0xcc); 
	DS18B20_WriteByte(0xBE);  	//开启温度转换	千万别忘了！ ！	！
	temp[0]=DS18B20_ReadByte(); 
	temp[1]=DS18B20_ReadByte();
	temper=temp[1]; 
	temper<<=8;  
	temper|=temp[0]; 
	if(temp[1]>7)
	{ 
		//temper=((~temper)+1)*6.25; 
		temper=65536-temper;
		t_f=1;
	}
	else
	{
		temper=temper*6.25;
		t_f=0;
	}
	#if 1
	if(t_f==1)
	{ 		 //负温
		T_H=temper>>4; 
		T_L=(temper&0x0f);
	} 					  
	else
	{ 				 //正温
		T_H=(temper/1000)*10+((temper%1000)/100);
		T_L=(((temper%1000)%100)/10)*10+(((temper%1000)%100)%10);
	}
	Delay_us(1000000);
	printf("\r\n 18B20实时温度 -------------------------- \r\n"); 
	if(t_f==0) 
		printf("摄氏度 = %d.%d℃	\r\n", T_H,T_L);
	else if(t_f==1) 
		printf("摄氏度 = -%d.%d℃ \r\n", T_H,T_L);
	#endif
		  

	return temper;
} 
void DS18B20_GPIO_Output_test(void)
{
	int i = 0;
	printf("DS18B20_GPIO_Output_test\r\n");
	//DS18B20_GPIO_Output();
	//Read_temp();
	//printf("PA8....\r\n");
	while(i<5)
	{
		
		i++;
		Read_temp();
		Delay_us(1000);//490us--960us 
	}
}	


