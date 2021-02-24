
//#include "usart.h"
//#include "I2C.h"
#include  <math.h>    //Keil library  
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_MY_spi.h"
#include "delay.h"
#include "LSM303.h"
#include "define.h"





uint8 LSM303WriteReg(uint8 reg, uint8 dat)
{
  uint8 buff[2];
  
  buff[0] = reg;          //先?送寄存器
  buff[1] = dat;          //再?送?据
  
  LSM303_CS_OUT = 0;
  spi_mosi(LSM303_SPI, buff, buff, 2); //?送buff里?据，并采集到 buff里
  LSM303_CS_OUT = 1;
  
  /*返回??寄存器的值*/
  return buff[0];
}
//brief：读取1个Byte（8bit）
//parameter：reg，待读寄存器
//return：reg的内容
uint8 LSM303ReadReg(uint8 reg)
{
  uint8 buff[2];
  
  buff[0] = reg|0x80;
  buff[1] = 0;
//    PB(B,15,CRL) = 1;           //设置SCK引脚为复用
  LSM303_CS_OUT = 0;                    //拉低LSM片选信号
  spi_mosi(LSM303_SPI, buff, buff, 2); //此间16个clk，2个byte交换，读取之数据位于buff[1]
  LSM303_CS_OUT = 1;                    //拉高LSM片选信号
//    PB3_OUT = 1;                //设置SCK引脚电平为1
//    PB(B,15,CRL) = 0;           //设置SCK引脚为推挽输出
  
  /*返回寄存器的值*/
  return buff[1];
}

char LSM303D_Init()
{//IRQ=OD,INT_POLARITY=1,NEED PULL UP,TRIGGERD AT FALLING EDGE
//    gpio_init(LSM303_ACC_DRDY_PIN, GPIO_Mode_IPU, interupt,GPIO_Speed_50MHz,0);                              //初始化IRQ管脚为输入
//  gpio_int_cfg(LSM303_ACC_DRDY_PIN,LSM303_ACC_DRDY_Line,EXTI_Trigger_Falling);

  if(I_AM_LSM303D!=LSM303ReadReg(LSM303D_WHO_AM_I))
    return 1;
//  LSM303WriteReg(INT_CTRL_M,INT_POLARITY<<INT_POLARITY_SHIFT
//                 |(LSM_INT_PIN_CFG<<INT_PIN_CFG_SHIFT));
/********************************acc set********************************/  
  LSM303WriteReg(CTRL1,(LSM_ACC_DATA_RATE<<LSM_ACC_DATA_RATE_SHIFT)             //set ACC data rate = 400Hz
                        |ACC_XYZ_ENABLE<<ACC_XYZ_ENABLE_SHIFT);                 //enable acc of x,y,z
  LSM303WriteReg(CTRL2,(ACC_FILTER_BANDWIDTH<<ACC_FILTER_BANDWIDTH_SHIFT)       //filter bandwidth = 773HZ
                 |(LSM_ACC_FULL_SCALE<<LSM_ACC_FULL_SCALE_SHIFT));              //acc full scale = +-12g
//  LSM303WriteReg(CTRL3,ACC_DATA_READY_INT<<ACC_DATA_READY_INT_SHIFT             //enable acc data ready INT
//                 |MAG_DATA_READY_INT<<MAG_DATA_READY_INT_SHIFT);           //
//  LSM303WriteReg(CTRL4,0x04);
/********************************compass set********************************/  
  LSM303WriteReg(CTRL5,MAGNETIC_DATA_RATE<<MAGNETIC_DATA_RATE_SHIFT);           //magnetic data rate=100Hz
  LSM303WriteReg(CTRL6,LSM_MAG_FULL_SCALE<<LSM_MAG_FULL_SCALE_SHIFT);           //magnetic full scale=+-2gauss
  LSM303WriteReg(CTRL7,LSM_MAG_SENSOR_MODE<<LSM_MAG_SENSOR_MODE_SHIFT);         //magnetic sensor mode = continuous
  
//  LSM303WriteReg(INT_CTRL_M,0xE1);
  return 0;
}


//************初始化L3G4200D的加速度*********************************
void LSM303A_Init( void )
{
//    
//  I2C_WriteOneByte(I2C1,LSM303A_I2C_ADDR,LSM303A_CTRL_REG1, 0x37);
//  I2C_WriteOneByte(I2C1,LSM303A_I2C_ADDR,LSM303A_CTRL_REG4, 0x00);
  	u8 LSM303A_Init_Data[2] = {
			0x37,	/* LSM303A_CTRL_REG1	NormalMode, ODR:400Hz, XYZEnable */
			0x10	/* LSM303A_CTRL_REG4	Range:+-4g                       */
		};

//	I2C_DMA_WriteReg(LSM303A_Init_Data,   LSM303A_I2C_ADDR, LSM303A_CTRL_REG1, 1);Delay_1ms(1);
//	I2C_DMA_WriteReg(LSM303A_Init_Data+1, LSM303A_I2C_ADDR, LSM303A_CTRL_REG4, 1);Delay_1ms(1);
                LSM303WriteReg(LSM303A_CTRL_REG1,LSM303A_Init_Data[0]);
                LSM303WriteReg(LSM303A_CTRL_REG4,LSM303A_Init_Data[1]);

}
//************初始化L3G4200D的磁感应*********************************

