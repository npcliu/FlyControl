#ifndef _L3GD20_H_
#define _L3GD20_H_

#define L3GD20_SPI      SPI3

#define GYRO_CS_PIN     PB8
#define GYRO_CS_OUT     PB8_OUT

#define GYRO_INT_PIN     PB9
#define GYRO_IRQ_Line    EXTI_Line9
#define GYRO_IRQn        EXTI9_5_IRQn

//GPIO_InitTypeDef GPIO_Initure;

//#define GYRO_CS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
//#define GYRO_CS_PIN GPIO_PIN_1
//#define GYRO_CS_GPIO_PORT GPIOC    
//#define GYRO_CS_HIGH GYRO_CS_GPIO_PORT->BSRR=1<<1
//#define GYRO_CS_LOW GYRO_CS_GPIO_PORT->BSRR=1<<17
//
//#define GYRO_INT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
//#define GYRO_INT_GPIO_PORT GPIOA
//#define GYRO_INT1_PIN GPIO_PIN_1
//#define GYRO_INT2_PIN GPIO_PIN_2

#define L3GD20_ID                     0xD4
#define L3GD20_ADDR_ID                0x0F
#define L3GD20_PWR_ON                 0x3F   
#define L3GD20_CTRL_REG1_ADDR         0x20
#define L3GD20_CTRL_REG2_ADDR         0x21
#define L3GD20_CTRL_REG3_ADDR         0x22
#define L3GD20_CTRL_REG4_ADDR         0x23
#define L3GD20_CTRL_REG5_ADDR         0x23
#define L3GD20_OUT_X_L_ADDR           0x28 
#define L3GD20_FULLSCALE_250          ((uint8_t)0x00)
#define L3GD20_FULLSCALE_500          ((uint8_t)0x10)
#define L3GD20_FULLSCALE_2000         ((uint8_t)0x20) 
#define L3GD20_FULLSCALE_SELECTION    ((uint8_t)0x30)
#define L3GD20_SENSITIVITY_250DPS     ((float)8.75f)         
/*gyroscope sensitivity with 250 dps full scale [DPS/LSB]  */
#define L3GD20_SENSITIVITY_500DPS     ((float)17.50f)        
/*gyroscope sensitivity with 500 dps full scale [DPS/LSB]  */
#define L3GD20_SENSITIVITY_2000DPS    ((float)70.00f)        
/*gyroscope sensitivity with 2000 dps full scale [DPS/LSB] */
#define L3GD20_FILTER                0x00   
#define L3GD20_FILTER_ON             0x10
#define L3GD20_ADR_X_LO              0x28
#define L3GD20_ADR_X_HI              0x29
#define L3GD20_ADR_Y_LO              0x2A
#define L3GD20_ADR_Y_HI              0x2B
#define L3GD20_ADR_Z_LO              0x2C
#define L3GD20_ADR_Z_HI              0x2D

char L3GD20_Init(uint8 L3gd20_full_scale);
void L3GD20_Read(short *pgyro);
char L3GD20GyroCalibration(short *PGYRO);


#endif
