#ifndef __CALCULATION_H
#define __CALCULATION_H

#include "define.h"

#define ANG_CTRL_RATE   0.3            //ƫת�ǵı��ʣ���ң���յ���ADC�źų��Ըñ��ʵõ�����ƫת�Ƕ�

//#define WATCH_INTEGRAL_ANGLE    //�۲����ݻ��ֽǶȣ������Ҫ�����������ݻ��ֵĽǶ�����˺꣬����ע�͸ö��壬
//#define WATCH_DIFF_COEF         //�۲���˲���ֵĽ��
    
extern float angle[3];
extern float offset_angle[3];
extern float x_b,y_b;       
extern float z_p,z_d;
extern float x_p_o;                     //x(x axis)_p(proportion)_o(out loop)_t(ten shape fly)x��ʮ�η��п��Ʋ���
extern float x_p_i;
extern float x_d_i;
extern float y_p_o;
extern float y_p_i;
extern float y_d_i;

extern float angx_err;
extern float angy_err;
extern float xcq,ycq;

//PACC_ANG CreatList(uint32 len);
void CaliFilt(float *pfilted_acc,float *pfilted_gyro,float *pfilted_cps,const PACC pacc,const PGYRO pgyro,const PCOMPASS pcps,short* pacc_chip_data,short* pgyro_chip_data,short* pcps_chip_data);
//void GPSCal(nmeaINFO *info, uint32 th_vs_mg);
void AttCalc(float * pangle,float *pacc,float* pgyro,float *pcps, uint8 mod);
void PWMCalc(uint8 mod);


#endif
