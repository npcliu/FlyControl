#include "stdio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_MY_spi.h"

/*!
*  @brief      SPI发送接收函数
*  @param      SPIn_e          SPI模块(SPI0、SPI1)
*  @param      SPIn_PCSn_e     片选管脚编号
*  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
*  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
*  @since      v5.0
*  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,buff,buff,2);    //发送buff的内容，并接收到buff里，长度为2字节
*/
void spi_mosi(SPI_TypeDef*  SPIx, uint8 *modata, uint8 *midata, uint32 len)
{
  uint32 i = 0;
  
  /***************** 发送len个数据 *******************/
  for(i = 0; i<len; i++)
  {
      while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //等待发送缓冲区空闲

      SPIx->DR = modata[i];
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);  //检测接收缓冲区非空

    if(midata != NULL)
      midata[i] = SPIx->DR;            //保存接收到的数据
    else
      SPIx->DR;
  }
}
/*!
*  @brief      SPI发送接收函数
*  @param      SPIn_e          SPI模块(SPI0、SPI1)
*  @param      SPIn_PCSn_e     片选管脚编号
*  @param      mocmd           发送的命令缓冲区地址(不需要接收则传 NULL)
*  @param      micmd           发送命令时接收到的数据的存储地址(不需要接收则传 NULL)
*  @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
*  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
*  @since      v5.0
*  Sample usage:           spi_mosi(SPI0,SPIn_PCS0,cmd,NULL,buff,buff,1,2);    //发送cmd/buff的内容，不接收cmd发送时的数据，接收buff发送时的数据到buff里，长度分别为1、2字节
*/
void spi_mosi_cmd(SPI_TypeDef*  SPIx, uint8 *mocmd, uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len)
{
//  SPI_MemMapPtr spi_ptr = SPIN[spin];
  uint32 i = 0;
  
//  while((~SPI_S_REG(spi_ptr))&SPI_S_SPTEF_MASK);//等待发送缓冲区空闲
//  while((SPI_S_REG(spi_ptr) & SPI_S_SPRF_MASK))    //data avalable if SPRF=1
//    SPI_DL_REG(spi_ptr);                     //直接读取数据来清空接收缓冲区
  for(i = 0; i < cmdlen; i++)  /***************** 发送len个address *******************/
  {
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //等待发送缓冲区空闲
    SPIx->DR = mocmd[i];
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);  //检测接收缓冲区非空
    if(micmd != NULL)
      micmd[i] = SPIx->DR;            //保存接收到的数据
    else
      SPIx->DR;
  }
  for(i = 0; i<len; i++)  /***************** 发送len个data *******************/
  {
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //等待发送缓冲区空闲
    SPIx->DR = modata[i]; 
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);  //检测接收缓冲区非空
    if(midata != NULL)
      midata[i] = SPIx->DR;                //保存接收到的数据
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
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*使能SWD 禁用JTAG*/
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
* 函数 ：SPI_ExchangeByte() => 通过SPI进行数据交换                          *
* 输入 ：需要写入SPI的值                                                    *
* 输出 ：通过SPI读出的值                                                    *
============================================================================*/
uint8 SPI_ExchangeByte(uint8 input)
{
  SPI_I2S_SendData(SPI1, input);
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return (SPI_I2S_ReceiveData(SPI1));
}



