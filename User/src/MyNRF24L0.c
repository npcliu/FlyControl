#include "common.h"
#include "string.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_MY_spi.h"

#include "MyNRF24L0.h"

#include "delay.h"
//gpio控制CE和IRQ
//#define NRF_CE = 1       GPIO_SET(NRF_CE_PTXn,1)
//#define NRF_CE_LOW()        GPIO_SET(NRF_CE_PTXn,0)           //CE置低
//#define NRF_Read_IRQ()      GPIO_SET(NRF_IRQ_PTXn)

// 用户配置 发送和 接收地址，频道
uint8 TX_ADDRESS[5] = {0x34, 0x43, 0x10, 0x10, 0x01};   // 定义一个静态发送地址
uint8 RX_ADDRESS[5] = {0x34, 0x43, 0x10, 0x10, 0x01};

#define CHANAL          40                              //频道选择

// 内部配置参量
#define TX_ADR_WIDTH    ADR_WIDTH                       //发射地址宽度
#define TX_PLOAD_WIDTH  DATA_PACKET                     //发射数据通道有效数据宽度0~32Byte
#define RX_ADR_WIDTH    ADR_WIDTH                       //接收地址宽度
#define RX_PLOAD_WIDTH  DATA_PACKET                     //接收数据通道有效数据宽度0~32Byte

//内部寄存器操作函数声明
static  uint8   nrf_writereg(uint8 reg, uint8 dat);
static  uint8   nrf_readreg (uint8 reg, uint8 *dat);
static  uint8   nrf_writebuf(uint8 reg , uint8 *pBuf, uint32 len);
static  uint8   nrf_readbuf (uint8 reg, uint8 *pBuf, uint32  len);

/*!
*  @brief      NRF24L01+ 模式标记
*/
volatile uint8  nrf_mode = NOT_INIT;
//volatile uint8  nrf_rx_front = 0, nrf_rx_rear = 0;              //接收FIFO的指针
volatile uint8  nrf_rx_flag = QUEUE_EMPTY;
uint8 NRF_ISR_RX_FIFO[RX_FIFO_PACKET_NUM][DATA_PACKET];         //中断接收的FIFO

volatile uint8    *nrf_irq_tx_addr      = NULL;
volatile uint32    nrf_irq_tx_pnum      = 0;                    //pnum = (len+MAX_ONCE_TX_NUM -1)  / MAX_ONCE_TX_NUM
volatile uint8      nrf_irq_tx_flag  = 0;                     //0 表示成功 ，1 表示 发送失败

uint8 nrf_rciv[32] = {0};         //中断接收的FIFO
uint8 tx_flag = 0;
uint8 rx_flag = 0;

/*!
*  @brief      NRF24L01+初始化，默认进入接收模式
*  @return     初始化成功标记，0为初始化失败，1为初始化成功
*  @since      v5.0
*  Sample usage:
while(!nrf_init())                                     //初始化NRF24L01+ ,等待初始化成功为止
{
printf("\n  NRF与MCU连接失败，请重新检查接线。\n");
                        }
printf("\n      NRF与MCU连接成功！\n");
*/

uint8 nrf_init(nrf_mode_e _nrf_mode_e)
{
  gpio_init(NRF_CE_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 0);                               //初始化CE，默认进入待机模式
  gpio_init(NRF_IRQ_PIN, IPU, interupt,GPIO_Speed_50MHz,0);                              //初始化IRQ管脚为输入
  gpio_int_cfg(NRF_IRQ_PIN,NRF_IRQ_Line,EXTI_Trigger_Falling);
  //配置NRF寄存器
  NRF_CE = 0;
//      SPI3->DR = 0;
  nrf_writereg(NRF_WRITE_REG + SETUP_AW, ADR_WIDTH - 2);          //设置地址长度为 TX_ADR_WIDTH
  nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);                    //设置RF通道为CHANAL
