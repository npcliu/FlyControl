#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "stm32f10x.h"
#include "math.h"
#include "define.h"
#include "delay.h"
#include "Mpu6050.h"
#include "MyNRF24L0.h"
#include "ISR.h"                //time flag
#include "Task.h"
#include "interact.h"
#include "canlibration.h"
#include "calculation.h"        //���ļ��еĲ�����Ҫ���ͳ�ȥ
#include "usart.h"	
#include "filter.h"
#include "bmp180.h"
#include "ms5611.h"

PROCEDURE (* Task[MAX_TASK_NUM])(char input) = NULL;              //����ָ�룬ÿ��ָ�����һ��������ڣ���Щ�������Ƿ��жϺ�����Ҳ���Խ�����
PROCEDURE next_procedure = stb_pre;

char send_wave_flag = 0;
char cali_acc_flag = 0;
char cali_gyro_flag = 0;
char need_restart_flag = 0;     //���Ĵ��������ú������������Ч����
char cali_compass = 0;
char pwr_low_flag = 1;               //������������־
int expriment_time = 0;//ʵ��ģʽ�¼�¼ʵ��ʱ��
#define MAX_EXPRIMENT_TIME 200//5ms��¼һ�Σ���200��ʾ����¼ʱ��Ϊ1s
float angley_data[MAX_EXPRIMENT_TIME] = {0};//y��Ƕ�����
int pwm0,pwm1,pwm2,pwm3;//��¼ʵ�鿪ʼʱ��pwm������
extern int pwm[4];
char Filter_init_flag = 0;//�������˲�����ʼ����ɺ���ܿ����߿��Ƴ���
PROCEDURE StbPrep(char input)
{
  TIM_SetCompare1(TIM2,0);
  TIM_SetCompare2(TIM2,0);
  TIM_SetCompare3(TIM2,0);
  TIM_SetCompare4(TIM2,0);
  send_wave_flag = 1;
  //���������־��ȷ���Ƿ��޸��˲������޸���flash�Ĳ�����������Ҫ������
  return standby;
}

