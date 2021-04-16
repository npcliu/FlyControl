#ifndef __MS5611_H
#define __MS5611_H

//#include "myiic.h"
#include "stdio.h"
#include "delay.h"

#define MS5611_MSCL_PIN        PD1
#define MS5611_MSDA_PIN        PC2
#define MS5611_MSDA_IN         PC2_IN
#define MS5611_MSCL_OUT        PD1_OUT
#define MS5611_MSDA_OUT        PC2_OUT
#define MS5611_MSDA_MOD        PC2_MOD
#define MS5611_MSDA_PUL        PC2_PUL

typedef struct __MS5611
{
  uint16_t Cal_C[7]; 
  uint32_t D1_Pres,D2_Temp; 
  int32_t Pressure;				
  int32_t dT,Temperature2;
  int64_t OFF,SENS;  
  float Aux;
  int64_t OFF2,SENS2;
  int32_t Temperature;//除以100得到真实温度
  float altitude;
  float altitude_init;//记录起飞前初始高度
  char update;//数据更新标志位
}_ms5611;

void MS5611_IIC_Init(void);
void MS561101BA_Reset(void);
void MS561101BA_readPROM(void);

uint32_t MS561101BA_DO_CONVERSION(u8 command);
void MS561101BA_GetTemperature(u8 OSR_Temp);
void MS561101BA_GetPressure(u8 OSR_Pres);
void MS561101BA_Init(void);
void MS5611GetTemperatureAndPressure(void);

#endif
