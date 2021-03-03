#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"

/* ------------ RCC registers bit address in the alias region ----------------*/
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

/* --- EVENTCR Register -----*/

/* Alias word address of EVOE bit */
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((uint8_t)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))

/* ---  MAPR Register ---*/ 
/* Alias word address of MII_RMII_SEL bit */ 
#define MAPR_OFFSET                 (AFIO_OFFSET + 0x04) 
#define MII_RMII_SEL_BitNumber      ((u8)0x17) 
#define MAPR_MII_RMII_SEL_BB        (PERIPH_BB_BASE + (MAPR_OFFSET * 32) + (MII_RMII_SEL_BitNumber * 4))


#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)

/**
* @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @retval None
*/
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  if (GPIOx == GPIOA)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
  }    
  else if (GPIOx == GPIOE)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
  } 
  else if (GPIOx == GPIOF)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
  }
  else
  {
    if (GPIOx == GPIOG)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
    }
  }
}

/**
* @brief  Deinitializes the Alternate Functions (remap, event control
*   and EXTI configuration) registers to their default reset values.
* @param  None
* @retval None
*/
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/**
* @brief  Initializes the GPIOx peripheral according to the specified
*         parameters in the GPIO_InitStruct.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
*         contains the configuration information for the specified GPIO peripheral.
* @retval None
*/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
  /*---------------------------- GPIO Mode Configuration -----------------------*/
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
  if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
  {
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode */
    currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
  }
  /*---------------------------- GPIO CRL Configuration ------------------------*/
  /* Configure the eight low port pins */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* Get the port pins position */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding low control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        }
        else
        {
          /* Set the corresponding ODR bit */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }
  /*---------------------------- GPIO CRH Configuration ------------------------*/
  /* Configure the eight high port pins */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* Get the port pins position */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* Set the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