PROCEDURE Standby(char input)
{
  //      TIM_SetCompare4(TIM2,(int)(1.3*nrf_rciv[TH_ADC_OFFSET]+500));  //Y2
  //      pwm[y_p] = (int)(1.3*nrf_rciv[TH_ADC_OFFSET]+500);  //У׼
  
  static uint32 count_delet = 0;
  
  expriment_time = 0;//ʵ��ģʽ�µ�ʵ��ʱ������
  Filter_init_flag = 0;//�������˲�����ʼ����ɺ���ܿ����߿��Ƴ���
  send_wave_flag = 0;
  if(send_wave_flag)
    SCISend_to_Own(USART1);
  else;
  /**************************25ms******************************/
  if(pit_25ms_flag)        //��25ms���ڷ��ͣ���1����50ms,���鿴SCISend_to_Own����
  {
    if(count_delet<ctrl[4]+5)
      count_delet++;
    else
    {
      count_delet=0;
 //     UARTSendFloat(gyro.x*MPU6050GYRO_SCALE_DEG);
 //     UARTSendFloat(gyro.y*MPU6050GYRO_SCALE_DEG);
 //     UARTSendFloat(gyro.z*MPU6050GYRO_SCALE_DEG);
 //     UARTSendFloat(acc.x*MPU6050ACC_SCALE_G*9.81);
 //     UARTSendFloat(acc.y*MPU6050ACC_SCALE_G*9.81);
 //     UARTSendFloat(acc.z*MPU6050ACC_SCALE_G*9.81);
 //     float norm = sqrt(compass.x*compass.x + compass.y*compass.y + compass.z*compass.z);
 //     UARTSendFloat(compass.x/norm);
 //     UARTSendFloat(compass.y/norm);
 //     UARTSendFloat(compass.z/norm);
    }
    pit_25ms_flag = 0;
  }
  if(cali_gyro_flag)       //gyroscope callibration
  {
    if(0==GyroCalibration());
    else
    {
      BUZZER_OUT = 1;          DelayMs(10);          BUZZER_OUT = 0;          DelayMs(1000);
    }
    cali_gyro_flag = 0;
  }
  if(cali_acc_flag)
  {
    if(AccCalibration(&acc, &gyro));
    else
    {
      cali_acc_flag = 0;            BUZZER_OUT = 1;            DelayMs(10);            BUZZER_OUT = 0;            DelayMs(1000);
    }
  }
  if(cali_compass)
  {
    cali_compass = 0;
    CompassCalibration(50);
  }
  
  TIM_SetCompare4(TIM3,(int)(1.3*nrf_rciv[TH_ADC_OFFSET]+500));  //Y2
  
  if('z'==nrf_rciv[BACKUP1_OFFSET]&&(angley_data[0] != 0))//����ʵ��ģʽ�²ɼ������ݣ�����ɼ����˲Żᷢ
  {
    
    printf("send data to computer\r\n");
    printf("pwm0 = %d\r\n",pwm0);
    printf("pwm1 = %d\r\n",pwm1);
    printf("angley_data : ");
    for(int i = 0;i<MAX_EXPRIMENT_TIME;i++)
    {
      printf("%f ",angley_data[i]);
    }
  }
  extern _bmp180 bmp180;
  extern _ms5611 ms5611;
  //if(bmp180.altitude_init!=0&&(ms5611.altitude_init!=0))//�����ʼ�߶�Ϊ0��˵�����ڻ�û�����ʼ�߶ȣ���ʱ�������
if((ms5611.altitude_init!=0))//�����ʼ�߶�Ϊ0��˵�����ڻ�û�����ʼ�߶ȣ���ʱ�������

  {
    if('f'==nrf_rciv[PLANE_MODE_OFFSET] && (0==need_restart_flag))
      return att_hld_pre;

    else if('p'==nrf_rciv[PLANE_MODE_OFFSET])
    {
      return standby;
    }
    else if('e'==nrf_rciv[PLANE_MODE_OFFSET])
    {
      return experiment_pro;
    }  
    else
      return standby;
  }
  else
    return standby;
}