//  nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x24);                   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
  nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x08);                   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);                      //使能通道0的自动应答
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);                  //使能通道0的接收地址
  //RX模式配置
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写RX节点地址
  nrf_writereg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);         //选择通道0的有效数据宽度
  nrf_writereg(FLUSH_RX, NOP);                                    //清除RX FIFO寄存器
  //TX模式配置
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //写TX节点地址
  nrf_writereg(NRF_WRITE_REG + SETUP_RETR, 0x1F);                 //设置自动重发间隔时间:250us + 86us;最大自动重发次数:15次
  nrf_writereg(FLUSH_TX, NOP);                                    //清除TX FIFO寄存器
  if(TX_MODE==_nrf_mode_e)
    nrf_tx_mode();                                                 //默认进入接收模式
  else if(RX_MODE==_nrf_mode_e)
    nrf_rx_mode();                                                 //默认进入接收模式
  NRF_CE = 1;
  
  return nrf_link_check();
}

/*!
*  @brief      NRF24L01+写寄存器
*  @param      reg         寄存器
*  @param      dat         需要写入的数据
*  @return     NRF24L01+ 状态
*  @since      v5.0
*  Sample usage:    nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);   //设置RF通道为CHANAL
*/
uint8 nrf_writereg(uint8 reg, uint8 dat)
{
  uint8 buff[2];
  
  buff[0] = reg;          //先发送寄存器
  buff[1] = dat;          //再发送数据
  
  NRF_CSN = 0;
  spi_mosi(NRF_SPI, buff, buff, 2); //发送buff里数据，并采集到 buff里
  NRF_CSN = 1;
  
  /*返回状态寄存器的值*/
  return buff[0];
}

/*!
*  @brief      NRF24L01+读寄存器
*  @param      reg         寄存器
*  @param      dat         需要读取的数据的存放地址
*  @return     NRF24L01+ 状态
*  @since      v5.0
*  Sample usage:
uint8 data;
nrf_readreg(STATUS,&data);
*/
uint8 nrf_readreg(uint8 reg, uint8 *dat)
{
  uint8 buff[2];
  
  buff[0] = reg;          //先发送寄存器
  NRF_CSN = 0;
  spi_mosi(NRF_SPI, buff, buff, 2); //发送buff数据，并保存到buff里
  NRF_CSN = 1;
  *dat = buff[1];                         //提取第二个数据
  /*返回状态寄存器的值*/
  return buff[0];
}

/*!
*  @brief      NRF24L01+写寄存器一串数据
*  @param      reg         寄存器
*  @param      pBuf        需要写入的数据缓冲区
*  @param      len         需要写入数据长度
*  @return     NRF24L01+ 状态
*  @since      v5.0
*  Sample usage:    nrf_writebuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址
*/
uint8 nrf_writebuf(uint8 reg , uint8 *pBuf, uint32 len)
{
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg , &reg, pBuf, NULL, 1 , len); //发送 reg ，pBuf 内容，不接收
  NRF_CSN = 1;
  return reg;    //返回NRF24L01的状态
}

/*!
*  @brief      NRF24L01+读寄存器一串数据
*  @param      reg         寄存器
*  @param      dat         需要读取的数据的存放地址
*  @param      len         需要读取的数据长度
*  @return     NRF24L01+ 状态
*  @since      v5.0
*  Sample usage:
uint8 data;
nrf_readreg(STATUS,&data);
*/
uint8 nrf_readbuf(uint8 reg, uint8 *pBuf, uint32 len)
{
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg , &reg, NULL, pBuf, 1 , len); //发送reg，接收到buff
  NRF_CSN = 1;
  
  return reg;    //返回NRF24L01的状态
}

