#include "stm32f10x.h"
#include "delay.h"
#include "define.h"
#include "sys_init.h"
#include "ISR.h"
#include "MyIIC.h"              //IIC PORT initial
#include "Mpu6050.h"
#include "QMC5883.h"
#include "stm32f10x_MY_spi.h"
#include "LSM303.h"
#include "stm32f10x_MY_Flash.h"
#include "MyNRF24L0.h"
#include "L3GD20.h"
#include "Task.h"
#include "interact.h"           //uart initial

ACC acc = {.x=1,.y=2,.z=3,.kx = 1,.ky = 1,.kz = 1,.x_off=0,.y_off=0,.z_off=0};   //安全性考虑，缺省斜率应为1，而不能设为0；截距应设为0
ACC LSM_acc_adc = {.x=1,.y=2,.kx = 1,.ky=1,.kz=1,.x_off=0,.y_off=0,.z_off=0};
GYRO gyro = {.x_offset=0,.y_offset=0,.z_offset=0};
GYRO L3GDgyro_adc = {.x_offset=44,.y_offset=62,.z_offset=109};
COMPASS compass = {.x_offset = 39,.y_offset=-486,.z_offset=253};        //680;503;-2124
COMPASS LSM_compass = {.x_offset = 0,.y_offset=0,.z_offset=0};

uint8 pipl_dir;             //people direction平面投影图中人相对飞机的方向，实际用于指定十字飞行方式还是X飞行方式，0用于十字，1用于X
float acc_angle[2][3] = {0};    //两个加速度计，3个轴的分量
int  temperature;
int  pressure;
float bat_voltage = 0;          //电池电压
float gc[4][8] = {0};
float ctrl[5] = {0};

extern char pwr_low_flag;               //电池能量不足标志
char GPS_CMD_send_flag = 0;
uint8 GPS_send_buff[25] = "$PSRF103,00,00,00,01*24";
SCom sCom;

/***************************  main  *****************************/
int main(void)
{
  __disable_irq();                              //关闭所有中断
  delay_init();
  DelayMs(80);                                  //wait for power up stable
  next_procedure = TaskInit(Task);                               //init task and task pointer
  ParamInit((short *)&gyro+3,&acc, &compass);       //read initial data from flash
  AdcInit();                                                    //
    gpio_init(RED_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(BLUE_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(RGB_R_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_init(RGB_G_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_init(RGB_B_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);					 //PB.5 ???
  gpio_init(BUZZER_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(LIGHT_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_init(GYRO_CS_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 1);                               //初始化CE，默认进入待机模式
  gpio_init(LSM303_CS_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 1);                               //初始化CE，默认进入待机模式
  gpio_init(NRF_CSN_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 1);                               //初始化CE，默认进入待机模式

#ifdef HARD_IIC
  i2c_init(I2C1,0x0FFFFFFF);
#else
  IIC_Port_Init();    //MPU6050 & Compass QMC5883L use the same IIC bus
#endif
  bat_voltage = BatVoltageGet();
  PIT4_Init(INTERUPT_CYC_IN_MS);
  TIM2_PWM_Init(50,10000,0);
  TIM3_PWM_Init(50,10000,0);                    //this is PWM on backup pin
  TIM_SetCompare4(TIM3,900);  //Y2
  
  SPI3_Init();
  UartInit();
  printf("aaaaa");
  //ComInit();
  //  BUZZER_OUT = 0;
  //  DelayMs(300);         //等待MPU6050自身上电初始化完成
  while(false==MPUInit())
    DelayMs(500);
  //  HMC5883Init();
  QMC5883Init();
  //  bmp085_init();                //初始化BMP085
  
  SET_SPIn_POLARITY(3,0);                 //set spi'3' polarity to '0'
  if(nrf_init(RX_MODE))                         //初始化NRF24L01+
    while(1);
  SET_SPIn_POLARITY(3,1);                 //set spi'3' polarity to '1'
  
  L3GD20_Init(L3GD20_FULLSCALE_250);
  LSM303D_Init();
  
  SET_SPIn_POLARITY(3,0);                 //set spi'3' polarity to '1'
  
  NVIC_Config();
  __enable_irq();
  
  while(1)
  {
/*****************************所有模式都会执行的程序*******************************/
    if(pit_5s_flag)
    {
      bat_voltage = BatVoltageGet();
      if(bat_voltage<10.5 && bat_voltage>9)             //降落后，电压回升接近11.1V，可用于长期储存
      {
        pwr_low_flag = 1;
      }
      else if(1==pwr_low_flag)
      {
        pwr_low_flag = 0;
      }
      //      if(GPS_CMD_send_flag)
      //      {
      //        SendGPSPkg(USART3, GPS_send_buff);
      //        GPS_CMD_send_flag = 0;
      //      }
      pit_5s_flag = 0;
      RGB_B_OUT = 1;
    }
    next_procedure = Task[next_procedure](1);
  }
}
