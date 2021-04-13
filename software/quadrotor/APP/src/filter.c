#include "stm32f10x.h"
#include "filter.h"
#include "math.h"
#include "define.h"

#define _fs      200//采样频率：1000.0/INTERUPT_CYC_IN_MS  
//二阶低通滤波器
float SecondOrderLPF(LPFParam *_PLPFParam)
{
  float output;

  float _ohm=tan(3.1415926 * (_PLPFParam->fd) / _fs); 

  //经过化简后的模拟角频率（wa = 2*fs*tan(pi*fd/fs)）
  //然后用双线性变换法映射到Z域，然后用线性时不变系统的可交换性化简传递函数，就变成了下述程序
  float _c  =  1 + 1.414 * _ohm + _ohm * _ohm    ; 
  float _b0    =  _ohm * _ohm / _c;
  float _b1   =  2.0*_b0;
  float _b2   =  _b0;
  float _a1   =  2.0 * (_ohm * _ohm - 1.0) / _c;
  float _a2   =  (1.0 - 1.414 * _ohm + _ohm * _ohm) / _c;

   //二阶低通滤波器，截止频率fs为1HZ
  //_d为传递函数的中间变量
  float _d = _PLPFParam->input - _a1 * _PLPFParam->_d_delay_element_1 - _a2 * _PLPFParam->_d_delay_element_2;
  output = _b0 * _d + _b1 * _PLPFParam->_d_delay_element_1 + _b2 *_PLPFParam-> _d_delay_element_2;
  _PLPFParam->_d_delay_element_2 = _PLPFParam->_d_delay_element_1;
  _PLPFParam->  _d_delay_element_1 = _d;
   return output;
}

/********************************一维卡尔曼滤波器********************************/

/**
* @brief Init_KalmanInfo   初始化滤波器的初始值
* @param info  滤波器指针
* @param Q 预测噪声方差 由系统外部测定给定
* @param R 测量噪声方差 由系统外部测定给定
*/
void Init_KalmanInfo(KalmanInfo* info, double Q, double R)
{
	info->A = 1;  //标量卡尔曼
	info->H = 1;  //
	info->P = 50;  //后验状态估计值误差的方差的初始值（不要为0问题不大）
	info->Q = Q;    //预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
	info->R = R;    //测量（观测）噪声方差 可以通过实验手段获得
	info->filterValue = 0;// 测量的初始值
}
double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement)
{
	//预测下一时刻的值
	double predictValue = kalmanInfo->A* kalmanInfo->filterValue;   //x的先验估计由上一个时间点的后验估计值和输入信息给出，此处需要根据基站高度做一个修改
	
	//求协方差
	kalmanInfo->P = kalmanInfo->A*kalmanInfo->A*kalmanInfo->P + kalmanInfo->Q;  //计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q
	double preValue = kalmanInfo->filterValue;  //记录上次实际坐标的值
 
	//计算kalman增益
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
	//修正结果，即计算滤波值
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//更新后验估计
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
	return  kalmanInfo->filterValue;
}