#include "common.h"
#include "string.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_MY_spi.h"

#include "MyNRF24L0.h"

#include "delay.h"
//gpio����CE��IRQ
//#define NRF_CE = 1       GPIO_SET(NRF_CE_PTXn,1)
//#define NRF_CE_LOW()        GPIO_SET(NRF_CE_PTXn,0)           //CE�õ�
//#define NRF_Read_IRQ()      GPIO_SET(NRF_IRQ_PTXn)

// �û����� ���ͺ� ���յ�ַ��Ƶ��
uint8 TX_ADDRESS[5] = {0x34, 0x43, 0x10, 0x10, 0x01};   // ����һ����̬���͵�ַ
uint8 RX_ADDRESS[5] = {0x34, 0x43, 0x10, 0x10, 0x01};

#define CHANAL          40                              //Ƶ��ѡ��

// �ڲ����ò���
#define TX_ADR_WIDTH    ADR_WIDTH                       //�����ַ���
#define TX_PLOAD_WIDTH  DATA_PACKET                     //��������ͨ����Ч���ݿ��0~32Byte
#define RX_ADR_WIDTH    ADR_WIDTH                       //���յ�ַ���
#define RX_PLOAD_WIDTH  DATA_PACKET                     //��������ͨ����Ч���ݿ��0~32Byte

//�ڲ��Ĵ���������������
static  uint8   nrf_writereg(uint8 reg, uint8 dat);
static  uint8   nrf_readreg (uint8 reg, uint8 *dat);
static  uint8   nrf_writebuf(uint8 reg , uint8 *pBuf, uint32 len);
static  uint8   nrf_readbuf (uint8 reg, uint8 *pBuf, uint32  len);

/*!
*  @brief      NRF24L01+ ģʽ���
*/
volatile uint8  nrf_mode = NOT_INIT;
//volatile uint8  nrf_rx_front = 0, nrf_rx_rear = 0;              //����FIFO��ָ��
volatile uint8  nrf_rx_flag = QUEUE_EMPTY;
uint8 NRF_ISR_RX_FIFO[RX_FIFO_PACKET_NUM][DATA_PACKET];         //�жϽ��յ�FIFO

volatile uint8    *nrf_irq_tx_addr      = NULL;
volatile uint32    nrf_irq_tx_pnum      = 0;                    //pnum = (len+MAX_ONCE_TX_NUM -1)  / MAX_ONCE_TX_NUM
volatile uint8      nrf_irq_tx_flag  = 0;                     //0 ��ʾ�ɹ� ��1 ��ʾ ����ʧ��

uint8 nrf_rciv[32] = {0};         //�жϽ��յ�FIFO
uint8 tx_flag = 0;
uint8 rx_flag = 0;

/*!
*  @brief      NRF24L01+��ʼ����Ĭ�Ͻ������ģʽ
*  @return     ��ʼ���ɹ���ǣ�0Ϊ��ʼ��ʧ�ܣ�1Ϊ��ʼ���ɹ�
*  @since      v5.0
*  Sample usage:
while(!nrf_init())                                     //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ
{
printf("\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\n");
                        }
printf("\n      NRF��MCU���ӳɹ���\n");
*/

uint8 nrf_init(nrf_mode_e _nrf_mode_e)
{
  gpio_init(NRF_CE_PIN, Out_PP,n_interupt,GPIO_Speed_50MHz, 0);                               //��ʼ��CE��Ĭ�Ͻ������ģʽ
  gpio_init(NRF_IRQ_PIN, IPU, interupt,GPIO_Speed_50MHz,0);                              //��ʼ��IRQ�ܽ�Ϊ����
  gpio_int_cfg(NRF_IRQ_PIN,NRF_IRQ_Line,EXTI_Trigger_Falling);
  //����NRF�Ĵ���
  NRF_CE = 0;
//      SPI3->DR = 0;
  nrf_writereg(NRF_WRITE_REG + SETUP_AW, ADR_WIDTH - 2);          //���õ�ַ����Ϊ TX_ADR_WIDTH
  nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);                    //����RFͨ��ΪCHANAL
