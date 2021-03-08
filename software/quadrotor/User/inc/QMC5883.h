#ifndef _QMC5883_H_
#define _QMC5883_H_     1
//
//typedef struct
//{
//  short x, y, z;
//  short x_offset,y_offset,z_offset;
//}COMPASS;

/*********************************compass QMC5883*********************************/
#define COMPASS_ADDR     0x1A             //due to Compass_Read_Addr = Compass_Write_Addr+1;so I only define COMPASS_ADDR
#define Data_Output_X_LSB_Reg_Of_Compass        0x00
#define Data_Output_X_MSB_Reg_Of_Compass        0x01
#define Data_Output_Y_LSB_Reg_Of_Compass        0x02
#define Data_Output_Y_MSB_Reg_Of_Compass        0x03
#define Data_Output_Z_LSB_Reg_Of_Compass        0x04
#define Data_Output_Z_MSB_Reg_Of_Compass        0x05
#define CONTROL_REG_1   0x09
#define SET_RESET_PERIOD_REG    0x0B
#define SET_RESET_PERIOD_REG    0x0B





void QMC5883Init(void);
void ReadQMC5883(short *pcompass);

#endif