void LSM303M_Init( void )
{

//	I2C_WriteOneByte(I2C1,LSM303M_I2C_ADDR,LSM303M_CRB_REG, 0xa0);
//	
//	I2C_WriteOneByte(I2C1,LSM303M_I2C_ADDR,LSM303M_MR_REG, 0x00);
  	u8 LSM303M_Init_Data[2] = {
			0x20,	/* LSM303M_CRB_REG	Range:+-1.3Gauss           */
			0x00	/* LSM303M_MR_REG		Continuous-conversion mode */
		};
//	I2C_DMA_WriteReg(LSM303M_Init_Data,   LSM303M_I2C_ADDR, LSM303M_CRB_REG, 1);Delay_1ms(1);
//	I2C_DMA_WriteReg(LSM303M_Init_Data+1, LSM303M_I2C_ADDR, LSM303M_MR_REG,  1);Delay_1ms(1);
        LSM303WriteReg(LSM303M_CRB_REG,LSM303M_Init_Data[0]);
        LSM303WriteReg(LSM303M_MR_REG,LSM303M_Init_Data[1]);

}

//********读取磁感应数据************************************

void LSM303M_Raed(short *pcompass)
{
        uint8 compassbuff[2] = {0};            //one byte buffer for receiving data from slave
  compassbuff[0] = LSM303ReadReg(OUT_X_L_M);
  compassbuff[1] = LSM303ReadReg(OUT_X_H_M);
  pcompass[0] = ((compassbuff[1]<<8)|compassbuff[0]);//HMC5883.x_off;
  compassbuff[0] = LSM303ReadReg(OUT_Y_L_M);
  compassbuff[1] = LSM303ReadReg(OUT_Y_H_M);
  pcompass[1] = ((compassbuff[1]<<8)|compassbuff[0]);//HMC5883.y_off;
  compassbuff[0] = LSM303ReadReg(OUT_Z_L_M);
  compassbuff[1] = LSM303ReadReg(OUT_Z_H_M);
  pcompass[2] = ((compassbuff[1]<<8)|compassbuff[0]);//Compass.z_off;

//  switch(f & 0xE0)
//  {
//  case LSM303DLHC_FS_1_3_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_3Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_3Ga;
//    break;
//  case LSM303DLHC_FS_1_9_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_9Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_9Ga;
//    break;
//  case LSM303DLHC_FS_2_5_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_2_5Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_2_5Ga;
//    break;
//  case LSM303DLHC_FS_4_0_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4Ga;
//    break;
//  case LSM303DLHC_FS_4_7_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4_7Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4_7Ga;
//    break;
//  case LSM303DLHC_FS_5_6_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_5_6Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_5_6Ga;
//    break;
//  case LSM303DLHC_FS_8_1_GA:
//    Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;
//    Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;
//    break;
//  }
//  
 /* 
  for(i=0; i<2; i++)
  {
		cen=(int16_t)(((int16_t)buffer[2*i] << 8) + buffer[2*i+1]) ;
    Data[i]=(float)(cen)*1000/Magn_Sensitivity_XY;
  
  }
	*/
//	cen=(int16_t)(((uint16_t)buffer[0] << 8) + buffer[1]);
//  Data[0]=(float)((int16_t)cen)*1000/Magn_Sensitivity_Z;
//	
//  cen=(int16_t)(((uint16_t)buffer[2] << 8) + buffer[3]);
//  Data[2]=(float)((int16_t)cen)*1000/Magn_Sensitivity_Z;
//
//	cen=(int16_t)(((uint16_t)buffer[4] << 8) + buffer[5]);
//  Data[1]=(float)((int16_t)cen)*1000/Magn_Sensitivity_Z;
  /*
	cen=Data[1];
	Data[1]=Data[2];
	Data[2]=cen;
	*/
}
//********读取加速度数据************************************
void LSM303A_Raed(short *pacc)
{
  //  static unsigned char buffer[6]={0}, ctrlx[2],i; 
  //	unsigned char cDivider;
  //  signed short  int pnRawData[3];
  //	float LSM_Acc_Sensitivity=LSM_Acc_Sensitivity_2g;
  //	int16_t cen;
  //	I2C_Read(I2C1,LSM303A_I2C_ADDR,LSM303A_OUT_X_L|0x80,buffer,6);
  uint8 compassbuff[2] = {0};
  compassbuff[0] = LSM303ReadReg(LSM303A_OUT_X_L);
  compassbuff[1] = LSM303ReadReg(LSM303A_OUT_X_H);
  pacc[0]  = ((compassbuff[1]<<8)|compassbuff[0]);
  compassbuff[0] = LSM303ReadReg(LSM303A_OUT_Y_L);
  compassbuff[1] = LSM303ReadReg(LSM303A_OUT_Y_H);
  pacc[1] = ((compassbuff[1]<<8)|compassbuff[0]);
  compassbuff[0] = LSM303ReadReg(LSM303A_OUT_Z_L);
  compassbuff[1] = LSM303ReadReg(LSM303A_OUT_Z_H);
  pacc[2] = ((compassbuff[1]<<8)|compassbuff[0]);

  //	I2C_Read(I2C1,LSM303A_I2C_ADDR,LSM303A_CTRL_REG4|0x80,ctrlx,2);
  
  //  if(ctrlx[1]&0x40)
  //    cDivider=64;
  //  else
  //    cDivider=16;
  
  /* check in the control register4 the data alignment*/
//  if(!(ctrlx[0] & 0x40) || (ctrlx[1] & 0x40)) /* Little Endian Mode or FIFO mode */
//  {
//    for(i=0; i<3; i++)
//    {
//			cen=(int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i];
//      pnRawData[i]=(int16_t)(cen)/cDivider;
//     
//    }
//  }
//  else /* Big Endian Mode */
//  {
//    for(i=0; i<3; i++)
//      pnRawData[i]=((int16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i+1])/cDivider;
//  }
  /* Read the register content */
//  I2C_Read(I2C1,LSM303A_I2C_ADDR,LSM303A_CTRL_REG4,ctrlx,2);


//  if(ctrlx[1]&0x40)
//  {
//    /* FIFO mode */
//    LSM_Acc_Sensitivity = 0.25;
//  }
//  else
//  {
//    /* normal mode */
//    /* switch the sensitivity value set in the CRTL4*/
//    switch(ctrlx[0] & 0x30)
//    {
//    case LSM303DLHC_FULLSCALE_2G:
//      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
//      break;
//    case LSM303DLHC_FULLSCALE_4G:
//      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
//      break;
//    case LSM303DLHC_FULLSCALE_8G:
//      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
//      break;
//    case LSM303DLHC_FULLSCALE_16G:
//      LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
//      break;
//    }
//  }

  /* Obtain the mg value for the three axis */
//  for(i=0; i<3; i++)
//  {
//    Data[i]=(float)pnRawData[i]*LSM_Acc_Sensitivity;
//  }
}