//  nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x24);                   //����TX�������,0db����,2Mbps,���������濪��
  nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x08);                   //����TX�������,0db����,2Mbps,���������濪��  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);                      //ʹ��ͨ��0���Զ�Ӧ��
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);                  //ʹ��ͨ��0�Ľ��յ�ַ
  //RXģʽ����
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //дRX�ڵ��ַ
  nrf_writereg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);         //ѡ��ͨ��0����Ч���ݿ��
  nrf_writereg(FLUSH_RX, NOP);                                    //���RX FIFO�Ĵ���
  //TXģʽ����
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //дTX�ڵ��ַ
  nrf_writereg(NRF_WRITE_REG + SETUP_RETR, 0x1F);                 //�����Զ��ط����ʱ��:250us + 86us;����Զ��ط�����:15��
  nrf_writereg(FLUSH_TX, NOP);                                    //���TX FIFO�Ĵ���
  if(TX_MODE==_nrf_mode_e)
    nrf_tx_mode();                                                 //Ĭ�Ͻ������ģʽ
  else if(RX_MODE==_nrf_mode_e)
    nrf_rx_mode();                                                 //Ĭ�Ͻ������ģʽ
  NRF_CE = 1;
  
  return nrf_link_check();
}

/*!
*  @brief      NRF24L01+д�Ĵ���
*  @param      reg         �Ĵ���
*  @param      dat         ��Ҫд�������
*  @return     NRF24L01+ ״̬
*  @since      v5.0
*  Sample usage:    nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);   //����RFͨ��ΪCHANAL
*/
uint8 nrf_writereg(uint8 reg, uint8 dat)
{
  uint8 buff[2];
  
  buff[0] = reg;          //�ȷ��ͼĴ���
  buff[1] = dat;          //�ٷ�������
  
  NRF_CSN = 0;
  spi_mosi(NRF_SPI, buff, buff, 2); //����buff�����ݣ����ɼ��� buff��
  NRF_CSN = 1;
  
  /*����״̬�Ĵ�����ֵ*/
  return buff[0];
}

/*!
*  @brief      NRF24L01+���Ĵ���
*  @param      reg         �Ĵ���
*  @param      dat         ��Ҫ��ȡ�����ݵĴ�ŵ�ַ
*  @return     NRF24L01+ ״̬
*  @since      v5.0
*  Sample usage:
uint8 data;
nrf_readreg(STATUS,&data);
*/
uint8 nrf_readreg(uint8 reg, uint8 *dat)
{
  uint8 buff[2];
  
  buff[0] = reg;          //�ȷ��ͼĴ���
  NRF_CSN = 0;
  spi_mosi(NRF_SPI, buff, buff, 2); //����buff���ݣ������浽buff��
  NRF_CSN = 1;
  *dat = buff[1];                         //��ȡ�ڶ�������
  /*����״̬�Ĵ�����ֵ*/
  return buff[0];
}

/*!
*  @brief      NRF24L01+д�Ĵ���һ������
*  @param      reg         �Ĵ���
*  @param      pBuf        ��Ҫд������ݻ�����
*  @param      len         ��Ҫд�����ݳ���
*  @return     NRF24L01+ ״̬
*  @since      v5.0
*  Sample usage:    nrf_writebuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ
*/
uint8 nrf_writebuf(uint8 reg , uint8 *pBuf, uint32 len)
{
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg , &reg, pBuf, NULL, 1 , len); //���� reg ��pBuf ���ݣ�������
  NRF_CSN = 1;
  return reg;    //����NRF24L01��״̬
}

/*!
*  @brief      NRF24L01+���Ĵ���һ������
*  @param      reg         �Ĵ���
*  @param      dat         ��Ҫ��ȡ�����ݵĴ�ŵ�ַ
*  @param      len         ��Ҫ��ȡ�����ݳ���
*  @return     NRF24L01+ ״̬
*  @since      v5.0
*  Sample usage:
uint8 data;
nrf_readreg(STATUS,&data);
*/
uint8 nrf_readbuf(uint8 reg, uint8 *pBuf, uint32 len)
{
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg , &reg, NULL, pBuf, 1 , len); //����reg�����յ�buff
  NRF_CSN = 1;
  
  return reg;    //����NRF24L01��״̬
}

