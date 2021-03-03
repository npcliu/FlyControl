#include "stm32f10x.h"
#include "define.h"
#include "delay.h"
#include "math.h"
#include "MyIIC.h"              //某些引脚操作
#include "Mpu6050.h"
#include "L3GD20.h"
#include "QMC5883.h"
#include "calculation.h"

#define ACC_CALI_COUNT  1000            //acc calibration data sum time

extern char AccCalibration(PACC pacc,PGYRO pgyro);

uint32 FlashWriteAddr = 0x0803F800;
//Brief:save important parameters to Nonvolatile memory,then you can get these parameters by reading flash when initialization;
//  because of the properties of the flash,you must rewrite all of the data in that page,evenif just one parameters has changed
//Parameters:
//  ;
//Return:void;
//Example:;  
void SaveToFlash()
{
  FLASH_Unlock();
  FLASH_ErasePage(STM32F103RC_FLASH_PAGE127_ADDR);
  
  FlashWriteAddr = STM32F103RC_FLASH_PAGE127_ADDR;
  FLASH_ProgramHalfWord((uint32_t)FlashWriteAddr + GYRO_XOFF_ADDR_OFFSET, gyro.x_offset);//(uint16_t)gyro[3]
  FLASH_ProgramHalfWord((uint32_t)FlashWriteAddr + GYRO_YOFF_ADDR_OFFSET, gyro.y_offset);//(uint16_t)gyro[3]
  FLASH_ProgramHalfWord((uint32_t)FlashWriteAddr + GYRO_ZOFF_ADDR_OFFSET, gyro.z_offset);//(uint16_t)gyro[3]
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + ACC_KX_ADDR_OFFSET, *(uint32*)&acc.kx);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + ACC_KY_ADDR_OFFSET, *(uint32*)&acc.ky);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + ACC_KZ_ADDR_OFFSET, *(uint32*)&acc.kz);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + ACC_XOFF_ADDR_OFFSET, *(uint32*)&acc.x_off);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + ACC_YOFF_ADDR_OFFSET, *(uint32*)&acc.y_off);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + ACC_ZOFF_ADDR_OFFSET, *(uint32*)&acc.z_off);//(uint32_t)
  
  FLASH_ProgramHalfWord((uint32_t)FlashWriteAddr + COMPASS_XOFF_ADDR_OFFSET, compass.x_offset);//(uint16_t)gyro[3]
  FLASH_ProgramHalfWord((uint32_t)FlashWriteAddr + COMPASS_YOFF_ADDR_OFFSET, compass.y_offset);//(uint16_t)gyro[3]
  FLASH_ProgramHalfWord((uint32_t)FlashWriteAddr + COMPASS_ZOFF_ADDR_OFFSET, compass.z_offset);//(uint16_t)gyro[3]
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + COMPASS_KX_ADDR_OFFSET, *(uint32*)&compass.kx);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + COMPASS_KY_ADDR_OFFSET, *(uint32*)&compass.ky);//(uint32_t)
  FLASH_ProgramWord((uint32_t)FlashWriteAddr + COMPASS_KZ_ADDR_OFFSET, *(uint32*)&compass.kz);//(uint32_t)
  FLASH_Lock();
}

