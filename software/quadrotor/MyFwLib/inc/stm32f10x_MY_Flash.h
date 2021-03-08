/**
  ******************************************************************************
  * @file    stm32f10x_MY_Flash.h
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
#ifndef __STM32F10x_MY_FLASH_H
#define __STM32F10x_MY_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

//    #define SPIB(X,n,REG)         (((_32type*)(&(SPI##X->REG)))->b##n)//per bit

uint8_t FLASH_ReadByte(uint32_t address);
uint16_t FLASH_ReadHalfWord(uint32_t address);
uint32_t FLASH_ReadWord(uint32_t address);
float FlashReadFloat(uint32_t address);





#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_MY_SPI_H */