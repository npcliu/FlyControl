#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "MyIIC.h"
#include "delay.h"

//uint8 buff[2] = {0};            //one byte buffer for receiving data from slave
int test_data[5] = {0};
//初始化IIC端口
void IIC_Port_Init(void)
{
  gpio_init(MSCL_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_init(MSDA_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
}
//产生IIC起始信号
void IIC_Start(void)    //START:when CLK is high,DATA change form high to low
{
  MSDA_MOD = LJL_OOD;     //sda线输出
  MSDA_OUT=1;
  MSCL_OUT=1;
  MSDA_OUT=0;
}
//产生IIC停止信号
void IIC_Stop(void) //STOP:when CLK is high DATA change form low to high
{
  MSDA_MOD = LJL_OOD;     //sda线输出
  MSDA_OUT=0;
  MSCL_OUT=1;
  MSDA_OUT=1;//发送I2C总线结束信号
}
/***********等待应答信号到来;返回值：1，接收应答失败 0，接收应答成功********/
static BOOL IIC_Wait_Ack(void)         //IN;SCL=1-0
{
  static uint32 count = 0;
  
  MSDA_MOD = LJL_IFLT;      //SDA设置为输入  
  MSCL_OUT = 1;
  while(MSDA_IN)
  {
    DelayUs(1);
    if(count<800)
    count++;
    else
    {
      count = 0;
      return false;
    }
  }
  MSCL_OUT=0;             //时钟输出0 
  
  return true;
}
/*******************产生ACK应答************/
void IIC_Ack(void)      //SCL=0,OUT,SCL=1-0
{
  MSCL_OUT=0;
  MSDA_MOD = LJL_OOD;     //sda线输出
  MSCL_OUT=1;
  MSCL_OUT=0;
}
/**************不产生ACK应答***************/
void IIC_NAck(void)     //SCL=0,OUT,SDA=1,SCL=1-0
{
  MSCL_OUT=0;
  MSDA_MOD = LJL_OOD;     //sda线输出
  MSDA_OUT=1;
  MSCL_OUT=1;
  MSCL_OUT=0;
}

/****IIC发送一个字节;返回从机有无应答:1，有应答;0，无应答**********/
//测试：
//(txd&0x80)!=0：360ns
//(txd&0x80)>0:  460ns
//if((txd&0x80)>>7)
//MSDA_OUT=1;
//else
//MSDA_OUT=0;：  450ns
//if((txd&0x80)==0x80)         //1
//MSDA_OUT=1;
//else
//MSDA_OUT=0;    390ns
//for展开很慢
//static uint8 t;slower
void IIC_Send_Byte(uint8 txd)           //OUT SCL=0 DADA SCL = 0
{
  uint8 t;
  MSDA_MOD = LJL_OOD;     //sda线输出
  MSCL_OUT=0;             //时钟输出0 
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
//    DelayUs(1);
    MSCL_OUT=1;
//    DelayUs(1);
    MSCL_OUT=0;
  }
}
//读1个字节，读完一个字节后如果master希望再读一个字节，发送ACK；不再读取，发送nACK以提示slave准备接受stop信号
uint8 IIC_Read_Byte(uint8 ack)          //IN,SCL=010...OUT,SDA~,SCL=0
{
  uint8 i,receive=0;
  for(i=0;i<8;i++)
  {
    MSCL_OUT=0;
    DelayUs(1);
    MSCL_OUT=1;
    DelayUs(1);
    receive<<=1;
    if(MSDA_IN)
      receive++;     //if(gpio_get(PTE25))receive++;   
    DelayUs(1);
  }
  if (!ack)
    IIC_NAck();             //发送nACK,//SCL=0,OUT,SDA=1,SCL=1-0
  else
    IIC_Ack();              //发送ACK,//SCL=0,OUT,SCL=1-0
  
  return receive;
}
/**************单字节写入
inner function***********/
BOOL Single_Write(uint8 slave_addr, uint8 reg_addr, uint8 data)		     //void
{
  IIC_Start();                  //OUT SDA=1,    SCL=1         SDA=0,  
  IIC_Send_Byte(slave_addr);    //OUT           SCL=0 DADA              SCL=0
  if(false==IIC_Wait_Ack())
    return false;               //IN;           SCL=1                   SCL=0
  IIC_Send_Byte(reg_addr);      //OUT           SCL=0 DADA              SCL=0
  if(false==IIC_Wait_Ack())
    return false;               //IN;           SCL=1                   SCL=0
  IIC_Send_Byte(data);          //OUT           SCL=0 DADA              SCL=0
  if(false==IIC_Wait_Ack())
    return false;               //IN;           SCL=1                   SCL=0
  IIC_Stop();                   //OUT, SDA=0,   SCL=0，SDA=1,
  return true;
}

/********************单字节读取
inner function******************/
uint8 Single_Read(uint8 slave_addr,uint8 reg_addr)
{
  uint8 recieve_data = 0;
  IIC_Start();                   //OUT SDA=1,    SCL=1         SDA=0,    SCL=0
  IIC_Send_Byte(slave_addr);     //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();                //IN;           SCL=1                   SCL=0
  IIC_Send_Byte((uint8)reg_addr);//OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();                //IN;           SCL=1                   SCL=0
  IIC_Start();                   //OUT SDA=1,    SCL=1         SDA=0,    SCL=0
  IIC_Send_Byte(slave_addr+1);   //OUT           SCL=0 DADA              SCL=0
  IIC_Wait_Ack();                //IN;           SCL=1                   SCL=0
  recieve_data = IIC_Read_Byte(0);     //IN,           SCL=010...OUT,SDA~,     SCL=0
  IIC_Stop();                    //OUT, SDA=0,   SCL=0，SDA=1
  
  return recieve_data;
}						      

unsigned short twobmp085_read(u8 bmp085_RCOMMAND)
{
  uint8 buff[2] = {0};
  buff[1] = Single_Read(BMP085_SlaveAddress,bmp085_RCOMMAND);
  buff[0] = Single_Read(BMP085_SlaveAddress,bmp085_RCOMMAND+1);
//  if(0x80==buff[1])
//    buff[1] = buff[1];
  return ((buff[1]<<8)|buff[0]);
}

short bmp085ReadTemp(void)
{
  uint8 buff[2] = {0};
  Single_Write(BMP085_SlaveAddress,0xF4,0x2E);
  
  DelayMs(10);	// max time is 4.5ms
  
  buff[1] = Single_Read(BMP085_SlaveAddress,0xF6);
  buff[0] = Single_Read(BMP085_SlaveAddress,0xF6+1);
    
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