/*!
*  @brief      检测NRF24L01+与MCU是否正常连接
*  @return     NRF24L01+ 的通信状态，0表示通信不正常，1表示正常
*  @since      v5.0
*  Sample usage:
while(nrf_link_check() == 0)
{
printf("\n通信失败");
                    }
*/
uint8 nrf_link_check(void)
{
#define NRF_CHECH_DATA  0xC2        //此值为校验数据时使用，可修改为其他值
  
  uint8 reg;
  
  uint8 buff[5] = {NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA};
  uint8 i;
  
  reg = NRF_WRITE_REG + TX_ADDR;
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg, NULL , buff, NULL, 1 , 5); //写入校验数据
  NRF_CSN = 1;
  
  DelayUs(100);
  reg = TX_ADDR;
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg, NULL , NULL, buff, 1 , 5); //读取校验数据
  NRF_CSN = 1;
  
  /*比较*/
  for(i = 0; i < 5; i++)
  {
    if(buff[i] != NRF_CHECH_DATA)
    {
      return 1 ;        //MCU与NRF不正常连接
    }
  }
  return 0 ;             //MCU与NRF成功连接
}

/*!
*  @brief      NRF24L01+进入接收模式
*  @since      v5.0
*/
void nrf_rx_mode(void)
{
  NRF_CE = 0;
  
  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);          //使能通道0的自动应答
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);      //使能通道0的接收地址
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写RX节点地址
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0B | (IS_CRC16 << 2));       //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
  /* 清除中断标志*/
  nrf_writereg(NRF_WRITE_REG + STATUS, 0xff);
  nrf_writereg(FLUSH_RX, NOP);                    //清除RX FIFO寄存器
  /*CE拉高，进入接收模式*/
  NRF_CE = 1;
  
  nrf_mode = RX_MODE;
}

/*!
*  @brief      NRF24L01+进入发送模式
*  @since      v5.0
*/
void nrf_tx_mode(void)
{
  volatile uint32 i;
  
  NRF_CE = 0;
  //DELAY_MS(1);
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //写TX节点地址
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //设置RX节点地址 ,主要为了使能ACK
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0A | (IS_CRC16 << 2)); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
  /*CE拉高，进入发送模式*/
  NRF_CE = 1;
  
  nrf_mode = TX_MODE;
  
  i = 0x0fff;
  while(i--);         //CE要拉高一段时间才进入发送模式
  
  //DELAY_MS(1);
  
  
}

