#ifndef _MPU6050_H_
#define _MPU6050_H_     1

#ifdef  HARD_IIC
#define	MPU6050_Addr    0x68	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改  11010000
#else
#define	MPU6050_Addr    0xD0	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改  11010000
#endif
/*************************定义MPU6050内部地址**********************************/
#define	SMPLRT_DIV	0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	MPU6050_CONFIG	0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG	0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	        //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define I2C_MST_STATUS  0x36    
#define INT_PIN_CFG     0x37    //manual @P28
#define INT_ENABLE      0x38    //manual @P29
#define I2C_SLV4_DONE   (I2C_MST_STATUS & 64)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H	0x41
#define	TEMP_OUT_L	0x42

#define	GYRO_XOUT_H	0x43
#define	GYRO_XOUT_L	0x44	
#define	GYRO_YOUT_H	0x45
#define	GYRO_YOUT_L	0x46
#define	GYRO_ZOUT_H	0x47
#define	GYRO_ZOUT_L	0x48

#define	USER_CTRL 	0x6A	        //电源管理，典型值：0x00(正常启樱，内部8MHz晶振)      0x80,失能温度传感器
#define	PWR_MGMT_1	0x6B	        //电源管理，典型值：0x00(正常启樱，内部8MHz晶振)      0x80,失能温度传感器
#define	WHO_AM_I	0x75	        //IIC地址寄存器(默认数值0x68，只读)  
/*********************DATA UNIT & scale definition****************************/
//16bit signed data scale from -32768 to 32767
//Full scale Range of the MPU6050 Gyroscope; ref:register manual[Page:14];
//example:full scale 250 degree/second means that if gyroscope data is 32767(max signed 16bit) reprent rotation speed if 250 degree/second
#define MPU6050GYRO_SCALE_250_DPS  (0.0076294f)  // 0.0087500 dps/LSB
#define MPU6050GYRO_SCALE_500_DPS  (0.0152592f)  // 0.0175000 dps/LSB
#define MPU6050GYRO_SCALE_1000_DPS (0.0305185f)  // 0.0175000 dps/LSB
#define MPU6050GYRO_SCALE_2000_DPS (0.0610370f)  // 0.0700000 dps/LSB
#define MPU6050GYRO_SCALE_DEG       MPU6050GYRO_SCALE_2000_DPS      //this definition is used during the whole process
//Full scale Range of the MPU6050 Accelerometer; ref:register manual[Page:15];
//example:full scale 2g means that if Accelerometer data is 32767(max signed 16bit) reprent accelerated speed is 2g m/s^2,which g = 9.8m/s^2
#define MPU6050ACC_SCALE_2G    (0.00006104f)  // 0.00006250 g/LSB
#define MPU6050ACC_SCALE_4G    (0.00012207f)  // 0.00012500 g/LSB
#define MPU6050ACC_SCALE_8G    (0.00024415f)  // 0.00025000 g/LSB
#define MPU6050ACC_SCALE_16G   (0.00048830f)  // 0.00025000 g/LSB
#define MPU6050ACC_SCALE_G      MPU6050ACC_SCALE_16G      //this definition is used during the whole process

//#define ADC_1G          4095
//#define GyroAD_Limit    10
/************************&&&&**hardware definition****************************/
#define MPUINT_PIN      PC14
#define MPUINT_Line     EXTI_Line14

//#define CALIBRATING_GYRO_CYCLES             1000
//#define CALIBRATING_ACC_CYCLES              400

typedef struct
{
  short x, y, z;
  short x_offset, y_offset, z_offset;
}MPU6050_GYRO,*MPU6050_PGYRO;

#define   MPU_INT_EN(n) Single_Write(MPU6050_Addr, INT_ENABLE, n)

extern BOOL MPUInit(void);
void MPUReadAcc(short *pacc);
void MPUReadGyr(short *pgyro);
char MPU6050GyroCalibration(short *PGYRO);

#endif