/********************************************/
float Data_conversion(float *AccBuffer,float *MagBuffer)
{
  unsigned char i;
	float HeadingValue = 0.0f;
	float fNormAcc,fSinRoll,fCosRoll,fSinPitch,fCosPitch = 0.0f;
  float fTiltedX,fTiltedY = 0.0f;
	float fcosf=0;
	      for(i=0;i<3;i++)
        AccBuffer[i] /= 100.0f;
      
      fNormAcc = sqrt((AccBuffer[0]*AccBuffer[0])+(AccBuffer[1]*AccBuffer[1])+(AccBuffer[2]*AccBuffer[2]));
      
      fSinRoll = AccBuffer[1]/fNormAcc;
      fCosRoll = sqrt(1.0-(fSinRoll * fSinRoll));
      fSinPitch = AccBuffer[0]/fNormAcc;
      fCosPitch = sqrt(1.0-(fSinPitch * fSinPitch));

      
      fTiltedX = MagBuffer[0]*fCosPitch + MagBuffer[2]*fSinPitch;
      fTiltedY = MagBuffer[0]*fSinRoll*fSinPitch + MagBuffer[1]*fCosRoll - MagBuffer[2]*fSinRoll*fCosPitch;
			
      fcosf=fTiltedX /sqrt(fTiltedX*fTiltedX+fTiltedY*fTiltedY);
	
			if(fTiltedY>0)
			  HeadingValue = (float)(acos(fcosf)*R2D);
			else
				HeadingValue =360-(float)(acos(fcosf)*R2D);
			
      //HeadingValue = (float) ((atan2f((float)fTiltedY,(float)fTiltedX))*180)/PI;
      HeadingValue+=11;//地磁的北极和地球的北极相差11度左右。
			if(HeadingValue>360)
				HeadingValue=HeadingValue-360;
   
	    return HeadingValue ;
	
	
}






