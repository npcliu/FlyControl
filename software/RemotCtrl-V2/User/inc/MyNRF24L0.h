/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_NRF24L0.h
 * @brief      NRF24L0��������ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2014-01-04
 */

#ifndef _VCAN_NRF24L0_H_
#define _VCAN_NRF24L0_H_     1
//������Ӳ������
#define NRF_SPI         SPI1
//#define NRF_CS          SPI_PCS0
#define NRF_CSN_PIN      PC4
#define NRF_CSN          PC4_OUT

#define NRF_CE_PIN      PB0
#define NRF_CE          PB0_OUT
   
#define NRF_IRQ_PIN     PC5
#define NRF_IRQ_LINE    EXTI_Line5
#define NRF_IRQn        EXTI9_5_IRQn


//�������û����õ�ѡ��
#define DATA_PACKET             32      //һ�δ�������֧�ֵ��ֽ�����1~32��
#define ADR_WIDTH               5       //�����ַ���ȣ�3~5��
#define IS_CRC16                1       //1��ʾʹ�� CRC16��0��ʾ ʹ��CRC8 (0~1)
#define CHANAL                  40                              //Ƶ��ѡ��

// �ڲ����ò���
#define TX_ADR_WIDTH    ADR_WIDTH                       //�����ַ���
#define TX_PLOAD_WIDTH  DATA_PACKET                     //��������ͨ����Ч���ݿ��0~32Byte

#define RX_ADR_WIDTH    ADR_WIDTH                       //���յ�ַ���
#define RX_PLOAD_WIDTH  DATA_PACKET                     //��������ͨ����Ч���ݿ��0~32Byte
/******************************** NRF24L01+ �Ĵ������� �궨��***************************************/
// SPI(nRF24L01) commands , NRF��SPI����궨�壬���NRF����ʹ���ĵ�
#define NRF_READ_REG    0x00    // Define read command to register
#define NRF_WRITE_REG   0x20    // Define write command to register
#define RD_RX_PLOAD     0x61    // Define RX payload register address
#define WR_TX_PLOAD     0xA0    // Define TX payload register address
#define FLUSH_TX        0xE1    // Define flush TX register command
#define FLUSH_RX        0xE2    // Define flush RX register command
#define REUSE_TX_PL     0xE3    // Define reuse TX payload register command
#define NOP             0xFF    // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses) ��NRF24L01 ��ؼĴ�����ַ�ĺ궨��
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

//������Ҫ��״̬���
#define TX_FULL     0x01        //TX FIFO �Ĵ�������־�� 1 Ϊ ����0Ϊ ����
#define RX_EMPTY    0x01        //recieve fifo of nrf24l01+ is empty    
#define RX_FULL     0x02        //recieve fifo of nrf24l01+ is full
#define MAX_RT      0x10        //�ﵽ����ط������жϱ�־λ
#define TX_DS       0x20        //��������жϱ�־λ
#define RX_DR       0x40        //���յ������жϱ�־λ

//#define SD_SUCESS     1         //send successfully
//#define SD_TIME_OUT     2       //send time out
//���õ��������


/**
 *  @brief NRFģ�鷢��״̬
 */
typedef enum
{
    NRF_TXING,              //������
    NRF_TX_ERROR,           //���ʹ���
    NRF_TX_TIME_OUT,           //����time out
    NRF_TX_OK,              //�������
} nrf_tx_state_e;

//NRF24L01+״̬
typedef enum
{
  NOT_INIT = 0,
  TX_MODE,
  RX_MODE,
} nrf_mode_e;

typedef enum
{
  QUEUE_EMPTY = 0,        //���п�ģʽ��ֻ�������
  QUEUE_NORMAL,           //����ģʽ��������������У������в��ղ���
  QUEUE_FULL,             //������ģʽ������������ӣ�����������
} nrf_rx_queueflag_e; //�жϽ���ʱ������״̬���λ

//extern uint8 tx_flag;                              //���ͽ��
//extern uint8 rx_flag;                              //���ͽ��
extern uint8 nrf_rciv[DATA_PACKET];         //�жϽ��յ�FIFO
extern uint8 irq_tx_buff[DATA_PACKET];
extern volatile nrf_tx_state_e nrf_tx_sta;                     //

//��������
extern  uint8   nrf_init(nrf_mode_e _nrf_mode_e);                     //��ʼ��NRF24L01+
extern  uint8   nrf_link_check(void);               //���NRF24L01+�뵥Ƭ���Ƿ�ͨ������
extern  uint32  nrf_rx(uint8 *rxbuf, uint32 len);   //��������
extern  uint8   nrf_tx(uint8 *txbuf, uint32 len);   //��������

extern  nrf_tx_state_e nrf_tx_state();             //��鷢��״̬(�������ݺ��ѯ�Ƿ��ͳɹ�)
extern  void nrf_handler(void);                  //NRF24L01+ �� �жϷ�����
void SPI1_Init(void);

//����ĺ��� ���� ������Ϣ������� �ĺ���ʹ�ã�һ���û��ò���
uint8  nrf_rx_fifo_check(uint32 offset,uint16 * val);    //��ȡ ����FIFO ������
void    nrf_rx_mode(void);           //�������ģʽ
void    nrf_tx_mode(void);           //���뷢��ģʽ

#endif      //_VCAN_NRF24L0_H_
