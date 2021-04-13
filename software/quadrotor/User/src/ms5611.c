#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "ms5611.h"
#include "math.h"
#include "define.h"
#include "filter.h"
//3?��??��IIC
void MS5611_IIC_Init(void)
{					     
  //gpio_init(MS5611_MSCL_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  //gpio_init(MS5611_MSDA_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  
  //SCL
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE); //?����PD0��PD1�˿���ΪGPIO
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );	//��1?��GPIOB����?��
     
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //��?������?3?
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_SetBits(GPIOD,GPIO_Pin_1); 	//PB6,PB7 ��?3???
  
  //SDA
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );	//��1?��GPIOB����?��
     
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //��?������?3?
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
     
}
//2������IIC?e��?D?o?
void MS5611_IIC_Start(void)
{
	MS5611_MSDA_MOD = 0X7;     //sda??��?3?
	MS5611_MSDA_OUT=1;	  	  
	MS5611_MSCL_OUT=1;
	delay_us(1);
 	MS5611_MSDA_OUT=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	MS5611_MSCL_OUT=0;//?����?I2C����??��?��?��?����?��?��?����?��y?Y 
}	  
//2������IIC����?1D?o?
void MS5611_IIC_Stop(void)
{
	MS5611_MSDA_MOD = 0X7;
	MS5611_MSCL_OUT=0;
	MS5611_MSDA_OUT=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	MS5611_MSCL_OUT=1; 
	MS5611_MSDA_OUT=1;//����?��I2C����???����?D?o?
	delay_us(1);							   	
}
//�̨���y��|��eD?o?��?����
//����???�̡�o1��??����?��|��e����㨹
//        0��??����?��|��e3��1|
u8 MS5611_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MS5611_MSDA_MOD = 0X4;      //SDA����???a��?��?  
	MS5611_MSDA_OUT=1;delay_us(1);	   
	MS5611_MSCL_OUT=1;delay_us(1);	 
	while(MS5611_MSDA_IN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MS5611_IIC_Stop();
			return 1;
		}
	}
	MS5611_MSCL_OUT=0;//����?����?3?0 	   
	return 0;  
} 

void delay2(unsigned int x)
{
   unsigned int i;
   for(i=0;i<x;i++);
}


//2������ACK��|��e
void MS5611_IIC_Ack(void)
{
	MS5611_MSCL_OUT=0;
	MS5611_MSDA_MOD = 0X7;
	MS5611_MSDA_OUT=0;
	delay_us(1);
	MS5611_MSCL_OUT=1;
	delay_us(1);
	MS5611_MSCL_OUT=0;
}
//2?2������ACK��|��e		    
void MS5611_IIC_NAck(void)
{
	MS5611_MSCL_OUT=0;
	MS5611_MSDA_MOD = 0X7;
	MS5611_MSDA_OUT=1;
	delay_us(1);
	MS5611_MSCL_OUT=1;
	delay_us(1);
	MS5611_MSCL_OUT=0;
}					 				     
//IIC����?����???��??��
//����??�䨮?����D?T��|��e
//1��?��D��|��e
//0��??T��|��e			  
void MS5611_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    MS5611_MSDA_MOD = 0X7;    
    MS5611_MSCL_OUT=0;//��-�̨�����?��?a��?��y?Y��?��?
    for(t=0;t<8;t++)
    {              
        MS5611_MSDA_OUT=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(1);   //??TEA5767?a��y???������??��?��?D?��?
		MS5611_MSCL_OUT=1;
		delay_us(1); 
		MS5611_MSCL_OUT=0;	
		delay_us(1);
    }	 
} 	    
//?��1??��??����?ack=1������?����?��ACK��?ack=0��?����?��nACK   
u8 MS5611_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MS5611_MSDA_MOD = 0X4;//SDA����???a��?��?
    for(i=0;i<8;i++ )
	{
        MS5611_MSCL_OUT=0; 
        delay_us(1);
		MS5611_MSCL_OUT=1;
        receive<<=1;
        if(MS5611_MSDA_IN)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MS5611_IIC_NAck();//����?��nACK
    else
        MS5611_IIC_Ack(); //����?��ACK   
    return receive;
}

void MS5611_I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info)
{
     MS5611_IIC_Start();
     MS5611_IIC_Send_Byte(DeviceAddr);
     MS5611_IIC_Wait_Ack();
     MS5611_IIC_Send_Byte(info);
     MS5611_IIC_Wait_Ack();
     MS5611_IIC_Stop();

}

