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
#include "calculation.h"        //该文件中的参数需要发送出去
#include "usart.h"	
PROCEDURE (* Task[MAX_TASK_NUM])(char input) = NULL;              //函数指针，每个指针代表一个函数入口，这些函数都是非中断函数，也可以叫任务。
PROCEDURE next_procedure = stb_pre;

char send_wave_flag = 0;
char cali_acc_flag = 0;
char cali_gyro_flag = 0;
char need_restart_flag = 0;     //更改传感器设置后必须重启以生效更改
char cali_compass = 0;
char pwr_low_flag = 1;               //电池能量不足标志
extern int pwm[4];

PROCEDURE StbPrep(char input)
{
  TIM_SetCompare1(TIM2,0);
  TIM_SetCompare2(TIM2,0);
  TIM_SetCompare3(TIM2,0);
  TIM_SetCompare4(TIM2,0);
  send_wave_flag = 1;
  //检查重启标志，确认是否修改了参数，修改了flash的参数？或者需要重启的
  return standby;
}

PROCEDURE Standby(char input)
{
  //      TIM_SetCompare4(TIM2,(int)(1.3*nrf_rciv[TH_ADC_OFFSET]+500));  //Y2
  //      pwm[y_p] = (int)(1.3*nrf_rciv[TH_ADC_OFFSET]+500);  //校准
  
  static uint32 count_delet = 0;
  send_wave_flag = 0;
  if(send_wave_flag)
    SCISend_to_Own(USART1);
  else;
  /**************************25ms******************************/
  if(pit_25ms_flag)        //以25ms周期发送，画1个点50ms,详情看SCISend_to_Own函数
  {
    if(count_delet<ctrl[4]+5)
      count_delet++;
    else
    {
      count_delet=0;
//      UARTSendFloat(gyro.x*MPU6050GYRO_SCALE_DEG);
//      UARTSendFloat(gyro.y*MPU6050GYRO_SCALE_DEG);
//      UARTSendFloat(gyro.z*MPU6050GYRO_SCALE_DEG);
//      UARTSendFloat(acc.x*MPU6050ACC_SCALE_G*9.81);
//      UARTSendFloat(acc.y*MPU6050ACC_SCALE_G*9.81);
//      UARTSendFloat(acc.z*MPU6050ACC_SCALE_G*9.81);
      float norm = sqrt(compass.x*compass.x + compass.y*compass.y + compass.z*compass.z);
      printf("水平面下的偏航角为 ： %f\r\n",R2D*atan((float)compass.y/compass.x));
      
//      UARTSendFloat(compass.x/norm);
//      UARTSendFloat(compass.y/norm);
//      UARTSendFloat(compass.z/norm);
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
  
  TIM_SetCompare4(TIM3,(int)(1.3*nrf_rciv[TH_ADC_OFFSET]+500));  //舵机端口输出油门信号，可用于校准电调
  
  
  
  if('f'==nrf_rciv[PLANE_MODE_OFFSET] && (0==need_restart_flag))
    return att_hld_pre;
  else if('p'==nrf_rciv[PLANE_MODE_OFFSET])
  {
    return set_para_pre;
  }
  else
    return standby;
}

PROCEDURE AttHldPrep(char input)
{
  TIM_SetCompare1(TIM2,STOP_PWM);      //置最低转速脉宽,准备起飞
  TIM_SetCompare2(TIM2,STOP_PWM);
  TIM_SetCompare3(TIM2,STOP_PWM);
  TIM_SetCompare4(TIM2,STOP_PWM);
  
  return att_hld;
}

extern char nrf_int_flag;          //看门狗标志，定时置位
char nrf_break = 1;             //无线失联状态
PROCEDURE AttHld(char input)
{
  //  if(*p_send_wave_flag && pit_25ms_flag)        //以25ms周期发送，画1个点50ms,详情看SCISend_to_Own函数
  //  {
  //      SCISend_to_Own(USART1);
  //    pit_25ms_flag = 0;
  //  }
  
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
    if(nrf_int_flag>=20)        //超過1秒未收到信号才关油门
    {
      nrf_rciv[TH_ADC_OFFSET] = 0;          //如果你想坠机的话就关闭油门
      BUZZER_OUT = 0;               //蜂鸣报警（50ms的急促报警声）
    }
    else if(nrf_int_flag)
    {
      nrf_break = 1;
      BUZZER_OUT = 1;               //蜂鸣报警（50ms的急促报警声）
      if(nrf_rciv[TH_ADC_OFFSET] > 80)          //大于80，才收至平衡位置，否则你不插遥控直接就转了
        nrf_rciv[TH_ADC_OFFSET] = 80;          //如果你想坠机的话就关闭油门，我估计油门80刚好平衡
    }
    else if(nrf_break)
    {
      nrf_break = 0;               //蜂鸣报警（50ms的急促报警声）
      BUZZER_OUT = 0;               //蜂鸣报警（50ms的急促报警声）
    }
    if(nrf_int_flag<100)
      nrf_int_flag ++;           //置位查验标志（nrf接收中断复位标志）
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
  
  if('l'==nrf_rciv[PLANE_MODE_OFFSET])
  {
    BUZZER_OUT = 0;
    nrf_int_flag = 0;
    return stb_pre;
  }
  else
    return att_hld;
}

PROCEDURE PosHldPrep()
{
  //检查GPS定位
  return stb_pre;
}
/************************GPS 定点*****************************/
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
  need_restart_flag = 1;        //设置参数则需要重启
//  nrf_tx_mode();
//  DelayMs(50);
//  SendParametersToRC();
//  nrf_rx_mode();
  
  return set_para;
}
//参数设置模式不能直接进入飞行模式，比如MPU6050滤波器改成高带宽，必须要重新初始化6050才生效，所以要重启不允许直接飞
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
{//实验模式下不用PID
  static int save_data_time = 0;
  if('k'==nrf_rciv[BACKUP1_OFFSET])
  {//这里用油门控制脉冲幅度的大小，按开始实验按钮前应先固定油门位置，点击结束按钮后才能改变油门位置
    //throttle = 1.3*nrf_rciv[TH_ADC_OFFSET]+500;
    amplitude = 500;
    pwm[0] = (int)(amplitude);//
    pwm[1] = 0;//
    pwm[2] = 0;
    pwm[3] = 0;
    TIM_SetCompare1(TIM2,pwm[0]);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
    TIM_SetCompare2(TIM2,pwm[1]);  //X2
        TIM_SetCompare3(TIM2,pwm[2]);  //Y1
        TIM_SetCompare4(TIM2,pwm[3]);  //Y2
//    pwm[x_n] = (int)(throttle + xcq + ycq - pwm_of_dir);//
//    pwm[x_p] = (int)(throttle - xcq - ycq - pwm_of_dir);//
//    pwm[y_n] = (int)(throttle + xcq - ycq + pwm_of_dir);
//    pwm[y_p] = (int)(throttle - xcq + ycq + pwm_of_dir);
//    
//      if(nrf_rciv[TH_ADC_OFFSET]>2)
//  {
//    TIM_SetCompare1(TIM2,pwm[x_n]);  //X1       //actually if you write the parameters as:throttle + y2 - pwm_of_dir,it will be differen inside of this function
//    TIM_SetCompare2(TIM2,pwm[x_p]);  //X2
//    TIM_SetCompare3(TIM2,pwm[y_n]);  //Y1
//    TIM_SetCompare4(TIM2,pwm[y_p]);  //Y2
//  }else
//  {
//    TIM_SetCompare1(TIM2,STOP_PWM);      
//    TIM_SetCompare2(TIM2,STOP_PWM);
//    TIM_SetCompare3(TIM2,STOP_PWM);
//    TIM_SetCompare4(TIM2,STOP_PWM);
//  }

    SaveAngleDataToNode(angle);//把解算的角度存到链表中
    save_data_time++;
    if(save_data_time > anglex_Node_Number + 1)
      save_data_time = anglex_Node_Number + 1;
  }
  else if ('t'==nrf_rciv[BACKUP1_OFFSET] || (save_data_time > anglex_Node_Number))//遥控器发送停止命令，或时间大于500ms就停止实验
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
  if('s'==nrf_rciv[BACKUP1_OFFSET])
  {//向电脑发送链表中的数据
    if(pit_25ms_flag)        //以25ms周期发送，画1个点50ms,详情看SCISend_to_Own函数
    {
      send_type = 'e';
      SCISend_to_Own(USART1);
      pit_25ms_flag = 0;
    }
  }
  if('l'==nrf_rciv[PLANE_MODE_OFFSET])
    return stb_pre;
  else
    return experiment_pro;
}

//Brief:R(Real)T(time)DataTX,实时数据传输模式
//PROCEDURE RTDataTX(char input)
//{
//  
//}

//Brief:函数（任务）初始化，函数和指针对应起来;注意检查任务指针是否超过最大任务数量
//Parameters:
//  _Task:函数指针数组，该数组罗列了所有非中断函数;
//Return:
//  下一次需要执行的任务，例如stb_pre，则下一次调用函数将进入StbPrep函数;
//Example:;
PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char))
{
  assert(stb_pre>=0 && stb_pre<MAX_TASK_NUM);
  _Task[stb_pre] = StbPrep;                        //_Task[0]是StandByPrepare函数的地址
  assert(standby>=0 && standby<MAX_TASK_NUM);
  _Task[standby] = Standby;
  assert(att_hld_pre>=0 && att_hld_pre<MAX_TASK_NUM);
  _Task[att_hld_pre] = AttHldPrep;
  assert(att_hld>=0 && att_hld<MAX_TASK_NUM);
  _Task[att_hld] = AttHld;
  assert(set_para_pre>=0 && set_para_pre<MAX_TASK_NUM);
  _Task[set_para_pre] = SetParaPre;
  assert(set_para>=0 && set_para<MAX_TASK_NUM);
  _Task[set_para] = SetParam;           //检查指针
  assert(experiment_pro>=0 && experiment_pro<MAX_TASK_NUM);
  _Task[experiment_pro] = Experiment;           //检查指针
  
  return stb_pre;
}