/*!
*  @brief      ���NRF24L01+��MCU�Ƿ���������
*  @return     NRF24L01+ ��ͨ��״̬��0��ʾͨ�Ų�������1��ʾ����
*  @since      v5.0
*  Sample usage:
while(nrf_link_check() == 0)
{
printf("\nͨ��ʧ��");
                    }
*/
uint8 nrf_link_check(void)
{
#define NRF_CHECH_DATA  0xC2        //��ֵΪУ������ʱʹ�ã����޸�Ϊ����ֵ
  
  uint8 reg;
  
  uint8 buff[5] = {NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA};
  uint8 i;
  
  reg = NRF_WRITE_REG + TX_ADDR;
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg, NULL , buff, NULL, 1 , 5); //д��У������
  NRF_CSN = 1;
  
  DelayUs(100);
  reg = TX_ADDR;
  NRF_CSN = 0;
  spi_mosi_cmd(NRF_SPI, &reg, NULL , NULL, buff, 1 , 5); //��ȡУ������
  NRF_CSN = 1;
  
  /*�Ƚ�*/
  for(i = 0; i < 5; i++)
  {
    if(buff[i] != NRF_CHECH_DATA)
    {
      return 1 ;        //MCU��NRF����������
    }
  }
  return 0 ;             //MCU��NRF�ɹ�����
}

/*!
*  @brief      NRF24L01+�������ģʽ
*  @since      v5.0
*/
void nrf_rx_mode(void)
{
  NRF_CE = 0;
  
  nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);          //ʹ��ͨ��0���Զ�Ӧ��
  nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);      //ʹ��ͨ��0�Ľ��յ�ַ
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //дRX�ڵ��ַ
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0B | (IS_CRC16 << 2));       //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
  /* ����жϱ�־*/
  nrf_writereg(NRF_WRITE_REG + STATUS, 0xff);
  nrf_writereg(FLUSH_RX, NOP);                    //���RX FIFO�Ĵ���
  /*CE���ߣ��������ģʽ*/
  NRF_CE = 1;
  
  nrf_mode = RX_MODE;
}

/*!
*  @brief      NRF24L01+���뷢��ģʽ
*  @since      v5.0
*/
void nrf_tx_mode(void)
{
  volatile uint32 i;
  
  NRF_CE = 0;
  //DELAY_MS(1);
  nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //дTX�ڵ��ַ
  nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //����RX�ڵ��ַ ,��ҪΪ��ʹ��ACK
  nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0A | (IS_CRC16 << 2)); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
  /*CE���ߣ����뷢��ģʽ*/
  NRF_CE = 1;
  
  nrf_mode = TX_MODE;
  
  i = 0x0fff;
  while(i--);         //CEҪ����һ��ʱ��Ž��뷢��ģʽ
  
  //DELAY_MS(1);
  
  
}