//端口初始化函数
//参数：pin：引脚名称
//      mode：引脚模式
//      interupt_flag：是否开启引脚中断标志
//      speed：速度
//      level：初始化电平
//例子：
//      初始化端口PB9，端口模式IPU，开启端口中断，端口速度GPIO_Speed_50MHz，输入电平1
//      gpio_init(PB9,IPU,interupt,GPIO_Speed_50MHz,1);
void gpio_init(PTXn_e pin, GPIOMode_TypeDef mode, GPIOIfInterupt_Typedef interupt_flag, GPIOSpeed_TypeDef speed, uint8 initial_level)
{
  uint8 port;           //确定pin属于pa，pb，pc……
  uint8 which_port_s_RCC = 0;   //确定是GPIO_Pin_0~GPIO_Pin_15中的哪一个port的RCC
  uint8 pin_offset = 0;
  uint8 ctrl_reg_offset = 0;
  GPIO_TypeDef * port_to_portADDR = 0; //0==port，即PA组时，port_to_port_ADDR取PORTA
  
  port = pin/16;        //PA的所有端口port = 0，PB = 1，……
  which_port_s_RCC = (0==port)*RCC_APB2Periph_GPIOA+(1==port)*RCC_APB2Periph_GPIOB+(2==port)*RCC_APB2Periph_GPIOC+(3==port)*RCC_APB2Periph_GPIOD+(4==port)*RCC_APB2Periph_GPIOE;
  switch(port)
  {
  case 0:
    {
      port_to_portADDR = GPIOA;
    }break;
  case 1:
    {
      port_to_portADDR = GPIOB;
    }break;
  case 2:
    {
      port_to_portADDR = GPIOC;
    }break;
  case 3:
    {
      port_to_portADDR = GPIOD;
    }break;
  case 4:
    {
      port_to_portADDR = GPIOE;
    }break;
  default:break;
  }
  pin_offset = pin%16;
//  GPIO_Pin = (0==pin_offset)*GPIO_Pin_0+(1==pin_offset)*GPIO_Pin_1+(2==pin_offset)*GPIO_Pin_2+(3==pin_offset)*GPIO_Pin_3+(4==pin_offset)*GPIO_Pin_4+(5==pin_offset)*GPIO_Pin_5+(6==pin_offset)*GPIO_Pin_6+(7==pin_offset)*GPIO_Pin_7+(8==pin_offset)*GPIO_Pin_8+(9==pin_offset)*GPIO_Pin_9+(10==pin_offset)*GPIO_Pin_10+(11==pin_offset)*GPIO_Pin_11+(12==pin_offset)*GPIO_Pin_12+(13==pin_offset)*GPIO_Pin_13+(14==pin_offset)*GPIO_Pin_14+(15==pin_offset)*GPIO_Pin_15;
  //配置时钟  
  if(interupt_flag)                     //如果是中断模式，需要是能辅助时钟RCC_APB2Periph_AFIO，用于外部中断
    RCC_APB2PeriphClockCmd(which_port_s_RCC|RCC_APB2Periph_AFIO, ENABLE);	 //??PB???
  else                                  //如果不是中断模式，只需要开启端口时钟即可
//    RCC_APB2PeriphClockCmd(which_port_s_RCC, ENABLE);	 //??PB???
        RCC->APB2ENR |= which_port_s_RCC; //RCC->APB2ENR = RCC->APB2ENR|RCC_APB2Periph;

  uint32_t currentmode = 0x00;// currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /*---------------------------- GPIO Mode Configuration -----------------------*/
  currentmode = ((uint32_t)mode) & ((uint32_t)0x0F);
  if ((((uint32_t)mode) & ((uint32_t)0x10)) != 0x00)
  {
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(speed));
    /* Output mode */
    currentmode |= (uint32_t)speed;
  }
    port_to_portADDR->ODR |= (initial_level<<pin_offset);         //preset the voltage levelof proper pin,then set the pin as output,
  /*---------------------------- GPIO CRL Configuration --manual page 171----------------------*/
  /* Configure the eight low port pins */
  if (pin_offset<8)//(((uint32_t)GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = port_to_portADDR->CRL;
    ctrl_reg_offset = pin_offset<<2;
    /* Clear the corresponding low control register bits */
    pinmask = ((uint32_t)0x0F) << ctrl_reg_offset;
    tmpreg &= ~pinmask;
    /* Write the mode configuration in the corresponding bits */
    tmpreg |= (currentmode << ctrl_reg_offset);
    port_to_portADDR->CRL = tmpreg;
  }
  /*---------------------------- GPIO CRH Configuration --manual page 172----------------------*/
  /* Configure the eight high port pins; */
  else if(pin_offset>=8)
  {
    tmpreg = port_to_portADDR->CRH;
    ctrl_reg_offset = (pin_offset-8)<<2;
    /* Clear the corresponding high control register bits */
    pinmask = ((uint32_t)0x0F) << ctrl_reg_offset;
    tmpreg &= ~pinmask;
    /* Write the mode configuration in the corresponding bits */
    tmpreg |= (currentmode << ctrl_reg_offset);
    /* Reset the corresponding ODR bit */
    port_to_portADDR->CRH = tmpreg;     //port configuration register high
  }
    if (mode == GPIO_Mode_IPD)              //manual page 161
      port_to_portADDR->ODR &= ~((1u) << pin_offset);
    else if (mode == GPIO_Mode_IPU)          /* Set the corresponding ODR bit */
      port_to_portADDR->ODR |= ((1u) << pin_offset);
}
/*
EXTI_Line:引脚编号
示例：    gpio_int_cfg(PC6,EXTI_Line6,EXTI_Trigger_Rising);       //配置中断

*/
void gpio_int_cfg(PTXn_e pin,uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger)
{
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
//  EXTI_InitStructure.EXTI_Line    = EXTI_Line9;
//  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
  uint8 port;
  uint32_t tmp = 0;
  
  port = pin/16;        //PA的所有端口port = 0，PB = 1，……
//  which_port_s_RCC = (0==port)*GPIO_PortSourceGPIOA+(1==port)*GPIO_PortSourceGPIOB+(2==port)*RCC_APB2Periph_GPIOC+(3==port)*RCC_APB2Periph_GPIOD+(4==port)*RCC_APB2Periph_GPIOE;
//Test(3,4);
      GPIO_EXTILineConfig(port,pin);

  tmp = (uint32_t)EXTI_BASE;
//  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)       //既然都是设置中断方式了，肯定要使能中断，所以取消判断
//  {
    /* Clear EXTI line configuration */
    EXTI->IMR &= ~EXTI_Line;
    EXTI->EMR &= ~EXTI_Line;
    
    tmp += EXTI_Mode_Interrupt;

    *(__IO uint32_t *) tmp |= EXTI_Line;

    /* Clear Rising Falling edge configuration */
    EXTI->RTSR &= ~EXTI_Line;
    EXTI->FTSR &= ~EXTI_Line;
    
    /* Select the trigger for the selected external interrupts */
    if (EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* Rising Falling edge */
      EXTI->RTSR |= EXTI_Line;
      EXTI->FTSR |= EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_Trigger;

      *(__IO uint32_t *) tmp |= EXTI_Line;
    }
//  }
//  else
//  {
//    tmp += EXTI_Mode_Interrupt;
//
//    /* Disable the selected external lines */
//    *(__IO uint32_t *) tmp &= ~EXTI_Line;
//  }
}