PROCEDURE AttHldPrep(char input)
{
  TIM_SetCompare1(TIM2,STOP_PWM);      //�����ת������,׼�����
  TIM_SetCompare2(TIM2,STOP_PWM);
  TIM_SetCompare3(TIM2,STOP_PWM);
  TIM_SetCompare4(TIM2,STOP_PWM);
  
  return att_hld;
}
extern float filted_acc[2][3];
extern char nrf_int_flag;          //���Ź���־����ʱ��λ
char nrf_break = 1;             //����ʧ��״̬
PROCEDURE AttHld(char input)
{
   send_wave_flag = 0;
    if(send_wave_flag && pit_25ms_flag)        //��25ms���ڷ��ͣ���1����50ms,���鿴SCISend_to_Own����
    { 
        SCISend_to_Own(USART1);
      pit_25ms_flag = 0;
    }
  
  if(nrf_rciv[TH_ADC_OFFSET]>2)
  {
    TIM_SetCompare1(TIM2,pwm[x_n]);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
    TIM_SetCompare2(TIM2,pwm[x_p]);  //X2
    TIM_SetCompare3(TIM2,pwm[y_n]);  //Y1
    TIM_SetCompare4(TIM2,pwm[y_p]);  //Y2
  }else
  {
    TIM_SetCompare1(TIM2,STOP_PWM);      
    TIM_SetCompare2(TIM2,STOP_PWM);
    TIM_SetCompare3(TIM2,STOP_PWM);
    TIM_SetCompare4(TIM2,STOP_PWM);
  }
  
  /**************************50ms******************************/
  if(pit_50ms_flag)
  {
    if(nrf_int_flag>=20)        //���^1��δ�յ��źŲŹ�����
    {
      nrf_rciv[TH_ADC_OFFSET] = 0;          //�������׹���Ļ��͹ر�����
      BUZZER_OUT = 0;               //����������50ms�ļ��ٱ�������
    }
    else if(nrf_int_flag)
    {
      nrf_break = 1;
      BUZZER_OUT = 1;               //����������50ms�ļ��ٱ�������
      if(nrf_rciv[TH_ADC_OFFSET] > 80)          //����80��������ƽ��λ�ã������㲻��ң��ֱ�Ӿ�ת��
        nrf_rciv[TH_ADC_OFFSET] = 80;          //�������׹���Ļ��͹ر����ţ��ҹ�������80�պ�ƽ��
    }
    else if(nrf_break)
    {
      nrf_break = 0;               //����������50ms�ļ��ٱ�������
      BUZZER_OUT = 0;               //����������50ms�ļ��ٱ�������
    }
    if(nrf_int_flag<100)
      nrf_int_flag ++;           //��λ�����־��nrf�����жϸ�λ��־��
    pit_50ms_flag = 0;
  }
  /**************************500ms******************************/
  if(pit_500ms_flag)
  {
    RED_OUT = ~RED_OUT;
    if(pwr_low_flag)
      BUZZER_TRN;
    else
      BUZZER_OUT = 0;
    pit_500ms_flag = 0;
  }
  
  if('l'==nrf_rciv[PLANE_MODE_OFFSET])
  {
    BUZZER_OUT = 0;
    nrf_int_flag = 0;
    return stb_pre;
  }
  else if('h'==nrf_rciv[PLANE_MODE_OFFSET])//���붨��ģʽ
  {
        nrf_int_flag = 0;
    return height_hld_pre;
    
  }
  else
    return att_hld;
}
KalmanInfo kalmanFilter_of_MPU6050_acc;

PROCEDURE HeiHldPrep(char input)
{
  Init_KalmanInfo(&kalmanFilter_of_MPU6050_acc,38,39000);
  Filter_init_flag = 1;
  return height_hld;
}
//����ģʽ
PROCEDURE HeiHld(char input)
{
  
   send_wave_flag = 1;
    if(send_wave_flag && pit_25ms_flag)        //��25ms���ڷ��ͣ���1����50ms,���鿴SCISend_to_Own����
    {
        SCISend_to_Own(USART1);
      pit_25ms_flag = 0;
    }  
  
  if(nrf_rciv[TH_ADC_OFFSET]>2)
  {
    TIM_SetCompare1(TIM2,pwm[x_n]);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
    TIM_SetCompare2(TIM2,pwm[x_p]);  //X2
    TIM_SetCompare3(TIM2,pwm[y_n]);  //Y1
    TIM_SetCompare4(TIM2,pwm[y_p]);  //Y2
  }else
  {
    TIM_SetCompare1(TIM2,STOP_PWM);      
    TIM_SetCompare2(TIM2,STOP_PWM);
    TIM_SetCompare3(TIM2,STOP_PWM);
    TIM_SetCompare4(TIM2,STOP_PWM);
  }
  
  /**************************50ms******************************/
  if(pit_50ms_flag)
  {
    if(nrf_int_flag>=20)        //���^1��δ�յ��źŲŹ�����
    {
      nrf_rciv[TH_ADC_OFFSET] = 0;          //�������׹���Ļ��͹ر�����
      BUZZER_OUT = 0;               //����������50ms�ļ��ٱ�������
    }
    else if(nrf_int_flag)
    {
      nrf_break = 1;
      BUZZER_OUT = 1;               //����������50ms�ļ��ٱ�������
      if(nrf_rciv[TH_ADC_OFFSET] > 80)          //����80��������ƽ��λ�ã������㲻��ң��ֱ�Ӿ�ת��
        nrf_rciv[TH_ADC_OFFSET] = 80;          //�������׹���Ļ��͹ر����ţ��ҹ�������80�պ�ƽ��
    }
    else if(nrf_break)
    {
      nrf_break = 0;               //����������50ms�ļ��ٱ�������
      BUZZER_OUT = 0;               //����������50ms�ļ��ٱ�������
    }
    if(nrf_int_flag<100)
      nrf_int_flag ++;           //��λ�����־��nrf�����жϸ�λ��־��
    
      
    pit_50ms_flag = 0;
  }
  
//  extern char pit_80ms_flag;
//  extern uint8  nrf_mode;
//  if(pit_80ms_flag)  
//  {
//    if(nrf_mode == TX_MODE)
//    {
//      irq_tx_buff[BACKUP2_OFFSET] = (uint8)gc[3][0];
//      nrf_tx(irq_tx_buff,DATA_PACKET);
//    }
//    pit_80ms_flag = 0;
//  }
//    extern uint8 test_commincation;
//  if(nrf_mode == RX_MODE)
//  {
//    printf("%d\r\n",test_commincation);
//  }
  /**************************500ms******************************/
  if(pit_500ms_flag)
  {

    if(pwr_low_flag)
      BUZZER_TRN;
    else
      BUZZER_OUT = 0;
    pit_500ms_flag = 0;
  }
  
  if('l'==nrf_rciv[PLANE_MODE_OFFSET])
  {
    BUZZER_OUT = 0;
    nrf_int_flag = 0;
    return stb_pre;
  }
  else if('f'==nrf_rciv[PLANE_MODE_OFFSET])
  {
    BUZZER_OUT = 0;
    nrf_int_flag = 0;
    return att_hld_pre;
  }
  else
    return height_hld;  
}
PROCEDURE PosHldPrep()
{
  //���GPS��λ
  return stb_pre;
}
/************************GPS ����*****************************/
PROCEDURE PosHld()
{
  return pos_hld;
}

