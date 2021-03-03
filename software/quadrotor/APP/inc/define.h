#ifndef __DEFINE_H
#define __DEFINE_H

//#define MY_WOOD 1               //我的木机
#define CARBON_FIBRE 1                  //协会碳纤机
//#define 

#define PI 3.1415926f
#define PI_DIV_180      0.01745329252f
#define D2R           0.01745329252f  //Degree to rad
#define R2D           57.29577f       //Rad to Degree

#define INTERUPT_CYC_IN_MS    5               //ms

#define MIN_PWM         500            //用于设定电调的油门行程下限(详见油门行程说明)
#define STOP_PWM        500            //电机停转
#define MAX_PWM         1000           //用于设定电调的油门行程下限(详见油门行程说明)

//#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)

#define RED_PIN        PB12             //闪烁周期=控制周期，低电平时间为控制程序消耗时间
#define BLUE_PIN         PB13           //定时中断周期为周期，低电平时间为服务程序消耗时间
#define RGB_R_PIN       PC10
#define RGB_G_PIN       PC11
#define RGB_B_PIN       PC12
#define BUZZER_PIN      PB0
#define LIGHT_PIN       PC5
#define ADC_PIN         PC3
#define ADC_EN_PIN      PC2
#define BAT_ADC_CH      13

#define RED_OUT            PB12_OUT
#define BLUE_OUT             PB13_OUT
#define RGB_R_OUT           PC10_OUT
#define RGB_G_OUT           PC11_OUT
#define RGB_B_OUT           PC12_OUT
#define BUZZER_OUT          PB0_OUT
#define LIGHT_OUT           PC5_OUT
#define ADC_EN_OUT          PC2_OUT

#define BLUE_TRN        PB13_TRN
#define RGB_R_TRN       PC10_TRN
#define RGB_G_TRN       PC11_TRN
#define RGB_B_TRN       PC12_TRN
#define BUZZER_TRN      PB0_TRN

#define TX              1       // 发送模式
#define RX              0       // 接收模式

#define PLANE_MODE_OFFSET      0      //用于定位飞机模式接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define TH_ADC_OFFSET          1       //用于定位油门（throttle）接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define DR_ADC_OFFSET          2       //用于定位油门（throttle）接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define UD_ADC_OFFSET          3       //用于定位前后（up back）倾斜接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define LR_ADC_OFFSET          4       //用于定位左右（left right）倾斜接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define GPS_MODE_OFFSET        5       //用于定位GPS状态接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define CALI_GYRO_OFFSET       6       //用于定位校准陀螺仪状态接在收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define CALI_CMP_OFFSET        7       //用于定位校准GPS状态接收到遥控数据的第几个，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define BACKUP1_OFFSET         8       //备用通道1，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改
#define BACKUP2_OFFSET         9       //备用通道2，需要和遥控程序和飞控程序数据包匹配；如果修改必须两个同时修改

/*传感器配置参数*/
#define STM32F103RC_FLASH_PAGE126_ADDR  0x0803F000
#define MPU6050_ACC_FULLSCALE_OFFSET   0                 //1 Byte
#define MPU6050_GYRO_FULLSCALE_OFFSET   1                //1 Byte
#define MPU6050_FILTER_OFFSET   2                        //1 Byte
//#define ACC_KX_ADDR_OFFSET      8                       //data location offset  in flash
//#define ACC_KY_ADDR_OFFSET      12                       //data location offset  in flash
//#define ACC_KZ_ADDR_OFFSET      16                       //data location offset  in flash
//#define ACC_XOFF_ADDR_OFFSET      20                       //data location offset  in flash
//#define ACC_YOFF_ADDR_OFFSET      24                       //data location offset  in flash
//#define ACC_ZOFF_ADDR_OFFSET      28                       //data location offset  in flash
/*传感器校准参数*/
#define STM32F103RC_FLASH_PAGE127_ADDR  0x0803F800      //2Kb~0x0803FFFF
#define GYRO_XOFF_ADDR_OFFSET   0                       //gyroscope x offset data location offset  in flash
#define GYRO_YOFF_ADDR_OFFSET   2                       //gyroscope Y offset data location offset  in flash
#define GYRO_ZOFF_ADDR_OFFSET   4                       //gyroscope Z offset data location offset  in flash
#define ACC_KX_ADDR_OFFSET      8                       //data location offset  in flash
#define ACC_KY_ADDR_OFFSET      12                       //data location offset  in flash
#define ACC_KZ_ADDR_OFFSET      16                       //data location offset  in flash
#define ACC_XOFF_ADDR_OFFSET      20                       //data location offset  in flash
#define ACC_YOFF_ADDR_OFFSET      24                       //data location offset  in flash
#define ACC_ZOFF_ADDR_OFFSET      28                       //data location offset  in flash
#define COMPASS_XOFF_ADDR_OFFSET      32                       //data location offset  in flash
#define COMPASS_YOFF_ADDR_OFFSET      34                       //data location offset  in flash
#define COMPASS_ZOFF_ADDR_OFFSET      36                       //data location offset  in flash
#define COMPASS_KX_ADDR_OFFSET      38                       //data location offset  in flash
#define COMPASS_KY_ADDR_OFFSET      42                       //data location offset  in flash
#define COMPASS_KZ_ADDR_OFFSET      46                       //data location offset  in flash