//int acc_mode_tollerant = 300;           //
float _amtp = 0.14;             //当加速度('a'cc)模('m'old)在1g附近浮动('t'olerance)时允许数据采集，否则不准；该值即浮动百分比('p'ercentage)
float xyz_t = 0.18;             //当加速度计单个轴值在1g*(1+-xyz_t)附近浮动时，开始校准该轴
int acc_x_tollerant = 400;
int acc_y_tollerant = 400;
int acc_z_tollerant = 400;
//不要超过3.5dps
float _gt = 3.5;                //当角速度('g'yro)不超过('t'olerance)3.5dps时，仍为保持稳定了，才允许数据采集，否则不准
int gyro_x_tollerant = 60;
int gyro_y_tollerant = 60;
int gyro_z_tollerant = 60;
//Brief:accelerometer calibration function
//Parameters:
//  pacc:;
//  pgyro:;
//Return:void;
//Example:;
char AccCalibration(PACC pacc,PGYRO pgyro)
{
  float acc_min[3] = {0};               //加速度计3个轴的输出最小值
  float acc_max[3] = {0};               //
  char end_max_of_axis[3] = {1,1,1};    //是否获得最大值的标志
  char end_min_of_axis[3] = {1,1,1};
  ACC _acc_adc  = {0};
  GYRO _gyro_adc = {0};
  uint32 _acc_mode = 0;
  float sum = 0;
  uint32 count = 0;
  char axis_to_cali_flag = 0;        //正在校准的轴：0=x轴，1=y轴，2=z轴
  char step = 20;
  float _1g_value = 1.0/MPU6050ACC_SCALE_G;     //this value is set according to the acc fullscale;example:if fullscale is +-16g,raw data is signed 16bit(32767);then acc should output 2048 when feeded with 1g
  int a_t_v = (int)(_1g_value*(1-xyz_t));           //acceleration tolerant value此数据代表允许的加速度映射到原始数据上以做比较之用，假如1g=2048，该值一般在1680左右
  int r_t_v = (int)(_gt/MPU6050GYRO_SCALE_DEG);        //rotation tolerant value此数据代表允许的转动角速度映射到原始数据上以做比较之用
  MPU_INT_EN(0);                                //关闭MPU6050中断，不然MPU的data ready 中断会引发读冲突
  while(1)
  {
    MPUReadAcc((short*)&_acc_adc);
    MPUReadGyr((short*)&_gyro_adc);  //chip output data
    _acc_mode = (uint32)sqrt(_acc_adc.x*_acc_adc.x + _acc_adc.y*_acc_adc.y + _acc_adc.z*_acc_adc.z);
//if stable?
    if(_acc_mode>_1g_value*(1+_amtp) || _acc_mode<_1g_value*(1-_amtp) || _gyro_adc.x>r_t_v || _gyro_adc.x<-r_t_v || _gyro_adc.y>r_t_v || _gyro_adc.y<-r_t_v || _gyro_adc.z>r_t_v || _gyro_adc.z<-r_t_v)  //ensure no acceleration
    {
      RGB_R_OUT = 0;      RGB_G_OUT = 0;      RGB_B_OUT = 0;
      DelayMs(100);
      RGB_R_OUT = 1;      RGB_G_OUT = 1;      RGB_B_OUT = 1;
      sum = 0;
      count = 0;
      continue;                         //not stable,then continue
    }
    if(_acc_adc.x>a_t_v && end_max_of_axis[0])            //通过stable检测，x轴未校准？&&x值在范围内？
    {
      RGB_R_OUT = 0;      RGB_G_OUT = 1;      RGB_B_OUT = 1;
      sum += _acc_adc.x;
      count++;
      axis_to_cali_flag = 0;
      step = 0;
    }
    else if(0==step)
    {
      RGB_R_OUT = 1;
      count = 0;
      sum = 0;
      step = 20;
    }else;
    if(_acc_adc.x<-a_t_v && end_min_of_axis[0])
    {
      RGB_R_OUT = 0;      RGB_G_OUT = 1;      RGB_B_OUT = 1;
      sum += _acc_adc.x;
      count++;
      axis_to_cali_flag = 0;
      step = 1;
    }
    else if(1==step)
    {
      RGB_R_OUT = 1;
      count = 0;
      sum = 0;
      step = 20;
    }else;
    if(_acc_adc.y>a_t_v && end_max_of_axis[1])
    {
      RGB_R_OUT = 1;      RGB_G_OUT = 0;      RGB_B_OUT = 1;
      sum += _acc_adc.y;
      count++;
      axis_to_cali_flag = 1;
      step = 2;
    }
    else if(2==step)
    {
      RGB_G_OUT = 1;
      count = 0;
      sum = 0;
      step = 20;
    }
    if(_acc_adc.y<-a_t_v && end_min_of_axis[1])
    {
      RGB_R_OUT = 1;      RGB_G_OUT = 0;      RGB_B_OUT = 1;
      sum += _acc_adc.y;
      count++;
      axis_to_cali_flag = 1;
      step = 3;
    }
    else if(3==step)
    {
      RGB_G_OUT = 1;
      count = 0;
      sum = 0;
      step = 20;
    }
    if(_acc_adc.z>a_t_v && end_max_of_axis[2])
    {
      RGB_R_OUT = 1;      RGB_G_OUT = 1;      RGB_B_OUT = 0;
      sum += _acc_adc.z;
      count++;
      axis_to_cali_flag = 2;
      step = 4;
    }
    else if(4==step)
    {
      RGB_B_OUT = 1;
      count = 0;
      sum = 0;
      step = 20;
    }
    if(_acc_adc.z<-a_t_v && end_min_of_axis[2])
    {
      RGB_R_OUT = 1;      RGB_G_OUT = 1;      RGB_B_OUT = 0;
      sum += _acc_adc.z;
      count++;
      axis_to_cali_flag = 2;
      step = 5;
    }
    else if(5==step)
    {
      RGB_B_OUT = 1;
      count = 0;
      sum = 0;
      step = 20;
    }
    DelayMs(1);
    if(count>ACC_CALI_COUNT)
    {
      if(sum<0)
      {
        acc_min[axis_to_cali_flag] = sum/ACC_CALI_COUNT;
        end_min_of_axis[axis_to_cali_flag] = 0;
        count = 0;
      }
      else
      {
        acc_max[axis_to_cali_flag] = sum/ACC_CALI_COUNT;
        end_max_of_axis[axis_to_cali_flag] = 0;
        count = 0;
      }
    }
    if(0==(end_min_of_axis[0]+end_min_of_axis[1]+end_min_of_axis[2] + end_max_of_axis[0]+end_max_of_axis[1]+end_max_of_axis[2]))
      break;
  }
  MPU_INT_EN(1);                //允许MPU中断
  pacc->x_off = (acc_max[0]*(-_1g_value) - acc_min[0]*(_1g_value))/(acc_max[0] - acc_min[0]);
  pacc->y_off = (acc_max[1]*(-_1g_value) - acc_min[1]*(_1g_value))/(acc_max[1] - acc_min[1]);
  pacc->z_off = (acc_max[2]*(-_1g_value) - acc_min[2]*(_1g_value))/(acc_max[2] - acc_min[2]);
  pacc->kx = (2*_1g_value)/(acc_max[0]-acc_min[0]);
  pacc->ky = (2*_1g_value)/(acc_max[1]-acc_min[1]);
  pacc->kz = (2*_1g_value)/(acc_max[2]-acc_min[2]);
  
  SaveToFlash();        
  RGB_R_OUT = 1;
  RGB_G_OUT = 1;
  RGB_B_OUT = 1;
  return 0;             //0 means there is no problems left behind
}