PROCEDURE Landing(char input)
{
  
  return land;
}

PROCEDURE SetParaPre(char input)
{
  need_restart_flag = 1;        //���ò�������Ҫ����
//  nrf_tx_mode();
//  DelayMs(50);
//  SendParametersToRC();
//  nrf_rx_mode();
  
  return set_para;
}
//��������ģʽ����ֱ�ӽ������ģʽ������MPU6050�˲����ĳɸߴ�������Ҫ���³�ʼ��6050����Ч������Ҫ����������ֱ�ӷ�
PROCEDURE SetParam(char input)
{
  if(0==strncmp((const char *)nrf_rciv,"FEParamGet",10))
  {
    if(0==strncmp((char const *)(nrf_rciv+10),"xpo",3))
    {
      irq_tx_buff[0] = 6;
      memcpy(irq_tx_buff+1,(void *)&x_p_o,sizeof(x_p_o));
    }
    else if(0==strncmp((const char *)(nrf_rciv+10),"xpi",3))
    {
      irq_tx_buff[0] = 6;
      memcpy(irq_tx_buff+1,(void *)&x_p_i,sizeof(x_p_i));
    }
    else if(0==strncmp((const char *)(nrf_rciv+10),"xdi",3))
    {
      irq_tx_buff[0] = 6;
      memcpy(irq_tx_buff+1,(void *)&x_d_i,sizeof(x_d_i));
    }
    else if(0==strncmp((const char *)(nrf_rciv+10),"ypo",3))
    {
      irq_tx_buff[0] = 6;
      memcpy(irq_tx_buff+1,(void *)&x_d_i,sizeof(y_p_o));
    }
    else if(0==strncmp((const char *)(nrf_rciv+10),"ypi",3))
    {
      irq_tx_buff[0] = 6;
      memcpy(irq_tx_buff+1,(void *)&x_d_i,sizeof(y_p_i));
    }
    else if(0==strncmp((const char *)(nrf_rciv+10),"ydi",3))
    {
      irq_tx_buff[0] = 6;
      memcpy(irq_tx_buff+1,(void *)&x_d_i,sizeof(y_d_i));
    }
//    else if(0==strncmp((const char *)(nrf_rciv+10),"ydi",3))
//    {
//      irq_tx_buff[0] = 6;
//      memcpy(irq_tx_buff+1,(void *)&x_d_i,sizeof(y_d_i));
//    }
    RGB_G_OUT = 0;
    DelayMs(500);
    RGB_G_OUT = 1;
    DelayMs(500);
    RGB_G_OUT = 0;
    DelayMs(500);
    RGB_G_OUT = 1;
    
    nrf_tx_mode();
    DelayMs(20);
    nrf_tx(irq_tx_buff,DATA_PACKET);//DATA_PACKET
    DelayMs(20);
    nrf_rx_mode();
    memset(nrf_rciv,0,DATA_PACKET);
  }

  if('l'==nrf_rciv[PLANE_MODE_OFFSET])
    return stb_pre;
  else
    return set_para;
}
short amplitude = 0;
#include "calculation.h"

