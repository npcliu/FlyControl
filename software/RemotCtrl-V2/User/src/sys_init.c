#include "include.h"
#include "MyIIC.h"
#include "ILI9341.h"
#include "define.h"

void ParameterInit()
{
  //  basic = 12.4;
  //  x_p1 = 0.8;//0.43;
  //  x_h1 = 0.025;//0.011;
  //  x_i1 = 0.005;//0.01;
  //  x_p2 = 0.9;//1;
  //  x_i2 = 0;
  //  x_d2 = 0;
  //  
  //  y_p1 = 1;
  //  y_h1 = 0.030;
  //  y_i1 = 0.013;
  //  y_p2 = 0.9;
  //  y_i2 = 0;
  //  y_d2 = 0;
  //  
  //  z_p1 = 2;
  //  z_d1 = 0.05;
  //  offset_angle_x = 0;//5;
  //  offset_angle_y = 1;//5;
  //  offset_angle_z = 0;
  
}

//
///************************************************
//*  函数名称：SCISend_to_PIDDebug
//*  功能说明：SCI发送到串口示波器
//*  参数说明：UARTn为第几个串口
//*  函数返回：无
//0：红1：蓝2：浅蓝3：黄4：浅绿
//*  修改时间：2014-1-14    已经测试DLUT上位机默认数据为short格式;因此数据不要超过-32767~32767区间
//0：red;1=blue;2=light blue;3=yellow;4=light green;5=dark green
//*************************************************/
//char send_type = 0;
//void SCISend_to_Own(USART_TypeDef* USARTx)
//{
//  static short send_data[3][8] = { { 0 }, { 0 }, { 0 } };
//  int i = 0,j = 0;
//  switch(send_type)
//  {
//  case 'x':               //[x倾角，x倾角期望][y分量加速度][？]
////    send_data[0][0] = (short)ang_x;
////    send_data[0][1] = (short)((nrf_rciv[LR_ADC]-127)*-ANG_CTRL_RATE);
////    send_data[0][2] = (short)(x2);
////    send_data[1][0] = (short)Acc_ADC.x;
////    send_data[1][1] = (short)0;
////    send_data[2][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_X_WATCH];
////    send_data[2][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_X_WATCH];
//    break;
//  case 'X':               //[x倾角，x倾角期望][y角速度][？]
////    send_data[0][0] = (short)ang_x;
////    send_data[0][1] = (short)((nrf_rciv[LR_ADC]-127)*-ANG_CTRL_RATE);
////    send_data[1][0] = (short)0;
////    send_data[1][1] = (short)w_y;
////    send_data[2][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_X_WATCH];
////    send_data[2][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_X_WATCH];
//    break;
//  case 'y':               //[y倾角，y倾角期望][y分量加速度][？]
////    send_data[0][0] = (short)ang_y;
////    send_data[0][1] = (short)((nrf_rciv[UB_ADC]-127)*ANG_CTRL_RATE);
////    send_data[0][2] = (short)(-y2);
////    send_data[1][0] = (short)Acc_ADC.y;
////    send_data[1][1] = (short)0;
////    send_data[2][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_Y_WATCH];
////    send_data[2][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_Y_WATCH];
//    break;
//  case 'Y':               //[y倾角，y倾角期望][x角速度][？]
////    send_data[0][0] = (short)ang_y;
////    send_data[0][1] = (short)((nrf_rciv[UB_ADC]-127)*ANG_CTRL_RATE);
////    send_data[1][0] = (short)0;
////    send_data[1][1] = (short)w_x;
////    send_data[2][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_Y_WATCH];
////    send_data[2][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_Y_WATCH];
//    break;
//  case 'z':               //[偏航角][？][地磁计x,y,z]
////    send_data[0][0] = (short)ang_z;
////    send_data[0][1] = (short)0;
////    send_data[1][0] = (short)gc[DBG_TMP_ANG_WATCH][DBG_COMPASS_TMP_ANG_Z_WATCH];
////    send_data[1][1] = (short)gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_Z_WATCH];
////    send_data[2][0] = (short)gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_X_WATCH]/10;
////    send_data[2][1] = (short)gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_Y_WATCH]/10;
////    send_data[2][2] = (short)gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_Z_WATCH]/10;
////    send_data[2][3] = (short)gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_M_WATCH]/10;
//    break;
//  case 'Z':               //[偏航角][z轴角速度][偏航角参量]
////    send_data[0][0] = (short)ang_z;
////    send_data[0][1] = (short)0;
////    send_data[1][0] = (short)w_z;
////    send_data[1][1] = (short)0;
////    send_data[2][0] = (short)0;
//    send_data[2][1] = (short)0;
//    send_data[2][2] = (short)0;
//    break;
//  case 'p':               //[？][压力][0]
//    send_data[0][0] = (short)(0);
//    send_data[0][1] = (short)0;
//    send_data[1][0] = (short)(pressure-ctrl[0]);
//    send_data[1][1] = (short)0;
//    send_data[2][0] = (short)0;
//    send_data[2][1] = (short)0;
//    break;
//  case 'g':              //[GPS高度][速度][方向]
////    send_data[0][0] = (short)(0);
////    send_data[0][1] = (short)0;
////    send_data[1][0] = (short)(0);
////    send_data[1][1] = (short)0;
////    send_data[2][0] = (short)info.direction;
////    send_data[2][1] = (short)0;
//    break;
//  case 'G':              //[经纬][0][0]
////    send_data[0][0] = (short)(angx_err);
////    send_data[0][1] = (short)(angy_err);
////    send_data[1][0] = (short)(0);
////    send_data[1][1] = (short)(0);
////    send_data[2][0] = (short)ang_z;
////    send_data[2][1] = (short)0;
//    break;
//  case 13:              //[UTC时][分][秒]
////    send_data[0][0] = (short)(info.utc.hour);
////    send_data[0][1] = (short)(0);
////    send_data[1][0] = (short)(info.utc.min);
////    send_data[1][1] = (short)0;
////    send_data[2][0] = (short)(info.utc.sec);
////    send_data[2][1] = (short)0;
//    break;
//  default:break;
//  }
//  /*凌立印象上位机示波器协议*/
//  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
//  DelayUs(1);                                   //经测试，此处delay要加上，推测while完成到发送时间太短，usart来不及
//  USARTx->DR = 'S';
//  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
//  DelayUs(1);
//  USARTx->DR = 'T';
//  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
//  DelayUs(1);
//  for (i = 0; i < 3; i++)
//    for (j = 0; j < 8; j++)
//    {
//      USART_SendData(USART1,send_data[i][j]);
//      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
//      DelayUs(1);
//      USART_SendData(USART1,send_data[i][j]>>8);
//      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
//      DelayUs(1);
//    }
////    USARTx->DR = 'S';
////    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);      
////    DelayUs(1);
////    USARTx->DR = 'T';
//}
//void MemInit(void)              //创建链表
//{
////  p_acc_angx_list = CreatList(ACC_FILT_ORDER);
////  p_acc_angy_list = CreatList(ACC_FILT_ORDER);
////  pnow = p_acc_angx_list->p_next;
//}
//LED引脚初始化
void GpioInit(void)
{
  //  gpio_init(RED_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
//  gpio_init(KEY2_PIN,IPU,n_interupt,GPIO_Speed_50MHz,1);                        //设置上拉（内部100K电阻），有滤波电容需要一定时间充电，不能立即读引脚
 
  gpio_init(BLUE_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,0);
  //PB.5 ???
  //  gpio_init(KEY1_PIN,IPU,interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  //  gpio_int_cfg(KEY1_PIN,ENSW_IRQ_LINE,EXTI_Trigger_Falling);
  
  gpio_init(ENSW_PIN,IPU,interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_int_cfg(ENSW_PIN,ENSW_IRQ_LINE,EXTI_Trigger_Falling);
  
  gpio_init(ENA_PIN,IPU,interupt,GPIO_Speed_50MHz,1);
  gpio_int_cfg(ENA_PIN,ENA_IRQ_LINE,EXTI_Trigger_Falling);
  gpio_init(ENB_PIN,IPU,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  
  
  
  gpio_init(KEY2_PIN,IPU,interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_int_cfg(KEY2_PIN,KEY2_IRQ_LINE,EXTI_Trigger_Falling);
  
  gpio_init(KEY1_PIN,IPU,interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  gpio_int_cfg(KEY1_PIN,KEY1_IRQ_LINE,EXTI_Trigger_Falling);
  //  gpio_init(RGB_B_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,1);					 //PB.5 ???
  //  gpio_init(BUZZER_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  //  gpio_init(LIGHT_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
}

//#define BAT_VOL_FILT_LEN        8
float FiltedBatVoltage()
{
  static uint16 bat[BAT_VOL_FILT_LEN] = {0};
  static int pointer = 0;
  static float voltsum = 0;
  float filted_data = 0;
  
  BAT_SEN_EN_OUT = 1;
  
  if(pointer<BAT_VOL_FILT_LEN-1)
    pointer++;
  else
    pointer = 0;
  voltsum -= bat[pointer];
  bat[pointer] = GetAdc(BAT_ADC,BAT_ADC_CHANNEL);
  voltsum += bat[pointer];
  filted_data = voltsum/BAT_VOL_FILT_LEN;
  
  BAT_SEN_EN_OUT = 0;
  
  return filted_data/(BAT_ADC_MAX+1)*5.086666;   //电池电压经过主二极管有0.33v压降，分压电阻分别是10.0K和18.0K
}
uint32 PhotoRESVoltageGet()
{
  uint16 adc_val = 0;
  //  float prop_voltage = 0;       //按分压比推算得到的电压
  PHOTORES_EN_OUT = 0;
  DelayUs(50);                //等待一段时间，让模拟电路的电压稳定
  adc_val = GetAdc(PHOTORES_ADC,PHOTORES_ADC_CHANNEL);         //4095=3.3v
  PHOTORES_EN_OUT = 1;
  //  prop_voltage = adc_val/4096.0*3.32*(2.7+0.91)/0.91;   //实测3.28V
  //  0.2249x2 - 2.4531x + 12.999
  //  if(prop_voltage<=6)         //稳压二极管作用后实测拟合的公式(二次拟合，在8-15V之间较准确，)
  return adc_val;
  //  else
  //    return 0.2249*prop_voltage*prop_voltage - 2.4531*prop_voltage + 12.999;
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

void Adc0Init(void)             //battery voltage sense
{
  //  gpio_init(ADC_EN_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  //  gpio_init(ADC_PIN,AIN,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHZ
  ADC_InitTypeDef ADC_InitStructure;
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //不开启扫描 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //触发软件 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);  /* Enable ADC1 reset calibration register */   
  while(ADC_GetResetCalibrationStatus(ADC1));  /* Check the end of ADC1 reset calibration register */
  ADC_StartCalibration(ADC1);  /* Start ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

void Adc1Init(void)
{
  gpio_init(PHOTORES_EN_PIN,Out_OD,n_interupt,GPIO_Speed_50MHz,0);					 //PB.5 ???
  gpio_init(BAT_SEN_EN_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,0);					 //PB.5 ???
  gpio_init(TH_ADC_PIN,AIN,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(DR_ADC_PIN,AIN,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(UD_ADC_PIN,AIN,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  gpio_init(LR_ADC_PIN,AIN,n_interupt,GPIO_Speed_50MHz,0);						 //PB.5 ???
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);//12MHZ
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                    //独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                         //不开启扫描 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                   //单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //触发软件 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;                               //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);  /* Enable ADC1 reset calibration register */   
  while(ADC_GetResetCalibrationStatus(ADC1));  /* Check the end of ADC1 reset calibration register */
  ADC_StartCalibration(ADC1);  /* Start ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);       //使能ADC1的软件转换启动功能
}

u16 GetAdc(ADC_TypeDef* ADCx,u8 ch)
{  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADCx, ENABLE);  /* Start ADC1 Software Conversion */ 
  while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));  /* Test if the ADC1 EOC flag is set or not */ 
  return ADC_GetConversionValue(ADCx);   /*Returns the ADC1 Master data value of the last converted channel*/
}

//u8 sendbuf[1024];
//u8 receivebuf[1024];

void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = NRF_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}
u16 poitr_off_pan = 30/2;//pointer offset from pannel,箭标位置相对于参数盘位置的偏移
//显示调参界面所有的提示信息，包括返回光标等等，无记忆，需要参数指定用户在上一次调参退出时光标和参数的编号
//Parameters:
//  cursur:光标显示的位置，从上到下0~7;
//  selected_para:光标所指向的参数的序号，0~最大参数数-1;
//Return:
//  void;
//Example:
//  LCD_Show_Para(5, 8);//光标在屏幕从上到下第5+1个位置，并且正好指向8号参数
void LCD_Show_Para(int cursur, int selected_para)             //lcd显示触摸后的提示信息函数
{
  uint8 i = 0;
  uint16 sx= 1,ex = 29;
  LCD_Fill(0, 30, lcd.scrn_y-64, lcd.scrn_y, CYAN);                   //adjust area
  LCD_Fill(lcd.scrn_x-30,lcd.scrn_x, lcd.scrn_y-64, lcd.scrn_y, CYAN);           //return area
  int show_p = selected_para - cursur;
  if(show_p<0)
    show_p = 0;
  for(i=show_p;i<show_p+8;i++)
  {
    LCD_Fill(sx, ex, 0, 64, params[i].dis_ifo.pan.colr);                       //Sense
    LcdSS(sx+6,0,c1608,params[i].name, params[i].dis_ifo.pan_dat.data_colr, params[i].dis_ifo.pan.colr);
    sx+=30;
    ex+=30;
  }
  //  LcdSS(8,239-22,16," Save", lcd.pcolr);
  LcdSS(239-22,320-64+8,c1608,"Return", RED,CYAN);
  LcdSS(8,320-64+8,c1608,"Adjust", RED,CYAN);
  LCDDrawPointer(cursur*30+poitr_off_pan, 64+4,9, 3,WHITE);
  LcdSS(cursur*30+poitr_off_pan-c1206/2,64+4+20,c1206,params[selected_para].ins, lcd.pcolr,lcd.bcolr);
}

//update the wirless remote control link status on lcd,
//if the state not changed ,lcd will not be refreshed to save time except force_update=1
void UpdateRmtCtrlStatus(nrf_tx_state_e _tx_flag,uint8 force_update,u16 pcolor,u16 bcolor)
{
  static char last_tx_flag = 0;
  if(!force_update)
    if(last_tx_flag==_tx_flag || NRF_TXING==_tx_flag)     //if the status not changed or nrf is transmitting,no need to refresh lcd
      return ;
  
  LcdSS(lcd.scrn_x-c1206,160,c1206,"RF link:", YELLOW,bcolor);
  if(NRF_TX_OK==_tx_flag)
    LcdSS(lcd.scrn_x-c1206,160+c1206/2*strlen("RF link:"),c1206,"  Good  ", GREEN,bcolor);
  else if(NRF_TX_TIME_OUT==_tx_flag)
    LcdSS(lcd.scrn_x-c1206,160+c1206/2*strlen("RF link:"),c1206,"   Bad  ", RED,bcolor);
  else
    LcdSS(lcd.scrn_x-c1206,160+c1206/2*strlen("RF link:"),c1206,"Not know", RED,bcolor);
  last_tx_flag = _tx_flag;
}
//update the plane mode on lcd
void UpdatePlaneMod(const PLANE_MODE _mode,u16 pcolor,u16 bcolor)
{
  LcdSS(lcd.scrn_x-c1206,c1206/2,c1206, "Mode:", YELLOW,bcolor);
  
  switch(_mode)
  {
  case lock:
    {
      LcdSS(lcd.scrn_x-c1206,c1206/2*strlen("Mode:")+c1206/2,c1206, "Lock", RED,bcolor);
    }break;
  case fly:
    {
      LcdSS(lcd.scrn_x-c1206,c1206/2*strlen("Mode:")+c1206/2,c1206, "Hold Attitude", GREEN, bcolor);
    }break;
  case height_fix:
    {
      LcdSS(lcd.scrn_x-c1206,c1206/2*strlen("Mode:")+c1206/2,c1206, "Hold Height", GREEN, bcolor);
    }break;
  case experiment:
    {
       LcdSS(lcd.scrn_x-c1206,c1206/2*strlen("Mode:"),c1206, "experiment", GREEN, bcolor);
    }break;
  default:break;
  }
}
