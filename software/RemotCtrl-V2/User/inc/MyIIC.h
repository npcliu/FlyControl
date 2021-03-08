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

#define MSCL_PIN        PE14
#define MSDA_PIN        PE15
#define MSDA_IN         PE15_IN
#define MSCL_OUT        PE14_OUT
#define MSDA_OUT        PE15_OUT
#define MSDA_MOD        PE15_MOD
#define MSDA_PUL        PE15_PUL
//#define SDA_IN_M()      gpio_ddr(PTE0,GPI)
//#define SDA_OUT_M()     gpio_ddr(PTE0,GPO)


/*********************************compass QMC5883*********************************/
#define COMPASS_ADDR     0x1A             //due to Compass_Read_Addr = Compass_Write_Addr+1;so I only define COMPASS_ADDR
#define BMP085_SlaveAddress  0xee	 ////定义器件在IIC总线中的从地址

#define Data_Output_X_LSB_Reg_Of_Compass        0x00
#define Data_Output_X_MSB_Reg_Of_Compass        0x01
#define Data_Output_Y_LSB_Reg_Of_Compass        0x02
#define Data_Output_Y_MSB_Reg_Of_Compass        0x03
#define Data_Output_Z_LSB_Reg_Of_Compass        0x04
#define Data_Output_Z_MSB_Reg_Of_Compass        0x05

extern void IIC_Port_Init(void);                       //初始化IIC的IO口				 
void CompassInit(void);
void ReadCompass();
extern void Single_Write(unsigned char slave_addr, unsigned char reg_addr, unsigned char data);		     //void
void Single_Read(uint8 slave_addr,uint8 reg_addr, uint8* pdata);
short bmp085ReadTemp(void);
short bmp085ReadPressure(void);
extern   unsigned int ut;
extern unsigned long up;
#endif