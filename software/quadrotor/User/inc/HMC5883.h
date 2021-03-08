#ifndef _HMC5883_H_
#define _HMC5883_H_     1


/*********************************compass QMC5883*********************************/

#define HMC5883_ADDR     0x3C            //due to Compass_Read_Addr = Compass_Write_Addr+1;so I only define COMPASS_ADDR
#define Data_Output_X_MSB_Reg_Of_HMC5883        0x03
#define Data_Output_X_LSB_Reg_Of_HMC5883        0x04
#define Data_Output_Z_MSB_Reg_Of_HMC5883        0x05
#define Data_Output_Z_LSB_Reg_Of_HMC5883        0x06
#define Data_Output_Y_MSB_Reg_Of_HMC5883        0x07
#define Data_Output_Y_LSB_Reg_Of_HMC5883        0x08

#define CONFIG_REG_A    0x00
#define CONFIG_REG_B    0x01
#define MODE_REG        0x02
#define STATUS_REG      0x09
#define ID_REG_A      0x10
#define ID_REG_B      0x11
#define ID_REG_C      0x12


void   HMC5883Init();
//void CompassInit(void);
void ReadHMC5883(short *pcompass);

#endif