/*!
*  @brief      NRF24L01+数据接收
*  @param      rxbuf       接收缓冲区地址
*  @param      len         最大接收长度
*  @return     实际接收的数据长度
*  Sample usage:
relen = nrf_rx(buff,DATA_PACKET);               //等待接收一个数据包，数据存储在buff里
if(relen != 0)
{
printf("\n接收到数据:%s",buff);             //打印接收到的数据 （这里接收到的是字符串才可以用printf哦！）
            }

*  @since      v5.0
*/
//uint32  nrf_rx(uint8 *rxbuf, uint32 len)
//{
//  uint32 tmplen = 0;
//  uint8 tmp;
//  
//  while( (nrf_rx_flag != QUEUE_EMPTY) && (len != 0) )
//  {
//    if(len < DATA_PACKET)
//    {
//      vcan_cpy(rxbuf, (uint8 *)&(NRF_ISR_RX_FIFO[nrf_rx_front]), len);
//      
//      NRF_CE_LOW();           //进入待机状态
//      nrf_rx_front++;                //由于非空，所以可以直接出队列
//      if(nrf_rx_front >= RX_FIFO_PACKET_NUM)
//      {
//        nrf_rx_front = 0;          //重头开始
//      }
//      tmp =  nrf_rx_rear;
//      if(nrf_rx_front == tmp)       //追到屁股了，接收队列空
//      {
//        nrf_rx_flag = QUEUE_EMPTY;
//      }
//      NRF_CE = 1;          //进入接收模式
//      
//      tmplen += len;
//      return tmplen;
//    }
//    
//    vcan_cpy(rxbuf, (uint8 *)&(NRF_ISR_RX_FIFO[nrf_rx_front]), DATA_PACKET);
//    rxbuf   += DATA_PACKET;
//    len     -= DATA_PACKET;
//    tmplen  += DATA_PACKET;
//    
//    NRF_CE_LOW();           //进入待机状态
//    
//    nrf_rx_front++;                //由于非空，所以可以直接出队列
//    
//    if(nrf_rx_front >= RX_FIFO_PACKET_NUM)
//    {
//      nrf_rx_front = 0;          //重头开始
//    }
//    tmp  = nrf_rx_rear;
//    if(nrf_rx_front == tmp)       //追到屁股了，接收队列空
//    {
//      nrf_rx_flag = QUEUE_EMPTY;
//    }
//    else
//    {
//      nrf_rx_flag = QUEUE_NORMAL;
//    }
//    NRF_CE = 1;          //进入接收模式
//  }
//  
//  return tmplen;
//}
//
/*!
*  @brief      NRF24L01+数据发送
*  @param      rxbuf       发送缓冲区地址
*  @param      len         发送长度
*  @return     发送结果，0表示发送失败，1为发送中。最终发送结果需要调用nrf_tx_state()判断。
*  Sample usage:
if(nrf_tx(buff,DATA_PACKET) == 1 );         //发送一个数据包：buff（包为32字节）
{
//等待发送过程中，此处可以加入处理任务

while(nrf_tx_state() == NRF_TXING);         //等待发送完成

if( NRF_TX_OK == nrf_tx_state () )
{
printf("\n发送成功:%d",i);
i++;                                    //发送成功则加1，可验证是否漏包
                        }
                        else
{
printf("\n发送失败:%d",i);
                        }
                    }
                    else
{
printf("\n发送失败:%d",i);
                    }

*  @since      v5.0
*/
uint8    nrf_tx(uint8 *txbuf, uint32 len)
{
  nrf_irq_tx_flag = 0;        //复位标志位
  
  if((txbuf == 0 ) || (len == 0))
  {
    return 0;
  }
  
  if(nrf_irq_tx_addr == 0 )
  {
    nrf_irq_tx_pnum = (len - 1) / DATA_PACKET;        // 进 1 求得 包 的数目
    nrf_irq_tx_addr = txbuf;
    if( nrf_mode != TX_MODE)
    {
      nrf_tx_mode();
    }
    //需要 先发送一次数据包后才能 中断发送
    NRF_CE = 0;    /*ce为低，进入待机模式1*/
    nrf_writebuf(WR_TX_PLOAD, txbuf, DATA_PACKET);    /*写数据到TX BUF 最大 32个字节*/
    NRF_CE = 1;    /*CE为高，txbuf非空，发送数据包 */
    
    return 1;
  }
  else
  {
    return 0;
  }
}


/*!
*  @brief      检查NRF24L01+发送状态
*  @return     发送结果，参考 nrf_tx_state_e 枚举的定义。
*  Sample usage:
if(nrf_tx(buff,DATA_PACKET) == 1 );         //发送一个数据包：buff（包为32字节）
{
//等待发送过程中，此处可以加入处理任务

while(nrf_tx_state() == NRF_TXING);         //等待发送完成

if( NRF_TX_OK == nrf_tx_state () )
{
printf("\n发送成功:%d",i);
i++;                                    //发送成功则加1，可验证是否漏包
                        }
                        else
{
printf("\n发送失败:%d",i);
                        }
                    }
                    else
{
printf("\n发送失败:%d",i);
                    }

*  @since      v5.0
*/
nrf_tx_state_e nrf_tx_state ()
{
  /*
  if(nrf_mode != TX_MODE)
  {
  return NRF_NOT_TX;
}
  */
  
  if((nrf_irq_tx_addr == 0) && (nrf_irq_tx_pnum == 0))
  {
    if(nrf_irq_tx_flag)    //发送完成
    {
      return NRF_TX_ERROR;
    }
    else
    {
      return NRF_TX_OK;
    }
  }
  else
  {
    return NRF_TXING;
  }
}

