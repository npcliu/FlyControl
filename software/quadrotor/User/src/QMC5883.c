#include "stdio.h"
#include "stm32f10x_gpio.h"
#include "MyIIC.h"
#include "QMC5883.h"

void QMC5883Init(void)
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

void ReadQMC5883(short *pcompass)
{
uint8 compassbuff[2] = {0};            //one byte buffer for receiving data from slave
  compassbuff[0] = Single_Read(COMPASS_ADDR, Data_Output_X_LSB_Reg_Of_Compass);
  compassbuff[1] = Single_Read(COMPASS_ADDR, Data_Output_X_MSB_Reg_Of_Compass);
  pcompass[0] = ((compassbuff[1]<<8)|compassbuff[0]);
  compassbuff[0] = Single_Read(COMPASS_ADDR, Data_Output_Y_LSB_Reg_Of_Compass);
  compassbuff[1] = Single_Read(COMPASS_ADDR, Data_Output_Y_MSB_Reg_Of_Compass);
  pcompass[1] = ((compassbuff[1]<<8)|compassbuff[0]);
  compassbuff[0] = Single_Read(COMPASS_ADDR, Data_Output_Z_LSB_Reg_Of_Compass);
  compassbuff[1] = Single_Read(COMPASS_ADDR, Data_Output_Z_MSB_Reg_Of_Compass);
  pcompass[2] = ((compassbuff[1]<<8)|compassbuff[0]);
}

















