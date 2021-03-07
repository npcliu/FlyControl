#include <assert.h>
#include "stm32f10x_gpio.h"
#include "MyIIC.h"
#include "Mpu6050.h"
#include "define.h"
#include "delay.h"
#include "stm32f10x_MY_Flash.h"
#include "ISR.h"                //��Ҫ�ж϶�ȡ����ԭʼ���ݣ��������ж��ж��������


BOOL MPUInit(void)
{
  gpio_init(MPUINT_PIN, IN_FLOATING, interupt,GPIO_Speed_50MHz,0);                              //��ʼ��IRQ�ܽ�Ϊ����
  gpio_int_cfg(MPUINT_PIN,MPUINT_Line,EXTI_Trigger_Rising);
  
  uint8 filt_para = FLASH_ReadByte(STM32F103RC_FLASH_PAGE126_ADDR + MPU6050_FILTER_OFFSET); //read from flash
  if(0==filt_para || 1==filt_para || 2==filt_para || 3==filt_para || 4==filt_para || 5==filt_para || 6==filt_para);
  else
    filt_para = 3;                      //�˲���ȱʡֵ��Ϊ5
  
  int gyro_scale_config = 0;              //gyroscope default config as 0,that means 250DPS,if MPU6050GYRO_SCALE_DEG is outrange of any scale predefinition,this value also equals to 0
  gyro_scale_config = 0*(MPU6050GYRO_SCALE_DEG>MPU6050GYRO_SCALE_250_DPS-0.0001 && MPU6050GYRO_SCALE_DEG<MPU6050GYRO_SCALE_250_DPS+0.0001)\
    +1*(MPU6050GYRO_SCALE_DEG>MPU6050GYRO_SCALE_500_DPS-0.0001 && MPU6050GYRO_SCALE_DEG<MPU6050GYRO_SCALE_500_DPS+0.0001)\
    +2*(MPU6050GYRO_SCALE_DEG>MPU6050GYRO_SCALE_1000_DPS-0.0001 && MPU6050GYRO_SCALE_DEG<MPU6050GYRO_SCALE_1000_DPS+0.0001)\
    +3*(MPU6050GYRO_SCALE_DEG>MPU6050GYRO_SCALE_2000_DPS-0.0001 && MPU6050GYRO_SCALE_DEG<MPU6050GYRO_SCALE_2000_DPS+0.0001);
assert(0==gyro_scale_config || 1==gyro_scale_config || 2==gyro_scale_config || 3==gyro_scale_config);       //check if the data is valid

  int acc_scale_config = 0;              //acc default config as 0,that means 2g,if MPU6050ACC_SCALE_G is outrange of any scale predefinition,this value also equals to 0
acc_scale_config = 0*(MPU6050ACC_SCALE_G>MPU6050ACC_SCALE_2G-1e-6 && MPU6050ACC_SCALE_G<MPU6050ACC_SCALE_2G+1e-6)\
  +1*(MPU6050ACC_SCALE_G>MPU6050ACC_SCALE_4G-1e-6 && MPU6050ACC_SCALE_G<MPU6050ACC_SCALE_4G+1e-6)\
  +2*(MPU6050ACC_SCALE_G>MPU6050ACC_SCALE_8G-1e-6 && MPU6050ACC_SCALE_G<MPU6050ACC_SCALE_8G+1e-6)\
  +3*(MPU6050ACC_SCALE_G>MPU6050ACC_SCALE_16G-1e-6 && MPU6050ACC_SCALE_G<MPU6050ACC_SCALE_16G+1e-6);
assert(0==acc_scale_config || 1==acc_scale_config || 2==acc_scale_config || 3==acc_scale_config);       //check if the data is valid
  const uint8 param[][2] = 
  {
    {PWR_MGMT_1,        1     }, // ��Դѡ��
    {SMPLRT_DIV,        4     }, // �����ǲ�����=1k/1+num Hz?
    {MPU6050_CONFIG,    filt_para     }, // ��ͨ�˲�Ƶ�ʣ��Ĵ����ֲ�P23
    {GYRO_CONFIG,       gyro_scale_config<<3  }, // ����������2000dps  16.4-1��/s    ���ٶȱ��ʼĴ���
    {ACCEL_CONFIG,      acc_scale_config<<3  }, // ACCEL_CONFIG ���ٶȼ����� 16g   2048->1g
    {INT_PIN_CFG,       0x12  }, // �Ĵ����ֲ�P28��data readyʱ����50us pulse
    {INT_ENABLE,        1    }, // ??????????
    {USER_CTRL,         0x00  }, // ?????I2C?
  };
  uint8 i;
  for(i=0; i<8; i++)
#ifdef HARD_IIC
    i2c_write_reg(I2C1, MPU6050_Addr,  param[i][0], param[i][1]);
#else
  if(false==Single_Write(MPU6050_Addr, param[i][0], param[i][1]))
    return false;
#endif
  return true;
}

//Brief�������ٶȼ����
//Parameters��
//  pacc��short���͵�ַ�����ṩ3�����ϵ�short�ڴ�
//Retrun��void;
//Example:;
void MPUReadAcc(short *pacc)
{
  uint8 accbuff[2] = {0};            //one byte buffer for receiving data from slave
#ifdef HARD_IIC
  accbuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_XOUT_L);
  accbuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_XOUT_H);
  pacc[0] = (accbuff[1]<<8)|accbuff[0];
  accbuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_YOUT_L);
  accbuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_YOUT_H);
  pacc[1] = (accbuff[1]<<8)|accbuff[0];
  accbuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_ZOUT_L);
  accbuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,ACCEL_ZOUT_H);
  pacc[2] = (accbuff[1]<<8)|accbuff[0];