#define X_B_ADDR_OFFSET         50                      //data location offset  in flash
#define X_P1_ADDR_OFFSET        54                      //data location offset  in flash
#define X_I1_ADDR_OFFSET        58                      //data location offset  in flash
#define X_D1_ADDR_OFFSET        62              //data location offset  in flash
#define X_P2_ADDR_OFFSET        68              //data location offset  in flash
#define X_I2_ADDR_OFFSET        72              //data location offset  in flash
#define X_D2_ADDR_OFFSET        76              //data location offset  in flash

#define Y_B_ADDR_OFFSET         84              //data location offset  in flash
#define Y_P1_ADDR_OFFSET        88              //data location offset  in flash
#define Y_I1_ADDR_OFFSET        92              //data location offset  in flash
#define Y_D1_ADDR_OFFSET        96              //data location offset  in flash
#define Y_P2_ADDR_OFFSET        100             //data location offset  in flash
#define Y_I2_ADDR_OFFSET        104             //data location offset  in flash
#define Y_D2_ADDR_OFFSET        108             //data location offset  in flash

#define Z_P_ADDR_OFFSET        112              //data location offset  in flash
#define Z_I_ADDR_OFFSET        116              //data location offset  in flash
#define Z_D_ADDR_OFFSET        120              //data location offset  in flash

#define ANGLE_XY_CONTROL_RATE   124             //遥控器值对于倾斜角的控制比例
#define ANGLE_Z_CONTROL_RATE   128              //data location offset  in flash


typedef struct
{
  short x, y, z;                        //经线性校准后的加速度计值，ref.原理：线性映射部分
  float kx,ky,kz;                       //线性映射之斜率
  float x_off,y_off,z_off;              //线性映射之截距
}ACC,*PACC;

typedef struct
{
  short x, y, z;                        //经零偏校准后的陀螺仪计值，ref.原理：线性映射部分
  short x_offset, y_offset, z_offset;   //零偏值
}GYRO,*PGYRO;
typedef struct
{
  short x, y, z;                        //电子罗盘校准后的值，ref.原理：线性映射部分
  float kx,ky,kz;                       //线性映射之斜率
  short x_offset,y_offset,z_offset;     //线性映射之截距
}COMPASS,*PCOMPASS;

typedef enum
{
  x_n = 0,              //x-,negative
  x_p,                  //x+,positive
  y_n,
  y_p
}PWM_GPIO_ENUM;

extern ACC acc;
extern ACC LSM_acc_adc;
extern GYRO gyro;
extern GYRO L3GDgyro_adc;
extern COMPASS compass;
extern COMPASS LSM_compass;

extern float acc_angle[2][3];    //两个加速度计，3个轴的分量

//typedef unsigned char uint8;
//typedef unsigned int uint32;

extern uint8 pipl_dir;
extern int  temperature;
extern int  pressure;
extern float bat_voltage;          //电池电压


extern float gc[4][8];          //输出观察变量        
extern float ctrl[5];           //输入控制变量


















//typedef uint8_t           uint8;
//typedef uint16_t          INT16U;
#define DEF_COM_RECV_BUF_SIZE  1024
#define DEF_COM_SEND_BUF_SIZE  50
typedef struct
{
  uint8 SendBuf[DEF_COM_SEND_BUF_SIZE];
  uint8 RecvBuf[DEF_COM_RECV_BUF_SIZE];
  uint32 RecvLen;
  uint8 fSending :1;
  uint8 fRecv :1;
}SCom;
extern SCom sCom;

//extern void gpio_int_cfg(PTXn_e pin,uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger);
//extern void Filt();
extern void PWM_Calculation(uint8 mod);
 
//STM32F103V8T6:100PIN, 64KB FLASH(0x0800 0000~)medium density device,20KB SRAM(0x2000 0000~)
//      0x0800 0000-FLASH-080? FFFF...0x2000 0000-SRAM-0x2000 1FFF
//STM32F103RCT6:256KB FLASH=128page*2KByte high density,
//      48KByteSRAM，from0x2000 0000-0x2000 BFFF
//查看手册：STM32F103xCxDxE手册中，表格中STM32F103Rx一列列出3个项分别表示RC，RD，RE
#endif