/*!
*  @brief      NRF24L01+���ݽ���
*  @param      rxbuf       ���ջ�������ַ
*  @param      len         �����ճ���
*  @return     ʵ�ʽ��յ����ݳ���
*  Sample usage:
relen = nrf_rx(buff,DATA_PACKET);               //�ȴ�����һ�����ݰ������ݴ洢��buff��
if(relen != 0)
{
printf("\n���յ�����:%s",buff);             //��ӡ���յ������� ��������յ������ַ����ſ�����printfŶ����
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
//      NRF_CE_LOW();           //�������״̬
//      nrf_rx_front++;                //���ڷǿգ����Կ���ֱ�ӳ�����
//      if(nrf_rx_front >= RX_FIFO_PACKET_NUM)
//      {
//        nrf_rx_front = 0;          //��ͷ��ʼ
//      }
//      tmp =  nrf_rx_rear;
//      if(nrf_rx_front == tmp)       //׷��ƨ���ˣ����ն��п�
//      {
//        nrf_rx_flag = QUEUE_EMPTY;
//      }
//      NRF_CE = 1;          //�������ģʽ
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
//    NRF_CE_LOW();           //�������״̬
//    
//    nrf_rx_front++;                //���ڷǿգ����Կ���ֱ�ӳ�����
//    
//    if(nrf_rx_front >= RX_FIFO_PACKET_NUM)
//    {
//      nrf_rx_front = 0;          //��ͷ��ʼ
//    }
//    tmp  = nrf_rx_rear;
//    if(nrf_rx_front == tmp)       //׷��ƨ���ˣ����ն��п�
//    {
//      nrf_rx_flag = QUEUE_EMPTY;
//    }
//    else
//    {
//      nrf_rx_flag = QUEUE_NORMAL;
//    }
//    NRF_CE = 1;          //�������ģʽ
//  }
//  
//  return tmplen;
//}
//
/*!
*  @brief      NRF24L01+���ݷ���
*  @param      rxbuf       ���ͻ�������ַ
*  @param      len         ���ͳ���
*  @return     ���ͽ����0��ʾ����ʧ�ܣ�1Ϊ�����С����շ��ͽ����Ҫ����nrf_tx_state()�жϡ�
*  Sample usage:
if(nrf_tx(buff,DATA_PACKET) == 1 );         //����һ�����ݰ���buff����Ϊ32�ֽڣ�
{
//�ȴ����͹����У��˴����Լ��봦������

while(nrf_tx_state() == NRF_TXING);         //�ȴ��������

if( NRF_TX_OK == nrf_tx_state () )
{
printf("\n���ͳɹ�:%d",i);
i++;                                    //���ͳɹ����1������֤�Ƿ�©��
                        }
                        else
{
printf("\n����ʧ��:%d",i);
                        }
                    }
                    else
{
printf("\n����ʧ��:%d",i);
                    }

*  @since      v5.0
*/
uint8    nrf_tx(uint8 *txbuf, uint32 len)
{
  nrf_irq_tx_flag = 0;        //��λ��־λ
  
  if((txbuf == 0 ) || (len == 0))
  {
    return 0;
  }
  
  if(nrf_irq_tx_addr == 0 )
  {
    nrf_irq_tx_pnum = (len - 1) / DATA_PACKET;        // �� 1 ��� �� ����Ŀ
    nrf_irq_tx_addr = txbuf;
    if( nrf_mode != TX_MODE)
    {
      nrf_tx_mode();
    }
    //��Ҫ �ȷ���һ�����ݰ������ �жϷ���
    NRF_CE = 0;    /*ceΪ�ͣ��������ģʽ1*/
    nrf_writebuf(WR_TX_PLOAD, txbuf, DATA_PACKET);    /*д���ݵ�TX BUF ��� 32���ֽ�*/
    NRF_CE = 1;    /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */
    
    return 1;
  }
  else
  {
    return 0;
  }
}


