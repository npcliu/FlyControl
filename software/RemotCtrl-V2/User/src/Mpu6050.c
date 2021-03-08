#include "stm32f10x_gpio.h"
#include "MyIIC.h"
#include "Mpu6050.h"
#include "define.h"
#include "delay.h"

void MPUInit(void)
{
  gpio_init(MPUINT_PIN, IN_FLOATING, interupt,GPIO_Speed_50MHz,0);                              //初始化IRQ管脚为输入
  gpio_int_cfg(MPUINT_PIN,EXTI_Line8,EXTI_Trigger_Rising);
  
  const uint8 param[][2] = 
  {
    {PWR_MGMT_1,        1     }, // 电源选项
    {SMPLRT_DIV,        4     }, // 陀螺仪采样率????4??,1k/4,????25Hz?
    {MPU6050_CONFIG,    5     }, // 低通滤波频率，寄存器手册P23
    {GYRO_CONFIG,       3<<3  }, // 陀螺仪量程2000dps  16.4-1°/s    角速度倍率寄存器
    {ACCEL_CONFIG,      3<<3  }, // ACCEL_CONFIG 加速度计量程 16g   2048最大
    {INT_PIN_CFG,       0x12  }, // 寄存器手册P28；data ready时发射50us pulse
    {INT_ENABLE,        1    }, // ??????????
    {USER_CTRL,         0x00  }, // ?????I2C?
  };
  uint8 i;
  for(i=0; i<8; i++)
#ifdef HARD_IIC
    i2c_write_reg(I2C1, MPU6050_Addr,  param[i][0], param[i][1]);
#else
  Single_Write(MPU6050_Addr, param[i][0], param[i][1]);
#endif
}

//读加速度计并转化到数组
void MPUReadAcc()
{
  uint8 accbuff[2] = {0};            //one byte buffer for receiving data from slave
#ifdef HARD_IIC
  accbuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_XOUT_L);
  accbuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_XOUT_H);
  Acc_ADC.x = (accbuff[1]<<8)|accbuff[0];
  accbuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_YOUT_L);
  accbuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_YOUT_H);
  Acc_ADC.y = (accbuff[1]<<8)|accbuff[0];
  accbuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_ZOUT_L);
  accbuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_ZOUT_H);
  Acc_ADC.z = (accbuff[1]<<8)|accbuff[0];
#else
  Single_Read(MPU6050_Addr,ACCEL_XOUT_L, &accbuff[0]);
  Single_Read(MPU6050_Addr,ACCEL_XOUT_H, &accbuff[1]);
  Acc_ADC.x = (accbuff[1]<<8)|accbuff[0];
  Single_Read(MPU6050_Addr,ACCEL_YOUT_L, &accbuff[0]);
  Single_Read(MPU6050_Addr,ACCEL_YOUT_H, &accbuff[1]);
  Acc_ADC.y = (accbuff[1]<<8)|accbuff[0];
  Single_Read(MPU6050_Addr,ACCEL_ZOUT_L, &accbuff[0]);  //读取计算Y轴数据  陀螺仪乘 0.001064225
  Single_Read(MPU6050_Addr,ACCEL_ZOUT_H, &accbuff[1]);
  Acc_ADC.z = (accbuff[1]<<8)|accbuff[0];
#endif  
}

//读陀螺仪计并转化到数组
void MPUReadGyr()  //   9.8011/2048 = 0.004785693
{
  uint8 gyrobuff[2] = {0};            //one byte buffer for receiving data from slave
#ifdef HARD_IIC
  gyrobuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_XOUT_L);
  gyrobuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_XOUT_H);
  Gyro_ADC.x = ((gyrobuff[1]<<8)|gyrobuff[0]) + 18.5;
  gyrobuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_YOUT_L);
  gyrobuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_YOUT_H);
  Gyro_ADC.y = ((gyrobuff[1]<<8)|gyrobuff[0]) - 30.8;
  gyrobuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_ZOUT_L);
  gyrobuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_ZOUT_H);
  Gyro_ADC.z = (gyrobuff[1]<<8)|gyrobuff[0];
#else
  Single_Read(MPU6050_Addr,GYRO_XOUT_L, &gyrobuff[0]);
  Single_Read(MPU6050_Addr,GYRO_XOUT_H, &gyrobuff[1]);
  Gyro_ADC.x = ((gyrobuff[1]<<8)|gyrobuff[0]);
  //读取计算X轴数据 加速度计乘 0.0023897
  Single_Read(MPU6050_Addr,GYRO_YOUT_L, &gyrobuff[0]);
  Single_Read(MPU6050_Addr,GYRO_YOUT_H, &gyrobuff[1]);
  Gyro_ADC.y = ((gyrobuff[1]<<8)|gyrobuff[0]);
  //读取计算Y轴数据  陀螺仪乘 0.001064225
  Single_Read(MPU6050_Addr,GYRO_ZOUT_L, &gyrobuff[0]);
  Single_Read(MPU6050_Addr,GYRO_ZOUT_H, &gyrobuff[1]);
  Gyro_ADC.z = (gyrobuff[1]<<8)|gyrobuff[0];
