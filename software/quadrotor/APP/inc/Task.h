#ifndef __TASK_H
#define __TASK_H

//#define MAX_TASK_NUM    10

typedef enum
{
  stb_pre = 0,                      //standby prepare���������ģʽǰ��׼��
  standby,                              //standby��������
  att_hld_pre,                              //��̬���Ʒ�ǰ׼��
  att_hld,                               //��̬����ģʽ
  height_hld_pre,
  height_hld,
  pos_hld_prep,                               //�ռ�λ�ÿ���ģʽ��ǰ׼��
  pos_hld,                               //�ռ�λ�ÿ���ģʽ
  land,                         //����
  set_para_pre,                 //set_parameters      //��������׼������
  set_para,                             //�������ó���
  experiment_pro,
  MAX_TASK_NUM,
}PROCEDURE;                             //�����̡�/�����񡱣�


extern PROCEDURE (* Task[MAX_TASK_NUM])(char input);
extern PROCEDURE next_procedure;


PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char));   //����ָ������;

#endif