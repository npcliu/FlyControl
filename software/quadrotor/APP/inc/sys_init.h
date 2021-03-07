#ifndef __SYS_INIT_H
#define __SYS_INIT_H

//extern short send_data[3][8];
//extern int gyrogc[50];
//extern short ac1, ac2,ac3,b1, b2,mb,mc,md;
//extern unsigned short ac4,ac5,ac6;

//void Display_Welcome_Message(void);
//void ParamInit(short *p_gyro_offset, PACC p_acc ,PCOMPASS p_compass,uint8 _pipl_dir);
//void CompassCalibration(int ristrict);
//extern float *xa_apo;
//extern float *Pa_apo;
//extern float Rot_matrix[9];
//extern float *eulerAngle;
//extern float *debugOutput;



//uint8 MotorStart(int desire_speed,float _duty);
//void MotorShutDown();
//void PwmCtrl(int phase,float _duty);
void AdcInit();
float BatVoltageGet();
void ParamInit(short *p_gyro_offset, PACC p_acc,PCOMPASS p_compass);
//uint16 GetAdc(uint8 ch);
//void GpioInit();
//void MotorCtrlPIN_Init();
//void HallPinInit();
//void KeyInit();
//void gpio_int_cfg(PTXn_e pin,uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger);
//void TIM3_PWM_Init(u16 arr,u16 psc);
//void TIM2_Cap_Init(u16 arr,u16 psc);
//void NvicConfig();
//uint8 SPI_ExchangeByte(uint8 input);
//void SPI1_Init(void);
void SPI3_Init(void);
//void RF_Initial(uint8 mode);
void NVIC_Config(void);
//void SXGPIOInit();
//void USARTSend(USART_TypeDef* USARTx, char*loca,uint32 byte_leng);
//void WaveSend(USART_TypeDef* USARTx);
//void bmp085Convert();
//void bmp085_init(void);
//short twobmp085_read(u8 bmp085_RCOMMAND);
//double BMP085_Get_Altitude(long Pressure);
//void UARTSendFloat(float d);
//void MemInit(void);              //´´½¨Á´±í
//BOOL MemoryAllocate();

#endif