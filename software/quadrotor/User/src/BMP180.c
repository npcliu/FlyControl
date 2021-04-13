#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "MyIIC.h"
#include "delay.h"
#include "math.h"
#include "bmp180.h"
#include "filter.h"
//2������IIC?e��?D?o?
void BMP_IIC_Start(void)
{
	MSDA_MOD = 0x7;      //sda??��?3?
	MSDA_OUT=1;	  	  
	MSCL_OUT=1;
	delay_us(1);
 	MSDA_OUT=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	MSCL_OUT=0;//?����?I2C����??��?��?��?����?��?��?����?��y?Y 
}	  
//2������IIC����?1D?o?
void BMP_IIC_Stop(void)
{
	MSDA_MOD = 0x7;//sda??��?3?
	MSCL_OUT=0;
	MSDA_OUT=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	MSCL_OUT=1; 
	MSDA_OUT=1;//����?��I2C����???����?D?o?
	delay_us(1);							   	
}
//�̨���y��|��eD?o?��?����
//����???�̡�o1��??����?��|��e����㨹
//        0��??����?��|��e3��1|
u8 BMP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MSDA_MOD = 0x4;       //SDA����???a��?��?  
	MSDA_OUT=1;delay_us(1);	   
	MSCL_OUT=1;delay_us(1);	 
	while(MSDA_IN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			BMP_IIC_Stop();
			return 1;
		}
	}
	MSCL_OUT=0;//����?����?3?0 	   
	return 0;  
} 
//2������ACK��|��e
void BMP_IIC_Ack(void)
{
	MSCL_OUT=0;
	MSDA_MOD = 0x7; 
	MSDA_OUT=0;
	delay_us(1);
	MSCL_OUT=1;
	delay_us(1);
	MSCL_OUT=0;
}
//2?2������ACK��|��e		    
void BMP_IIC_NAck(void)
{
	MSCL_OUT=0;
	MSDA_MOD = 0x7; 
	MSDA_OUT=1;
	delay_us(1);
	MSCL_OUT=1;
	delay_us(1);
	MSCL_OUT=0;
}					 				     
//IIC����?����???��??��
//����??�䨮?����D?T��|��e
//1��?��D��|��e
//0��??T��|��e			  
void BMP_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MSDA_MOD = 0x7;  	    
    MSCL_OUT=0;//��-�̨�����?��?a��?��y?Y��?��?
    for(t=0;t<8;t++)
    {              
        //MSDA_OUT=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			MSDA_OUT=1;
		else
			MSDA_OUT=0;
		txd<<=1; 	  
		delay_us(1);   //??TEA5767?a��y???������??��?��?D?��?
		MSCL_OUT=1;
		delay_us(1); 
		MSCL_OUT=0;	
		delay_us(1);
    }	 
} 	    
//?��1??��??����?ack=1������?����?��ACK��?ack=0��?����?��nACK   
u8 BMP_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MSDA_MOD = 0x4;//SDA����???a��?��?
    for(i=0;i<8;i++ )
	{
        MSCL_OUT=0; 
        delay_us(1);
		MSCL_OUT=1;
        receive<<=1;
        if(MSDA_IN)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        BMP_IIC_NAck();//����?��nACK
    else
        BMP_IIC_Ack(); //����?��ACK   
    return receive;
}

#define BMP180_ADDR 0XEE
#define BMP180_oss 3//ȡֵλ0��3��3��ʾ�߾���
 //�洢BMP180���ݵĽṹ
_bmp180 bmp180;

//дһ�����ݵ�BMP180
void BMP_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(WriteAddr);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(DataToWrite);
	BMP_IIC_Wait_Ack();
	BMP_IIC_Stop();
}

//��BMP180��һ���ֽ�����
uint8_t BMP_ReadOneByte(uint8_t ReadAddr)
{
	uint8_t data = 0;
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(ReadAddr);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR+1);
	BMP_IIC_Wait_Ack();
	
	data = BMP_IIC_Read_Byte(0);
	BMP_IIC_Stop();
	
	return data;
}

