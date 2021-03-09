#include <stdio.h>
#include "math.h"
#include "stm32f10x_usart.h"
#include "delay.h"
/********数据源*********/
#include "calculation.h"
#include "MyNRF24L0.h"
#include "define.h"
#include <info.h>
#include "DataStructure.h"
#include "interact.h"


PSNODE anglex_pnode;
PSNODE angley_pnode;
PSNODE anglez_pnode;


void UARTSendFloat(float d)
{
   int i = 0;
   
   for(i=0;i<4;i++)
   {
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
      USART_SendData(USART1,(*(uint32*)&d)>>(i<<3));
      DelayUs(1);
   }
}
extern short amplitude;
/************************************************
*  函数名称：SCISend_to_PIDDebug，函数调用2次画1个点
*  功能说明：SCI发送到串口示波器
*  参数说明：UARTn为第几个串口
*  函数返回：无
0=红;  1=蓝; 2=浅蓝; 3=黄 4=浅绿
*  修改时间：2014-1-14    已经测试DLUT上位机默认数据为short格式;因此数据不要超过-32767~32767区间
0：red;1=blue;2=light blue;3=yellow;4=light green;5=dark green
*************************************************/
char send_type = 'X';
extern float pwm_of_dir;
void SCISend_to_Own(USART_TypeDef* USARTx)
{
//  static uint32 count = 0;
  static short send_data[3][8] = { { 0 }, { 0 }, { 0 } };
  int i = 0,j = 0;
  switch(send_type)
  {
  case 'x':               //[x倾角，x倾角期望][y分量加速度][？]
    send_data[0][0] = (short)angle[0]; //         
    send_data[0][1] = (short)((nrf_rciv[LR_ADC_OFFSET]-127)*ANG_CTRL_RATE);
    send_data[0][2] = (short)(gyro.y/164.0);//
    send_data[0][3] = (short)(xcq);//
    send_data[1][0] = (short)angle[0];
    send_data[1][1] = (short)(gyro.y/100.0);
    send_data[1][2] = (short)(acc.x/100.0);
    send_data[1][3] = (short)gc[1][0];
    send_data[1][4] = (short)gc[1][2];
    send_data[1][5] = (short)xcq;
    send_data[2][0] = (short)angle[0];
    send_data[2][1] = (short)acc_angle[0][0];
    send_data[2][2] = (short)gc[0][0];
    break;
  case 'X':               //[x倾角，x倾角期望][y角速度][？]
    send_data[0][0] = (short)acc_angle[0][0];
    send_data[0][1] = (short)acc_angle[0][1];
    send_data[1][0] = (short)0;
    send_data[1][1] = (short)0;
//    send_data[2][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_X_WATCH];
//    send_data[2][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_X_WATCH];
    break;
  case 'y':               //[y倾角，y倾角期望][y分量加速度][？]
    send_data[0][0] = (short)angle[1];
    send_data[0][1] = (short)((nrf_rciv[UD_ADC_OFFSET]-127)*ANG_CTRL_RATE);
    send_data[0][2] = (short)(ycq);
    send_data[1][0] = (short)acc.y;
    send_data[1][1] = (short)0;
    send_data[2][0] = (short)angle[1];
    send_data[2][1] = (short)acc_angle[0][1];
    send_data[2][2] = (short)gc[0][1];
    break;
  case 'Y':               //[y倾角，y倾角期望][x角速度][？]
    send_data[0][0] = (short)angle[1];
    send_data[0][1] = (short)((nrf_rciv[UD_ADC_OFFSET]-127)*ANG_CTRL_RATE);
    send_data[1][0] = (short)0;
    send_data[1][1] = (short)gyro.x;
//    send_data[2][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_Y_WATCH];
//    send_data[2][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_Y_WATCH];
    break;
  case 'z':               //[偏航角][？][地磁计x,y,z]
    send_data[0][0] = (short)(compass.x);
    send_data[1][1] = (short)((float)compass.y/10)*1000;
    send_data[2][2] = (short)((float)compass.z/10)*1000;
    
    break;
  case 'Z':               //[偏航角][z轴角速度][偏航角参量]
    send_data[0][0] = (short)angle[2];
//    send_data[0][1] = (short)compass.x/100;
//    send_data[0][2] = (short)compass.y/100;
//    send_data[0][3] = (short)gc[DBG_TMP_ANG_WATCH][DBG_COMPASS_TMP_ANG_Z_WATCH];
    send_data[1][0] = (short)acc.x;
    send_data[1][1] = (short)acc.y;
    send_data[1][2] = (short)acc.z;
    send_data[2][0] = (short)compass.x;
    send_data[2][1] = (short)compass.y;
    send_data[2][2] = (short)compass.z;
//    send_data[2][3] = (short)gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_M_WATCH];
    break;
  case 'p':               //[？][压力][0]
    send_data[0][0] = (short)compass.x;
    send_data[0][1] = (short)compass.y;
    send_data[0][2] = (short)compass.z;
    send_data[1][0] = (short)(pressure);
    send_data[1][1] = (short)0;
    send_data[2][0] = (short)0;
    send_data[2][1] = (short)0;
    break;
  case 'g':              //[GPS高度][速度][方向]
    send_data[0][0] = (short)(info.elv-550);
    send_data[0][1] = (short)0;
    send_data[1][0] = (short)((info.speed/3.6*10));
    send_data[1][1] = (short)0;
    send_data[2][0] = (short)info.direction;
    send_data[2][1] = (short)0;
    break;
  case 'G':              //[经纬][0][0]
    send_data[0][0] = (short)(angx_err);
    send_data[0][1] = (short)(angy_err);
    send_data[1][0] = (short)((info.lat-3.0588727600000002E+3)*1e4);
    send_data[1][1] = (short)((info.lon-1.040013563E+4)*1e4);
    send_data[2][0] = (short)angle[2];
    send_data[2][1] = (short)0;
    break;
  case 'e':
    {
      if(anglex_pnode->pNext != NULL)
      {
        anglex_pnode = anglex_pnode->pNext;//第一个节点存的链表的长度，所以直接跳过这个节点从第二个节点开始显示
        send_data[0][0] =(short)(100*(anglex_pnode->u16data)); //    
        send_data[0][1] = amplitude;
      }
      if(angley_pnode->pNext != NULL){
        angley_pnode = angley_pnode->pNext;
        send_data[1][0] =(short)(100*(angley_pnode->u16data)); //      
      }
      if(anglez_pnode->pNext != NULL){
        anglez_pnode = anglez_pnode->pNext;
        send_data[2][0] =(short)(100*(anglez_pnode->u16data)); //      
      }
    }
    break;
  case 'm':            //send messured data to matlab
    {
    }break;            //no need to send fram message outer this switch,so just return
  default:
    send_data[0][0] = (short)(acc.x);
    //    send_data[0][1] = (short)(acc.y);
    //    send_data[0][2] = (short)(acc.z);
    send_data[1][0] = (short)(LSM_acc_adc.x/2);
    //    send_data[1][1] = (short)LSM_acc_adc.y;
    //    send_data[1][2] = (short)LSM_acc_adc.z;
    send_data[2][0] = (short)(acc.x);
    send_data[2][1] = (short)0;
    break;
  }
  /*凌立印象上位机示波器协议*/
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
  DelayUs(1);                                   //经测试，此处delay要加上，推测while完成到发送时间太短，usart来不及
  USARTx->DR = 'S';
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
  DelayUs(1);
  USARTx->DR = 'T';
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
  DelayUs(1);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 8; j++)
    {
      USART_SendData(USART1,send_data[i][j]);
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
      DelayUs(1);
      USART_SendData(USART1,send_data[i][j]>>8);
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
      DelayUs(1);
    }
  //    USARTx->DR = 'S';
  //    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
  //    DelayUs(1);
  //    USARTx->DR = 'T';
}
//Parameters:
//  type:该参数的数据类型;
//  parr:参数的地址;
void SendParametersToRC()
{
  //  约定char=0，uint8=1;short=2;uint16=3;int=4,uint32=5,float=6,double=7，

//    nrf_tx((uint8*)&x_p1,4);
//    nrf_tx((uint8*)&x_i1,4);
//    nrf_tx((uint8*)&x_d1,4);
//    nrf_tx((uint8*)&x_p2,4);
//    nrf_tx((uint8*)&x_i2,4);
//    nrf_tx((uint8*)&x_d2,4);
//
//    nrf_tx((uint8*)&y_p1,4);
//    nrf_tx((uint8*)&y_i1,4);
//    nrf_tx((uint8*)&y_d1,4);
//    nrf_tx((uint8*)&y_p2,4);
//    nrf_tx((uint8*)&y_i2,4);
//    nrf_tx((uint8*)&y_d2,4);
//    
//    nrf_tx((uint8*)&z_p,4);
//    nrf_tx((uint8*)&z_i,4);
//    nrf_tx((uint8*)&z_d,4);
}