/**
* @brief  Fills each GPIO_InitStruct member with its default value.
* @param  GPIO_InitStruct : pointer to a GPIO_InitTypeDef structure which will
*         be initialized.
* @retval None
*/
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* Reset GPIO init structure parameters values */
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/**
* @brief  Reads the specified input port pin.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_Pin:  specifies the port bit to read.
*   This parameter can be GPIO_Pin_x where x can be (0..15).
* @retval The input port pin value.
*/
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
* @brief  Reads the specified GPIO input data port.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @retval GPIO input data port value.
*/
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/**
* @brief  Reads the specified output data port bit.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_Pin:  specifies the port bit to read.
*   This parameter can be GPIO_Pin_x where x can be (0..15).
* @retval The output port pin value.
*/
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
* @brief  Reads the specified GPIO output data port.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @retval GPIO output data port value.
*/
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->ODR);
}

/**
* @brief  Sets the selected data port bits.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_Pin: specifies the port bits to be written.
*   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
* @retval None
*/
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
//  _32type i;
//  (&i)->b0=1;
//  ((_32type*)(&(GPIOA->ODR)))->b1 = 0;
    
//  GPIOx->BSRR = GPIO_Pin;
//  PD10_OUT = 1;
//  GPIOx->ODR |= (1<<10);
    GPIOx->BRR = GPIO_Pin;
}

/**
* @brief  Clears the selected data port bits.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_Pin: specifies the port bits to be written.
*   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
* @retval None
*/
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
//  GPIOx->BRR = GPIO_Pin;
  GPIOx->ODR &= ~(1<<10);
}

/**
* @brief  Sets or clears the selected data port bit.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_Pin: specifies the port bit to be written.
*   This parameter can be one of GPIO_Pin_x where x can be (0..15).
* @param  BitVal: specifies the value to be written to the selected bit.
*   This parameter can be one of the BitAction enum values:
*     @arg Bit_RESET: to clear the port pin
*     @arg Bit_SET: to set the port pin
* @retval None
*/
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

/**
* @brief  Writes data to the specified GPIO data port.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  PortVal: specifies the value to be written to the port output data register.
* @retval None
*/
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/**
* @brief  Locks GPIO Pins configuration registers.
* @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
* @param  GPIO_Pin: specifies the port bit to be written.
*   This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
* @retval None
*/
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint32_t tmp = 0x00010000;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  tmp |= GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Reset LCKK bit */
  GPIOx->LCKR =  GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Read LCKK bit*/
  tmp = GPIOx->LCKR;
  /* Read LCKK bit*/
  tmp = GPIOx->LCKR;
}

/**
* @brief  Selects the GPIO pin used as Event output.
* @param  GPIO_PortSource: selects the GPIO port to be used as source
*   for Event output.
*   This parameter can be GPIO_PortSourceGPIOx where x can be (A..E).
* @param  GPIO_PinSource: specifies the pin for the Event output.
*   This parameter can be GPIO_PinSourcex where x can be (0..15).
* @retval None
*/
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmpreg = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmpreg = AFIO->EVCR;
  /* Clear the PORT[6:4] and PIN[3:0] bits */
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (uint32_t)GPIO_PortSource << 0x04;
  tmpreg |= GPIO_PinSource;
  AFIO->EVCR = tmpreg;
}

