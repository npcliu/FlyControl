#ifndef _FILTER_H_
#define _FILTER_H_


typedef struct
{           
  float fd;    //数字截止频率                   //线性映射之斜率
  float input;     //信号输入   
  float _d_delay_element_1;
  float _d_delay_element_2;
}LPFParam, *PLPFParam;//二阶低通滤波器参数

// 一维卡尔曼滤波器信息结构体
typedef  struct{
	double filterValue;  //k-1时刻的滤波值，即是k-1时刻的值
	double kalmanGain;   //   Kalamn增益
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //预测过程噪声偏差的方差
	double R;   //测量噪声偏差，(系统搭建好以后，通过测量统计实验获得)
	double P;   //估计误差协方差
}  KalmanInfo;

float SecondOrderLPF(LPFParam *_PLPFParam);
void Init_KalmanInfo(KalmanInfo* info, double Q, double R);
double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement);
float AltitudeFusion(float ms5611_relative_altitude,float bmp180_relative_altitude);
double BMP180AndMS5611KalmanFilterFusion();
void AccAndMS5611KalmanFilterFusion(float* x,float altitude,float acc);

//void kalamFilterFusion(const float x[3], float altitude, float acc, float P[9], float
//                 filterx[3], float TEMPP[9]);
#endif