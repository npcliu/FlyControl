#ifndef __SYS_INIT_H
#define __SYS_INIT_H

#include "lcd.h"
typedef struct
{
//  uint8 index;          //index of the parameter
  char name[10];        //name 
  float value;
  char ins[50];        //instruction
  PANNELD_DATA dis_ifo;                //this data display position and other information on lcd;
}PARAMETER;

//extern short send_data[3][8];
//extern short ac1, ac2,ac3,b1, b2,mb,mc,md;
//extern unsigned short ac4,ac5,ac6;
extern PARAMETER params[PARAMETER_NUM];
extern u16 poitr_off_pan;

//void Display_Welcome_Message(void);
void ParameterInit();
void CompassCalibration(void);

//uint8 MotorStart(int desire_speed,float _duty);
//void MotorShutDown();
//void PwmCtrl(int phase,float _duty);
void Adc1Init();
void Adc3Init();
float FiltedBatVoltage();
uint32 PhotoRESVoltageGet();
u16 GetAdc(ADC_TypeDef* ADCx,u8 ch);
void GpioInit(void);
//void MotorCtrlPIN_Init();
//void HallPinInit();
//void KeyInit();
//void gpio_int_cfg(PTXn_e pin,uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger);
//void TIM3_PWM_Init(u16 arr,u16 psc);
//void TIM2_Cap_Init(u16 arr,u16 psc);
//void NvicConfig();
uint8 SPI_ExchangeByte(uint8 input);
void RF_Initial(uint8 mode);
void NVIC_Config(void);
void SXGPIOInit();
void USARTSend(USART_TypeDef* USARTx, char*loca,uint32 byte_leng);
void SCISend_to_Own(USART_TypeDef* USARTx);
void WaveSend(USART_TypeDef* USARTx);
void bmp085Convert();
void bmp085_init(void);
short twobmp085_read(u8 bmp085_RCOMMAND);
double BMP085_Get_Altitude(long Pressure);
void UartInit();
void MemInit(void);              //创建链表
void UpdateRmtCtrlStatus(const char _tx_flag,uint8 force_update,u16,u16);
void UpdatePlaneMod(const PLANE_MODE _mode,u16,u16);
void LCD_Show_Para(int cursur, int selected_para);             //lcd显示触摸后的提示信息函数

#endif