#define DR_ADC_CENTER   127     //direction center value of the adc
float angle_z_ctrl_rate = 0.6;
//brief：This function is used to convert raw data from Remote Controller to plane parameters
//such as convert direction stick adc value 127 to direction angle(offset_angle[2]) 0;
//example：
void RCDenote()
{
  static uint8 last_DR_value = 0;
  if(nrf_rciv[TH_ADC_OFFSET]>20)                //apply direction control only when plane in the air,cause unlock plane will change direction stick
    if(nrf_rciv[DR_ADC_OFFSET]>DR_ADC_CENTER)
    {
      if(nrf_rciv[DR_ADC_OFFSET]>last_DR_value)
        offset_angle[2] += angle_z_ctrl_rate*(nrf_rciv[DR_ADC_OFFSET] - last_DR_value);
    }
    else if(nrf_rciv[DR_ADC_OFFSET]<DR_ADC_CENTER)
    {
      if(nrf_rciv[DR_ADC_OFFSET]<last_DR_value)
        offset_angle[2] += angle_z_ctrl_rate*(nrf_rciv[DR_ADC_OFFSET] - last_DR_value);
    }
    else;
  last_DR_value = nrf_rciv[DR_ADC_OFFSET];
}


//u8 sendbuf[1024];
//u8 receivebuf[1024];

//初始化链表
void NodeInit()
{
  anglex_pnode = CratSList(anglex_Node_Number, 0);
  angley_pnode = CratSList(angley_Node_Number, 0);
  anglez_pnode = CratSList(anglez_Node_Number, 0);
}

//把角度数据存到链表中
void SaveAngleDataToNode(float * angle){

  if(anglex_pnode->pNext != NULL)//如果到达链表末尾，就增加节点
  {
    anglex_pnode->u16data = angle[0];//把x角度数据写入链表
    anglex_pnode = anglex_pnode->pNext;//前进到下一个结点
  }

  if(angley_pnode->pNext != NULL)//如果到达链表末尾，就增加节点
  {
    angley_pnode->u16data = angle[1];//把y角度数据写入链表
    angley_pnode = angley_pnode->pNext;//前进到下一个结点  
  }

  if(anglez_pnode->pNext != NULL)//如果到达链表末尾，就增加节点
  {
     anglez_pnode->u16data = angle[2];//把x角度数据写入链表
     anglez_pnode = anglez_pnode->pNext;//前进到下一个结点  
  }

}