char GyroCalibration(void)
{
  MPU6050GyroCalibration((short *)(&gyro)+3);
  L3GD20GyroCalibration((short *)(&L3GDgyro_adc)+3);
  
  SaveToFlash();
  return 0;
}

short x_min = 0,x_max = 0,y_min = 0,y_max = 0,z_min = 0,z_max = 0;//-4857,4772,-8420,230,-11900,-510
void CompassCalibration(int ristrict)
{
  uint32 i = 0;
  BUZZER_OUT = 0; 
  MPU_INT_EN(0);
  
  //z
  do{
    MPUReadAcc((short*)&acc);
//    AccAngleCal(&acc,angle,&compass);                       //角度在范围内就开始校准
    if(angle[0]>-2 && angle[0]<2 && angle[1]>-2 && angle[1]<2)
    {
      i++;
      if(i<15)
        BUZZER_TRN;
      else
        BUZZER_OUT = 1;  
      DelayMs(100);
    }
    else
    {
      i = 0;
      BUZZER_OUT = 0;  
    }
  }while(i<18);
  BUZZER_OUT = 0;
  
  ReadQMC5883((short*)&compass);
//  compass.x += compass.x_offset;        //读出来得是修正数据，原始数据需要补偿得到
//  compass.y += compass.y_offset;
//  compass.z += compass.z_offset;
  short x_start = compass.x;
  short y_start = compass.y;
  x_min = compass.x;
  x_max = compass.x;
  y_min = compass.y;
  y_max = compass.y;
  while(1)
  {
    ReadQMC5883((short*)&compass);
//    compass.x += compass.x_offset;        //读出来得是修正数据，原始数据需要补偿得到
//    compass.y += compass.y_offset;
//    compass.z += compass.z_offset;
    if(x_min>compass.x)
    {
      x_min = compass.x;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(x_max<compass.x)
    {
      x_max = compass.x;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(y_min>compass.y)
    {
      y_min = compass.y;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(y_max<compass.y)
    {
      y_max = compass.y;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(x_max-x_min>8000 && y_max-y_min>8000 && compass.x>x_start-ristrict && compass.x<x_start+ristrict && compass.y>y_start-ristrict && compass.y<y_start+ristrict)
    {
      BUZZER_OUT = 1;
      DelayMs(500);
      BUZZER_OUT = 0;
      break;
    }
  }
  
  DelayMs(900);
  //x/y
  i = 0;
  do{
    MPUReadAcc((short*)&acc);
    ReadQMC5883((short*)&compass);
//    compass.x += compass.x_offset;        //读出来得是修正数据，原始数据需要补偿得到
//    compass.y += compass.y_offset;
//    compass.z += compass.z_offset;
//    AccAngleCal(&acc,angle,&compass);                       //角度在范围内就开始校准
    if((angle[1]>-92 && angle[1]<-88) || (angle[1]>88 && angle[1]<92))
    {
      i++;
      if(i<15)
        BUZZER_TRN;
      else
        BUZZER_OUT = 1;  
      DelayMs(100);
    }
    else
    {
      i = 0;
      BUZZER_OUT = 0;  
    }
  }while(i<18);
  BUZZER_OUT = 0;
  
  short z_start = compass.z;
  z_min = compass.z;
  z_max = compass.z;
  while(1)
  {
    ReadQMC5883((short*)&compass);
//    compass.x += compass.x_offset;        //读出来得是修正数据，原始数据需要补偿得到
//    compass.y += compass.y_offset;
//    compass.z += compass.z_offset;
    if(z_min>compass.z)
    {
      z_min = compass.z;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(z_max<compass.z)
    {
      z_max = compass.z;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(z_max-z_min>7500 && compass.z>z_start-ristrict && compass.z<z_start+ristrict)
    {
      BUZZER_OUT = 1;
      DelayMs(500);
      BUZZER_OUT = 0;
      break;
    }
  }
  compass.x_offset = (short)(-4096.0*(x_max + x_min)/(x_max - x_min));
  compass.y_offset = (y_min+y_max)/2;
  compass.z_offset = (z_min+z_max)/2;
  compass.kx = (4096.0)/(x_max-x_min);
  compass.ky = (4096.0)/(y_max-y_min);
  compass.kz = (4096.0)/(z_max-z_min);
  MPU_INT_EN(1);
  SaveToFlash();
}