/**
* @brief  Enables or disables the Event Output.
* @param  NewState: new state of the Event output.
*   This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) EVCR_EVOE_BB = (uint32_t)NewState;
}

/**
* @brief  Changes the mapping of the specified pin.
* @param  GPIO_Remap: selects the pin to remap.
*   This parameter can be one of the following values:
*     @arg GPIO_Remap_SPI1             : SPI1 Alternate Function mapping
*     @arg GPIO_Remap_I2C1             : I2C1 Alternate Function mapping
*     @arg GPIO_Remap_USART1           : USART1 Alternate Function mapping
*     @arg GPIO_Remap_USART2           : USART2 Alternate Function mapping
*     @arg GPIO_PartialRemap_USART3    : USART3 Partial Alternate Function mapping
*     @arg GPIO_FullRemap_USART3       : USART3 Full Alternate Function mapping
*     @arg GPIO_PartialRemap_TIM1      : TIM1 Partial Alternate Function mapping
*     @arg GPIO_FullRemap_TIM1         : TIM1 Full Alternate Function mapping
*     @arg GPIO_PartialRemap1_TIM2     : TIM2 Partial1 Alternate Function mapping
*     @arg GPIO_PartialRemap2_TIM2     : TIM2 Partial2 Alternate Function mapping
*     @arg GPIO_FullRemap_TIM2         : TIM2 Full Alternate Function mapping
*     @arg GPIO_PartialRemap_TIM3      : TIM3 Partial Alternate Function mapping
*     @arg GPIO_FullRemap_TIM3         : TIM3 Full Alternate Function mapping
*     @arg GPIO_Remap_TIM4             : TIM4 Alternate Function mapping
*     @arg GPIO_Remap1_CAN1            : CAN1 Alternate Function mapping
*     @arg GPIO_Remap2_CAN1            : CAN1 Alternate Function mapping
*     @arg GPIO_Remap_PD01             : PD01 Alternate Function mapping
*     @arg GPIO_Remap_TIM5CH4_LSI      : LSI connected to TIM5 Channel4 input capture for calibration
*     @arg GPIO_Remap_ADC1_ETRGINJ     : ADC1 External Trigger Injected Conversion remapping
*     @arg GPIO_Remap_ADC1_ETRGREG     : ADC1 External Trigger Regular Conversion remapping
*     @arg GPIO_Remap_ADC2_ETRGINJ     : ADC2 External Trigger Injected Conversion remapping
*     @arg GPIO_Remap_ADC2_ETRGREG     : ADC2 External Trigger Regular Conversion remapping
*     @arg GPIO_Remap_ETH              : Ethernet remapping (only for Connectivity line devices)
*     @arg GPIO_Remap_CAN2             : CAN2 remapping (only for Connectivity line devices)
*     @arg GPIO_Remap_SWJ_NoJTRST      : Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST
*     @arg GPIO_Remap_SWJ_JTAGDisable  : JTAG-DP Disabled and SW-DP Enabled
*     @arg GPIO_Remap_SWJ_Disable      : Full SWJ Disabled (JTAG-DP + SW-DP)
*     @arg GPIO_Remap_SPI3             : SPI3/I2S3 Alternate Function mapping (only for Connectivity line devices)
*                                        When the SPI3/I2S3 is remapped using this function, the SWJ is configured
*                                        to Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST.   
*     @arg GPIO_Remap_TIM2ITR1_PTP_SOF : Ethernet PTP output or USB OTG SOF (Start of Frame) connected
*                                        to TIM2 Internal Trigger 1 for calibration (only for Connectivity line devices)
*                                        If the GPIO_Remap_TIM2ITR1_PTP_SOF is enabled the TIM2 ITR1 is connected to 
*                                        Ethernet PTP output. When Reset TIM2 ITR1 is connected to USB OTG SOF output.    
*     @arg GPIO_Remap_PTP_PPS          : Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devices)
*     @arg GPIO_Remap_TIM15            : TIM15 Alternate Function mapping (only for Value line devices)
*     @arg GPIO_Remap_TIM16            : TIM16 Alternate Function mapping (only for Value line devices)
*     @arg GPIO_Remap_TIM17            : TIM17 Alternate Function mapping (only for Value line devices)
*     @arg GPIO_Remap_CEC              : CEC Alternate Function mapping (only for Value line devices)
*     @arg GPIO_Remap_TIM1_DMA         : TIM1 DMA requests mapping (only for Value line devices)
*     @arg GPIO_Remap_TIM9             : TIM9 Alternate Function mapping (only for XL-density devices)
*     @arg GPIO_Remap_TIM10            : TIM10 Alternate Function mapping (only for XL-density devices)
*     @arg GPIO_Remap_TIM11            : TIM11 Alternate Function mapping (only for XL-density devices)
*     @arg GPIO_Remap_TIM13            : TIM13 Alternate Function mapping (only for High density Value line and XL-density devices)
*     @arg GPIO_Remap_TIM14            : TIM14 Alternate Function mapping (only for High density Value line and XL-density devices)
*     @arg GPIO_Remap_FSMC_NADV        : FSMC_NADV Alternate Function mapping (only for High density Value line and XL-density devices)
*     @arg GPIO_Remap_TIM67_DAC_DMA    : TIM6/TIM7 and DAC DMA requests remapping (only for High density Value line devices)
*     @arg GPIO_Remap_TIM12            : TIM12 Alternate Function mapping (only for High density Value line devices)
*     @arg GPIO_Remap_MISC             : Miscellaneous Remap (DMA2 Channel5 Position and DAC Trigger remapping, 
*                                        only for High density Value line devices)     
* @param  NewState: new state of the port pin remapping.
*   This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
  uint32_t tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_REMAP(GPIO_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if((GPIO_Remap & 0x80000000) == 0x80000000)
  {
    tmpreg = AFIO->MAPR2;
  }
  else
  {
    tmpreg = AFIO->MAPR;
  }
  
  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
  tmp = GPIO_Remap & LSB_MASK;
  
  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
  {
    tmp1 = ((uint32_t)0x03) << tmpmask;
    tmpreg &= ~tmp1;
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  else
  {
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  
  if (NewState != DISABLE)
  {
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }
  
  if((GPIO_Remap & 0x80000000) == 0x80000000)
  {
    AFIO->MAPR2 = tmpreg;
  }
  else
  {
    AFIO->MAPR = tmpreg;
  }  
}

/**
* @brief  Selects the GPIO pin used as EXTI Line.
* @param  GPIO_PortSource: selects the GPIO port to be used as source for EXTI lines.
*   This parameter can be GPIO_PortSourceGPIOx where x can be (A..G).
* @param  GPIO_PinSource: specifies the EXTI line to be configured.
*   This parameter can be GPIO_PinSourcex where x can be (0..15).
* @retval None
MANUAL PAGE 191 & 210
*/
void GPIO_EXTILineConfig(uint8 GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0;
  uint32 pin_offset = 0;
  int i = 0;
  
  /* Check the parameters */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  pin_offset = GPIO_PinSource%16;
  if(pin_offset<4)//?待考究是否是4，主要考究tmp和EXTICR数组下标的关系2019/12/1
  {
    for(i=0;i<4;i++)
    {
      if(GPIO_PortSource&(1<<i))
        AFIO->EXTICR[0] |= 1<<((pin_offset<<2) + i);
      else
        AFIO->EXTICR[0] &= ~(1<<((pin_offset<<2) + i));
    }
  }
  //manual page 191 & 210
  else
  {
  tmp = ((uint32_t)0x0F) << (0x04 * (pin_offset & (uint8_t)0x03));
  AFIO->EXTICR[pin_offset >> 0x02] &= ~tmp;
  AFIO->EXTICR[pin_offset >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (pin_offset & (uint8_t)0x03)));
  }
}
/**
* @brief  Selects the Ethernet media interface.
* @note   This function applies only to STM32 Connectivity line devices.  
* @param  GPIO_ETH_MediaInterface: specifies the Media Interface mode.
*   This parameter can be one of the following values:
*     @arg GPIO_ETH_MediaInterface_MII: MII mode
*     @arg GPIO_ETH_MediaInterface_RMII: RMII mode    
* @retval None
*/
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface) 
{ 
  assert_param(IS_GPIO_ETH_MEDIA_INTERFACE(GPIO_ETH_MediaInterface)); 
  
  /* Configure MII_RMII selection bit */ 
  *(__IO uint32_t *) MAPR_MII_RMII_SEL_BB = GPIO_ETH_MediaInterface; 
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
