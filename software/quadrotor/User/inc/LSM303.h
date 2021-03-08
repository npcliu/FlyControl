#ifndef __LSM303_H
#define __LSM303_H


#include "stm32f10x.h"
#include  <math.h>
/*=====================================================================================================*/
//typedef struct
//{
//  short x, y, z;
//  short x_offset,y_offset,z_offset;
//}COMPASS;

#define LSM303_SPI      SPI3
#define LSM303_CS_PIN        PB15    
#define LSM303_CS_OUT        PB15_OUT
#define LSM303_MAG_DRDY_PIN     PC7
#define LSM303_MAG_DRDY_PIN_IN  PC7_IN
#define LSM303_MAG_DRDY_Line    EXTI_Line7
#define LSM303_ACC_DRDY_PIN     PC8
#define LSM303_ACC_DRDY_PIN_IN  PC8_IN
#define LSM303_ACC_DRDY_Line    EXTI_Line8

#define LSM303D_SLAVE_ADDRESS   0x1E
#define LSM303D_WHO_AM_I        0x0F
#define I_AM_LSM303D            0x49
#define INT_CTRL_M              0x12            //interrupt & interrupt pin setting
#define CTRL1                   0x20
#define CTRL2                   0x21
#define CTRL3                   0x22            //INT1 PIN interrupt configuration
#define CTRL4                   0x23            //INT2 PIN interrupt configuration
#define CTRL5                   0x24            //temperature & magnetic & interrupt settings
#define CTRL6                   0x25            //magnetic full scal range
#define CTRL7                   0x26            //acc high-pass filter & temperature & magnetic settings
#define STATUS_A                0x27

#define LSM_INT_PIN_CFG            1            //0=push-pull;1=open drain;see manual page 31
#define INT_PIN_CFG_SHIFT      4                //0=up;1=down;see manual page 31
#define INT_POLARITY            1               //0=up;1=down;see manual page 31
#define INT_POLARITY_SHIFT      3               //0=up;1=down;see manual page 31

#define LSM_ACC_DATA_RATE       8               //
#define LSM_ACC_DATA_RATE_SHIFT 4
#define ACC_XYZ_ENABLE          7       //0=773HZ;1=194HZ;2=362HZ;3=50HZ;see manual page 34
#define ACC_XYZ_ENABLE_SHIFT    0       
#define ACC_FILTER_BANDWIDTH            0       //0=773HZ;1=194HZ;2=362HZ;3=50HZ;see manual page 35
#define ACC_FILTER_BANDWIDTH_SHIFT      6       
#define ACC_DATA_READY_INT              0       //0 for disable,1 for enable;see manual page 36
#define ACC_DATA_READY_INT_SHIFT        2
#define MAG_DATA_READY_INT              1       //0 for disable,1 for enable;see manual page 36
#define MAG_DATA_READY_INT_SHIFT        1
#define MAGNETIC_DATA_RATE              4       //0=3.125HZ;1=6.25HZ;2=12.5;3=25HZ;4=50HZ;5=100HZ(1);6=DO NOT USE;7=RESERVED;;see manual page 38
#define MAGNETIC_DATA_RATE_SHIFT        2
#define LSM_ACC_FULL_SCALE       3           //acc full-scale selection:0 for +-2g,1 for +-4 gauss,2=+-8,3=+-12;see manual page 38
#define LSM_ACC_FULL_SCALE_SHIFT 3
#define LSM_MAG_FULL_SCALE       0           //Magnetic full-scale selection:0 for +-2gauss,1 for +-4 gauss,2=+-8,3=+-12;see manual page 38
#define LSM_MAG_FULL_SCALE_SHIFT 6
#define LSM_MAG_SENSOR_MODE       0           //Magnetic sensor mode selection:0 for continuous-conversion mode,1=single-conversion mode,2,3=power-down mode;see manual page 39
#define LSM_MAG_SENSOR_MODE_SHIFT 0
// X
#define  OUT_X_L_M  0x08
#define  OUT_X_H_M  0x09
// Y
#define  OUT_Y_L_M  0x0A
#define  OUT_Y_H_M  0x0B
// Z
#define  OUT_Z_L_M  0x0C
#define  OUT_Z_H_M  0x0D



#define LSM303A_I2C_ADDR				((u8)0x32)
#define LSM303M_I2C_ADDR				((u8)0x3C)