//��BMP180��һ��16λ������
short BMP_ReadTwoByte(uint8_t ReadAddr)
{
	short data;
	uint8_t msb,lsb;
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(ReadAddr);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR+1);
	BMP_IIC_Wait_Ack();
	
	msb = BMP_IIC_Read_Byte(1);
	lsb = BMP_IIC_Read_Byte(0);
	
	BMP_IIC_Stop();
	
	data = msb <<8 | lsb;
	
	return data;
}

long BMP_ReadThreeByte(uint8_t ReadAddr)
{
	long data;
	uint8_t msb,lsb,xlsb;
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(ReadAddr);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(BMP180_ADDR+1);
	BMP_IIC_Wait_Ack();
	
	msb = BMP_IIC_Read_Byte(1);
        lsb = BMP_IIC_Read_Byte(1);
	xlsb = BMP_IIC_Read_Byte(0);
	
	BMP_IIC_Stop();
	
	data = (msb <<16 | lsb << 8 | xlsb) >> (8 - BMP180_oss);
	
	return data;
}


//��BMP180�Ļ�ȡ�������
void BMP_ReadCalibrationData(void)
{
	bmp180.AC1 = BMP_ReadOneByte(0xAA) << 8 | BMP_ReadOneByte(0xAB);
	bmp180.AC2 = BMP_ReadOneByte(0xAC) << 8 | BMP_ReadOneByte(0xAD);
	bmp180.AC3 = BMP_ReadOneByte(0xAE) << 8 | BMP_ReadOneByte(0xAF);
	bmp180.AC4 = BMP_ReadOneByte(0xB0) << 8 | BMP_ReadOneByte(0xB1);
	bmp180.AC5 = BMP_ReadOneByte(0xB2) << 8 | BMP_ReadOneByte(0xB3);
	bmp180.AC6 = BMP_ReadOneByte(0xB4) << 8 | BMP_ReadOneByte(0xB5);
	bmp180.B1  = BMP_ReadOneByte(0xB6) << 8 | BMP_ReadOneByte(0xB7);
	bmp180.B2  = BMP_ReadOneByte(0xB8) << 8 | BMP_ReadOneByte(0xB9);
	bmp180.MB  = BMP_ReadOneByte(0xBA) << 8 | BMP_ReadOneByte(0xBB);
	bmp180.MC  = BMP_ReadOneByte(0xBC) << 8 | BMP_ReadOneByte(0xBD);
	bmp180.MD  = BMP_ReadOneByte(0xBE) << 8 | BMP_ReadOneByte(0xBF);
}