void nrf_handler(void)
{
  uint8 nrf_state;
  uint8 nrf_fifo_sta = 0;
  
  nrf_readreg(STATUS, &nrf_state);  /*读取status寄存器的值  */
  nrf_writereg(NRF_WRITE_REG + STATUS, nrf_state);  /* 清除中断标志*/
  
  if(nrf_state & RX_DR) //接收到数据
  {
    NRF_CE = 0;
    nrf_readbuf(RD_RX_PLOAD, (uint8*)nrf_rciv, RX_PLOAD_WIDTH); //读取数据
    nrf_readreg(FIFO_STATUS_ADDR, &nrf_fifo_sta);        //read fifo status of 'nrf24l01+';according to the test nrf24l01+ has a 3 byte fifo,that is to say after 3 times haven't read receive fifo,it will be full
    if(nrf_fifo_sta & RX_EMPTY)                        //empty is normal
      rx_flag = 1;
    else if(nrf_fifo_sta & RX_FULL)
    {
      rx_flag = 2;//LCDShowStr(0,LCD_W-16,"NRF RX FIFO is full!clearing...",1,YELLOW,BLACK);
      nrf_writereg(FLUSH_RX, NOP);                    //清除RX FIFO寄存器
    }
    else
      rx_flag = 3;//      LCDShowStr(0,LCD_W-16,"there are some unread data in NRF RX FIFO!please check!",1,YELLOW,BLACK); 
    NRF_CE = 1;
  }
  else if(nrf_state & TX_DS) //发送完数据
  {
    if(nrf_mode != RX_MODE)      //发送完成后 默认 进入 接收模式
      nrf_rx_mode();
    tx_flag = 1;
  }
  else if(nrf_state & MAX_RT)      //发送超时
  {
    tx_flag = 2;
    //    nrf_irq_tx_flag = 1;                            //标记发送失败
//    LCDShowStr(0,LCD_W-16,"NRF send timeout!clearing send buff...",1,YELLOW,BLACK);
//    nrf_writereg(FLUSH_TX, NOP);                    //清除TX FIFO寄存器
    //有可能是 对方也处于 发送状态
    //放弃本次发送
    //    nrf_irq_tx_addr = 0;
    //    nrf_irq_tx_pnum = 0;
    nrf_rx_mode();                                  //进入 接收状态
  }
  else if(nrf_state & TX_FULL) //TX FIFO 满
  {
    ;//printf("\nTX_FULL");
  }
}

//检测 接收FIFO 的数据  (0 没接收够 、1 为接收正确)
//uint8  nrf_rx_fifo_check(uint32 offset,uint16 * val)
//{
//  uint8 rx_num = (offset + 1 + DATA_PACKET - 1 ) / DATA_PACKET;   //加1 是因为返回2个字节，最后一个自己所在的包数。
//  //+ DATA_PACKET - 1 是四舍五入
//  uint8 tmp;
//  if(nrf_rx_flag == QUEUE_EMPTY)
//  {
//    return 0;
//  }
//  
//  if(rx_num > RX_FIFO_PACKET_NUM)                                 //偏移太大，超过 FIFO 限制
//  {
//    return 0;
//  }
//  
//  rx_num = nrf_rx_front + rx_num - 1;                             //目标查询的 包的位置
//  tmp =  nrf_rx_rear;
//  if(nrf_rx_front <  tmp)                                 //接收数据在 一圈之内
//  {
//    if(rx_num >= nrf_rx_rear )                                  //没接收足够的数据
//    {
//      return 0;
//    }
//    
//    //获取数据
//    
//  }
//  else                                                            //越过一圈
//  {
//    if(rx_num >= RX_FIFO_PACKET_NUM)                            //超过一圈
//    {
//      rx_num -= RX_FIFO_PACKET_NUM;
//      
//      if( rx_num >= nrf_rx_rear )                             //还没接收够
//      {
//        return 0;
//      }
//    }
//    //获取数据
//  }
//  
//  //获取数据
//  *val = *(uint16 *)((char *)&NRF_ISR_RX_FIFO + ( rx_num*DATA_PACKET + (offset % DATA_PACKET - 2) )) ;
//  return 1;
//  
//}