uint16_t MS5611_I2C_Read_2Bytes(unsigned char DeviceAddr,unsigned char address)
 {
     unsigned char i,data_temp1,data_temp2;
     uint16_t data16;
     MS5611_IIC_Start();
     MS5611_IIC_Send_Byte(DeviceAddr);
     MS5611_IIC_Wait_Ack();
     MS5611_IIC_Send_Byte(address);
     MS5611_IIC_Wait_Ack();
     MS5611_IIC_Start();
     MS5611_IIC_Send_Byte(DeviceAddr+1);
     MS5611_IIC_Wait_Ack();	 
     data_temp1=MS5611_IIC_Read_Byte(1);
     data_temp2=MS5611_IIC_Read_Byte(0);	
     MS5611_IIC_Stop();

  	data16=(data_temp1<<8)|data_temp2;
  	return data16;
}
 
uint32_t MS5611_I2C_Read_3Bytes(unsigned char DeviceAddr,unsigned char address)
{
     unsigned char i,data_temp1,data_temp2,data_temp3;
  	 uint32_t data32;
     MS5611_IIC_Start();
     MS5611_IIC_Send_Byte(DeviceAddr);
     MS5611_IIC_Wait_Ack();
     MS5611_IIC_Send_Byte(address);
     MS5611_IIC_Wait_Ack();
     MS5611_IIC_Start();
     MS5611_IIC_Send_Byte(DeviceAddr+1);
     MS5611_IIC_Wait_Ack();
  	 
     data_temp1=MS5611_IIC_Read_Byte(1);
     data_temp2=MS5611_IIC_Read_Byte(1);	
     data_temp3=MS5611_IIC_Read_Byte(0);
     MS5611_IIC_Stop();

  	 data32=data_temp1*65535+data_temp2*256+data_temp3;
  	 return data32;
}


#define MS561101BA_ADDR   0xEE   //CBR=0 0x77 I2C address when CSB is connected to LOW (GND)
#define MS561101BA_D1 0x40
#define MS561101BA_D2 0x50
#define MS561101BA_RESET 0x1E

#define MS561101BA_OSR_256 0x00
#define MS561101BA_OSR_512 0x02
#define MS561101BA_OSR_1024 0x04
#define MS561101BA_OSR_2048 0x06
#define MS561101BA_OSR_4096 0x08
#define  MS561101BA_D1_OSR_4096 0x48 
#define  MS561101BA_D2_OSR_4096 0x58 

#define MS561101BA_PROM_BASE_ADDR 0xA0 // by adding ints from 0 to 6 we can read all the prom configuration values. 
// C1 will be at 0xA2 and all the subsequent are multiples of 2
#define MS561101BA_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS561101BA_PROM_REG_SIZE 2 // size in bytes of a prom registry.

_ms5611 ms5611;

  

void MS561101BA_Reset(void)
{
  MS5611_I2C_NoAddr_WriteByte(MS561101BA_ADDR,MS561101BA_RESET);
}

void MS561101BA_readPROM(void)
{   
  	u8 i;
  	for (i=0;i<=MS561101BA_PROM_REG_COUNT;i++) 
  	{
  		 ms5611.Cal_C[i]=MS5611_I2C_Read_2Bytes(MS561101BA_ADDR,MS561101BA_PROM_BASE_ADDR + (i * MS561101BA_PROM_REG_SIZE));
  	}
  	//printf("\n The MS561101BA is reading PROM : \r\n");
    //printf("\r\nC1 = %d\r\nC2 = %d\r\nC3 = %d\r\nC4 = %d\r\nC5 = %d\r\nC6 = %d\r\n",Cal_C[1],Cal_C[2],Cal_C[3],Cal_C[4],Cal_C[5],Cal_C[6]);  
}

void MS561101BA_Init(void)
{
  	MS561101BA_Reset();
  	DelayMs(100);
  	MS561101BA_readPROM();
  	DelayMs(100);
} 

