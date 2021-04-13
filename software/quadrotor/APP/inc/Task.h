#ifndef __TASK_H
#define __TASK_H

//#define MAX_TASK_NUM    10

typedef enum
{
  stb_pre = 0,                      //standby prepare，进入待机模式前的准备
  standby,                              //standby待机程序
  att_hld_pre,                              //姿态控制飞前准备
  att_hld,                               //姿态控制模式
  height_hld_pre,
  height_hld,
  pos_hld_prep,                               //空间位置控制模式肥前准备
  pos_hld,                               //空间位置控制模式
  land,                         //降落
  set_para_pre,                 //set_parameters      //参数设置准备工作
  set_para,                             //参数设置程序
  experiment_pro,
  MAX_TASK_NUM,
}PROCEDURE;                             //“进程”/“任务”，


extern PROCEDURE (* Task[MAX_TASK_NUM])(char input);
extern PROCEDURE next_procedure;


PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char));   //函数指针数组;

#endif