#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "define.h"
#include "sys_init.h"
#include "Task.h"
#include "MyNRF24L0.h"
#include "RGB888.h"
//#include "iar_dlmalloc.h"

PLANE_MODE plane_mod = lock;
PARAMETER params[PARAMETER_NUM] = {        //�û�������lLCD���޸ĵĲ���
  //    lcd����ʾ�Ĳ���������ֵ��С��������ʾ��Ϣ��������ʾ��ɫ=RED���ò����̵ı�����ɫ=YELLOW
  {.name="xpo",.value=0.5,.ins="Attitude out loop P of x aix;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = YELLOW}, //x���⻷��P������
  {.name="xpi",.value=0.5,.ins="Attitude inner loop P of x aix;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = LIGHTYELLOW},
  {.name="xdi",.value=0.5,.ins="Attitude inner loop D of x aix;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = OLIVE},
  {.name="ypo",.value=0.5,.ins="Attitude out loop P of y aix;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = GHOSTWHITE},
  {.name="ypi",.value=0.5,.ins="Attitude inner loop P of y aix;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = PLUM},
  {.name="ydi",.value=0.5,.ins="Attitude inner loop D of y aix;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = DARKORCHID},
  {.name="Bx",.value=0.5,.ins="Basic balance of X axis;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = VIOLET},
  {.name="By",.value=0.5,.ins="Basic balance of Y axis;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = CYAN},
  {.name="N",.value=0.5,.ins="Derivative coefficient;",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = MAGENTA},
  {.name="Ind",.value=0.5,.ins="Complementary filter index",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = LIGHTSLATEGRAY}
//  {.name="10xx",.value=0.5,.ins="this is 10",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = LIGHTBLUE},
//  {.name="11xx",.value=0.5,.ins="this is 11",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = MAGENTA},
//  {.name="12Pz",.value=0.5,.ins="this is 12",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = STEELBLUE},
//  {.name="13Iz",.value=0.5,.ins="this is 13",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = POWDERBLUE},
//  {.name="14Dz",.value=0.5,.ins="this is 14",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = AZURE},
//  {.name="15Py",.value=0.5,.ins="this is 15",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = LIGHTGREEN},
//  {.name="16Py",.value=0.5,.ins="this is 16",.dis_ifo.pan_dat.data_colr = RED,.dis_ifo.pan.colr = DARKCYAN}
};
float bat_voltage;                      //��ص�ѹ
volatile char pit_5ms_flag = 0;         //5ms��ʱ��־���ñ�־λ��ʱ��1��
char pit_50ms_flag = 0;
char pit_500ms_flag = 0;                //5sʱ���־
char pit_5s_flag = 0;                   //5sʱ���־
float ctrl[5] = {0};                    //��ʱ���Ʊ������ѻ���������
char GPS_mode = 0;
uint8 set_param_flag = 0x0;             //set parameter flag������ݸñ�־�ж��Ƿ����������ó���
uint8 confirm_flag = 0x0;
//char nrf_link_state = 'B';              //NFR���ߵ�״̬
//char right_hand_flag = 1;       //flag true,right hand throttle
//FPARAM bat_votg;

LCD_RCTG_INFO th_lcd_info;        //������ʾ����LCD�ϵ���ʾ��Ϣ
LCD_RCTG_INFO lr_lcd_info;
LCD_RCTG_INFO ud_lcd_info;
LCD_RCTG_INFO dr_lcd_info;
LCD_VAR_INFO bty_votg_lcd_info = {.unit = "V"};    //battery voltage show information like show bit��unit��position on the screen..
LCD_VAR_INFO nrf_send_times = {.unit = ""};

/***************************  main  *****************************/
int main(void)
{
  __disable_irq();
  delay_init();
  next_procedure = TaskInit(Task);
  gpio_init(PC11,Out_PP,n_interupt,GPIO_Speed_50MHz,0);
  gpio_init(PA8,Out_PP,n_interupt,GPIO_Speed_50MHz,0);
    gpio_init(S1_PIN,IPU,n_interupt,GPIO_Speed_50MHz,0);
  GpioInit();
  DelayMs(50);
  if(KEY2_IN)                   //���ݰ���״̬��ʼ��������ģʽ
    lcd.scan_dir = rlud;        //��������
  else
    lcd.scan_dir = lrdu;        //��������
  lcd.bcolr = BLACK;            //LCDĬ�ϱ���ɫ��printf��ʹ��
  LCD_Init(lcd.scan_dir,(u16*)&lcd.scrn_x,(u16*)&lcd.scrn_y,lcd.pcolr,lcd.bcolr);
  LcdParamInit();
  
  LcdSS(20,10,c1206,"WEL", lcd.pcolr,lcd.bcolr);
  LcdSS(200,50,c1608,"WELLCOME!", lcd.pcolr,lcd.bcolr);
  LcdSS(200,100,c2412,"WELLCOME!", lcd.pcolr,lcd.bcolr);
  DelayMs(100);
  Adc1Init();
  //  ParameterInit();
  th_lcd_info.colr = YELLOW;                //������ʾ����Ļ�ϵ���ʾλ�ü���С�ṹ��
  th_lcd_info.x_len = 0;
  th_lcd_info.y_len = 1;                           //����������ʾ���
  th_lcd_info.sx = lcd.scrn_x-10;
  if(rlud==lcd.scan_dir)
    th_lcd_info.sy = lcd.scrn_y-1;
  else if(lrdu==lcd.scan_dir)
    th_lcd_info.sy = 0;
  
  dr_lcd_info.colr = YELLOW;                //������ʾ����Ļ�ϵ���ʾλ�ü���С�ṹ��
  dr_lcd_info.x_len = 1;
  dr_lcd_info.y_len = 0;                           //����������ʾ���
  dr_lcd_info.sx = lcd.scrn_x-c1206-5;
    dr_lcd_info.sy = lcd.scrn_y/2;
  
    u16 center[2] = {150,280};
  ud_lcd_info.colr = YELLOW;                //������ʾ����Ļ�ϵ���ʾλ�ü���С�ṹ��
  ud_lcd_info.x_len = 0;
  ud_lcd_info.y_len = 2;                           //����������ʾ���
  ud_lcd_info.sx = center[0];
    ud_lcd_info.sy = center[1];
  lr_lcd_info.colr = YELLOW;                //������ʾ����Ļ�ϵ���ʾλ�ü���С�ṹ��
  lr_lcd_info.x_len = 2;
  lr_lcd_info.y_len = 0;                           //����������ʾ���
  lr_lcd_info.sx = center[0];
    lr_lcd_info.sy = center[1]+1;
    
    
  bty_votg_lcd_info.x=0;                        //��ص�ѹ����Ļ�ϵ���ʾλ�ü���С�ṹ��
  bty_votg_lcd_info.y=lcd.scrn_y - 7;
  bty_votg_lcd_info.acr_desm = 2;
//  bty_votg_lcd_info.unit = "V";
  bty_votg_lcd_info.size=c1206;
  bty_votg_lcd_info.data_colr=YELLOW;
  bty_votg_lcd_info.unit_colr = GREEN;
  int i = 0;
  for(i=0;i<BAT_VOL_FILT_LEN+2;i++)               //���˲��������ȳ�������Ϊ�и��������̣���ѹ��׼ȷ����ʾҪ����
    bat_voltage = FiltedBatVoltage();
  
  nrf_send_times.x = 217;
  nrf_send_times.y = 172;
  nrf_send_times.acr_desm = 0;
  nrf_send_times.size = c1206;
  nrf_send_times.data_colr = YELLOW;
  nrf_send_times.unit_colr = GREEN;
  
  PIT4_Init(INTERUPT_CYC_IN_MS);
  printf("Initial NRF...");
  if(nrf_init(TX_MODE))                         //��ʼ��NRF24L01+
  {
    printf("NRF Initial failed,Reinit 2 seconds later;\n");
    DelayMs(2000);
  }
  LCDDrawPointer(100,40, 5,0,YELLOW);
  LCDDrawPointer(101,39, 5,1,YELLOW);
  LCDDrawPointer(102,40, 5,2,YELLOW);
  LCDDrawPointer(101,41, 5,3,YELLOW);

  NVIC_Config();
  Gpio8080OptLcdClr(lcd.bcolr);
  LcdDrawLine(10,0,0,1,YELLOW);
  LcdDrawLine(239,0,100,2,YELLOW);

  UpdatePlaneMod(plane_mod,GREEN,BLACK);
  UpdateRmtCtrlStatus(nrf_tx_sta,1,GREEN,BLACK);
  BLUE_OUT = 1;
  __enable_irq();
  
  while(1)
  {
    next_procedure = Task[next_procedure](1);
  }
}
//int main(void)
//{
//  __disable_irq();
//  delay_init();
//  gpio_init(PC13,Out_OD,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
//  SPI1_Init();
//  while(!nrf_init(TX_MODE))                  //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ
//  {
//    BLUE_OUT = 0;    DelayMs(404);    BLUE_OUT = 1;    DelayMs(404);
//  }
//  NVIC_Config();
//    __enable_irq();
//  
//  nrf_tx("hello",3);//DATA_PACKET
//  DelayMs(1000);
//  while(1)
//  {
//    nrf_tx("hello",3);//DATA_PACKET
//    BLUE_OUT = 0;
//    DelayMs(500);
//    BLUE_OUT = 1;
//    DelayMs(500);
////    DelayMs(1000);
//  }
//}