//void MS561101BA_GetTemperature(u8 OSR_Temp)
//{
//    
//	  long long int dT1;
//  	ms5611.D2_Temp= MS561101BA_DO_CONVERSION(OSR_Temp);	
//  	DelayMs(10);
//  	
//  	ms5611.dT=ms5611.D2_Temp - ((ms5611.Cal_C[5])<<8);
//	  if(ms5611.dT<0)
//		{
//		//printf("dT is %d\r\n",dT);
//		  ms5611.dT=-ms5611.dT;
//		//printf("dT is %d\r\n",dT);
//	  //printf("Cal6 is %d\r\n",Cal_C[6]);
//		  dT1=ms5611.dT*(ms5611.Cal_C[6]>>8);  //����Ӧ����Cal_C[6]>>23,�����ֹ������Էֿ�����
//		  dT1=dT1>>15;
//		//printf("dT1 is %lld\r\n",dT1);
//  	  ms5611.Temperature=2000-dT1;
//			ms5611.dT=-ms5611.dT;
//		}else
//		{
//			dT1=ms5611.dT*(ms5611.Cal_C[6]>>8);
//			dT1=dT1>>15;
//			ms5611.Temperature=2000+dT1;
//		}
//    //printf("Temp is %d\r\n",Temperature);
//  
//}
//
//uint32_t MS561101BA_DO_CONVERSION(uint8_t command)
//{
//  		uint32_t conversion;
//  
//  	 MS5611_I2C_NoAddr_WriteByte(MS561101BA_ADDR,command);
//  	 
//  	DelayMs(10);
//   
//  	conversion=MS5611_I2C_Read_3Bytes(MS561101BA_ADDR,0);
//  
//     return conversion;
//  
//}
//
//void MS561101BA_GetPressure(u8 OSR_Pres)
// {
//	  uint64_t a;
//	  uint64_t b;
//	  uint64_t c;
//	  uint64_t d;
//	  uint16_t b1;
//	  uint16_t b2;
//	  uint16_t d1;
//	  uint16_t d2;
//  	ms5611.D1_Pres= MS561101BA_DO_CONVERSION(OSR_Pres);
//    //printf("dT is %d\r\n",dT);   //��֤֮ǰdT��û���Ĺ���
//  	DelayMs(10); 
//	 
//	  //OFF=(Cal_C[2]<<16)+((Cal_C[4]*dT)>>7);
//  	//SENS=(Cal_C[1]<<15)+((Cal_C[3]*dT)>>8);   
//	  //����̫�������㲻���������Ϲ�ʽ����a��b��c��d��b1��b2��d1��d2�ֿ����㣬������֤
//	 
//	 
//  	a=ms5611.Cal_C[2]<<15;
//	  c=ms5611.Cal_C[1]<<15;
//	  
//	  if(ms5611.dT<0)
//		{
//			ms5611.dT=-ms5611.dT;
//			b1=ms5611.dT>>3;
//			b2=ms5611.Cal_C[4]>>4;
//			d1=ms5611.dT>>4;
//			d2=ms5611.Cal_C[3]>>4;
//			b=b1*b2;
//			d=d1*d2;
//			ms5611.OFF=a-b;
//			//printf("OFF is %lld\r\n",OFF);    //���ڵ���
//			ms5611.OFF=ms5611.OFF+a;
//			//printf("OFF is %lld\r\n",OFF);
//			ms5611.SENS=c-d;
//			ms5611.dT=-ms5611.dT;    //��dT��������Ա���湫ʽ��
//		}else
//		{
//			b1=ms5611.dT>>3;
//			b2=ms5611.Cal_C[4]>>4;
//			d1=ms5611.dT>>4;
//			d2=ms5611.Cal_C[3]>>4;
//			b=b1*b2;
//			d=d1*d2;
//			ms5611.OFF=a+b;
//			//printf("OFF is %lld\r\n",OFF);
//			ms5611.OFF=ms5611.OFF+a;
//			//printf("OFF is %lld\r\n",OFF);
//			ms5611.SENS=c+d;
//		}
//  	if(ms5611.Temperature < 2000)// second order temperature compensation when under 20 degrees C
//  	{
//  		ms5611.Temperature2 = ((ms5611.dT*ms5611.dT)>>31);
//  		ms5611.Aux = (ms5611.Temperature-2000)*(ms5611.Temperature-2000);
//  		ms5611.OFF2 = 2.5*ms5611.Aux;
//  		ms5611.SENS2 = 1.25*ms5611.Aux;
//  		if(ms5611.Temperature < -1500)
//  		{
//  			ms5611.Aux = (ms5611.Temperature+1500)*(ms5611.Temperature+1500);
//  			ms5611.OFF2 = ms5611.OFF2 + 7*ms5611.Aux;
//  			ms5611.SENS2 = ms5611.SENS + 5.5*ms5611.Aux;
//  		}
//  	}else  //(Temperature > 2000)
//  	{
//  		ms5611.Temperature2 = 0;
//  		ms5611.OFF2 = 0;
//  		ms5611.SENS2 = 0;
//  	}
//		//printf("OFF is %lld\r\n",OFF);
//	  //printf("SENS is %lld\r\n",SENS);
//		//printf("OFF2 is %lld\r\n",OFF2);
//	  //printf("SENS2 is %lld\r\n",SENS2);
//  	
//  	ms5611.Temperature = ms5611.Temperature - ms5611.Temperature2;
//  	ms5611.OFF = ms5611.OFF - ms5611.OFF2;
//  	ms5611.SENS = ms5611.SENS - ms5611.SENS2;	
//		//printf("OFF is %lld\r\n",OFF);
//	  //printf("SENS is %lld\r\n",SENS);
//  
//  	ms5611.Pressure=((((ms5611.D1_Pres*ms5611.SENS)>>21)-ms5611.OFF)>>15);
//  
// }
//
//void MS5611GetTemperatureAndPressure(void) 
//{
//  MS561101BA_GetTemperature(MS561101BA_D2_OSR_4096);//0x58
//  MS561101BA_GetPressure(MS561101BA_D1_OSR_4096);		//0x48
//
//   ms5611.altitude = (float)((1.0f - pow((double)ms5611.Pressure / 101325.0f, 0.190295f)) * 44330.0f); 
//
//   printf("%f\r\n",ms5611.altitude);
//   printf("T : %2.2f ��C    \r\n ",(float)ms5611.Temperature/100);
// }

