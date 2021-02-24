#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "L3GD20.h"
#include "stm32f10x_MY_spi.h"
#include "delay.h"

int l3_data[3];

uint8 L3GD20WriteReg(uint8 reg, uint8 dat)
{
  uint8 buff[2];
  
  buff[0] = reg;          //先发送寄存器
  buff[1] = dat;          //再发送数据
  
  GYRO_CS_OUT = 0;
  spi_mosi(L3GD20_SPI, buff, buff, 2); //发送buff里数据，并采集到 buff里
  GYRO_CS_OUT = 1;
  
  /*返回状态寄存器的值*/
  return buff[0];
}
uint8 L3GD20ReadReg(uint8 reg)
{
  uint8 buff[2];
  
  buff[0] = reg|0x80;          //先发送寄存器
  buff[1] = 0;          //先发送寄存器
  GYRO_CS_OUT = 0;
  //    PB(B,15,CRL) = 1;           //设置SCK引脚为复用
  spi_mosi(L3GD20_SPI, buff, buff, 2); //发送buff数据，并保存到buff里
  GYRO_CS_OUT = 1;
  //  *dat = buff[1];                         //提取第二个数据
  /*返回状态寄存器的值*/
  return buff[1];
}

void L3GD20_IO_Init()
{
  gpio_init(GYRO_INT_PIN, IN_FLOATING, interupt,GPIO_Speed_50MHz,0);                              //初始化IRQ管脚为输入
  gpio_int_cfg(GYRO_INT_PIN,GYRO_IRQ_Line,EXTI_Trigger_Falling);
}
//brief：L3GD20初始化
//parameters:L3gd20_full_scale:full scale of the data output
//return：0：OK，1：Init failed
//  example:  L3GD20_Init(L3GD20_FULLSCALE_250);
char L3GD20_Init(uint8 L3gd20_full_scale)
{
  uint8_t tmp;
  
  L3GD20_IO_Init();
  tmp=L3GD20ReadReg(L3GD20_ADDR_ID);
  if(tmp==L3GD20_ID)
  {
    L3GD20WriteReg(L3GD20_CTRL_REG1_ADDR,L3GD20_PWR_ON);
    L3GD20WriteReg(L3GD20_CTRL_REG4_ADDR,L3gd20_full_scale);
    L3GD20WriteReg(L3GD20_CTRL_REG2_ADDR,L3GD20_FILTER);
    L3GD20WriteReg(L3GD20_CTRL_REG5_ADDR,L3GD20_FILTER_ON);
    
    return 0;
  }
  else
    return 1;
}
//brief:        read gyro data from L3GD20 and put it into pgyro buffer
//parameters:   pgyro:buffer addr of output data,for example:pgyro[0] is gyro_x 
//              data read from chip minused offset value which is stored at pgyro[3]
//example:    L3GD20_Read((short*)&L3GDgyro_adc);
//      L3GDgyro_adc stored structure:
//      typedef struct
//      {
//      short x, y, z;
//      short x_offset, y_offset, z_offset;
//      }GYRO,*PGYRO;
void L3GD20_Read(short *pgyro)
{
  pgyro[0]=(int16_t)((L3GD20ReadReg(L3GD20_ADR_X_HI)<<8)|
                     L3GD20ReadReg(L3GD20_ADR_X_LO)) - pgyro[3];
  pgyro[1]=(int16_t)((L3GD20ReadReg(L3GD20_ADR_Y_HI)<<8)|
                     L3GD20ReadReg(L3GD20_ADR_Y_LO)) - pgyro[4];
  pgyro[2]=(int16_t)((L3GD20ReadReg(L3GD20_ADR_Z_HI)<<8)|
                     L3GD20ReadReg(L3GD20_ADR_Z_LO)) - pgyro[5];
}

//breif:          L3GD20 GyroScope offset Calibration
//parameters:     PGYRO, output (offset) data buffer address,PGYRO[0] will be x_offset value stored place
//return:         0:success,1:calibration failed
//example:          L3GD20GyroCalibration((short *)(&L3GDgyro_adc)+3);
int l3gd_gyro_cali_tolerance = 60;
char L3GD20GyroCalibration(short *PGYRO)
{
  short _L3GDgyro_adc[6] = {0};         //L3GD20_Read will read _L3GDgyro_adc[3],[4],[5]
  int i = 0, j = 0,k = 0;
  float gyro_cali_sum[3] = {0};
  char recali_flag = 0;         //need recalibrate flag
  short gyro_min[3] = {0};
  short gyro_max[3] = {0};
  
//  for(k=0;k<3;k++)
//    PGYRO[k] = 0;                       //clear to zero,then read data will be RAW data
  L3GD20_Read(_L3GDgyro_adc);
  for(k=0;k<3;k++)
  {
    gyro_min[k] = _L3GDgyro_adc[k];
    gyro_max[k] = _L3GDgyro_adc[k];
  }
  for(i=0;i<3;i++)      //i,j 分时段采集
  {
    for(j=0;j<100;j++)
    {
      DelayMs(5);
      L3GD20_Read(_L3GDgyro_adc);
      for(k=0;k<3;k++)
      {
        gyro_cali_sum[k] += _L3GDgyro_adc[k];              //sum of the 3 axis' raw data 
        if(gyro_min[k]>_L3GDgyro_adc[k])
          gyro_min[k] = _L3GDgyro_adc[k];
        if(gyro_max[k]<_L3GDgyro_adc[k])
          gyro_max[k] = _L3GDgyro_adc[k];
        if(gyro_max[k] - gyro_min[k] > l3gd_gyro_cali_tolerance)        //must be turnning,should be stable
          recali_flag = 1;
      }
      if(recali_flag)
      {
        recali_flag = 0;
        i = -1;
        L3GD20_Read(_L3GDgyro_adc);
        for(k=0;k<3;k++)
        {
          gyro_min[k] = _L3GDgyro_adc[k];
          gyro_max[k] = _L3GDgyro_adc[k];
          gyro_cali_sum[k] = 0;
        }
        break;
      }
    }
    DelayMs(100);
  }
  PGYRO[0] = (short)gyro_cali_sum[0] / 300;
  PGYRO[1] = (short)gyro_cali_sum[1] / 300;
  PGYRO[2] = (short)gyro_cali_sum[2] / 300;
  return 0;
}
