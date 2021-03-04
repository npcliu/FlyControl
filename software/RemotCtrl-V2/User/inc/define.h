#ifndef __DEFINE_H
#define __DEFINE_H

#define PI 3.1415926f
#define PI_DIV_180 0.01745329252f

#define INTERUPT_CYC_IN_MS    1         //ms

#define MIN_PWM         500            //用于设定电调的油门行程下限(详见油门行程说明)
#define STOP_PWM        500            //电机停转
#define MAX_PWM         1000           //用于设定电调的油门行程下限(详见油门行程说明)

#define BAT_VOL_FILT_LEN        10

#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)

#define BLUE_PIN        PC13
#define BLUE_OUT        PC13_OUT
#define BLUE_TRN        PC13_TRN
/***************************ADC and thire ENABLE*******************************/
#define BAT_SEN_EN_PIN          PA1
#define BAT_SEN_EN_OUT          PA1_OUT
#define BAT_ADC_PIN             PA0
#define BAT_ADC                 ADC1
#define BAT_ADC_CHANNEL         0
#define BAT_ADC_MAX             4095            //ADC converter max output value

#define PHOTORES_EN_PIN         PA2
#define PHOTORES_EN_OUT         PA2_OUT
#define PHOTORES_ADC            ADC1
#define PHOTORES_ADC_CHANNEL    3

#define PHOTORES_ADC_PIN        PA3
#define TH_ADC_PIN              PC0
#define DR_ADC_PIN              PC1
#define UD_ADC_PIN              PC2
#define LR_ADC_PIN              PC3

#define TH_TO_LR_ADC            ADC1           //throttle to left and right all use ADC1
#define TH_ADC_CHANNEL          10           //throttle
#define DR_ADC_CHANNEL          11           //direction
#define UD_ADC_CHANNEL          12           //up and down
#define LR_ADC_CHANNEL          13           //left and right
#define TH_ADC_MAX              4095            //ADC converter max output value
#define DR_ADC_MAX              4095
#define UD_ADC_MAX              4095
#define LR_ADC_MAX              4095

//
#define KEY1_PIN                PD11
#define KEY1_IN                 PD11_IN
#define KEY1_IRQ_LINE           EXTI_Line11

#define KEY2_PIN                PD12
#define KEY2_IN                 PD12_IN
#define KEY2_IRQ_LINE           EXTI_Line12

#define S1_PIN                  PD9
#define S1_IN                  PD9_IN

/***************************       ENCODER      *******************************/
#define ENSW_PIN                 PD14
#define ENA_PIN                 PD15
#define ENB_PIN                 PC6
#define ENA_IN                  PD15_IN
#define ENB_IN                  PC6_IN

#define ENSW_IRQ_LINE            EXTI_Line14
#define ENA_IRQ_LINE            EXTI_Line15
//#define ENA_IRQn                EXTI15_10_IRQn                //needless to define,other pin already defined 

/***************************              *******************************/
//#define RED_OUT             PD0_OUT
#define BUZZER_OUT          PA4_OUT

#define BUZZER_TRN      PA4_TRN

//#define TX              1       // 发送模式
//#define RX              0       // 接收模式

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

#define PARAMETER_NUM           10      //可设置的参数个数
typedef enum
{
  lock = 0,
  fly,
  height_fix,           //hold the steady height
  experiment,
}PLANE_MODE;
//typedef enum
//{
//  standby_pre = 0,
//  standby,
//  fly_pre,
//  flying,
//  set_para_pre,   //set_parameters
//  set_para,
//}PROCEDURE;


//typedef struct
//{
//  float value;                          //浮点数的数值
//  unsigned char acr_desm;                //accurycy behind decimal，精确到小数点后多少位，用于显示
//  char unit[10];                        //单位
//  
//}FPARAM,*PFPARAM;//浮点数结构体，如果不用于显示，只定义成该结构体即可，如果要显示，则应该在LCD中定义该变量的显示信息

extern PLANE_MODE plane_mod;
extern float w_x, w_y, w_z;
extern float basic,x_p1,x_h1,x_i1,x_p2,x_d2,x_i2;
extern float y_p1,y_h1,y_i1,y_p2,y_d2,y_i2;
extern float z_p1,z_d1;
extern int  temperature;
extern int  pressure;
extern volatile char pit_5ms_flag;
extern char pit_50ms_flag;
extern char pit_500ms_flag;
extern char pit_5s_flag;
extern float bat_voltage;          //电池电压

//extern char uart_send_flag;
extern int test_data[5];
extern float x2,y2;

extern float gc[4][8];          //输出观察变量        
extern float ctrl[5];           //输入控制变量
//extern char nrf_link_state;
extern char waite_for_nrf_data;
//extern char gps_re_flag;

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

extern uint16 thrust_adc;
extern uint16 up_adc;
extern uint16 left_adc;
extern uint8 set_param_flag;
extern uint8 confirm_flag;
//extern char right_hand_flag;       //flag true,right hand throttle



//extern void gpio_int_cfg(PTXn_e pin,uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger);
extern void Filt();
extern void PWM_Calculation();

//STM32F103V8T6:100PIN, 64KB FLASH(0x0800 0000~)medium density device,20KB SRAM(0x2000 0000~)[https://blog.csdn.net/u011352311/article/details/17289365?utm_source=blogxgwz5]
//0x0800 0000-FLASH-080? FFFF...0x2000 0000-SRAM-0x2000 1FFF
//STM32F103RCT6:64PIN, 256KB FLASH(0x0800 0000~)high density device,xxKB SRAM(0x2000 0000~)
//0x0800 0000-FLASH-080? FFFF...0x2000 0000-SRAM-0x2000 1FFF
#endif