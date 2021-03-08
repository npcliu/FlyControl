#ifndef __DEFINE_H
#define __DEFINE_H

#define PI 3.1415926f
#define PI_DIV_180 0.01745329252f

#define INTERUPT_CYC_IN_MS    1         //ms

#define MIN_PWM         500            //�����趨����������г�����(��������г�˵��)
#define STOP_PWM        500            //���ͣת
#define MAX_PWM         1000           //�����趨����������г�����(��������г�˵��)

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

//#define TX              1       // ����ģʽ
//#define RX              0       // ����ģʽ

#define PLANE_MODE_OFFSET      0      //���ڶ�λ�ɻ�ģʽ���յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define TH_ADC_OFFSET          1       //���ڶ�λ���ţ�throttle�����յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define DR_ADC_OFFSET          2       //���ڶ�λ���ţ�throttle�����յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define UD_ADC_OFFSET          3       //���ڶ�λǰ��up back����б���յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define LR_ADC_OFFSET          4       //���ڶ�λ���ң�left right����б���յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define GPS_MODE_OFFSET        5       //���ڶ�λGPS״̬���յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define CALI_GYRO_OFFSET       6       //���ڶ�λУ׼������״̬�����յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define CALI_CMP_OFFSET        7       //���ڶ�λУ׼GPS״̬���յ�ң�����ݵĵڼ�������Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define BACKUP1_OFFSET         8       //����ͨ��1����Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�
#define BACKUP2_OFFSET         9       //����ͨ��2����Ҫ��ң�س���ͷɿس������ݰ�ƥ�䣻����޸ı�������ͬʱ�޸�

#define PARAMETER_NUM           10      //�����õĲ�������
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
//  float value;                          //����������ֵ
//  unsigned char acr_desm;                //accurycy behind decimal����ȷ��С��������λ��������ʾ
//  char unit[10];                        //��λ
//  
//}FPARAM,*PFPARAM;//�������ṹ�壬�����������ʾ��ֻ����ɸýṹ�弴�ɣ����Ҫ��ʾ����Ӧ����LCD�ж���ñ�������ʾ��Ϣ

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
extern float bat_voltage;          //��ص�ѹ

//extern char uart_send_flag;
extern int test_data[5];
extern float x2,y2;

extern float gc[4][8];          //����۲����        
extern float ctrl[5];           //������Ʊ���
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