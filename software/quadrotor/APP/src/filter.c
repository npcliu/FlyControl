#include "stm32f10x.h"
#include "delay.h"
#include "define.h"
#include "sys_init.h"
#include "ISR.h"
#include "MyIIC.h"              //IIC PORT initial
#include "Mpu6050.h"
#include "QMC5883.h"
#include "stm32f10x_MY_spi.h"
#include "LSM303.h"
#include "stm32f10x_MY_Flash.h"
#include "MyNRF24L0.h"
#include "L3GD20.h"
#include "Task.h"
#include "interact.h"           //uart initial
#include "bmp180.h"
#include "ms5611.h"
#include "math.h"
#include "matrix.h"
#include "filter.h"
#include "stdio.h"

#define _fs      1000.0/INTERUPT_CYC_IN_MS  
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
	
        //计算kalman增益
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
	//修正结果，即计算滤波值
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//更新后验估计
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
	return  kalmanInfo->filterValue;
}

//融合两个气压计测出的相对高度
float AltitudeFusion(float ms5611_relative_altitude,float bmp180_relative_altitude)
{
  float R_of_ms5611 = 0.0112, R_of_bmp180 = 0.1131;//bmp180和ms5611的测量噪声的方差
  return ((R_of_ms5611)*bmp180_relative_altitude + (R_of_bmp180)*ms5611_relative_altitude)/(R_of_ms5611+R_of_bmp180);
}

//效果不如AltitudeFusion
double BMP180AndMS5611KalmanFilterFusion()
{
        static double filterValue = 0;
	//预测下一时刻的值
	double predictValue = filterValue;   //x的先验估计由上一个时间点的后验估计值和输入信息给出，此处需要根据基站高度做一个修改
	static double BMP180_P = 50;
        static double MS5611_P = 50;
        double BMP180_R = 0.1131;
        double MS5611_R = 0.0112;
        double filter_Q = 0.0001;
        extern _bmp180 bmp180;
        extern _ms5611 ms5611;
        if(bmp180.altitude_init != 0 &&(ms5611.altitude_init != 0))
        {
          //求协方差
          BMP180_P = BMP180_P + filter_Q;  //计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q
          
          //计算kalman增益
          double BMP180_kalmanGain = BMP180_P / (BMP180_P + BMP180_R);  //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
          
          //修正结果，即计算滤波值
          double BMP180_filterValue = predictValue + (bmp180.altitude-bmp180.altitude_init - predictValue)*BMP180_kalmanGain;  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
          //更新后验估计
          BMP180_P = (1 - BMP180_kalmanGain)*BMP180_P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
   
          
          //预测下一时刻的值
          double predictValue = BMP180_filterValue;   //x的先验估计由上一个时间点的后验估计值和输入信息给出，此处需要根据基站高度做一个修改
          
          //求协方差
          MS5611_P = MS5611_P + BMP180_R;  //计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q
          
          //计算kalman增益
          double MS5611_kalmanGain = MS5611_P / (MS5611_P + MS5611_R);  //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
          
          //修正结果，即计算滤波值
          filterValue = predictValue + (ms5611.altitude-ms5611.altitude_init - predictValue)*MS5611_kalmanGain;  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
          //更新后验估计
          MS5611_P = (1 - MS5611_kalmanGain)*MS5611_P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
        }
        return filterValue;
}

//用这个函数的话ms5611算出的高度会出现nan，说明RCt6算力不够了
//加速度计和ms5611进行卡尔曼滤波。
//   |1 dt 0.5dt^2|
// x=|0 1 dt     | x
//  | 0 0 1     |

