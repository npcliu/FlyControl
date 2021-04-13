#ifndef __TASK_H
#define __TASK_H

typedef enum
{
  stb_pre = 0,
  standby,
  att_pre,
  att_hld,
  hei_hld_pre,//����ģʽ
  hei_hld,
  set_para_pre,   //set_parameters
  set_para,
  experiment_pro,       //ʵ��ģʽ
  MAX_TASK_NUM,
}PROCEDURE;                             //�����̡�/�����񡱣�


extern PROCEDURE (* Task[MAX_TASK_NUM])(char input);
extern PROCEDURE next_procedure;
//extern PROCEDURE procedure;


PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char));   //����ָ������

#endif