/*!
*  @brief      ���NRF24L01+����״̬
*  @return     ���ͽ�����ο� nrf_tx_state_e ö�ٵĶ��塣
*  Sample usage:
if(nrf_tx(buff,DATA_PACKET) == 1 );         //����һ�����ݰ���buff����Ϊ32�ֽڣ�
{
//�ȴ����͹����У��˴����Լ��봦������

while(nrf_tx_state() == NRF_TXING);         //�ȴ��������

if( NRF_TX_OK == nrf_tx_state () )
{
printf("\n���ͳɹ�:%d",i);
i++;                                    //���ͳɹ����1������֤�Ƿ�©��
                        }
                        else
{
printf("\n����ʧ��:%d",i);
                        }
                    }
                    else
{
printf("\n����ʧ��:%d",i);
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
    if(nrf_irq_tx_flag)    //�������
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
  
  nrf_readreg(STATUS, &nrf_state);  /*��ȡstatus�Ĵ�����ֵ  */
  nrf_writereg(NRF_WRITE_REG + STATUS, nrf_state);  /* ����жϱ�־*/
  
  if(nrf_state & RX_DR) //���յ�����
  {
    NRF_CE = 0;
    nrf_readbuf(RD_RX_PLOAD, (uint8*)nrf_rciv, RX_PLOAD_WIDTH); //��ȡ����
    nrf_readreg(FIFO_STATUS_ADDR, &nrf_fifo_sta);        //read fifo status of 'nrf24l01+';according to the test nrf24l01+ has a 3 byte fifo,that is to say after 3 times haven't read receive fifo,it will be full
    if(nrf_fifo_sta & RX_EMPTY)                        //empty is normal
      rx_flag = 1;
    else if(nrf_fifo_sta & RX_FULL)
    {
      rx_flag = 2;//LCDShowStr(0,LCD_W-16,"NRF RX FIFO is full!clearing...",1,YELLOW,BLACK);
      nrf_writereg(FLUSH_RX, NOP);                    //���RX FIFO�Ĵ���
    }
    else
      rx_flag = 3;//      LCDShowStr(0,LCD_W-16,"there are some unread data in NRF RX FIFO!please check!",1,YELLOW,BLACK); 
    NRF_CE = 1;
  }
  else if(nrf_state & TX_DS) //����������
  {
    if(nrf_mode != RX_MODE)      //������ɺ� Ĭ�� ���� ����ģʽ
      nrf_rx_mode();
    tx_flag = 1;
  }
  else if(nrf_state & MAX_RT)      //���ͳ�ʱ
  {
    tx_flag = 2;
    //    nrf_irq_tx_flag = 1;                            //��Ƿ���ʧ��
//    LCDShowStr(0,LCD_W-16,"NRF send timeout!clearing send buff...",1,YELLOW,BLACK);
//    nrf_writereg(FLUSH_TX, NOP);                    //���TX FIFO�Ĵ���
    //�п����� �Է�Ҳ���� ����״̬
    //�������η���
    //    nrf_irq_tx_addr = 0;
    //    nrf_irq_tx_pnum = 0;
    nrf_rx_mode();                                  //���� ����״̬
  }
  else if(nrf_state & TX_FULL) //TX FIFO ��
  {
    ;//printf("\nTX_FULL");
  }
}

//��� ����FIFO ������  (0 û���չ� ��1 Ϊ������ȷ)
//uint8  nrf_rx_fifo_check(uint32 offset,uint16 * val)
//{
//  uint8 rx_num = (offset + 1 + DATA_PACKET - 1 ) / DATA_PACKET;   //��1 ����Ϊ����2���ֽڣ����һ���Լ����ڵİ�����
//  //+ DATA_PACKET - 1 ����������
//  uint8 tmp;
//  if(nrf_rx_flag == QUEUE_EMPTY)
//  {
//    return 0;
//  }
//  
//  if(rx_num > RX_FIFO_PACKET_NUM)                                 //ƫ��̫�󣬳��� FIFO ����
//  {
//    return 0;
//  }
//  
//  rx_num = nrf_rx_front + rx_num - 1;                             //Ŀ���ѯ�� ����λ��
//  tmp =  nrf_rx_rear;
//  if(nrf_rx_front <  tmp)                                 //���������� һȦ֮��
//  {
//    if(rx_num >= nrf_rx_rear )                                  //û�����㹻������
//    {
//      return 0;
//    }
//    
//    //��ȡ����
//    
//  }
//  else                                                            //Խ��һȦ
//  {
//    if(rx_num >= RX_FIFO_PACKET_NUM)                            //����һȦ
//    {
//      rx_num -= RX_FIFO_PACKET_NUM;
//      
//      if( rx_num >= nrf_rx_rear )                             //��û���չ�
//      {
//        return 0;
//      }
//    }
//    //��ȡ����
//  }
//  
//  //��ȡ����
//  *val = *(uint16 *)((char *)&NRF_ISR_RX_FIFO + ( rx_num*DATA_PACKET + (offset % DATA_PACKET - 2) )) ;
//  return 1;
//  
//}


