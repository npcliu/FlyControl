#ifndef __DEFINE_H
#define __DEFINE_H

//#define MY_WOOD 1               //�ҵ�ľ��
#define CARBON_FIBRE 1                  //Э��̼�˻�
//#define 

#define PI 3.1415926f
#define PI_DIV_180      0.01745329252f
#define D2R           0.01745329252f  //Degree to rad
#define R2D           57.29577f       //Rad to Degree

#define INTERUPT_CYC_IN_MS    5               //ms

#define MIN_PWM         500            //�����趨����������г�����(��������г�˵��)
#define STOP_PWM        500            //���ͣת
#define MAX_PWM         1000           //�����趨����������г�����(��������г�˵��)

//#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)

#define RED_PIN        PB12             //��˸����=�������ڣ��͵�ƽʱ��Ϊ���Ƴ�������ʱ��
#define BLUE_PIN         PB13           //��ʱ�ж�����Ϊ���ڣ��͵�ƽʱ��Ϊ�����������ʱ��
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

#define TX              1       // ����ģʽ
#define RX              0       // ����ģʽ

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

/*���������ò���*/
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
/*������У׼����*/
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

#define ANGLE_XY_CONTROL_RATE   124             //ң����ֵ������б�ǵĿ��Ʊ���
#define ANGLE_Z_CONTROL_RATE   128              //data location offset  in flash


typedef struct
{
  short x, y, z;                        //������У׼��ļ��ٶȼ�ֵ��ref.ԭ������ӳ�䲿��
  float kx,ky,kz;                       //����ӳ��֮б��
  float x_off,y_off,z_off;              //����ӳ��֮�ؾ�
}ACC,*PACC;

typedef struct
{
  short x, y, z;                        //����ƫУ׼��������Ǽ�ֵ��ref.ԭ������ӳ�䲿��
  short x_offset, y_offset, z_offset;   //��ƫֵ
}GYRO,*PGYRO;
typedef struct
{
  short x, y, z;                        //��������У׼���ֵ��ref.ԭ������ӳ�䲿��
  float kx,ky,kz;                       //����ӳ��֮б��
  short x_offset,y_offset,z_offset;     //����ӳ��֮�ؾ�
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

extern float acc_angle[2][3];    //�������ٶȼƣ�3����ķ���

//typedef unsigned char uint8;
//typedef unsigned int uint32;

extern uint8 pipl_dir;
extern int  temperature;
extern int  pressure;
extern float bat_voltage;          //��ص�ѹ


extern float gc[4][8];          //����۲����        
extern float ctrl[5];           //������Ʊ���


















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
//      48KByteSRAM��from0x2000 0000-0x2000 BFFF
//�鿴�ֲ᣺STM32F103xCxDxE�ֲ��У������STM32F103Rxһ���г�3����ֱ��ʾRC��RD��RE
#endif