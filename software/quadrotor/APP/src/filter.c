#include "stm32f10x.h"
#include "filter.h"
#include "math.h"
#include "define.h"

#define _fs      200//����Ƶ�ʣ�1000.0/INTERUPT_CYC_IN_MS  
//���׵�ͨ�˲���
float SecondOrderLPF(LPFParam *_PLPFParam)
{
  float output;

  float _ohm=tan(3.1415926 * (_PLPFParam->fd) / _fs); 

  //����������ģ���Ƶ�ʣ�wa = 2*fs*tan(pi*fd/fs)��
  //Ȼ����˫���Ա任��ӳ�䵽Z��Ȼ��������ʱ����ϵͳ�Ŀɽ����Ի��򴫵ݺ������ͱ������������
  float _c  =  1 + 1.414 * _ohm + _ohm * _ohm    ; 
  float _b0    =  _ohm * _ohm / _c;
  float _b1   =  2.0*_b0;
  float _b2   =  _b0;
  float _a1   =  2.0 * (_ohm * _ohm - 1.0) / _c;
  float _a2   =  (1.0 - 1.414 * _ohm + _ohm * _ohm) / _c;

   //���׵�ͨ�˲�������ֹƵ��fsΪ1HZ
  //_dΪ���ݺ������м����
  float _d = _PLPFParam->input - _a1 * _PLPFParam->_d_delay_element_1 - _a2 * _PLPFParam->_d_delay_element_2;
  output = _b0 * _d + _b1 * _PLPFParam->_d_delay_element_1 + _b2 *_PLPFParam-> _d_delay_element_2;
  _PLPFParam->_d_delay_element_2 = _PLPFParam->_d_delay_element_1;
  _PLPFParam->  _d_delay_element_1 = _d;
   return output;
}

/********************************һά�������˲���********************************/

/**
* @brief Init_KalmanInfo   ��ʼ���˲����ĳ�ʼֵ
* @param info  �˲���ָ��
* @param Q Ԥ���������� ��ϵͳ�ⲿ�ⶨ����
* @param R ������������ ��ϵͳ�ⲿ�ⶨ����
*/
void Init_KalmanInfo(KalmanInfo* info, double Q, double R)
{
	info->A = 1;  //����������
	info->H = 1;  //
	info->P = 50;  //����״̬����ֵ���ķ���ĳ�ʼֵ����ҪΪ0���ⲻ��
	info->Q = Q;    //Ԥ�⣨���̣��������� Ӱ���������ʣ����Ը���ʵ���������
	info->R = R;    //�������۲⣩�������� ����ͨ��ʵ���ֶλ��
	info->filterValue = 0;// �����ĳ�ʼֵ
}
double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement)
{
	//Ԥ����һʱ�̵�ֵ
	double predictValue = kalmanInfo->A* kalmanInfo->filterValue;   //x�������������һ��ʱ���ĺ������ֵ��������Ϣ�������˴���Ҫ���ݻ�վ�߶���һ���޸�
	
	//��Э����
	kalmanInfo->P = kalmanInfo->A*kalmanInfo->A*kalmanInfo->P + kalmanInfo->Q;  //������������� p(n|n-1)=A^2*p(n-1|n-1)+q
	double preValue = kalmanInfo->filterValue;  //��¼�ϴ�ʵ�������ֵ
 
	//����kalman����
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H�� / (H P(k|k-1) H�� + R)
	//����������������˲�ֵ
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //���ò������Ϣ���ƶ�x(t)�Ĺ��ƣ�����������ƣ����ֵҲ�������  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//���º������
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//������������  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
	return  kalmanInfo->filterValue;
}