PROCEDURE Experiment(char input)
{
  if('k'==nrf_rciv[BACKUP1_OFFSET]&&(expriment_time<MAX_EXPRIMENT_TIME))
  {//���������ſ���������ȵĴ�С������ʼʵ�鰴ťǰӦ�ȹ̶�����λ�ã����������ť����ܸı�����λ��
    //throttle = 1.3*nrf_rciv[TH_ADC_OFFSET]+500;
    if(pit_5ms_flag == 1)
    {  
      if(expriment_time == 0){
        pwm0 = pwm[x_n]-5;
        pwm1 = pwm[x_p]+5;
        

      }
      //printf("%d   %d\r\n",pwm0,pwm1);
      if(pwm0>10000)pwm0 = 10000;
      else if(pwm0<0)pwm0 = 0;
      if(pwm1>10000)pwm1 = 10000;
      else if(pwm1<0)pwm1 = 0;
      TIM_SetCompare1(TIM2,pwm0);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
      TIM_SetCompare2(TIM2,pwm1);  //X2
      TIM_SetCompare3(TIM2,pwm[y_n]);  //Y1
      TIM_SetCompare4(TIM2,pwm[y_p]);  //Y2
     
      angley_data[expriment_time] = angle[0];
      expriment_time++;
      
      pit_5ms_flag  = 0;
      //printf("%f ",angle[0]);
      //printf("start the experiment\r\n");

    }
    
    
  }
  else if ('t'==nrf_rciv[BACKUP1_OFFSET]||(expriment_time>=MAX_EXPRIMENT_TIME))//ң��������ֹͣ�����ʱ�����500ms��ֹͣʵ��
  {
    if(expriment_time>=MAX_EXPRIMENT_TIME&&('k'==nrf_rciv[BACKUP1_OFFSET]))
    {
      nrf_rciv[BACKUP1_OFFSET] = 't';
      BUZZER_OUT = 0;               //��������
    }

    //printf("stop the experiment\r\n");
//    printf("pwm0 = %d     ",pwm[x_n]);
//    printf("pwm1 = %d     ",pwm[x_p]);
//    printf("pwm2 = %d     ",pwm[y_n]);
//    printf("pwm3 = %d\r\n",pwm[y_p]);
    
    if(nrf_rciv[TH_ADC_OFFSET]>2)
    {
      TIM_SetCompare1(TIM2,pwm[x_n]);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
      TIM_SetCompare2(TIM2,pwm[x_p]);  //X2
      TIM_SetCompare3(TIM2,pwm[y_n]);  //Y1
      TIM_SetCompare4(TIM2,pwm[y_p]);  //Y2
    }else
    {
      TIM_SetCompare1(TIM2,STOP_PWM);      
      TIM_SetCompare2(TIM2,STOP_PWM);
      TIM_SetCompare3(TIM2,STOP_PWM);
      TIM_SetCompare4(TIM2,STOP_PWM);
    }
    
    
    
    /**************************50ms******************************/
    if(pit_50ms_flag)
    {
      if(nrf_int_flag>=20)        //���^1��δ�յ��źŲŹ�����
      {
        nrf_rciv[TH_ADC_OFFSET] = 0;          //�������׹���Ļ��͹ر�����
        BUZZER_OUT = 0;               //����������50ms�ļ��ٱ�������
      }
      else if(nrf_int_flag)
      {
        nrf_break = 1;
        BUZZER_OUT = 1;               //����������50ms�ļ��ٱ�������
        if(nrf_rciv[TH_ADC_OFFSET] > 80)          //����80��������ƽ��λ�ã������㲻��ң��ֱ�Ӿ�ת��
          nrf_rciv[TH_ADC_OFFSET] = 80;          //�������׹���Ļ��͹ر����ţ��ҹ�������80�պ�ƽ��
      }
      else if(nrf_break)
      {
        nrf_break = 0;               //����������50ms�ļ��ٱ�������
        BUZZER_OUT = 0;               //����������50ms�ļ��ٱ�������
      }
      if(nrf_int_flag<254)
        nrf_int_flag ++;           //��λ�����־��nrf�����жϸ�λ��־��
      pit_50ms_flag = 0;
    }
    /**************************500ms******************************/
    if(pit_500ms_flag)
    {
      if(pwr_low_flag)
        BUZZER_TRN;
      else
        BUZZER_OUT = 0;
      pit_500ms_flag = 0;
    }
  }
  else
  {
    pwm[0] = 0;//
    pwm[1] = 0;//
    pwm[2] = 0;
    pwm[3] = 0;
    TIM_SetCompare1(TIM2,pwm[0]);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
    TIM_SetCompare2(TIM2,pwm[1]);  //X2
    TIM_SetCompare3(TIM2,pwm[2]);  //Y1
    TIM_SetCompare4(TIM2,pwm[3]);  //Y2
  }

  if('l'==nrf_rciv[PLANE_MODE_OFFSET])
    return stb_pre;
  else
    return experiment_pro;
}

