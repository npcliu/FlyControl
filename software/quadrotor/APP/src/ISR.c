#include <string.h>
#include "stm32f10x.h"
#include "define.h"
#include "Mpu6050.h"
#include "MyNRF24L0.h"
#include "QMC5883.h"
#include "L3GD20.h"
#include "calculation.h"
#include <parser.h>
#include "interact.h"

/***************************定时中断,triggerd by TIM*******************************/
//extern float filted_acc[2][3];              //filted acc data,2 row means 2 acceleration chip,3 means 3aixs; filted_acc[0][1] reprsent chip 0,y axis filted data
//extern float filted_gyro[2][3];             //filted gyro data;
//extern float filted_cps[2][3];              //filted compass data
short acc_chip_out[3] = {0};                   //data the chip gives
short gyro_chip_out[3] = {0};                   //data the chip gives
short cps_chip_out[3] = {0};                   //data the chip gives
float filted_acc[2][3] = {0};              //filted acc data,2 row means 2 acceleration chip,3 means 3aixs; filted_acc[0][1] reprsent chip 0,y axis filted data
float filted_gyro[2][3] = {0};             //filted gyro data;
float filted_cps[2][3] = {0};              //filted compass data
char pit_5ms_flag = 0;
char pit_25ms_flag = 0;
char pit_50ms_flag = 0;
char pit_500ms_flag = 0;
char pit_5s_flag = 0;         //5s时间标志
void TIM4_IRQHandler(void)
{
  static uint32 irq_count = 0;
  BLUE_OUT = 0;
//  GPSCal(&info, 1);
      CaliFilt(filted_acc[0],filted_gyro[0],filted_cps[0],&acc,&gyro,&compass,acc_chip_out,gyro_chip_out,cps_chip_out);
//    AccAngleCal(&LSM_acc_adc,&acc_ang_x[1],&acc_ang_y[1]);
    AttCalc(angle, filted_acc[0], filted_gyro[0], filted_cps[0], pipl_dir);                       //
    PWMCalc(pipl_dir);

  irq_count++;
  if(irq_count>=100000)
    irq_count = 0;
  if(0==irq_count%5)           //INTERUPT_CYC_IN_MS*5 = 25ms is this period检查一次nrf的接收情况
    pit_25ms_flag = 1;
  if(0==irq_count%10)           //INTERUPT_CYC_IN_MS*10 = 50ms is this period检查一次nrf的接收情况
    pit_50ms_flag = 1;
  if(0==irq_count%100)         //INTERUPT_CYC_IN_MS*100 is this period
    pit_500ms_flag = 1;
  if(0==irq_count%1000)
    pit_5s_flag = 1;
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
  BLUE_OUT = 1;
}
void EXTI15_10_IRQHandler()
{
/****************************triggerd by MPU6050 IRQ pin*******************************/
  if(EXTI_GetITStatus(MPUINT_Line)!=RESET)
  {
    RED_OUT = 0;

    MPUReadAcc(acc_chip_out);//taks 0.26ms
    MPUReadGyr(gyro_chip_out);//
    ReadQMC5883(cps_chip_out);
//    LSM303M_Raed((short*)&LSM_compass);
//    ReadHMC5883((short*)&compass);
//    Multiple_Read_QMC5883((short*)&compass);
//    AccAngleCal(&acc,acc_angle[0],&compass);
//    CaliFilt(filted_acc[0],filted_gyro[0],filted_cps[0],&acc,&gyro,&compass,acc_chip_out,gyro_chip_out,cps_chip_out);
////    AccAngleCal(&LSM_acc_adc,&acc_ang_x[1],&acc_ang_y[1]);
//    AttCalc(angle, filted_acc[0], filted_gyro[0], filted_cps[0]);                       //
//    PWM_Calculation();
    
    EXTI_ClearITPendingBit(MPUINT_Line);
    RED_OUT = 1;
  }
  else
    assert(0);
}
char nrf_int_flag = 1;          //看门狗标志，定时置位
void EXTI9_5_IRQHandler()
{
/****************************triggerd by NRF IRQ pin*******************************/
  if(EXTI_GetITStatus(NRF_IRQ_Line)!=RESET)
  {
    EXTI_ClearITPendingBit(NRF_IRQ_Line);
    RGB_B_TRN;
    nrf_int_flag = 0;                   //喂狗
//SET_SPIn_POLARITY(3,1);                 //set spi'3' polarity to '1'
    //    
    //    LSM303A_Raed((short*)&LSM_acc_adc);
    //    L3GD20_Read((short*)&L3GDgyro_adc);
//SET_SPIn_POLARITY(3,0);                 //set spi'3' polarity to '1'
    nrf_handler();
    RCDenote();                 //加入参数模式后此句应放在飞行模式中
  }
  else if(EXTI_GetITStatus(GYRO_IRQ_Line)!=RESET)
  {
    EXTI_ClearITPendingBit(GYRO_IRQ_Line);
  }
}

uint16 DMA_RX_size = 0;
#define Usart1RecLength 256
uint16 Uart1_RevBuf_Tail = 0;
uint16 Uart1_RevBuf_Head = 0;
uint8 Uart1_RevBuf[Usart1RecLength];

char usart_GPS_sd_flag = 0;
uint32 dma_max_recieve;
nmeaINFO info;
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
  {
    Uart1_RevBuf[Uart1_RevBuf_Tail] = USART_ReceiveData(USART3);    //读取接收到的数据
    if(usart_GPS_sd_flag)
      USART_SendData(USART1, Uart1_RevBuf[Uart1_RevBuf_Tail]);
    Uart1_RevBuf_Tail++;
    if(Uart1_RevBuf_Tail > Usart1RecLength-1)
      Uart1_RevBuf_Tail = 0;    
  }
  if(USART_GetITStatus(USART3,USART_IT_IDLE)!= RESET)
  {
    sCom.RecvLen = (uint32)sizeof(sCom.RecvBuf) - DMA1_Channel3->CNDTR;
    if(dma_max_recieve<sCom.RecvLen)
      dma_max_recieve = sCom.RecvLen;
    DMA1_Channel3->CCR &= ~DMA_CCR3_EN; /* Channel disabled */
    DMA1->IFCR |= DMA_IFCR_CGIF3;       //Channel 3 Global interrupt clear
    DMA1_Channel3->CNDTR = (uint32)sizeof(sCom.RecvBuf); /* Number of data to transfer */
    DMA1_Channel3->CCR |= DMA_CCR3_EN; /* Channel enabled */
    
    if(USART3->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE)) /* Noise/Framing/Parity error is detected */
    {
    }
    else
    {
      sCom.fRecv = 1;
    }
    uint32 sr = USART3->SR; /* clear sr */
    uint32 dr = USART3->DR; /* clear dr */
    
    //    gps_re_flag = 1;
    nmeaPARSER parser;
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
    nmea_parse(&parser, (char*)sCom.RecvBuf, (int)strlen((char*)sCom.RecvBuf), &info);
    nmea_parser_destroy(&parser);
    GPSCal(&info, 1);
  }
}

void DMA1_Channel3_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TC3);
  DMA_ClearITPendingBit(DMA1_IT_TE3);
  DMA_Cmd(DMA1_Channel3, DISABLE); 
  DMA1_Channel3->CNDTR = 1024;
  DMA_Cmd(DMA1_Channel3, ENABLE);
}