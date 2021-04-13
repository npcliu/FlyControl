#include "include.h" 

char nrf_int_flag = 1;          //看门狗标志，定时置位
char pwr_low = 1;               //电池能量不足
char nrf_break = 1;             //无线失联状态
uint32 light_ness = 0;        //LCD背光亮度
int expriment_time = 0;//记录实验模式下的实验时间  
extern uint8 start_expriment_flag;  
extern uint32 timetstcount;
int encoder = 0;
int key2_press_time = 0;//按键按下的时间
void TIM4_IRQHandler(void)/*******************定时中断*******************/
{
//  timetstcount++;
  
  static uint32 irq_count = 0;
  static uint8 flag500ms_time = 0;
  
    key2_press_time ++;//这个叫相对延时，按键按下后这里会从零开始加
  if(key2_press_time>10000)//防止溢出
    key2_press_time = 10000;
  
  if(start_expriment_flag){
    expriment_time++;
    if(expriment_time >10000)
      expriment_time = 10000;
  }
  else
    expriment_time = 0;
  
  pit_5ms_flag = 1;
  irq_count++;
  if(irq_count>=100000)
    irq_count = 0;
  PA8_OUT = 0;
//  nrf_tx(irq_tx_buff,DATA_PACKET);//DATA_PACKET
  
  if(0==irq_count%10)           //INTERUPT_CYC_IN_MS*10 = 50ms is this period检查一次nrf的接收情况
  {
    pit_50ms_flag = 1;
    if(nrf_int_flag<126)
      nrf_int_flag ++;           //置位查验标志（nrf接收中断复位标志）
    if(nrf_int_flag>=2)        //超過1秒未收到信号才关油门
    {
      nrf_int_flag = 0;                   //喂狗
      nrf_handler();
    }

    //    light_ness = (photo_res_voltage)*3;
//    if(light_ness>=9000)
//      light_ness = 9000;              //enough dark
    TIM_SetCompare3(TIM1,1000);  
  }
  extern char pit_20ms_flag;
  if(0==irq_count%4)
  {
    pit_20ms_flag = 1;
  }
  switch(irq_count%100)         //INTERUPT_CYC_IN_MS*100 is this period
  {
  case 0:
    {
    pit_500ms_flag = 1;
      flag500ms_time++;
      if(flag500ms_time>=10)             //10
      {
        flag500ms_time = 0;
        pit_5s_flag = 1;
      }
    }break;
  default:  break;
  }
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
  PA8_OUT = 1;
}
void EXTI9_5_IRQHandler()
{
    EXTI_ClearITPendingBit(EXTI_Line5);
    nrf_int_flag = 0;                   //喂狗
    nrf_handler();
}
//  uint32 countyyy = 0;
void EXTI0_IRQHandler()
{
    EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI15_10_IRQHandler()
{
  if(EXTI_GetITStatus(ENSW_IRQ_LINE)!=RESET)
  {
    confirm_flag = ~confirm_flag;
    EXTI_ClearITPendingBit(ENSW_IRQ_LINE);
  }
  else if(EXTI_GetITStatus(ENA_IRQ_LINE)!=RESET)      //ENA
  {
    if(ENB_IN)
    {
      encoder++;
    }
    else
    {
      encoder--;
    }
//    RGB_R_OUT = 1;
//    nrf_int_flag = 0;                   //喂狗
//    RGB_R_OUT = 0;
    EXTI_ClearITPendingBit(ENA_IRQ_LINE);
//    DelayUs(500);
//            BLUE_OUT = 0;
  }
  else if(EXTI_GetITStatus(KEY1_IRQ_LINE)!=RESET)
  {
     if(plane_mod != experiment)//如果小于枚举类型的最大值，就可以加，否则就回到枚举类型的最小值
        plane_mod += 1;
      else 
        plane_mod = lock;
      EXTI_ClearITPendingBit(KEY1_IRQ_LINE);              //在硬件上按下KEY1进入这里
  }
  else if(EXTI_GetITStatus(KEY2_IRQ_LINE)!=RESET)      //ENA
  {
     if(key2_press_time > 400){
      key2_press_time = 0;
      if(irq_tx_buff[PLANE_MODE_OFFSET] == 'e'){
        start_expriment_flag += 1;       //在实验模式按下按鍵，开始实验，开始实验后再按下按键，停止实验
        if(start_expriment_flag>1)
          start_expriment_flag = 0;
        
      }
      else
      set_param_flag = ~set_param_flag;
    }
    EXTI_ClearITPendingBit(KEY2_IRQ_LINE);
  }
  else
    while(1);
}

//uint16 DMA_RX_size = 0;
//void LumMod_Uart_DMA_Rx_Data(void)
//{
//  DMA_Cmd(DMA1_Channel3, DISABLE);       // 关闭DMA ，防止干扰
//  DMA_ClearFlag( DMA1_FLAG_GL3 );           // 清DMA标志位
//  DMA_RX_size = 1024 - DMA_GetCurrDataCounter(DMA1_Channel3); //获得接收到的字节数
//  DMA1_Channel3->CNDTR = 1024;    //  重新赋值计数值，必须大于等于最大可能接收到的数据帧数目
//  DMA_Cmd(DMA1_Channel3, ENABLE);        /* DMA 开启，等待数据。注意，如果中断发送数据帧的速率很快，MCU来不及处理此次接收到的数据，中断又发来数据的话，这里不能开启，否则数据会被覆盖。有2种方式解决。*/
//}
//#define Usart1RecLength 256
//uint16 Uart1_RevBuf_Tail = 0;
//uint16 Uart1_RevBuf_Head = 0;
//uint8 Uart1_RevBuf[Usart1RecLength];
//
//char usart_GPS_sd_flag = 0;
//uint32 dma_max_recieve;
//nmeaINFO info;
//void USART3_IRQHandler(void)
//{
//  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
//  {
////    RED_OUT = 0;
//    Uart1_RevBuf[Uart1_RevBuf_Tail] = USART_ReceiveData(USART3);    //读取接收到的数据
//    if(usart_GPS_sd_flag)
//      USART_SendData(USART1, Uart1_RevBuf[Uart1_RevBuf_Tail]);
//    Uart1_RevBuf_Tail++;
//    if(Uart1_RevBuf_Tail > Usart1RecLength-1)
//    {
//      Uart1_RevBuf_Tail = 0;    
//    }
////    RED_OUT = 1;
//  }
//  if(USART_GetITStatus(USART3,USART_IT_IDLE)!= RESET)
//  {
//    sCom.RecvLen = (uint32)sizeof(sCom.RecvBuf) - DMA1_Channel3->CNDTR;
//    if(dma_max_recieve<sCom.RecvLen)
//      dma_max_recieve = sCom.RecvLen;
//    DMA1_Channel3->CCR &= ~DMA_CCR3_EN; /* Channel disabled */
//    DMA1->IFCR |= DMA_IFCR_CGIF3;       //Channel 3 Global interrupt clear
//    DMA1_Channel3->CNDTR = (uint32)sizeof(sCom.RecvBuf); /* Number of data to transfer */
//    DMA1_Channel3->CCR |= DMA_CCR3_EN; /* Channel enabled */
//    
//    if(USART3->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE)) /* Noise/Framing/Parity error is detected */
//    {
//    }
//    else
//    {
//      sCom.fRecv = 1;
//    }
//    uint32 sr = USART3->SR; /* clear sr */
//    uint32 dr = USART3->DR; /* clear dr */
//    
//    RED_OUT = 0;
////    gps_re_flag = 1;
//    nmeaPARSER parser;
//    nmea_zero_INFO(&info);
////    RED_OUT = 1;
//    nmea_parser_init(&parser);
////    RED_OUT = 0;
//    nmea_parse(&parser, sCom.RecvBuf, (int)strlen(sCom.RecvBuf), &info);
//    nmea_parser_destroy(&parser);
//GPSCal(&info, 1);
//RED_OUT = 1;
//  }
//}

//void DMA1_Channel3_IRQHandler(void)
//{
//  DMA_ClearITPendingBit(DMA1_IT_TC3);
//  DMA_ClearITPendingBit(DMA1_IT_TE3);
//  DMA_Cmd(DMA1_Channel3, DISABLE); 
//  DMA1_Channel3->CNDTR = 1024;
//  DMA_Cmd(DMA1_Channel3, ENABLE);
//}