//y=[1 0 0
//   0 0 1]*x
//x = [位置，速度，加速度]
//y=[位置，加速度]
#define dt      (float)0.02//应该按ms5611的刷新速率来
void AccAndMS5611KalmanFilterFusion(float* x,float altitude,float acc)
{
  float Q[3][3] = {{0.00001,0,0},{0,0,0},{0,0,0.005}};//过程噪声
  float R[2][2] = {{0.0112,0},{0,0.5}};//观测噪声
  float A[3][3] = {{1,dt,0.5*dt*dt},{0,1,dt},{0,0,1}};
  float AT[3][3] = {{1,0,0},{dt,1,0},{0.5*dt*dt,dt,1}};
  float H[2][3] = {{1,0,0},{0,0,1}};
  float HT[3][2] = {{1,0},{0,0},{0,1}};  
  static float P[3][3] = {{10,0,0},{0,10,0},{0,0,10}};
  float predict_x[3] = {0};
  float K[3][2];

  float A_plus_P[3][3];
  float PHT[3][2]  ;
  float HPHT_plus_R[2][2];
  float inv_of_HPHT_plus_R[2][2];
  float Hpredictx[2];
  float I__KH[3][3];
  float P_temp[3][3];
  float temp;
  
  //predict_x=Ax
  for(char i = 0;i<3;i++)
  {
    predict_x[i] = A[i][0]*x[0] + A[i][1]*x[1] + A[i][2]*x[2];
  }
  //A*P
  
  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<3;j++)
      {
        A_plus_P[i][j] = A[i][0]*P[0][j]+A[i][1]*P[1][j]+A[i][2]*P[2][j];
      }
  }
  //P=APA^T+Q
  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<3;j++)
      {
        P[i][j] = A_plus_P[i][0]*AT[0][j]+A_plus_P[i][1]*AT[1][j]+A_plus_P[i][2]*AT[2][j] + Q[i][j];
      }
  }
  //PH^T
  
  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<2;j++)
      {
        PHT[i][j] = P[i][0]*HT[0][j]+P[i][1]*HT[1][j]+P[i][2]*HT[2][j];
      }
  }
  //HPH^T+R
  
  for(char i = 0;i<2;i++)
  {
      for(char j = 0;j<2;j++)
      {
        HPHT_plus_R[i][j] = H[i][0]*PHT[0][j]+H[i][1]*PHT[1][j]+H[i][2]*PHT[2][j] + R[i][j];
      }
  } 
  //HPH^T+R^(-1)
  
  temp = HPHT_plus_R[0][0]*HPHT_plus_R[1][1] - HPHT_plus_R[0][1]*HPHT_plus_R[1][0];
  if(temp!=0)
  {
    inv_of_HPHT_plus_R[0][0] =HPHT_plus_R[1][1]/temp;
    inv_of_HPHT_plus_R[0][1] =-HPHT_plus_R[1][0]/temp;
    inv_of_HPHT_plus_R[1][0] =-HPHT_plus_R[0][1]/temp;
    inv_of_HPHT_plus_R[1][1] =HPHT_plus_R[0][0]/temp;
  }
  else
    return ;
  //k=PHT(HPHT+R)^(-1)
  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<2;j++)
      {
        K[i][j] = PHT[i][0]*inv_of_HPHT_plus_R[0][j]+PHT[i][1]*inv_of_HPHT_plus_R[1][j];
      }
  } 
  //Hpredictx
  
  Hpredictx[0] = H[0][0]*predict_x[0]+H[0][1]*predict_x[1]+H[0][2]*predict_x[2];
  Hpredictx[1] = H[1][0]*predict_x[0]+H[1][1]*predict_x[1]+H[1][2]*predict_x[2];
  
  //K(y-Hpredictx)
  for(char i = 0;i<3;i++)
  {
     x[i] = predict_x[i] + K[i][0]*(altitude - Hpredictx[0])+K[i][1]*(acc - Hpredictx[1]);
 
  }
  //I-KH
  float I[3][3] = {1,0,0,0,1,0,0,0,1};
  
  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<3;j++)
      {
        I__KH[i][j] = I[i][j] - K[i][0]*H[0][j]-K[i][1]*H[1][j];
      }
  } 
  //(I-KH)P

  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<3;j++)
      {
        P_temp[i][j] = I__KH[i][0]*P[0][j]+I__KH[i][1]*P[1][j]+I__KH[i][2]*P[2][j];
      }
  }
  //计算完(I-KH)P后才能给P赋值
  for(char i = 0;i<3;i++)
  {
      for(char j = 0;j<3;j++)
      {
        P[i][j] = P_temp[i][j];
      }
  }  
}

