#include "stm32f10x_flash.h"
#include "stm32f10x_MY_Flash.h"

//��ȡָ����ַ�İ���(16λ����)
//Ҳ�ǰ��հ��ֶ�������ÿ�ζ�2���ֽ����ݷ���
uint8_t FLASH_ReadByte(uint32_t address)
{
  return *(__IO uint8_t*)address; 
}
uint16_t FLASH_ReadHalfWord(uint32_t address)
{
  return *(__IO uint16_t*)address; 
}

uint32_t FLASH_ReadWord(uint32_t address)
{
  return *(__IO uint32_t*)address; 
}

float FlashReadFloat(uint32_t address)
{
//  float data = 0;
  uint32 tmp = *(__IO uint32_t*)address;
  return *(float*)&tmp;
    }

