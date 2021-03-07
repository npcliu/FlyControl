#ifndef __MYIIC_H
#define __MYIIC_H
//#include "common.h"
//#include "MKL_port.h"
//#include "MKL_gpio.h"
//#include "include_own.h"


//#define MSCL_PIN        PE14
//#define MSDA_PIN        PE15
//#define MSCL1            PE14_OUT
//#define MSDA1            PE15_OUT

#define MSCL_PIN        PB6
#define MSDA_PIN        PB7
#define MSDA_IN         PB7_IN
#define MSCL_OUT        PB6_OUT
#define MSDA_OUT        PB7_OUT
#define MSDA_MOD        PB7_MOD
#define MSDA_PUL        PB7_PUL
//#define SDA_IN_M()      gpio_ddr(PTE0,GPI)
//#define SDA_OUT_M()     gpio_ddr(PTE0,GPO)


#define BMP085_SlaveAddress  0xee	 ////定义器件在IIC总线中的从地址

void IIC_Port_Init(void);                       //初始化IIC的IO口				 
void CompassInit(void);
//void ReadCompass();
extern BOOL Single_Write(unsigned char slave_addr, unsigned char reg_addr, unsigned char data);		     //void
uint8 Single_Read(uint8 slave_addr,uint8 reg_addr);
short bmp085ReadTemp(void);
short bmp085ReadPressure(void);
extern   unsigned int ut;
extern unsigned long up;
#endif