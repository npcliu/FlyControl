#include "stm32f10x.h"
#include "core_cm3.h"
#include "stm32f10x_gpio.h"
#include <define.h>
#include "MyIIC.h"
#include "delay.h"
//#include "MKL_port.h"
//#include "MKL_gpio.h"

//uint8 buff[2] = {0};            //one byte buffer for receiving data from slave
int test_data[5] = {0};
//��ʼ��IIC�˿�
void IIC_Port_Init(void)
{
  gpio_init(MSCL_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_init(MSDA_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
}
//����IIC��ʼ�ź�
void IIC_Start(void)    //START:when CLK is high,DATA change form high to low
{
  MSDA_MOD = LJL_OOD;     //sda�����
  MSDA_OUT=1;
  MSCL_OUT=1;
  MSDA_OUT=0;
}
//����IICֹͣ�ź�
void IIC_Stop(void) //STOP:when CLK is high DATA change form low to high
{
  MSDA_MOD = LJL_OOD;     //sda�����
  MSDA_OUT=0;
  MSCL_OUT=1;
  MSDA_OUT=1;//����I2C���߽����ź�
}
/***********�ȴ�Ӧ���źŵ���;����ֵ��1������Ӧ��ʧ�� 0������Ӧ��ɹ�********/
BOOL IIC_Wait_Ack(void)         //IN;SCL=1-0
{
  MSDA_MOD = LJL_IFLT;      //SDA����Ϊ����  
  MSCL_OUT = 1;
  while(MSDA_IN)
    DelayUs(2);
  MSCL_OUT=0;             //ʱ�����0 
  
  return true;
}
/*******************����ACKӦ��************/
void IIC_Ack(void)      //SCL=0,OUT,SCL=1-0
{
  MSCL_OUT=0;
  MSDA_MOD = LJL_OOD;     //sda�����
  MSCL_OUT=1;
  MSCL_OUT=0;
}
/**************������ACKӦ��***************/
void IIC_NAck(void)     //SCL=0,OUT,SDA=1,SCL=1-0
{
  MSCL_OUT=0;
  MSDA_MOD = LJL_OOD;     //sda�����
  MSDA_OUT=1;
  MSCL_OUT=1;
  MSCL_OUT=0;
}

/****IIC����һ���ֽ�;���شӻ�����Ӧ��:1����Ӧ��;0����Ӧ��**********/
//���ԣ�
//(txd&0x80)!=0��360ns
//(txd&0x80)>0:  460ns
//if((txd&0x80)>>7)
//MSDA_OUT=1;
//else
//MSDA_OUT=0;��  450ns
//if((txd&0x80)==0x80)         //1
//MSDA_OUT=1;
//else
//MSDA_OUT=0;    390ns
//forչ������
//static uint8 t;slower
void IIC_Send_Byte(uint8 txd)           //OUT SCL=0 DADA SCL = 0
{
  uint8 t;
  MSDA_MOD = LJL_OOD;     //sda�����
  MSCL_OUT=0;             //ʱ�����0 
  for(t=0;t<8;t++)
  {
#if 0
    if((txd&0x80)!=0)         //1
      MSDA_OUT=1;
    else
      MSDA_OUT=0;
#else
    MSDA_OUT = ((txd&0x80)!=0);
#endif
    txd<<=1;
    MSCL_OUT=1;
    MSCL_OUT=0;
  }
}
//��1���ֽڣ�����һ���ֽں����masterϣ���ٶ�һ���ֽڣ�����ACK�����ٶ�ȡ������nACK����ʾslave׼������stop�ź�
uint8 IIC_Read_Byte(uint8 ack)          //IN,SCL=010...OUT,SDA~,SCL=0
{
  uint8 i,receive=0;
  for(i=0;i<8;i++ )
  {
    MSCL_OUT=0;
    MSCL_OUT=1;
    receive<<=1;
    if(MSDA_IN)
      receive++;     //if(gpio_get(PTE25))receive++;   
  }
  if (!ack)
    IIC_NAck();             //����nACK,//SCL=0,OUT,SDA=1,SCL=1-0
  else
    IIC_Ack();              //����ACK,//SCL=0,OUT,SCL=1-0
  
  return receive;
}
/**************���ֽ�д��
inner function***********/
void Single_Write(uint8 slave_addr, uint8 reg_addr, uint8 data)		     //void
{
  IIC_Start();                  //OUT SDA=1,    SCL=1         SDA=0,  
  IIC_Send_Byte(slave_addr);    //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();               //IN;           SCL=1                   SCL=0
  IIC_Send_Byte(reg_addr);      //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();               //IN;           SCL=1                   SCL=0
  IIC_Send_Byte(data);          //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();               //IN;           SCL=1-                  SCL=0
  IIC_Stop();                   //OUT, SDA=0,   SCL=0��SDA=1,
}

/********************���ֽڶ�ȡ
inner function******************/
void Single_Read(uint8 slave_addr,uint8 reg_addr, uint8* pdata)
{
  IIC_Start();                   //OUT SDA=1,    SCL=1         SDA=0,    SCL=0
  IIC_Send_Byte(slave_addr);     //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();                //IN;           SCL=1                   SCL=0
  IIC_Send_Byte((uint8)reg_addr);//OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();                //IN;           SCL=1                   SCL=0
  IIC_Start();                   //OUT SDA=1,    SCL=1         SDA=0,    SCL=0
  IIC_Send_Byte(slave_addr+1);   //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();                //IN;           SCL=1                   SCL=0
  *pdata = IIC_Read_Byte(0);     //IN,           SCL=010...OUT,SDA~,     SCL=0
  IIC_Stop();                    //OUT, SDA=0,   SCL=0��SDA=1
}						      

void CompassInit(void)
{
  uint8 i = 0;
  const uint8 param[][2] = 
  {
    {0x09, 0x0D}, 
    {0x0B, 0x01},
    {0x20, 0x40}, 
    {0x21, 0x01},
  };
  for(i=0; i<4; ++i)
    Single_Write(COMPASS_ADDR, param[i][0], param[i][1]);
}
//void ReadCompass()
//{
//uint8 compassbuff[2] = {0};            //one byte buffer for receiving data from slave
//  Single_Read(COMPASS_ADDR, Data_Output_X_LSB_Reg_Of_Compass, &compassbuff[0]);
//  Single_Read(COMPASS_ADDR, Data_Output_X_MSB_Reg_Of_Compass, &compassbuff[1]);
//  Compass.x = ((compassbuff[1]<<8)|compassbuff[0])-42;//Compass.x_off;
//  Single_Read(COMPASS_ADDR, Data_Output_Y_LSB_Reg_Of_Compass, &compassbuff[0]);
//  Single_Read(COMPASS_ADDR, Data_Output_Y_MSB_Reg_Of_Compass, &compassbuff[1]);
//  Compass.y = ((compassbuff[1]<<8)|compassbuff[0])-4567;//Compass.y_off;
//  Single_Read(COMPASS_ADDR, Data_Output_Z_LSB_Reg_Of_Compass, &compassbuff[0]);
//  Single_Read(COMPASS_ADDR, Data_Output_Z_MSB_Reg_Of_Compass, &compassbuff[1]);
//  Compass.z = ((compassbuff[1]<<8)|compassbuff[0])-6676;//Compass.z_off;
//}
unsigned short twobmp085_read(u8 bmp085_RCOMMAND)
{
  uint8 buff[2] = {0};
  Single_Read(BMP085_SlaveAddress,bmp085_RCOMMAND,buff+1);
  Single_Read(BMP085_SlaveAddress,bmp085_RCOMMAND+1,buff);
//  if(0x80==buff[1])
//    buff[1] = buff[1];
  return ((buff[1]<<8)|buff[0]);
}

short bmp085ReadTemp(void)
{
  uint8 buff[2] = {0};
  Single_Write(BMP085_SlaveAddress,0xF4,0x2E);
  
  DelayMs(10);	// max time is 4.5ms
  
  Single_Read(BMP085_SlaveAddress,0xF6,buff+1);
  Single_Read(BMP085_SlaveAddress,0xF6+1,buff);
    
  return  ((buff[1]<<8)|buff[0]);
}
short bmp085ReadPressure(void)
{
  //  short _pressure = 0;
  
  Single_Write(BMP085_SlaveAddress,0xF4,0x34); //0x34 
  DelayMs(3);    	                  // max time is 4.5ms
  
  //  pressure = twobmp085_read(0xF6);
  
  return twobmp085_read(0xF6);	
  
}

