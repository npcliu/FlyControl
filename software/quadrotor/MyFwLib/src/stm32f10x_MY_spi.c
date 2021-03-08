#include "stdio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_MY_spi.h"

/*!
*  @brief      SPI���ͽ��պ���
*  @param      SPIn_e          SPIģ��(SPI0��SPI1)
*  @param      SPIn_PCSn_e     Ƭѡ�ܽű��
*  @param      modata          ���͵����ݻ�������ַ(����Ҫ������ NULL)
*  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
*  @since      v5.0
*  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,buff,buff,2);    //����buff�����ݣ������յ�buff�����Ϊ2�ֽ�
*/
void spi_mosi(SPI_TypeDef*  SPIx, uint8 *modata, uint8 *midata, uint32 len)
{
  uint32 i = 0;
  
  /***************** ����len������ *******************/
  for(i = 0; i<len; i++)
  {
      while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //�ȴ����ͻ���������

      SPIx->DR = modata[i];
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);  //�����ջ������ǿ�

    if(midata != NULL)
      midata[i] = SPIx->DR;            //������յ�������
    else
      SPIx->DR;
  }
}
/*!
*  @brief      SPI���ͽ��պ���
*  @param      SPIn_e          SPIģ��(SPI0��SPI1)
*  @param      SPIn_PCSn_e     Ƭѡ�ܽű��
*  @param      mocmd           ���͵����������ַ(����Ҫ������ NULL)
*  @param      micmd           ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
*  @param      modata          ���͵����ݻ�������ַ(����Ҫ������ NULL)
*  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
*  @since      v5.0
*  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,cmd,NULL,buff,buff,1,2);    //����cmd/buff�����ݣ�������cmd����ʱ�����ݣ�����buff����ʱ�����ݵ�buff����ȷֱ�Ϊ1��2�ֽ�
*/
void spi_mosi_cmd(SPI_TypeDef*  SPIx, uint8 *mocmd, uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
//  SPI_MemMapPtr spi_ptr = SPIN[spin];
  uint32 i = 0;
  
//  while((~SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);//�ȴ����ͻ���������
//  while((SPI_S_REG(spi_ptr) & SPI_S_SPRF_MASK))    //data avalable if SPRF=1
//    SPI_DL_REG(spi_ptr);                     //ֱ�Ӷ�ȡ��������ս��ջ�����
  for(i = 0; i < cmdlen; i++)  /***************** ����len��address *******************/
  {
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //�ȴ����ͻ���������
    SPIx->DR = mocmd[i];
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);  //�����ջ������ǿ�
    if(micmd != NULL)
      micmd[i] = SPIx->DR;            //������յ�������
    else
      SPIx->DR;
  }
  for(i = 0; i<len; i++)  /***************** ����len��data *******************/
  {
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //�ȴ����ͻ���������
    SPIx->DR = modata[i]; 
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);  //�����ջ������ǿ�
    if(midata != NULL)
      midata[i] = SPIx->DR;                //������յ�������
    else
      SPIx->DR;
  }
}
void SPI1_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
  /* NSS---->GPIO(LED) */
  SPI_SSOutputCmd(SPI1, ENABLE);
  /*PA5:SPI1_SCK,PA6:SPI1_MISO;PA7:SPI1_MOSI*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  /* Enable SPI1 */
  SPI_Cmd(SPI1, ENABLE);
}

void SPI3_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE);         //must enable clock before disable JTAG!!

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*ʹ��SWD ����JTAG*/
  /* NSS---->GPIO(LED) */
  SPI_SSOutputCmd(SPI3, ENABLE);
  /*PA5:SPI1_SCK,PA6:SPI1_MISO;PA7:SPI1_MOSI*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//;SPI_CPOL_Low
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//;SPI_CPHA_1Edge
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI3, &SPI_InitStructure);
  /* Enable SPI1 */
  SPI_Cmd(SPI3, ENABLE);
}

/*===========================================================================
* ���� ��SPI_ExchangeByte() => ͨ��SPI�������ݽ���                          *
* ���� ����Ҫд��SPI��ֵ                                                    *
* ��� ��ͨ��SPI������ֵ                                                    *
============================================================================*/
uint8 SPI_ExchangeByte(uint8 input)
{
  SPI_I2S_SendData(SPI1, input);
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return (SPI_I2S_ReceiveData(SPI1));
}



