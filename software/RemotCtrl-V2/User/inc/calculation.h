#ifndef __CALCULATION_H
#define __CALCULATION_H

//#define ACC_FILT_ORDER  50
#define ANG_CTRL_RATE   0.25            //ƫת�ǵı��ʣ���ң���յ���ADC�źų��Ըñ��ʵõ�����ƫת�Ƕ�

//typedef struct Node
//{
//  float data;
//  struct Node * p_next;
//}ACC_ANG,*PACC_ANG;

//extern PACC_ANG p_acc_angx_list;
//extern PACC_ANG p_acc_angy_list;
//extern PACC_ANG pnow;       
extern float angx_err;
extern float angy_err;

//void GPSCal(nmeaINFO *info, uint32 th_vs_mg);


#endif
