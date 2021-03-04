/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_NRF24L0.h
 * @brief      NRF24L0驱动函数实现
 * @author     山外科技
 * @version    v5.0
 * @date       2014-01-04
 */

#ifndef _VCAN_NRF24L0_H_
#define _VCAN_NRF24L0_H_     1
//以下是硬件配置
#define NRF_SPI         SPI1
//#define NRF_CS          SPI_PCS0
#define NRF_CSN_PIN      PC4
#define NRF_CSN          PC4_OUT

#define NRF_CE_PIN      PB0
#define NRF_CE          PB0_OUT
   
#define NRF_IRQ_PIN     PC5
#define NRF_IRQ_LINE    EXTI_Line5
#define NRF_IRQn        EXTI9_5_IRQn


//以下是用户配置的选项
#define DATA_PACKET             32      //一次传输最大可支持的字节数（1~32）
#define ADR_WIDTH               5       //定义地址长度（3~5）
#define IS_CRC16                1       //1表示使用 CRC16，0表示 使用CRC8 (0~1)
#define CHANAL                  40                              //频道选择

// 内部配置参量
#define TX_ADR_WIDTH    ADR_WIDTH                       //发射地址宽度
#define TX_PLOAD_WIDTH  DATA_PACKET                     //发射数据通道有效数据宽度0~32Byte

#define RX_ADR_WIDTH    ADR_WIDTH                       //接收地址宽度
#define RX_PLOAD_WIDTH  DATA_PACKET                     //接收数据通道有效数据宽度0~32Byte
/******************************** NRF24L01+ 寄存器命令 宏定义***************************************/
// SPI(nRF24L01) commands , NRF的SPI命令宏定义，详见NRF功能使用文档
#define NRF_READ_REG    0x00    // Define read command to register
#define NRF_WRITE_REG   0x20    // Define write command to register
#define RD_RX_PLOAD     0x61    // Define RX payload register address
#define WR_TX_PLOAD     0xA0    // Define TX payload register address
#define FLUSH_TX        0xE1    // Define flush TX register command
#define FLUSH_RX        0xE2    // Define flush RX register command
#define REUSE_TX_PL     0xE3    // Define reuse TX payload register command
#define NOP             0xFF    // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses) ，NRF24L01 相关寄存器地址的宏定义
#define CONFIG      0x00        // 'Config' register address
#define EN_AA       0x01        // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02        // 'Enabled RX addresses' register address
#define SETUP_AW    0x03        // 'Setup address width' register address
#define SETUP_RETR  0x04        // 'Setup Auto. Retrans' register address
#define RF_CH       0x05        // 'RF channel' register address
#define RF_SETUP    0x06        // 'RF setup' register address
#define STATUS      0x07        // 'Status' register address
#define OBSERVE_TX  0x08        // 'Observe TX' register address
#define CD          0x09        // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A        // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B        // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C        // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D        // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E        // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F        // 'RX address pipe5' register address
#define TX_ADDR     0x10        // 'TX address' register address
#define RX_PW_P0    0x11        // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12        // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13        // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14        // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15        // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16        // 'RX payload width, pipe5' register address
#define FIFO_STATUS_ADDR      0x17        // 'FIFO Status Register' register address

//几个重要的状态标记
#define TX_FULL     0x01        //TX FIFO 寄存器满标志。 1 为 满，0为 不满
#define RX_EMPTY    0x01        //recieve fifo of nrf24l01+ is empty    
#define RX_FULL     0x02        //recieve fifo of nrf24l01+ is full
#define MAX_RT      0x10        //达到最大重发次数中断标志位
#define TX_DS       0x20        //发送完成中断标志位
#define RX_DR       0x40        //接收到数据中断标志位

//#define SD_SUCESS     1         //send successfully
//#define SD_TIME_OUT     2       //send time out
//配置到这里结束


/**
 *  @brief NRF模块发送状态
 */
typedef enum
{
    NRF_TXING,              //发送中
    NRF_TX_ERROR,           //发送错误
    NRF_TX_TIME_OUT,           //发送time out
    NRF_TX_OK,              //发送完成
} nrf_tx_state_e;

//NRF24L01+状态
typedef enum
{
  NOT_INIT = 0,
  TX_MODE,
  RX_MODE,
} nrf_mode_e;

typedef enum
{
  QUEUE_EMPTY = 0,        //队列空模式，只可入队列
  QUEUE_NORMAL,           //正常模式，可正常出入队列，即队列不空不满
  QUEUE_FULL,             //队列满模式，满了则不再添加，丢弃掉数据
} nrf_rx_queueflag_e; //中断接收时，队列状态标记位

//extern uint8 tx_flag;                              //发送结果
//extern uint8 rx_flag;                              //发送结果
extern uint8 nrf_rciv[DATA_PACKET];         //中断接收的FIFO
extern uint8 irq_tx_buff[DATA_PACKET];
extern volatile nrf_tx_state_e nrf_tx_sta;                     //

//函数声明
extern  uint8   nrf_init(nrf_mode_e _nrf_mode_e);                     //初始化NRF24L01+
extern  uint8   nrf_link_check(void);               //检测NRF24L01+与单片机是否通信正常
extern  uint32  nrf_rx(uint8 *rxbuf, uint32 len);   //接收数据
extern  uint8   nrf_tx(uint8 *txbuf, uint32 len);   //发送数据

extern  nrf_tx_state_e nrf_tx_state();             //检查发送状态(发送数据后查询是否发送成功)
extern  void nrf_handler(void);                  //NRF24L01+ 的 中断服务函数
void SPI1_Init(void);

//下面的函数 留给 无线消息处理机制 的函数使用，一般用户用不着
uint8  nrf_rx_fifo_check(uint32 offset,uint16 * val);    //获取 接收FIFO 的数据
void    nrf_rx_mode(void);           //进入接收模式
void    nrf_tx_mode(void);           //进入发送模式

#endif      //_VCAN_NRF24L0_H_
