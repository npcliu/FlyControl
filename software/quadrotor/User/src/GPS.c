#include "stm32f10x.h"
//#include "math.h"
#include "GPS.h"

//����һ�����ݰ���GPS
void SendGPSPkg(USART_TypeDef* USARTx, uint8 *p)
{
  uint16 i = 0;
  for(i=0;i<23;i++)
    USART_SendData(USARTx, p[i]);
}
//����UBLOX NEO-6�ĸ�������	    
//measrate:����ʱ��������λΪms�����ٲ���С��200ms��5Hz��
//reftime:�ο�ʱ�䣬0=UTC Time��1=GPS Time��һ������Ϊ1��
//����ֵ:0,���ͳɹ�;����,����ʧ��.
unsigned char Ublox_Cfg_Rate(unsigned short measrate,unsigned char reftime)
{
  _ublox_cfg_rate cfg_ratedata={0};//(_ublox_cfg_rate *)USART2_TX_BUF;
  _ublox_cfg_rate *cfg_rate=&cfg_ratedata;//(_ublox_cfg_rate *)USART2_TX_BUF;
  if(measrate<200)
    return 1;	//С��200ms��ֱ���˳� 
  cfg_rate->header=0X62B5;	//cfg header
  cfg_rate->id=0X0806;	 	//cfg rate id
  cfg_rate->dlength=6;	 	//����������Ϊ6���ֽ�.
  cfg_rate->measrate=measrate;//������,ms
  cfg_rate->navrate=1;		//�������ʣ����ڣ����̶�Ϊ1
  cfg_rate->timeref=reftime; 	//�ο�ʱ��ΪGPSʱ��
  Ublox_CheckSum((unsigned char*)(&cfg_rate->id),sizeof(_ublox_cfg_rate)-4,&cfg_rate->cka,&cfg_rate->ckb);
  //	while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
  //	UART_DMA_Enable(DMA1_Channel7,sizeof(_ublox_cfg_rate));//ͨ��dma���ͳ�ȥ
  Ublox_Send_Date((unsigned char*)cfg_rate,sizeof(_ublox_cfg_rate));//�������ݸ�NEO-6M
  return 1;//Ublox_Cfg_Ack_Check();
}
void Ublox_Send_Date(unsigned char* dbuf,unsigned short len)
{
  unsigned short j;
  for(j=0;j<len;j++)//ѭ����������
  {
    while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR=dbuf[j];  
  }	
}
//GPSУ��ͼ���
//buf:���ݻ������׵�ַ
//len:���ݳ���
//cka,ckb:����У����.
void Ublox_CheckSum(unsigned char *buf,unsigned short len,unsigned char* cka,unsigned char*ckb)
{
  unsigned short i;
  *cka=0;*ckb=0;
  for(i=0;i<len;i++)
  {
    *cka=*cka+buf[i];
    *ckb=*ckb+*cka;
  }
}
//unsigned short USART2_Scan(u16 *len)
//{
//  unsigned short int ftemp = 0;
//  ftemp = RX2_Point;
//  *len=0;
//  if( ftemp != 0 )
//  {
//    delay_ms(100);
//    while ( ftemp != RX2_Point )
//    {
//      ftemp = RX2_Point;
//      delay_ms(100);
//    }
//    RX2_Point = 0;      /* ����ָ�� */														
//    *len= ftemp;
//    return 1;   /* ɨ�赽���ݣ�����1 */				
//  }
//  return 0;
//}
//���CFG����ִ�����
//����ֵ:0,ACK�ɹ�
//       1,���ճ�ʱ����
//       2,û���ҵ�ͬ���ַ�
//       3,���յ�NACKӦ��
//unsigned char Ublox_Cfg_Ack_Check(void)
//{
//  unsigned short len=0,i,rxlen;
//  unsigned char rval=0;
//  while(USART2_Scan(&rxlen)==0 && len<100)//�ȴ����յ�Ӧ��   
//  {
//    len++;
//    delay_ms(5);
//  }
//  if(len<100)   	//��ʱ����.
//  {
//    for(i=0;i<rxlen;i++)
//      if(RX2_Temp[i]==0XB5)
//        break;//����ͬ���ַ� 0XB5
//    if(i==rxlen)
//      rval=2;						//û���ҵ�ͬ���ַ�
//    else if(RX2_Temp[i+3]==0X00)
//      rval=3;//���յ�NACKӦ��
//    else rval=0;	   						//���յ�ACKӦ��
//  }
//  else rval=1;								//���ճ�ʱ����
//  MyPrintf("rval = %d\r\n",rval);
//  return rval;  
//}