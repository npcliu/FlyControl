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
      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //等待发送缓冲区空闲

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
  NRF_CSN = 0;
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
  
  NRF_CSN = 1;
}



