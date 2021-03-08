#include "stm32f10x.h"
//#include "math.h"
#include "GPS.h"

//发送一个数据包给GPS
void SendGPSPkg(USART_TypeDef* USARTx, uint8 *p)
{
  uint16 i = 0;
  for(i=0;i<23;i++)
    USART_SendData(USARTx, p[i]);
}
//配置UBLOX NEO-6的更新速率	    
//measrate:测量时间间隔，单位为ms，最少不能小于200ms（5Hz）
//reftime:参考时间，0=UTC Time；1=GPS Time（一般设置为1）
//返回值:0,发送成功;其他,发送失败.
unsigned char Ublox_Cfg_Rate(unsigned short measrate,unsigned char reftime)
{
  _ublox_cfg_rate cfg_ratedata={0};//(_ublox_cfg_rate *)USART2_TX_BUF;
  _ublox_cfg_rate *cfg_rate=&cfg_ratedata;//(_ublox_cfg_rate *)USART2_TX_BUF;
  if(measrate<200)
    return 1;	//小于200ms，直接退出 
  cfg_rate->header=0X62B5;	//cfg header
  cfg_rate->id=0X0806;	 	//cfg rate id
  cfg_rate->dlength=6;	 	//数据区长度为6个字节.
  cfg_rate->measrate=measrate;//脉冲间隔,ms
  cfg_rate->navrate=1;		//导航速率（周期），固定为1
  cfg_rate->timeref=reftime; 	//参考时间为GPS时间
  Ublox_CheckSum((unsigned char*)(&cfg_rate->id),sizeof(_ublox_cfg_rate)-4,&cfg_rate->cka,&cfg_rate->ckb);
  //	while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
  //	UART_DMA_Enable(DMA1_Channel7,sizeof(_ublox_cfg_rate));//通过dma发送出去
  Ublox_Send_Date((unsigned char*)cfg_rate,sizeof(_ublox_cfg_rate));//发送数据给NEO-6M
  return 1;//Ublox_Cfg_Ack_Check();
}
void Ublox_Send_Date(unsigned char* dbuf,unsigned short len)
{
  unsigned short j;
  for(j=0;j<len;j++)//循环发送数据
  {
    while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR=dbuf[j];  
  }	
}
//GPS校验和计算
//buf:数据缓存区首地址
//len:数据长度
//cka,ckb:两个校验结果.
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
//    RX2_Point = 0;      /* 重置指针 */														
//    *len= ftemp;
//    return 1;   /* 扫描到数据，返回1 */				
//  }
//  return 0;
//}
//检查CFG配置执行情况
//返回值:0,ACK成功
//       1,接收超时错误
//       2,没有找到同步字符
//       3,接收到NACK应答
//unsigned char Ublox_Cfg_Ack_Check(void)
//{
//  unsigned short len=0,i,rxlen;
//  unsigned char rval=0;
//  while(USART2_Scan(&rxlen)==0 && len<100)//等待接收到应答   
//  {
//    len++;
//    delay_ms(5);
//  }
//  if(len<100)   	//超时错误.
//  {
//    for(i=0;i<rxlen;i++)
//      if(RX2_Temp[i]==0XB5)
//        break;//查找同步字符 0XB5
//    if(i==rxlen)
//      rval=2;						//没有找到同步字符
//    else if(RX2_Temp[i+3]==0X00)
//      rval=3;//接收到NACK应答
//    else rval=0;	   						//接收到ACK应答
//  }
//  else rval=1;								//接收超时错误
//  MyPrintf("rval = %d\r\n",rval);
//  return rval;  
//}