#define LSM303A_CTRL_REG1				((u8)0x20)
#define LSM303A_CTRL_REG2				((u8)0x21)
#define LSM303A_CTRL_REG3				((u8)0x22)
#define LSM303A_CTRL_REG4				((u8)0x23)
#define LSM303A_CTRL_REG5				((u8)0x24)
#define LSM303A_HP_FILTER_RESET	((u8)0x25)
#define LSM303A_REFERENCE				((u8)0x26)
#define LSM303A_STATUS_REG			((u8)0x27)
#define LSM303A_OUT_X_L					((u8)0x28)
#define LSM303A_OUT_X_H					((u8)0x29)
#define LSM303A_OUT_Y_L					((u8)0x2A)
#define LSM303A_OUT_Y_H					((u8)0x2B)
#define LSM303A_OUT_Z_L					((u8)0x2C)
#define LSM303A_OUT_Z_H					((u8)0x2D)
#define LSM303A_INT1_CFG				((u8)0x30)
#define LSM303A_INT1_SOURCE			((u8)0x31)
#define LSM303A_INT1_INT1_THS		((u8)0x32)
#define LSM303A_INT1_DURATION		((u8)0x33)
#define LSM303A_INT2_CFG				((u8)0x34)
#define LSM303A_INT2_SOURCE			((u8)0x35)
#define LSM303A_INT2_THS				((u8)0x36)
#define LSM303A_INT2_DURATION		((u8)0x37)
#define LSM303A_MULTIPLE				((u8)0xA8)	// ³sÄòÅª¨ú¦a§}

#define LSM303M_CRA_REG					((u8)0x00)
#define LSM303M_CRB_REG					((u8)0x01)
#define LSM303M_MR_REG					((u8)0x02)
#define LSM303M_OUT_X_H					((u8)0x03)
#define LSM303M_OUT_X_L					((u8)0x04)
#define LSM303M_OUT_Z_H					((u8)0x05)
#define LSM303M_OUT_Z_L					((u8)0x06)
#define LSM303M_OUT_Y_H					((u8)0x07)
#define LSM303M_OUT_Y_L					((u8)0x08)
#define LSM303M_SR_REG					((u8)0x09)
#define LSM303M_IRA_REG					((u8)0x0A)
#define LSM303M_IRB_REG					((u8)0x0B)
#define LSM303M_IRC_REG					((u8)0x0C)
#define LSM303M_MULTIPLE				((u8)0x83)	// ³sÄòÅª¨ú¦a§}
/*=====================================================================================================*/

#define  LSM303DLHC_FS_1_3_GA               ((uint8_t) 0x20)  /*!< Full scale = ±1.3 Gauss */
#define  LSM303DLHC_FS_1_9_GA               ((uint8_t) 0x40)  /*!< Full scale = ±1.9 Gauss */
#define  LSM303DLHC_FS_2_5_GA               ((uint8_t) 0x60)  /*!< Full scale = ±2.5 Gauss */
#define  LSM303DLHC_FS_4_0_GA               ((uint8_t) 0x80)  /*!< Full scale = ±4.0 Gauss */
#define  LSM303DLHC_FS_4_7_GA               ((uint8_t) 0xA0)  /*!< Full scale = ±4.7 Gauss */
#define  LSM303DLHC_FS_5_6_GA               ((uint8_t) 0xC0)  /*!< Full scale = ±5.6 Gauss */
#define  LSM303DLHC_FS_8_1_GA               ((uint8_t) 0xE0)  /*!< Full scale = ±8.1 Gauss */



#define LSM303DLHC_M_SENSITIVITY_XY_1_3Ga     1100  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_1_9Ga     855   /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_2_5Ga     670   /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4Ga       450   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4_7Ga     400   /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_5_6Ga     330   /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_8_1Ga     230   /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_3Ga      980   /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_9Ga      760   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_2_5Ga      600   /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4Ga        400   /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4_7Ga      355   /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_5_6Ga      295   /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_8_1Ga      205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */


#define LSM_Acc_Sensitivity_2g     (float)     1            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     2           /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     4           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     12         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */

//#define PI                         (float)     3.14159265f


#define LSM303DLHC_FULLSCALE_2G            ((uint8_t)0x00)  /*!< ±2 g */
#define LSM303DLHC_FULLSCALE_4G            ((uint8_t)0x10)  /*!< ±4 g */
#define LSM303DLHC_FULLSCALE_8G            ((uint8_t)0x20)  /*!< ±8 g */
#define LSM303DLHC_FULLSCALE_16G           ((uint8_t)0x30)  /*!< ±16 g */


/*=====================================================================================================*/
char LSM303D_Init();
void LSM303A_Init( void );
void LSM303M_Init( void );
void LSM303M_Raed(short *pcompass);
void LSM303A_Raed(short *pacc);

float Data_conversion(float *AccBuffer,float *MagBuffer);
/*=====================================================================================================*/
#endif
