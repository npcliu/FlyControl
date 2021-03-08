#ifndef __GPS_H
#define __GPS_H

__packed typedef struct {
  unsigned short header;					//cfg header,固定为0X62B5(小端模式)
  unsigned short id;						//CFG RATE ID:0X0806 (小端模式)
  unsigned short dlength;				//数据长度
  unsigned short measrate;				//测量时间间隔，单位为ms，最少不能小于200ms（5Hz）
  unsigned short navrate;				//导航速率（周期），固定为1
  unsigned short timeref;				//参考时间：0=UTC Time；1=GPS Time；
  unsigned char  cka;					//校验CK_A
  unsigned char  ckb;					//校验CK_B
}_ublox_cfg_rate; 

unsigned char Ublox_Cfg_Rate(unsigned short measrate,unsigned char reftime);
void SendGPSPkg(USART_TypeDef* USARTx, uint8 *p);
void Ublox_CheckSum(unsigned char *buf,unsigned short len,unsigned char* cka,unsigned char*ckb);
void Ublox_Send_Date(unsigned char* dbuf,unsigned short len);
unsigned char Ublox_Cfg_Ack_Check(void);

//RMC(recommend minimum special GPS/transit data)：推荐最小定位信息
//VTG(track made good and ground speed):地面速度信息//GLL:地定位信息
//GGA(global positioning system fix data)：GPS定位信息
//GSA（GSP DOP and active satellite）:当前卫星信息
//GSV(GPS satellite in view)：可见卫星信息
//GLL

#endif