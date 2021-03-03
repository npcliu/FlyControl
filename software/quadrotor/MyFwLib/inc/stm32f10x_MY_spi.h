/**
  ******************************************************************************
  * @file    stm32f10x_MY_spi.h
  * @author  BD
  * @version V1
  * @date    31-May-2020
  * @brief   simple SPI
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef __STM32F10x_MY_SPI_H
#define __STM32F10x_MY_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif
   
#define SPIB(X,n,REG)         (((_32type*)(&(SPI##X->REG)))->b##n)//per bit
#define SET_SPIn_POLARITY(n,polarity)         SPIB(n,6,CR1) = 0;\
   SPIB(n,1,CR1) = polarity;\
     SPIB(n,0,CR1) = polarity;\
       SPIB(n,6,CR1) = 1;
//n: which SPI,example:n=2 means SPI2
// polarity:polarity of the SPI clock,[manual P741]
//DISABLE SPI3 according to the note on manual page 703
//Set clock polarity£»0: CK to 0 when idle
//Clock phase
//ENABLE SPI3 according to the note on manual page 703



void spi_mosi(SPI_TypeDef*  SPIx, uint8 *modata, uint8 *midata, uint32 len);
void spi_mosi_cmd(SPI_TypeDef*  SPIx, uint8 *mocmd, uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len);
void SPI1_Init(void);
void SPI3_Init(void);
   
   
   
   
#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_MY_SPI_H */