#ifndef _MPU6050_H_
#define _MPU6050_H_     1

#ifdef  HARD_IIC
#define	MPU6050_Addr    0x68	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�  11010000
#else
#define	MPU6050_Addr    0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�  11010000
#endif
/**************����MPU6050�ڲ���ַ********************/
#define	SMPLRT_DIV	0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	MPU6050_CONFIG	0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG	0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	        //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

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

#define	USER_CTRL 	0x6A	        //��Դ����������ֵ��0x00(������ӣ���ڲ�8MHz����)      0x80,ʧ���¶ȴ�����
#define	PWR_MGMT_1	0x6B	        //��Դ����������ֵ��0x00(������ӣ���ڲ�8MHz����)      0x80,ʧ���¶ȴ�����
#define	WHO_AM_I	0x75	        //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)  

#define MPU6050A_2mg    ((float)0.00006103f)  // 0.00006250 g/LSB
#define MPU6050A_4mg    ((float)0.00012207f)  // 0.00012500 g/LSB
#define MPU6050A_8mg    ((float)0.00024414f)  // 0.00025000 g/LSB

#define MPU6050G_s250dps  ((float)0.0076335f)  // 0.0087500 dps/LSB
#define MPU6050G_s500dps  ((float)0.0152671f)  // 0.0175000 dps/LSB
#define MPU6050G_s2000dps ((float)0.0609756f)  // 0.0700000 dps/LSB

#define ADC_1G          4095
#define GyroAD_Limit    10

#define MPUINT_PIN      PE8

#define CALIBRATING_GYRO_CYCLES             1000
#define CALIBRATING_ACC_CYCLES              400


extern void MPUInit(void);
void MPUReadAcc();
void MPUReadGyr();
uint8 GyroCalibration(void);

#endif