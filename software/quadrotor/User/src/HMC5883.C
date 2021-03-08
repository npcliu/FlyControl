#include "HMC5883.h"
#include "stdio.h"
#include "stm32f10x_gpio.h"
#include "MyIIC.h"

void HMC5883Init(void)
{
//  uint8 i = 0;
//  const uint8 param[][2] = 
//  {
//    {CONFIG_REG_A, 0x78}, 
//    {CONFIG_REG_B, 0x00},
//    {0x02, 0x00},       //continuous mode
////    {0x21, 0x01},
//  };
//  for(i=0; i<4; ++i)
    Single_Write(HMC5883_ADDR, CONFIG_REG_A, 0xF8);
    Single_Write(HMC5883_ADDR, CONFIG_REG_B, 0x00);
    Single_Write(HMC5883_ADDR, MODE_REG, 0x00);
}

void ReadHMC5883(short *pcompass)
{
  uint8 compassbuff[2] = {0};            //one byte buffer for receiving data from slave
  compassbuff[1] = Single_Read(HMC5883_ADDR, Data_Output_X_MSB_Reg_Of_HMC5883);
  compassbuff[0] = Single_Read(HMC5883_ADDR, Data_Output_X_LSB_Reg_Of_HMC5883);
//  Single_Read(HMC5883_ADDR, STATUS_REG, compassbuff);
  pcompass[0] = ((compassbuff[1]<<8)|compassbuff[0]);//HMC5883.x_off;
  compassbuff[1] = Single_Read(HMC5883_ADDR, Data_Output_Z_MSB_Reg_Of_HMC5883);
  compassbuff[0] = Single_Read(HMC5883_ADDR, Data_Output_Z_LSB_Reg_Of_HMC5883);
  pcompass[2] = ((compassbuff[1]<<8)|compassbuff[0]);//Compass.z_off;
  compassbuff[1] = Single_Read(HMC5883_ADDR, Data_Output_Y_MSB_Reg_Of_HMC5883);
  compassbuff[0] = Single_Read(HMC5883_ADDR, Data_Output_Y_LSB_Reg_Of_HMC5883);
  pcompass[1] = ((compassbuff[1]<<8)|compassbuff[0]);//HMC5883.y_off;
}

