#endif
}
//陀螺仪零偏校正
//返回：校正成功1，失败0
uint8 GyroCalibration(void)
{
  uint32 i = 0, j = 0;
  float gyro_cali_tmpx = 0, gyro_cali_tmpy = 0, gyro_cali_tmpz = 0;
  
  for(i=0;i<2;i++)
  {
    for(j=0;j<100;j++)
    {
      DelayMs(10);
      MPUReadGyr();
      gyro_cali_tmpx += Gyro_ADC.x;
      gyro_cali_tmpy += Gyro_ADC.y;
      gyro_cali_tmpz += Gyro_ADC.z;
    }
    DelayMs(1000);
  }
  gyro_cali_tmpx /= 200.0;
  gyro_cali_tmpy /= 200.0;
  gyro_cali_tmpz /= 200.0;
  if(gyro_cali_tmpx>(-1.26099996E+1-10)&&gyro_cali_tmpx<(-1.26099996E+1+10)
     && gyro_cali_tmpy>(1.9479999544E+1-10)&&gyro_cali_tmpy<(1.9479999544E+1+10)
       && gyro_cali_tmpz>(-1.2329999922E+1-10)&&gyro_cali_tmpz<(-1.2329999922E+1+10))
  {
    Gyro_ADC.x_offset = gyro_cali_tmpx;
    Gyro_ADC.y_offset = gyro_cali_tmpy;
    Gyro_ADC.z_offset = gyro_cali_tmpz;
    return 1;
  }
  else
    return 0;
}
short x_min = 0,x_max = 0,y_min = 0,y_max = 0,z_min = 0,z_max = 0;//-4857,4772,-8420,230,-11900,-510
void CompassCalibration(void)
{
  uint32 i = 0;
  BUZZER_OUT = 0; 
  
    do{
      MPUReadAcc();
      MPUReadGyr();
      ReadCompass();//Compass.x,Compass.y,Compass.z
      Filt();                       //角度在范围内就开始校准
      if(ang_x>-2 && ang_x<2 && ang_y>-2 && ang_y<2)
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
    
  short x_start = Compass.x;
  short y_start = Compass.y;
  x_min = Compass.x;
  x_max = Compass.x;
  y_min = Compass.y;
  y_max = Compass.y;
  while(1)
  {
    ReadCompass();      //Compass.x,Compass.y,Compass.z
    if(x_min>Compass.x)
    {
      x_min = Compass.x;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(x_max<Compass.x)
    {
      x_max = Compass.x;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(y_min>Compass.y)
    {
      y_min = Compass.y;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(y_max<Compass.y)
    {
      y_max = Compass.y;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(x_max-x_min>8000 && y_max-y_min>8000 && Compass.x>x_start-ctrl[0] && Compass.x<x_start+ctrl[0] && Compass.y>y_start-ctrl[0] && Compass.y<y_start+ctrl[0])
    {
      BUZZER_OUT = 1;
      DelayMs(500);
      BUZZER_OUT = 0;
      break;
    }
  }
  
  DelayMs(900);
//  BUZZER_OUT = 0;  
  i = 0;
  do{
    MPUReadAcc();
    MPUReadGyr();
    ReadCompass();//Compass.x,Compass.y,Compass.z
    Filt();                       //角度在范围内就开始校准
    if((ang_y>-92 && ang_y<-88) || (ang_y>88 && ang_y<92))
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
  
  short z_start = Compass.z;
  z_min = Compass.z;
  z_max = Compass.z;
  while(1)
  {
    ReadCompass();      //Compass.x,Compass.y,Compass.z
    if(z_min>Compass.z)
    {
      z_min = Compass.z;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(z_max<Compass.z)
    {
      z_max = Compass.z;
      BUZZER_OUT = 1;
      DelayMs(10);
      BUZZER_OUT = 0;
    }
    if(z_max-z_min>8000 && Compass.z>z_start-ctrl[0] && Compass.z<z_start+ctrl[0])
    {
      BUZZER_OUT = 1;
      DelayMs(500);
      BUZZER_OUT = 0;
      break;
    }
  }
  Compass.x_offset = (x_min+x_max)/2.0;
  Compass.y_offset = (y_min+y_max)/2.0;
  Compass.z_offset = (z_min+z_max)/2.0;
}