//long BMP_Read_UT(void)
//{
//	long temp = 0;
//	BMP_WriteOneByte(0xF4,0x2E);
//	
//	DelayMs(5);
//	temp = (long)BMP_ReadTwoByte(0xF6);
//	return temp;
//}
//
////�䨮BMP180?����??��DT?y��?�䨮???1
//long BMP_Read_UP(void)
//{
//	long pressure = 0;
//	
//	BMP_WriteOneByte(0xF4,0x34|(BMP180_oss<<6));
//	DelayMs(26);
//        pressure = (long)BMP_ReadThreeByte(0xF6);
//	
//	//pressure = (long)BMP_ReadTwoByte(0xF6);
//
//	pressure &= 0x0007FFFF;
//	
//	return pressure;
//}
//
////��???��?��?2?��y?????��o���䨮???1??DDDT?y��?2��????o���?
//void BMP_UncompemstatedToTrue(void)
//{
//	bmp180.UT = BMP_Read_UT();//�̨���?��??����?�䨪?��
//	bmp180.UT = BMP_Read_UT();//??DD�̨�?t��??����?DT?y2?��y
//	bmp180.UP = BMP_Read_UP();
//	
//	bmp180.X1 = ((bmp180.UT - bmp180.AC6) * bmp180.AC5) >> 15;
//	bmp180.X2 = (((long)bmp180.MC) << 11) / (bmp180.X1 + bmp180.MD);
//	bmp180.B5 = bmp180.X1 + bmp180.X2;
//	bmp180.Temp  = (bmp180.B5 + 8) >> 4;
//	
//	bmp180.B6 = bmp180.B5 - 4000;
//	bmp180.X1 = ((long)bmp180.B2 * (bmp180.B6 * bmp180.B6 >> 12)) >> 11;
//	bmp180.X2 = ((long)bmp180.AC2) * bmp180.B6 >> 11;
//	bmp180.X3 = bmp180.X1 + bmp180.X2;
//	
//	bmp180.B3 = ((((long)(bmp180.AC1 * 4 + bmp180.X3))<<BMP180_oss) + 2) /4;//�ӷ����ȼ�����λ�ߣ���������һ��Ҫ�����ţ���Ȼ���Ľ���ʹ���
//	bmp180.X1 = ((long)bmp180.AC3) * bmp180.B6 >> 13;
//	bmp180.X2 = (((long)bmp180.B1) *(bmp180.B6*bmp180.B6 >> 12)) >>16;
//	bmp180.X3 = ((bmp180.X1 + bmp180.X2) + 2) >> 2;
//	bmp180.B4 = ((long)bmp180.AC4) * (unsigned long)(bmp180.X3 + 32768) >> 15;
//	bmp180.B7 = ((unsigned long)bmp180.UP - bmp180.B3) * (50000>>BMP180_oss);//
//	
//	if(bmp180.B7 < 0x80000000)
//	{
//		bmp180.p = (bmp180.B7 * 2) / bmp180.B4;		
//	}
//	else
//	{
//		bmp180.p = (bmp180.B7 / bmp180.B4) * 2;
//	}
//	
//	bmp180.X1 = (bmp180.p >> 8) * (bmp180.p >>8);
//	bmp180.X1 = (((long)bmp180.X1) * 3038) >> 16;
//	bmp180.X2 = (-7357 * bmp180.p) >> 16;
//	
//	bmp180.p = bmp180.p + ((bmp180.X1 + bmp180.X2 + 3791) >> 4);
//	
//	bmp180.altitude = 44330 * (1-pow(((bmp180.p) / 101325.0),(1.0/5.255)));  
//}
//

