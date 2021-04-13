#ifndef __TASK_H
#define __TASK_H

typedef enum
{
  stb_pre = 0,
  standby,
  att_pre,
  att_hld,
  hei_hld_pre,//定高模式
  hei_hld,
  set_para_pre,   //set_parameters
  set_para,
  experiment_pro,       //实验模式
  MAX_TASK_NUM,
}PROCEDURE;                             //“进程”/“任务”，


extern PROCEDURE (* Task[MAX_TASK_NUM])(char input);
extern PROCEDURE next_procedure;
//extern PROCEDURE procedure;


PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char));   //函数指针数组

#endif