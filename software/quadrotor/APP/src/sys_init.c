#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "define.h"
//#include "sys_init.h"
#include "math.h"                       //数据nan判断
#include "stm32f10x_MY_Flash.h"         //读取flash
#include "calculation.h"                //将控制计算的参数初始化


//Brief:Get a float from flash;
//Parameters:
//  p_param:float need to be initial;
//  addr:address where float placed;
//Return:void;
//  Example://
//void GetFloatFromFlash(float *p_param, uint32_t addr)
//{
//  
//}
//Brief:Name-Param(Parameters)Init;initial plane parameters
//Parameters:
//  p_gyro_offset:address of the goroscope offset parameter;
//  p_acc:;
//  p_compass:;
//  _pipl_dir:see definition of <pipl_dir>;control parameter will initial with differnt value according to this param
//Return:void;
//  Example://
void ParamInit(short *p_gyro_offset, PACC p_acc,PCOMPASS p_compass)
{
  pipl_dir = 0;
  x_b = -5;
  y_b = 0;
  z_p = 2;
  z_d = 0.02;
#ifdef MY_WOOD                  //my wood plane
  if(pipl_dir)         //X形飞行
  {
    offset_angle[0] = 0;//5;
    offset_angle[1] = 0;//5;
    offset_angle[2] = 0;
    x_p_o = 80;                     //x(x axis)_p(proportion)_o(out loop)_x(x shape fly)x轴X形飞行控制参数
    x_p_i = 0.01;
    x_d_i = 0.001;
    y_p_o = 80;
    y_p_i = 0.01;
    y_d_i = 0.001;
  }
  else         //十形飞行
  {
    offset_angle[0] = 0;//5;
    offset_angle[1] = 0;//5;
    offset_angle[2] = 0;
    x_p_o = 100;                     //x(x axis)_p(proportion)_o(out loop)_t(ten shape fly)x轴十形飞行控制参数
    x_p_i = 0.015;
    x_d_i = 0.0012;
    y_p_o = 80;
    y_p_i = 0.015;
    y_d_i = 0.001;
  }
#elif CARBON_FIBRE                  //other plane
  if(pipl_dir)         //X形飞行
  {
    offset_angle[0] = 0;//5;
    offset_angle[1] = 0;//5;
    offset_angle[2] = 0;
    x_p_o = 60;                     //x(x axis)_p(proportion)_o(out loop)_x(x shape fly)x轴X形飞行控制参数
    x_p_i = 0.015;
    x_d_i = 0.0012;
    y_p_o = 60;
    y_p_i = 0.015;
    y_d_i = 0.001;
  }
  else         //十形飞行
  {
    offset_angle[0] = 0;//5;
    offset_angle[1] = 0;//5;
    offset_angle[2] = 0;
    x_p_o = 100;                     //x(x axis)_p(proportion)_o(out loop)_t(ten shape fly)x轴十形飞行控制参数
    x_p_i = 0.0257;
    x_d_i = 0.0012;
    y_p_o = 80;
    y_p_i = 0.0257;
    y_d_i = 0.001;
  }
#endif
  p_gyro_offset[0] = FLASH_ReadHalfWord(STM32F103RC_FLASH_PAGE127_ADDR + GYRO_XOFF_ADDR_OFFSET);        //从flash读取陀螺仪的零偏校准参数
  p_gyro_offset[1] = FLASH_ReadHalfWord(STM32F103RC_FLASH_PAGE127_ADDR + GYRO_YOFF_ADDR_OFFSET);
  p_gyro_offset[2] = FLASH_ReadHalfWord(STM32F103RC_FLASH_PAGE127_ADDR + GYRO_ZOFF_ADDR_OFFSET);
  float tmp = 0;
  
  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + ACC_KX_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)             //flash read -1 if flash is empty
    p_acc->kx = 1;                      //缺省值设为1
  else
    p_acc->kx = tmp;

  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + ACC_KY_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_acc->ky = 1;                //缺省值设为1
  else
    p_acc->ky = tmp;

  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + ACC_KZ_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_acc->kz = 1;                //缺省值设为1
  else
    p_acc->kz = tmp;
  
  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + ACC_XOFF_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_acc->x_off = 0;                //缺省值设为0
  else
    p_acc->x_off = tmp;

  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + ACC_YOFF_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_acc->y_off = 0;                //缺省值设为0
  else
    p_acc->y_off = tmp;

  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + ACC_ZOFF_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_acc->z_off = 0;                //缺省值设为0
  else
    p_acc->z_off = tmp;
  p_compass->x_offset = FLASH_ReadHalfWord(STM32F103RC_FLASH_PAGE127_ADDR + COMPASS_XOFF_ADDR_OFFSET);
  p_compass->y_offset = FLASH_ReadHalfWord(STM32F103RC_FLASH_PAGE127_ADDR + COMPASS_YOFF_ADDR_OFFSET);
  p_compass->z_offset = FLASH_ReadHalfWord(STM32F103RC_FLASH_PAGE127_ADDR + COMPASS_ZOFF_ADDR_OFFSET);
  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + COMPASS_KX_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_compass->kx = 1;                //缺省值设为1
  else
    p_compass->kx = tmp;

  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + COMPASS_KY_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_compass->ky = 1;                //缺省值设为1
  else
    p_compass->ky = tmp;

  tmp = FlashReadFloat(STM32F103RC_FLASH_PAGE127_ADDR + COMPASS_KZ_ADDR_OFFSET);
  if(isnan(tmp) || -1==tmp)     //flash read -1 if flash is empty
    p_compass->kz = 1;                //缺省值设为1
  else
    p_compass->kz = tmp;
  
}

