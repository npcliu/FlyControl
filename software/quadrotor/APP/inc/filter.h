#ifndef _FILTER_H_
#define _FILTER_H_


typedef struct
{           
  float fd;    //���ֽ�ֹƵ��                   //����ӳ��֮б��
  float input;     //�ź�����   
  float _d_delay_element_1;
  float _d_delay_element_2;
}LPFParam, *PLPFParam;//���׵�ͨ�˲�������

// һά�������˲�����Ϣ�ṹ��
typedef  struct{
	double filterValue;  //k-1ʱ�̵��˲�ֵ������k-1ʱ�̵�ֵ
	double kalmanGain;   //   Kalamn����
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //Ԥ���������ƫ��ķ���
	double R;   //��������ƫ�(ϵͳ����Ժ�ͨ������ͳ��ʵ����)
	double P;   //�������Э����
}  KalmanInfo;

float SecondOrderLPF(LPFParam *_PLPFParam);
void Init_KalmanInfo(KalmanInfo* info, double Q, double R);
double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement);
#endif