//Brief:R(Real)T(time)DataTX,ʵʱ���ݴ���ģʽ
//PROCEDURE RTDataTX(char input)
//{
//  
//}

//Brief:���������񣩳�ʼ����������ָ���Ӧ����;ע��������ָ���Ƿ񳬹������������
//Parameters:
//  _Task:����ָ�����飬���������������з��жϺ���;
//Return:
//  ��һ����Ҫִ�е���������stb_pre������һ�ε��ú���������StbPrep����;
//Example:;
PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char))
{
  assert(stb_pre>=0 && stb_pre<MAX_TASK_NUM);
  _Task[stb_pre] = StbPrep;                        //_Task[0]��StandByPrepare�����ĵ�ַ
  assert(standby>=0 && standby<MAX_TASK_NUM);
  _Task[standby] = Standby;
  assert(att_hld_pre>=0 && att_hld_pre<MAX_TASK_NUM);
  _Task[att_hld_pre] = AttHldPrep;
  assert(att_hld>=0 && att_hld<MAX_TASK_NUM);
  _Task[att_hld] = AttHld;
  assert(set_para_pre>=0 && set_para_pre<MAX_TASK_NUM);
  _Task[set_para_pre] = SetParaPre;
  assert(set_para>=0 && set_para<MAX_TASK_NUM);
  _Task[set_para] = SetParam;           //���ָ��
  assert(experiment_pro>=0 && experiment_pro<MAX_TASK_NUM);
  _Task[experiment_pro] = Experiment;           //���ָ��
  assert(height_hld_pre>=0 && height_hld_pre<MAX_TASK_NUM);
  _Task[height_hld_pre] = HeiHldPrep;           //���ָ��  
  assert(height_hld>=0 && height_hld<MAX_TASK_NUM);
  _Task[height_hld] = HeiHld;           //���ָ��    
  
  return stb_pre;
}