//general purpose pin引脚初始化
//void GpioInit()
//{
//  gpio_init(RED_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
//  gpio_init(BLUE_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
//  gpio_init(RGB_R_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
//  gpio_init(RGB_G_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
//  gpio_init(RGB_B_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);					 //PB.5 ???
//  gpio_init(BUZZER_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
//  gpio_init(LIGHT_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
//  gpio_init(GYRO_CS_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 1);                               //初始化CE，默认进入待机模式
//  gpio_init(LSM303_CS_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 1);                               //初始化CE，默认进入待机模式
//  gpio_init(NRF_CSN_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 1);                               //初始化CE，默认进入待机模式
//}
u16 GetAdc(u8 ch)
{  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  /* Start ADC1 Software Conversion */ 
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));  /* Test if the ADC1 EOC flag is set or not */ 
  return ADC_GetConversionValue(ADC1);   /*Returns the ADC1 Master data value of the last converted channel*/
}

float BatVoltageGet()
{
  uint16 adc_val = 0;
  float prop_voltage = 0;       //按分压比推算得到的电压
  ADC_EN_OUT = 1;
  DelayUs(10);                //等待一段时间，让模拟电路的电压稳定
  adc_val = GetAdc(BAT_ADC_CH);         //4095=3.3v
  ADC_EN_OUT = 0;
  prop_voltage = adc_val/4096.0*3.28*(2.7+0.91)/0.91;   //实测3.28V,0.2v的Pmos压降
  if(prop_voltage<=6)         //稳压二极管作用后实测拟合的公式(二次拟合，在8-15V之间较准确，)
    return prop_voltage;
  else
    return 0.2249*prop_voltage*prop_voltage - 2.4531*prop_voltage + 12.999+0.2;//+0.2:2021年1月31日21:40:03，测不准了，随便加的
}

void AdcInit(void)
{
  gpio_init(ADC_EN_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(ADC_PIN,AIN,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHZ
  ADC_InitTypeDef ADC_InitStructure;
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //不开启扫描 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //触发软件 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);  /* Enable ADC1 reset calibration register */   
  while(ADC_GetResetCalibrationStatus(ADC1));  /* Check the end of ADC1 reset calibration register */
  ADC_StartCalibration(ADC1);  /* Start ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //  NVIC_Init(&NVIC_InitStructure);
}
//float *xa_apo = NULL;
//float *Pa_apo = NULL;
//float Rot_matrix[9] = {1.f,  0,  0,
//0,  1.f,  0,
//0,  0,  1.f
//};		/**< init: identity matrix */
//float *eulerAngle = NULL;
//float *debugOutput = NULL;
//allocate memory for kalman filter
//BOOL MemoryAllocate(void)
//{
//  xa_apo = (float *)malloc(sizeof(float)*12);
//  if(NULL==xa_apo)
//    return false;
//  Pa_apo = (float *)malloc(sizeof(float)*144);
//  if(NULL==Pa_apo)
//    return false;
////  Rot_matrix = (float *)malloc(sizeof(float)*9);
////  if(NULL==Rot_matrix)
////    return false;
//  eulerAngle = (float *)malloc(sizeof(float)*3);
//  if(NULL==eulerAngle)
//    return false;
//  debugOutput = (float *)malloc(sizeof(float)*4);
//  if(NULL==debugOutput)
//    return false;
//  return true;
//}