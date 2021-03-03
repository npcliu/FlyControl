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
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //�ȴ����ͻ���������

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
  NRF_CSN = 0;
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
  
  NRF_CSN = 1;
}