KalmanInfo kalmanFilter_of_ms5611_altitude;
//�������Ӧ�÷ŵ�5ms�ж��в�������ִ��
void MS5611GetTemperatureAndPressure(void) 
{
  //MS561101BA_GetTemperature(MS561101BA_D2_OSR_4096);//0x58
  //MS561101BA_GetPressure(MS561101BA_D1_OSR_4096);		//0x48
  static char ms5611_get_T_time = 0;//���ڲ����¶ȵ���ʱ
  static char ms5611_geted_T_flag = 0;//�����¶���ɱ�־λ
  long long int dT1;
  //ms5611.D2_Temp= MS561101BA_DO_CONVERSION(MS561101BA_D2_OSR_4096);	
  ms5611_get_T_time++;		
  if(ms5611_get_T_time == 1&&(ms5611_geted_T_flag == 0))
    MS5611_I2C_NoAddr_WriteByte(MS561101BA_ADDR,MS561101BA_D2_OSR_4096);
  //DelayMs(10);
  else if(ms5611_get_T_time == 3&&(ms5611_geted_T_flag == 0))//10ms��ʱ����
  {
    ms5611.D2_Temp=MS5611_I2C_Read_3Bytes(MS561101BA_ADDR,0);
  
    ms5611_geted_T_flag = 1;
    ms5611.dT=ms5611.D2_Temp - ((ms5611.Cal_C[5])<<8);
    if(ms5611.dT<0)
    {
      //printf("dT is %d\r\n",dT);
      ms5611.dT=-ms5611.dT;
      //printf("dT is %d\r\n",dT);
      //printf("Cal6 is %d\r\n",Cal_C[6]);
      dT1=ms5611.dT*(ms5611.Cal_C[6]>>8);  //����Ӧ����Cal_C[6]>>23,�����ֹ������Էֿ�����
      dT1=dT1>>15;
      //printf("dT1 is %lld\r\n",dT1);
      ms5611.Temperature=2000-dT1;
      ms5611.dT=-ms5611.dT;
    }
    else
    {
      dT1=ms5611.dT*(ms5611.Cal_C[6]>>8);
      dT1=dT1>>15;
      ms5611.Temperature=2000+dT1;
    }
 
  } 
  uint64_t a;
  uint64_t b;
  uint64_t c;
  uint64_t d;
  uint16_t b1;
  uint16_t b2;
  uint16_t d1;
  uint16_t d2;
  //ms5611.D1_Pres= MS561101BA_DO_CONVERSION(MS561101BA_D1_OSR_4096);
  static char ms5611_get_P_time = 0;  
  if(ms5611_geted_T_flag == 1)
  {
    ms5611_get_P_time++;
    if(ms5611_get_P_time == 1)
      MS5611_I2C_NoAddr_WriteByte(MS561101BA_ADDR,MS561101BA_D1_OSR_4096);
    //DelayMs(10);
    else if(ms5611_get_P_time == 3)
    {
      ms5611.D1_Pres=MS5611_I2C_Read_3Bytes(MS561101BA_ADDR,0);

       ms5611_get_T_time = 0;//������ʱִ����ϣ�һ�β������������������һ�β���

      ms5611_geted_T_flag = 0;
      
      ms5611_get_P_time = 0;
      //OFF=(Cal_C[2]<<16)+((Cal_C[4]*dT)>>7);
      //SENS=(Cal_C[1]<<15)+((Cal_C[3]*dT)>>8);   
      //����̫�������㲻���������Ϲ�ʽ����a��b��c��d��b1��b2��d1��d2�ֿ����㣬������֤
      a=ms5611.Cal_C[2]<<15;
      c=ms5611.Cal_C[1]<<15;
      if(ms5611.dT<0)
      {
                      ms5611.dT=-ms5611.dT;
                      b1=ms5611.dT>>3;
                      b2=ms5611.Cal_C[4]>>4;
                      d1=ms5611.dT>>4;
                      d2=ms5611.Cal_C[3]>>4;
                      b=b1*b2;
                      d=d1*d2;
                      ms5611.OFF=a-b;
                      //printf("OFF is %lld\r\n",OFF);    //���ڵ���
                      ms5611.OFF=ms5611.OFF+a;
                      //printf("OFF is %lld\r\n",OFF);
                      ms5611.SENS=c-d;
                      ms5611.dT=-ms5611.dT;    //��dT��������Ա���湫ʽ��
              }else
              {
                      b1=ms5611.dT>>3;
                      b2=ms5611.Cal_C[4]>>4;
                      d1=ms5611.dT>>4;
                      d2=ms5611.Cal_C[3]>>4;
                      b=b1*b2;
                      d=d1*d2;
                      ms5611.OFF=a+b;
                      //printf("OFF is %lld\r\n",OFF);
                      ms5611.OFF=ms5611.OFF+a;
                      //printf("OFF is %lld\r\n",OFF);
                      ms5611.SENS=c+d;
              }
      if(ms5611.Temperature < 2000)// second order temperature compensation when under 20 degrees C
      {
              ms5611.Temperature2 = ((ms5611.dT*ms5611.dT)>>31);
              ms5611.Aux = (ms5611.Temperature-2000)*(ms5611.Temperature-2000);
              ms5611.OFF2 = 2.5*ms5611.Aux;
              ms5611.SENS2 = 1.25*ms5611.Aux;
              if(ms5611.Temperature < -1500)
              {
                      ms5611.Aux = (ms5611.Temperature+1500)*(ms5611.Temperature+1500);
                      ms5611.OFF2 = ms5611.OFF2 + 7*ms5611.Aux;
                      ms5611.SENS2 = ms5611.SENS + 5.5*ms5611.Aux;
              }
      }else  //(Temperature > 2000)
      {
              ms5611.Temperature2 = 0;
              ms5611.OFF2 = 0;
              ms5611.SENS2 = 0;
      }
              //printf("OFF is %lld\r\n",OFF);
        //printf("SENS is %lld\r\n",SENS);
              //printf("OFF2 is %lld\r\n",OFF2);
        //printf("SENS2 is %lld\r\n",SENS2);
      
      ms5611.Temperature = ms5611.Temperature - ms5611.Temperature2;
      ms5611.OFF = ms5611.OFF - ms5611.OFF2;
      ms5611.SENS = ms5611.SENS - ms5611.SENS2;	
              //printf("OFF is %lld\r\n",OFF);
        //printf("SENS is %lld\r\n",SENS);

      ms5611.Pressure=((((ms5611.D1_Pres*ms5611.SENS)>>21)-ms5611.OFF)>>15);

    
      ms5611.altitude = (float)((1.0f - pow((double)ms5611.Pressure / 101325.0f, 0.190295f)) * 44330.0f); 
     
      //printf("%f ",ms5611.altitude);
      
      static uint8 geted_ms5611_altitude_count = 0;//��¼��ø߶ȵĴ���

      //ȡ200��������ƽ����Ϊ��ʼ�߶�
      static float sum_of_ms5611_altitude = 0;
      if(geted_ms5611_altitude_count<=50)
        geted_ms5611_altitude_count++;
      if(geted_ms5611_altitude_count>50&&(geted_ms5611_altitude_count<=250))
      {
        sum_of_ms5611_altitude += ms5611.altitude;
        geted_ms5611_altitude_count++;
      }
      else if(geted_ms5611_altitude_count>250)
      {
        gc[3][4] = ms5611.altitude_init = sum_of_ms5611_altitude/200;
      }
      gc[3][5] = ms5611.altitude = KalmanFilter(&kalmanFilter_of_ms5611_altitude, ms5611.altitude);

    }
  }
  //printf("%f   %f\r\n",ms5611.altitude,ms5611.altitude_init);
   
   //printf("T : %2.2f ��C    \r\n ",(float)ms5611.Temperature/100);
}