#else
  accbuff[0] = Single_Read(MPU6050_Addr,ACCEL_XOUT_L);
  accbuff[1] = Single_Read(MPU6050_Addr,ACCEL_XOUT_H);
  pacc[0] = ((accbuff[1]<<8)|accbuff[0]);
  accbuff[0] = Single_Read(MPU6050_Addr,ACCEL_YOUT_L);
  accbuff[1] = Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
  pacc[1] = ((accbuff[1]<<8)|accbuff[0]);
  accbuff[0] = Single_Read(MPU6050_Addr,ACCEL_ZOUT_L);  //��ȡ����Y������  �����ǳ� 0.001064225
  accbuff[1] = Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
  pacc[2] = ((accbuff[1]<<8)|accbuff[0]);
#endif
}

//Brief��get gyroscope chip output data
//Parameters��
//  pgyro��short���͵�ַ�����ṩ3�����ϵ�short�ڴ�
//Retrun��void;
//Example:;
void MPUReadGyr(short *pgyro)
{
  uint8 gyrobuff[2] = {0};            //one byte buffer for receiving data from slave
#ifdef HARD_IIC
  gyrobuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_XOUT_L);
  gyrobuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_XOUT_H);
  pgyro[0] = ((gyrobuff[1]<<8)|gyrobuff[0]);
  gyrobuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_YOUT_L);
  gyrobuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_YOUT_H);
  pgyro[1] = ((gyrobuff[1]<<8)|gyrobuff[0]);
  gyrobuff[0] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_ZOUT_L);
  gyrobuff[1] = i2c_read_reg(I2C1, MPU6050_Addr,GYRO_ZOUT_H);
  pgyro[2] = ((gyrobuff[1]<<8)|gyrobuff[0]);
#else
  gyrobuff[0] = Single_Read(MPU6050_Addr,GYRO_XOUT_L);
  gyrobuff[1] = Single_Read(MPU6050_Addr,GYRO_XOUT_H);
  pgyro[0] = ((gyrobuff[1]<<8)|gyrobuff[0]);
  //��ȡ16bitX������
  gyrobuff[0] = Single_Read(MPU6050_Addr,GYRO_YOUT_L);
  gyrobuff[1] = Single_Read(MPU6050_Addr,GYRO_YOUT_H);
  pgyro[1] = ((gyrobuff[1]<<8)|gyrobuff[0]);
  //��ȡ16bit Y������
  gyrobuff[0] = Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
  gyrobuff[1] = Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
  pgyro[2] = ((gyrobuff[1]<<8)|gyrobuff[0]);
#endif
}
//��������ƫУ��
//���أ�У���ɹ�1��ʧ��0
int gyro_cali_tolerance = 10;
char MPU6050GyroCalibration(short *PGYRO)
{
//  short _MPU6050gyro_adc[6] = {0};      //MPUReadGyr will read _MPU6050gyro_adc[3],[4],[5]
  int i = 0, j = 0,k = 0;
  float gyro_cali_sum[3] = {0};
  char recali_flag = 0;         //need recalibrate flag
  short gyro_min[3] = {0};
  short gyro_max[3] = {0};
  
//  for(k=0;k<3;k++)
//    PGYRO[k] = 0;                       //clear to zero,then read data will be RAW data
//  MPUReadGyr(_MPU6050gyro_adc);
  while(0==gyro_chip_out[0] && 0==gyro_chip_out[1] && 0==gyro_chip_out[1]);//acc_chip_out���жϸ��£����ȫ��0����Ϊû�������������Ϊ��̫���ܲ��ȫ��0
  for(k=0;k<3;k++)
  {
    gyro_min[k] = gyro_chip_out[k];
    gyro_max[k] = gyro_chip_out[k];
    PGYRO[k] = 0;                       //clear to zero,then read
  }
  for(i=0;i<3;i++)      //i,j ��ʱ�βɼ�
  {
    for(j=0;j<100;j++)
    {
      DelayMs(5);
//      MPUReadGyr(_MPU6050gyro_adc);             //ע�ⲻҪ���ж��г�ͻ��
      for(k=0;k<3;k++)
      {
        gyro_cali_sum[k] += gyro_chip_out[k];
        if(gyro_min[k]>gyro_chip_out[k])
          gyro_min[k] = gyro_chip_out[k];
        if(gyro_max[k]<gyro_chip_out[k])
        {
          gyro_max[k] = gyro_chip_out[k];
          while(gyro_max[k]>1000);
        }
        if(gyro_max[k] - gyro_min[k] > gyro_cali_tolerance)     //���������ֵ�ж������������⣬����У׼
        {
          recali_flag = 1;
          break;
        }
      }
      if(recali_flag)
      {
        recali_flag = 0;
        i = -1;
//        MPUReadGyr(gyro_chip_out);
  while(0==gyro_chip_out[0] && 0==gyro_chip_out[1] && 0==gyro_chip_out[1]);//acc_chip_out���жϸ��£����ȫ��0����Ϊû�������������Ϊ��̫���ܲ��ȫ��0
        for(k=0;k<3;k++)
        {
          gyro_min[k] = gyro_chip_out[k];
          gyro_max[k] = gyro_chip_out[k];
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
