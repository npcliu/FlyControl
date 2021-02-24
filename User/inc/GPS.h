#ifndef __GPS_H
#define __GPS_H

__packed typedef struct {
  unsigned short header;					//cfg header,�̶�Ϊ0X62B5(С��ģʽ)
  unsigned short id;						//CFG RATE ID:0X0806 (С��ģʽ)
  unsigned short dlength;				//���ݳ���
  unsigned short measrate;				//����ʱ��������λΪms�����ٲ���С��200ms��5Hz��
  unsigned short navrate;				//�������ʣ����ڣ����̶�Ϊ1
  unsigned short timeref;				//�ο�ʱ�䣺0=UTC Time��1=GPS Time��
  unsigned char  cka;					//У��CK_A
  unsigned char  ckb;					//У��CK_B
}_ublox_cfg_rate; 

unsigned char Ublox_Cfg_Rate(unsigned short measrate,unsigned char reftime);
void SendGPSPkg(USART_TypeDef* USARTx, uint8 *p);
void Ublox_CheckSum(unsigned char *buf,unsigned short len,unsigned char* cka,unsigned char*ckb);
void Ublox_Send_Date(unsigned char* dbuf,unsigned short len);
unsigned char Ublox_Cfg_Ack_Check(void);

//RMC(recommend minimum special GPS/transit data)���Ƽ���С��λ��Ϣ
//VTG(track made good and ground speed):�����ٶ���Ϣ//GLL:�ض�λ��Ϣ
//GGA(global positioning system fix data)��GPS��λ��Ϣ
//GSA��GSP DOP and active satellite��:��ǰ������Ϣ
//GSV(GPS satellite in view)���ɼ�������Ϣ
//GLL

#endif