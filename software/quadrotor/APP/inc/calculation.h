#ifndef __CALCULATION_H
#define __CALCULATION_H

#include "define.h"

#define ANG_CTRL_RATE   0.3            //偏转角的比率，从遥控收到的ADC信号乘以该比率得到期望偏转角度

//#define WATCH_INTEGRAL_ANGLE    //观察陀螺积分角度，如果需要单独计算陀螺积分的角度则定义此宏，否则注释该定义，
//#define WATCH_DIFF_COEF         //观察带滤波差分的结果
    
extern float angle[3];
extern float offset_angle[3];
extern float x_b,y_b;       
extern float z_p,z_d;
extern float x_p_o;                     //x(x axis)_p(proportion)_o(out loop)_t(ten shape fly)x轴十形飞行控制参数
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