KalmanInfo kalmanFilter_of_bmp180_altitude;
//�û�ȡ�Ĳ������¶Ⱥʹ���ѹ���������������㺣�Σ��������Ӧ�÷ŵ�5ms�ж��в�������ִ��
void BMP_UncompemstatedToTrue(void)
{
  static uint8 geted_bmp180_altitude_count = 0;//��¼��ø߶ȵĴ���
  static char UT_time_count_flag = 0;
  static char read_UT_count = 0;
  long temp = 0;
  static char temp_use_flag = 1;//temp�����Ƿ�ʹ�ñ�־Ϊ��Ĭ��ʹ�á�ֻ������temp���ݲ����ٴζ�UT
  static char UP_time_count_flag = 0;
  
  //bmp180.UT = BMP_Read_UT();//��һ�ζ�ȡ����
  //bmp180.UT = BMP_Read_UT();//���еڶ��ζ�ȡ��������
  //bmp180.UP = BMP_Read_UP();

  if(UT_time_count_flag == 0&&(temp_use_flag))
    BMP_WriteOneByte(0xF4,0x2E);
  
  UT_time_count_flag ++;
  if(UT_time_count_flag >= 2&&(temp_use_flag))//�ⲿ�ж�5ms�ж�һ�Σ�����5ms��һ��
  {
    UT_time_count_flag = 0;
    temp = (long)BMP_ReadTwoByte(0xF6);
    read_UT_count++;
    
  }
  if(read_UT_count >= 1)//UTҪ�����β��ܶ�����ȷ���ݣ������ʾֻȡ�ڶ��ε���ֵ�������
  {
    if(temp_use_flag)
      bmp180.UT = temp;//���ڶ�����ȷ��UT�����ˣ����Զ�UP��
    temp_use_flag = 0;//�ջ�ȡ��TEMP,��δʹ��
    if(UP_time_count_flag == 0)
      BMP_WriteOneByte(0xF4,(0x34 | (BMP180_oss<<6)));
    UP_time_count_flag ++;
    if(UP_time_count_flag >= 3+BMP180_oss)
    {
      bmp180.UP = BMP_ReadThreeByte(0xF6);
      //bmp180.UP &= 0x000FFFFF;//UP��UT�������ˣ����Խ��м�����
      
      bmp180.X1 = ((bmp180.UT - bmp180.AC6) * bmp180.AC5) >> 15;
      bmp180.X2 = (((long)bmp180.MC) << 11) / (bmp180.X1 + bmp180.MD);
      bmp180.B5 = bmp180.X1 + bmp180.X2;
      bmp180.Temp  = (bmp180.B5 + 8) >> 4;
      
      bmp180.B6 = bmp180.B5 - 4000;
      bmp180.X1 = ((long)bmp180.B2 * (bmp180.B6 * bmp180.B6 >> 12)) >> 11;
      bmp180.X2 = ((long)bmp180.AC2) * bmp180.B6 >> 11;
      bmp180.X3 = bmp180.X1 + bmp180.X2;
      
      bmp180.B3 = (((((long)bmp180.AC1) * 4 + bmp180.X3)<<BMP180_oss) + 2) /4;////�ӷ����ȼ�����λ�ߣ���������һ��Ҫ�����ţ���Ȼ���Ľ���ʹ���
      bmp180.X1 = ((long)bmp180.AC3) * bmp180.B6 >> 13;
      bmp180.X2 = (((long)bmp180.B1) *(bmp180.B6*bmp180.B6 >> 12)) >>16;
      bmp180.X3 = ((bmp180.X1 + bmp180.X2) + 2) >> 2;
      bmp180.B4 = ((long)bmp180.AC4) * (unsigned long)(bmp180.X3 + 32768) >> 15;
      bmp180.B7 = ((unsigned long)bmp180.UP - bmp180.B3) * (50000>>BMP180_oss);
      
      if(bmp180.B7 < 0x80000000)
      {
              bmp180.p = (bmp180.B7 * 2) / bmp180.B4;		
      }
      else
      {
              bmp180.p = (bmp180.B7 / bmp180.B4) * 2;
      }
      
      bmp180.X1 = (bmp180.p >> 8) * (bmp180.p >>8);
      bmp180.X1 = (((long)bmp180.X1) * 3038) >> 16;
      bmp180.X2 = (-7357 * bmp180.p) >> 16;
      
      bmp180.p = bmp180.p + ((bmp180.X1 + bmp180.X2 + 3791) >> 4);
      
      bmp180.altitude = 44330 * (1-pow(((bmp180.p) / 101325.0),(1.0/5.255)));  
      //printf("%f ",bmp180.altitude);
      
      //���һ�μ���߶��������б�־λ���㣬׼����һ�μ���
      UP_time_count_flag = 0;
      UT_time_count_flag = 0;
      temp_use_flag = 1;//temp��ʹ����
      read_UT_count = 0;
      
      //ȡ200��������ƽ����Ϊ��ʼ�߶�
      static float sum_of_bmp180_altitude = 0;
      if(geted_bmp180_altitude_count<=50)
        geted_bmp180_altitude_count++;
      if(geted_bmp180_altitude_count>50&&(geted_bmp180_altitude_count<=250))
      {
        sum_of_bmp180_altitude += bmp180.altitude;
        geted_bmp180_altitude_count++;
      }
      else if(geted_bmp180_altitude_count>250)
      {
        gc[3][6] = bmp180.altitude_init = sum_of_bmp180_altitude/200;
      }
      gc[3][7] = bmp180.altitude = KalmanFilter(&kalmanFilter_of_bmp180_altitude, bmp180.altitude);
      
    }    
  }

}