//void kalamFilterFusion(const float x[3], float altitude, float acc, float P[9], float
//                 filterx[3], float TEMPP[9])
//{
//  int coffset;
//  int boffset;
//  int a_tmp;
//  float c[4];
//  float a[9];
//  float r;
//  static const float b[9] = { 1.0F, 0.02F, 0.0002F, 0.0F, 1.0F, 0.02F, 0.0F,
//    0.0F, 1.0F };
//
//  static const float Q[9] = { 1.0E-5F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
//    0.0F, 0.001F };
//
//  static const float R[4] = { 0.0112F, 0.0F, 0.0F, 0.1F };
//  
//  static const float b_a[9] = { 1.0F, 0.0F, 0.0F, 0.02F, 1.0F, 0.0F, 0.0002F,
//    0.02F, 1.0F };
//
//  float t;
//  float y[4];
//  float c_a[6];
//  static const signed char d_a[6] = { 1, 0, 0, 0, 0, 1 };
//
//  static const signed char b_b[6] = { 1, 0, 0, 0, 0, 1 };
//
//
//  float b_c[6];
//  static const signed char iv0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
//
//  float c_data[9];
//  float b_altitude[2];
//  int k;
//  int aoffset;
//  int temp;
//  static const signed char iv1[6] = { 1, 0, 0, 0, 0, 1 };
//
//  float c_data_idx_2;
//  for (coffset = 0; coffset < 3; coffset++) {
//    filterx[coffset] = 0.0F;
//    for (boffset = 0; boffset < 3; boffset++) {
//      a_tmp = coffset + 3 * boffset;
//      a[a_tmp] = 0.0F;
//      a[a_tmp] = (b_a[coffset] * P[3 * boffset] + b_a[coffset + 3] * P[1 + 3 *
//                  boffset]) + b_a[coffset + 6] * P[2 + 3 * boffset];
//      filterx[coffset] += b_a[a_tmp] * x[boffset];
//    }
//  }
//
//  for (coffset = 0; coffset < 3; coffset++) {
//    for (boffset = 0; boffset < 3; boffset++) {
//      a_tmp = coffset + 3 * boffset;
//      P[a_tmp] = ((a[coffset] * b[3 * boffset] + a[coffset + 3] * b[1 + 3 *
//                   boffset]) + a[coffset + 6] * b[2 + 3 * boffset]) + Q[a_tmp];
//    }
//  }
//
//  for (coffset = 0; coffset < 2; coffset++) {
//    for (boffset = 0; boffset < 3; boffset++) {
//      a_tmp = coffset + (boffset << 1);
//      c_a[a_tmp] = 0.0F;
//      c_a[a_tmp] = ((float)d_a[coffset] * P[3 * boffset] + 0.0F * P[1 + 3 *
//                    boffset]) + (float)d_a[coffset + 4] * P[2 + 3 * boffset];
//    }
//
//    for (boffset = 0; boffset < 2; boffset++) {
//      a_tmp = coffset + (boffset << 1);
//      c[a_tmp] = ((c_a[coffset] * (float)b_b[3 * boffset] + c_a[coffset + 2] *
//                   0.0F) + c_a[coffset + 4] * (float)b_b[2 + 3 * boffset]) +
//        R[a_tmp];
//    }
//  }
//
//  if (fabsf(c[1]) > fabsf(c[0])) {
//    r = c[0] / c[1];
//    t = 1.0F / (r * c[3] - c[2]);
//    y[0] = c[3] / c[1] * t;
//    y[1] = -t;
//    y[2] = -c[2] / c[1] * t;
//    y[3] = r * t;
//  } else {
//    r = c[1] / c[0];
//    t = 1.0F / (c[3] - r * c[2]);
//    y[0] = c[3] / c[0] * t;
//    y[1] = -r * t;
//    y[2] = -c[2] / c[0] * t;
//    y[3] = t;
//  }
//
//  for (coffset = 0; coffset < 3; coffset++) {
//    for (boffset = 0; boffset < 2; boffset++) {
//      a_tmp = coffset + 3 * boffset;
//      c_a[a_tmp] = 0.0F;
//      c_a[a_tmp] = (P[coffset] * (float)b_b[3 * boffset] + P[coffset + 3] * 0.0F)
//        + P[coffset + 6] * (float)b_b[2 + 3 * boffset];
//    }
//
//    b_c[coffset] = 0.0F;
//    r = c_a[coffset + 3];
//    b_c[coffset] = c_a[coffset] * y[0] + r * y[1];
//    b_c[coffset + 3] = 0.0F;
//    b_c[coffset + 3] = c_a[coffset] * y[2] + r * y[3];
//  }
//
//  for (a_tmp = 0; a_tmp < 3; a_tmp++) {
//    coffset = a_tmp * 3;
//    boffset = a_tmp << 1;
//    c_data[coffset] = 0.0F;
//    c_data[coffset + 1] = 0.0F;
//    c_data[coffset + 2] = 0.0F;
//    for (k = 0; k < 2; k++) {
//      aoffset = k * 3;
//      temp = iv1[boffset + k];
//      c_data[coffset] += (float)temp * b_c[aoffset];
//      c_data[coffset + 1] += (float)temp * b_c[aoffset + 1];
//      c_data[coffset + 2] += (float)temp * b_c[aoffset + 2];
//    }
//  }
//
//  for (coffset = 0; coffset < 9; coffset++) {
//    a[coffset] = (float)iv0[coffset] - c_data[coffset];
//  }
//
//  for (coffset = 0; coffset < 3; coffset++) {
//    for (boffset = 0; boffset < 3; boffset++) {
//      a_tmp = coffset + 3 * boffset;
//      TEMPP[a_tmp] = 0.0F;
//      TEMPP[a_tmp] = (a[coffset] * P[3 * boffset] + a[coffset + 3] * P[1 + 3 *
//                      boffset]) + a[coffset + 6] * P[2 + 3 * boffset];
//    }
//  }
//
//  b_altitude[0] = altitude;
//  b_altitude[1] = acc;
//  for (coffset = 0; coffset < 2; coffset++) {
//    b_altitude[coffset] -= ((float)d_a[coffset] * filterx[0] + 0.0F * filterx[1])
//      + (float)d_a[coffset + 4] * filterx[2];
//  }
//
//  r = 0.0F;
//  t = 0.0F;
//  c_data_idx_2 = 0.0F;
//  for (k = 0; k < 2; k++) {
//    aoffset = k * 3;
//    r += b_altitude[k] * b_c[aoffset];
//    t += b_altitude[k] * b_c[aoffset + 1];
//    c_data_idx_2 += b_altitude[k] * b_c[aoffset + 2];
//  }
//
//  filterx[0] += r;
//  filterx[1] += t;
//  filterx[2] += c_data_idx_2;
//}
//
