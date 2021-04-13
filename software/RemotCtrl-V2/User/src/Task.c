#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "stm32f10x.h"
#include "math.h"
#include "define.h"
#include "delay.h"
#include "Mpu6050.h"
#include "MyNRF24L0.h"
#include "ISR.h"                //time flag
#include "Task.h"
//#include "interact.h"
#include "RGB888.h"
#include "lcd.h"
#include "sys_init.h"

PROCEDURE next_procedure = stb_pre;
PROCEDURE (* Task[MAX_TASK_NUM])(char input) = NULL;

//extern int nrf_out_time;
uint16 th_adc = 0;
uint16 dr_adc = 0;
uint16 ud_adc = 0;
uint16 lr_adc = 0;
PSCAN_CRV_INFO p_thcrv;
char first_cycl = 1;//��һ�ν�ʵ��ģʽ�ı�־λ
char unlock_flag = 0;//ʵ��ģʽ�µĽ�����־λ
uint8 start_expriment_flag = 0x0;//ʵ��ģʽ�µĿ�ʼʵ���־

PROCEDURE StbPrep(char input)
{
  lcd.bcolr = BLACK;
  Gpio8080OptLcdClr(lcd.bcolr);
  EXTIEnable(ENSW_IRQ_LINE);             //enable to use encoder
  EXTIEnable(ENA_IRQ_LINE);             //enable to use encoder
  EXTIEnable(KEY2_IRQ_LINE);
  irq_tx_buff[PLANE_MODE_OFFSET] = 'l';    //�����л�������ģʽ
  irq_tx_buff[TH_ADC_OFFSET] = 0;
  irq_tx_buff[DR_ADC_OFFSET] = 0;
  irq_tx_buff[UD_ADC_OFFSET] = 0;
  irq_tx_buff[LR_ADC_OFFSET] = 0;
  
  plane_mod = lock;
  u16 last_color = lcd.pcolr;
  lcd.pcolr = PINK;
  printf("Shut down Remote control board(RCB):");
  lcd.pcolr = last_color;
  printf("Press K1 buttom for seconds;\n");
  
  lcd.pcolr = PINK;
  printf("Unlock Plane:");
  lcd.pcolr = last_color;
  printf("Keep throttle at 0 and turn it ");  lcd.pcolr = YELLOW;  printf("Right;\n");
  lcd.pcolr = PINK;
  printf("Lock Plane:");
  lcd.pcolr = last_color;
  printf("Keep throttle at 0 and turn it ");  lcd.pcolr = YELLOW;  printf("Left;\n");
  
  lcd.pcolr = PINK;
  printf("Hand choice:");
  lcd.pcolr = last_color;
  printf("Trun on RCB will enter right hand mode by default(throttle stick is on right hand),press KEY1 buttom when restart will enter left hand mode,just trun around RCB and have fun;\n");
  
  lcd.pcolr = PINK;
  printf("Note:");
  lcd.pcolr = last_color;
  printf("This RCB only take effect in less than several hundred meters,plane will shut down Motor when did't receive signal for 1 second,Good Luck!\n");
  
  lcd.pcolr = PINK;
  printf("Set parameters:");
  lcd.pcolr = last_color;
  printf("Hit KEY1 will enter set parameters mode;\n");
  lcd.pcolr = last_color;
    LCDShowVar(&bty_votg_lcd_info,bat_voltage);

//  UpdateRmtCtrlStatus(nrf_tx_sta,1,GREEN,lcd.bcolr);
//  UpdatePlaneMod(plane_mod,GREEN,lcd.bcolr);
//  LCDShowVar(&bty_votg_lcd_info,bat_voltage,2,"V");
  
//    if(NULL==(p_thcrv = (PSCAN_CRV_INFO)malloc(sizeof(SCAN_CRV_INFO))))
//    {
//        printf("allocate memory failed!\n");
//      exit(0);
//    }
//    else
//    {
//      p_thcrv->crv_ifo.pan.sx = 45;             //������ʾ��Χ��ʼx
//      p_thcrv->crv_ifo.pan.sy = 150;              //������ʾ��Χ��ʼy
//      p_thcrv->crv_ifo.pan.x_len=100;           //����x����ĳ��ȣ������߽�
//      p_thcrv->crv_ifo.pan.y_len = 168;
//      p_thcrv->crv_ifo.pan.colr=WHITE;          //���̵���ɫ
//      p_thcrv->crv_ifo.pan.iffil = false;       //�����Ƿ����
//      
//      
//      p_thcrv->crv_ifo.min = 5;                 //������ʾ��Χ������
//      p_thcrv->crv_ifo.min_label.data_colr = GREEN;
//      p_thcrv->crv_ifo.min_label.last_len = 0;
//      p_thcrv->crv_ifo.min_label.size = c1206;
//      p_thcrv->crv_ifo.min_label.acr_desm = 0;
//      p_thcrv->crv_ifo.min_label.unit[0] = '\0';        //�޵�λ
//      p_thcrv->crv_ifo.min_label.unit_colr = RED;
//      p_thcrv->crv_ifo.min_label.x = p_thcrv->crv_ifo.pan.sx+p_thcrv->crv_ifo.pan.x_len-(p_thcrv->crv_ifo.pan.x_len>0)+(p_thcrv->crv_ifo.pan.x_len<0)-p_thcrv->crv_ifo.min_label.size;//-1�Ǳ߽磬-size���ַ�λ����Ա߽��ƫ��
//      p_thcrv->crv_ifo.min_label.y = p_thcrv->crv_ifo.pan.sy+p_thcrv->crv_ifo.min_label.size/2+1;
//      
//      p_thcrv->crv_ifo.max = 250;
//      p_thcrv->crv_ifo.max_label.data_colr = GREEN;
//      p_thcrv->crv_ifo.max_label.last_len = 0;
//      p_thcrv->crv_ifo.max_label.size = c1206;
//      p_thcrv->crv_ifo.max_label.acr_desm = 0;
//      p_thcrv->crv_ifo.max_label.unit[0] = '\0';
//      p_thcrv->crv_ifo.max_label.unit_colr = RED;
//      p_thcrv->crv_ifo.max_label.x = p_thcrv->crv_ifo.pan.sx+1;//
//      p_thcrv->crv_ifo.max_label.y = p_thcrv->crv_ifo.pan.sy+3*p_thcrv->crv_ifo.min_label.size/2+1;
//      
//      p_thcrv->crv_ifo.axis_colr = ORANGE;      //����ϵ����ɫ
//      p_thcrv->crv_ifo.crvcolr = YELLOW;        //���ߵ���ɫ
//      p_thcrv->crv_ifo.ifslc = true;            //if slice�Ƿ���ֱ��������������
//      p_thcrv->pos = p_thcrv->crv_ifo.pan.sy+1; //��ʼ��ʾλ��
//      p_thcrv->sdatli = CratSList(abs(p_thcrv->crv_ifo.pan.y_len)-2,true);   //�洢��һ֡�������ݵ�����
//      if(NULL==p_thcrv->sdatli)
//      {
//        printf("allocate memory failed!\n");
//        free(p_thcrv);
//        exit(0);
//      }
//      p_thcrv->last_n = p_thcrv->sdatli->pNext; //�����������е���ʼ�洢λ��
//    }
  return standby;
}
uint32 timetstcount;
uint32 endval[5];
uint32 photo_res_voltage = 0;          //���������ѹ

PROCEDURE Standby(char input)           //standby procedure
{
  if(rlud==lcd.scan_dir)
  {
    th_adc = TH_ADC_MAX - GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
    dr_adc = DR_ADC_MAX - GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
    ud_adc = UD_ADC_MAX - GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
    lr_adc = LR_ADC_MAX - GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  }
  else if(lrdu==lcd.scan_dir)
  {
    th_adc = GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
    dr_adc = GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
    ud_adc = GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
    lr_adc = GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  }
  else;
  
  if(th_adc>TH_ADC_MAX)               //��λ��
    th_adc = TH_ADC_MAX;
  if(dr_adc>DR_ADC_MAX)
    dr_adc = DR_ADC_MAX;
  if(ud_adc>UD_ADC_MAX)
    ud_adc = UD_ADC_MAX;
  if(lr_adc>LR_ADC_MAX)
    lr_adc = LR_ADC_MAX;
  if(pit_50ms_flag)
  {
    //  if(nrf_tx_sta!=NRF_TXING)
//    if(S1_IN)
//      UpdatePlaneMod(lock,GREEN,BLACK);
//    //      plane_mod = height_fix;
//    else
//      UpdatePlaneMod(height_fix,GREEN,BLACK);
//    //      plane_mod = fly;
//    ScanShowF(p_thcrv,(u16)(((float)th_adc/TH_ADC_MAX)*255));
  DrawThLine(&th_lcd_info,(u16)(((float)th_adc/TH_ADC_MAX)*(lcd.scrn_x-24)),1);
  
//  short tmp = (char)((dr_adc>>4)+128);
//  DrawThLine(&dr_lcd_info,tmp,2);
//  tmp = (char)((ud_adc>>4)+128);
//  DrawThLine(&ud_lcd_info,tmp>>2,1);
//  tmp = (char)((lr_adc>>4)+128);
//  DrawThLine(&lr_lcd_info,-(tmp>>2),0);
    if(encoder)
    {
      //      Roll(encoder);
      encoder = 0;
    }
    UpdateRmtCtrlStatus(nrf_tx_sta,1,GREEN,BLACK);
    UpdatePlaneMod(plane_mod,ORANGE,BLACK);
    pit_50ms_flag = 0;
  }
  
  irq_tx_buff[TH_ADC_OFFSET] = th_adc>>4;       //�൱�ڳ���16�����ö�Ӧ�����255�����ǲ�������4λ��ֵ��������4096d=1000h����4=100������uint8=00��=256
  irq_tx_buff[DR_ADC_OFFSET] = dr_adc>>4;
  irq_tx_buff[UD_ADC_OFFSET] = ud_adc>>4;
  irq_tx_buff[LR_ADC_OFFSET] = lr_adc>>4;
  
  static uint32 tstcount = 0;
  if(confirm_flag)
  {
    tstcount++;
    irq_tx_buff[DR_ADC_OFFSET] = tstcount;
    nrf_tx(irq_tx_buff,DATA_PACKET);//DATA_PACKET
    LCDShowVar(&nrf_send_times,tstcount);
    confirm_flag = 0;
  }
  if(pit_500ms_flag)
  {
    nrf_tx(irq_tx_buff,DATA_PACKET);//DATA_PACKET
    pit_500ms_flag = 0;
  }
  photo_res_voltage = PhotoRESVoltageGet();
  
  if(pit_5s_flag)
  {
    bat_voltage = FiltedBatVoltage();
    LCDShowVar(&bty_votg_lcd_info,bat_voltage);
    
    pit_5s_flag = 0;
  }
  
   //�������׼��ģʽʱ�����б�־λ���³�ʼ��
  first_cycl = 1;
  unlock_flag = 0;
  if(irq_tx_buff[BACKUP1_OFFSET] != 'z')//irq_tx_buff[BACKUP1_OFFSET]Ĭ������µ���t������˵irq_tx_buff[BACKUP1_OFFSET]����ʼ��Ϊt
    irq_tx_buff[BACKUP1_OFFSET] = 't'; 
  if(ud_adc<510 && lr_adc>(3200))//������������ʵ��ģʽ�ɼ�������
  {
      irq_tx_buff[BACKUP1_OFFSET] = 'z'; 
      LcdSS(120,10,c1206, "Send the data", YELLOW,BLACK);
  }
  else
  {
     LcdSS(120,10,c1206, "                           ", BLACK,BLACK);
  }
//  static int i = 0;
  //          irq_tx_buff[5] = GPS_mode;
  if(set_param_flag)
  {
//    FreeSList(p_thcrv->sdatli);
//    free(p_thcrv);
    return set_para_pre;
  }
  else if(th_adc<2 && dr_adc<(1000/4096.0*4096.0))
  {
    if(NRF_TX_OK != nrf_tx_sta)
    {
      printf("RF link is bad,unlock denyd;\n");
      DelayMs(500);
      return standby;
    }
    else if(S2_IN)
    {
      printf("please press SW2 to left;\n");
      DelayMs(500);
      return standby;      
    }
//    FreeSList(p_thcrv->sdatli);
//    free(p_thcrv);
    plane_mod = fly;
    return att_pre;
  }
  else if(plane_mod == experiment){
    if(NRF_TX_OK != nrf_tx_sta)
    {
      printf("RF link is bad,unlock denyd;\n");
      DelayMs(500);
      plane_mod = lock;
      return standby;   
    }
    return experiment_pro;//����ʵ��ģʽ
  }
  else
    return standby;
}

PROCEDURE AttHldPrep(char input)
{
  EXTIDisable(ENSW_IRQ_LINE);
  EXTIDisable(ENA_IRQ_LINE);
  EXTIDisable(KEY2_IRQ_LINE);
  lcd.bcolr = RGB(0,0,35);
  Gpio8080OptLcdClr(lcd.bcolr);
  printf("Clear to take-off!\n");
  
  //  LcdSS(0,0,c2412,1,"Clear to take-off!", YELLOW,BLACK);
  //    UpdatePlaneMod(plane_mod);
  
  return att_hld;
}

PROCEDURE AttHld(char input)
{
  if(rlud==lcd.scan_dir)
  {
    th_adc = TH_ADC_MAX - GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
    dr_adc = DR_ADC_MAX - GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
    ud_adc = UD_ADC_MAX - GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
    lr_adc = LR_ADC_MAX - GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  }
  else if(lrdu==lcd.scan_dir)
  {
    th_adc = GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
    dr_adc = GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
    ud_adc = GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
    lr_adc = GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  }
  else
    return stb_pre;
  
  if(th_adc>TH_ADC_MAX)               //��λ��
    th_adc = TH_ADC_MAX;
  if(dr_adc>DR_ADC_MAX)
    dr_adc = DR_ADC_MAX;
  if(ud_adc>UD_ADC_MAX)
    ud_adc = UD_ADC_MAX;
  if(lr_adc>LR_ADC_MAX)
    lr_adc = LR_ADC_MAX;
  
  if(pit_50ms_flag)
  {
//    //  if(nrf_tx_sta!=NRF_TXING)
//    if(S1_IN)
//    {
      plane_mod = fly;
      irq_tx_buff[PLANE_MODE_OFFSET] = 'f';         //�÷���ģʽ
//    }
//    else
//    {
//      plane_mod = height_fix;
//      irq_tx_buff[PLANE_MODE_OFFSET] = 'h';         //�÷���ģʽ
//    }
//    
    pit_50ms_flag = 0;
  }
  
  irq_tx_buff[TH_ADC_OFFSET] = th_adc>>4;       //16λת����8λ
  irq_tx_buff[DR_ADC_OFFSET] = dr_adc>>4;
  irq_tx_buff[UD_ADC_OFFSET] = ud_adc>>4;
  irq_tx_buff[LR_ADC_OFFSET] = lr_adc>>4;
  
  nrf_tx(irq_tx_buff,DATA_PACKET);      //DATA_PACKET
  UpdateRmtCtrlStatus(nrf_tx_sta,1,GREEN,lcd.bcolr);
  UpdatePlaneMod(plane_mod,GREEN,lcd.bcolr);

  DelayMs(2);
  if(th_adc<2 && dr_adc>(2700/4096.0*4096.0))
  {
    plane_mod = lock;
    return stb_pre;
  }
  else if(S2_IN)//���뿪�����Ҵ򣬽��붨��ģʽ
  {
    if(NRF_TX_OK != nrf_tx_sta)
    {
      printf("RF link is bad,unlock denyd;\n");
      DelayMs(500);
      plane_mod = lock;
      return standby;   
    }
//    FreeSList(p_thcrv->sdatli);
//    free(p_thcrv);
    plane_mod = height_fix;
    return hei_hld_pre;    
  }
  else
    return att_hld;
}

PROCEDURE HeiHldPre(char input)
{
  EXTIDisable(ENSW_IRQ_LINE);
  EXTIDisable(ENA_IRQ_LINE);
  EXTIDisable(KEY2_IRQ_LINE);
  lcd.bcolr = RGB(0,0,35);
  Gpio8080OptLcdClr(lcd.bcolr);
  printf("height hold\n");
  
  
  //  LcdSS(0,0,c2412,1,"Clear to take-off!", YELLOW,BLACK);
  //    UpdatePlaneMod(plane_mod);
  
  return hei_hld;
}
PROCEDURE HeiHld(char input)
{
  if(rlud==lcd.scan_dir)
  {
    th_adc = TH_ADC_MAX - GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
    dr_adc = DR_ADC_MAX - GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
    ud_adc = UD_ADC_MAX - GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
    lr_adc = LR_ADC_MAX - GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  }
  else if(lrdu==lcd.scan_dir)
  {
    th_adc = GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
    dr_adc = GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
    ud_adc = GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
    lr_adc = GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  }
  else
    return stb_pre;
  
  if(th_adc>TH_ADC_MAX)               //��λ��
    th_adc = TH_ADC_MAX;
  if(dr_adc>DR_ADC_MAX)
    dr_adc = DR_ADC_MAX;
  if(ud_adc>UD_ADC_MAX)
    ud_adc = UD_ADC_MAX;
  if(lr_adc>LR_ADC_MAX)
    lr_adc = LR_ADC_MAX;
  
  if(pit_50ms_flag)
  {
//    //  if(nrf_tx_sta!=NRF_TXING)
//    if(S1_IN)
//    {
      plane_mod = height_fix;
      irq_tx_buff[PLANE_MODE_OFFSET] = 'h';         //�÷���ģʽ
//    }
//    else
//    {
//      plane_mod = height_fix;
//      irq_tx_buff[PLANE_MODE_OFFSET] = 'h';         //�÷���ģʽ
//    }
    
    pit_50ms_flag = 0;
  }
  extern nrf_mode_e  nrf_mode;
  if(nrf_mode == TX_MODE)
  {
//    pit_20ms_flag = 0;
    
    irq_tx_buff[TH_ADC_OFFSET] = th_adc>>4;       //16λת����8λ
    irq_tx_buff[DR_ADC_OFFSET] = dr_adc>>4;
    irq_tx_buff[UD_ADC_OFFSET] = ud_adc>>4;
    irq_tx_buff[LR_ADC_OFFSET] = lr_adc>>4;
    
    nrf_tx(irq_tx_buff,DATA_PACKET);      //DATA_PACKET
    
  }

  UpdateRmtCtrlStatus(nrf_tx_sta,1,GREEN,lcd.bcolr);
  UpdatePlaneMod(plane_mod,GREEN,lcd.bcolr);

  DelayMs(2);
  if(th_adc<2 && dr_adc>(2700/4096.0*4096.0))
  {
    plane_mod = lock;
    return stb_pre;
  }
  else if(!S2_IN)
  {
    plane_mod = fly;
    return att_pre;    
  }
  else
    return hei_hld;  
}
char waite_for_nrf_data = 0;
static int last_cursor = 3,cursor = 3;        //��Ļ��ʾ���λ�ã��ڼ�λ�������ڲ����ǹ�����ʾ������½�����Ļ��������Ͳ����½������ǲ����Ϲ�
static short parameter_pointer = 3,last_parameter_pointer = 3;//parameter_pointer�ǲ�������ţ��ж��ٸ��������ж��ٸ����
uint16 norm_lr_adc,norm_ud_adc;//normal adc value when don't touch the stick

//  static short ud_adc_derta[2] = {0};
PROCEDURE SetParaPre(char input)           //standby procedure
{
  lcd.bcolr = BLACK;
  Gpio8080OptLcdClr(lcd.bcolr);
  irq_tx_buff[PLANE_MODE_OFFSET] = 'p';    //�����л�������ģʽ
  nrf_tx(irq_tx_buff,DATA_PACKET);//DATA_PACKET
//  CratSList(10,0);
//        if(NULL==p_thcrv->sdatli)
//      {
//        printf("allocate memory failed!\n");
//        free(p_thcrv);
//        exit(0);
//      }
DelayMs(10);            //�ȴ��ɻ�����������ģʽ
/**********���Ͷ�ȡ�ɻ�������ָ��*************/
  printf("Getting param from plane...\n");
  int i;
  for(i=0;i<PARAMETER_NUM;++i)
  {
    strcpy((char*)irq_tx_buff,"FEParamGet");             //����ĸ������ܺ�ģʽ���ַ��غ�
    strcat((char*)irq_tx_buff,params[i].name);             //Ҫ��ȡ�Ĳ���������
    waite_for_nrf_data = 1;
    nrf_tx(irq_tx_buff,DATA_PACKET);//DATA_PACKET
    DelayMs(20);
    nrf_rx_mode();
    while(waite_for_nrf_data);            //�˴���ü�ʱ
    //�Ǳ߷������������ͣ����������ݣ��±߽���
    //  Լ��char=0��uint8=1;short=2;uint16=3;int=4,uint32=5,float=6,double=7��
    switch(nrf_rciv[0])
    {
    case 0:
      {
        params[i].value = *((char*)(nrf_rciv+1));
      }break;
    case 1:
      {
        params[i].value = *((uint8*)(nrf_rciv+1));
      }break;
    case 2:
      {
        params[i].value = *((short*)(nrf_rciv+1));
      }break;
    case 3:
      {
        params[i].value = *((uint16*)(nrf_rciv+1));
      }break;
    case 4:
      {
        params[i].value = *((int*)(nrf_rciv+1));
      }break;
    case 5:
      {
        params[i].value = *((uint32*)(nrf_rciv+1));
      }break;
    case 6:
      {
        params[i].value = *((float*)(nrf_rciv+1));
      }break;
    case 7:
      {
        params[i].value = *((double*)(nrf_rciv+1));
      }break;
    default:break;
    }
    printf("%s=%.2f\n",params[i].name,params[i].value);
  }
  
  DelayMs(1000);
  LCD_Show_Para(last_cursor,last_parameter_pointer);             //lcd��ʾ���������ʾ��Ϣ����
  irq_tx_buff[PLANE_MODE_OFFSET] = 'p';    //�����л�������ģʽ
  return set_para;
}
//LCD_VAR_INFO tmp_var = {.x=228,.y=94,.last_len=0,.size=c1206,.data_colr=YELLOW,.unit_colr=BLUE};
//LCD_VAR_INFO count_var = {.x=120,.y=200,.last_len=0,.size=c2412,.data_colr=YELLOW,.unit_colr=BLUE};
PROCEDURE SetParam(char input)           //standby procedure
{
//  static int count;
//  static int _250ms_flag = 0;
  static short show_p = 0;
//  short _ud_adc_derta = 0;
  uint8 i = 0;
  uint16 sx= 1,ex = 29;
  th_adc = GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
  dr_adc = GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
  ud_adc = GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
  lr_adc = GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
  if(pit_50ms_flag)
  {
//    pit_50ms_flag = 0;
//    LCDShowVar(&tmp_var,ud_adc-norm_ud_adc,0,"");
//    LCDShowVar(&count_var,count,0,"");
//    if(ud_adc>(norm_ud_adc+ctrl[2]) || ud_adc<(norm_ud_adc-ctrl[2]))
//    {
//      if(0==count)
//        _250ms_flag = (ud_adc>norm_ud_adc)-(ud_adc<norm_ud_adc);
//      if(count < ctrl[3]+10)
//        count += ((fabs(ud_adc-norm_ud_adc)-ctrl[2])/(ctrl[0]+10));
//      else
//        count = 0;
//    }
//    else if(ud_adc>(norm_ud_adc-ctrl[1]) && ud_adc<(norm_ud_adc+ctrl[1]))
//      count = 0;
  }
  
  if(confirm_flag)
  { }
  else if(encoder)//encoder
  {
    if(rlud==lcd.scan_dir)
      cursor += ((encoder>0)-(encoder<0));
    else if(lrdu==lcd.scan_dir)
      cursor -= ((encoder>0)-(encoder<0));
    if(cursor<0 || cursor>7)
    {
      if(cursor<0)
        show_p --;
      else
        show_p ++;
      if(show_p<0)
        show_p = 0;
      else if(show_p>PARAMETER_NUM-8)
        show_p = PARAMETER_NUM - 8;
      for(i=show_p;i<show_p+8;i++)              //���������б�
      {
        LCD_Fill(sx, ex, 0, 64, params[i].dis_ifo.pan.colr);                       //Sense
        LcdSS(sx+6,8,c1608,params[i].name, params[i].dis_ifo.pan_dat.data_colr, params[i].dis_ifo.pan.colr);
        sx+=30;
        ex+=30;
      }
    }
    else
    {
      LCDDrawPointer(last_cursor*30+poitr_off_pan,64+4, 9, 3,lcd.bcolr);
      LCDDrawPointer(cursor*30+poitr_off_pan, 64+4, 9, 3,WHITE);
    }
    if(cursor<0)
      cursor=0;
    else if(cursor>7)
      cursor = 7;
    parameter_pointer = show_p + cursor;
    LcdSS(last_cursor*30+poitr_off_pan-c1206/2,64+4+20,c1206,params[last_parameter_pointer].ins, lcd.bcolr,lcd.bcolr);
    LcdSS(cursor*30+poitr_off_pan-c1206/2,64+4+20,c1206,params[parameter_pointer].ins, lcd.pcolr,lcd.bcolr);
    encoder = 0;
    last_parameter_pointer = parameter_pointer;
    last_cursor = cursor;
  }
  
  if(set_param_flag)
    return set_para;
  else
    return stb_pre;
}
DATA_ON_LCD thr_data_on_lcd = {.x=120, .y=250, .last_length = 0, .size = c1206, .data_color = YELLOW, .unit_color = BLACK};
extern int expriment_time;
PROCEDURE ExperimentProcedure(char input){//ʵ��ģʽ

  irq_tx_buff[PLANE_MODE_OFFSET] = 'e';         //�ɻ�����ʵ��ģʽ
  if(first_cycl)//��һ�ν���˺���ʱ��start_expriment_flag��־λ���㣬�����´ν����ģʽֱ�ӿ�ʼʵ��
  {
    first_cycl = 0;
    start_expriment_flag = 0;
    
    lcd.bcolr = BLACK;
    Gpio8080OptLcdClr(lcd.bcolr);
  }
    if(pit_50ms_flag)
    {
        
        if(rlud==lcd.scan_dir)
        {
          th_adc = TH_ADC_MAX - GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
          dr_adc = DR_ADC_MAX - GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
          ud_adc = UD_ADC_MAX - GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
          lr_adc = LR_ADC_MAX - GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
        }
        else if(lrdu==lcd.scan_dir)
        {
          th_adc = GetAdc(TH_TO_LR_ADC,TH_ADC_CHANNEL);
          dr_adc = GetAdc(TH_TO_LR_ADC,DR_ADC_CHANNEL);
          ud_adc = GetAdc(TH_TO_LR_ADC,UD_ADC_CHANNEL);
          lr_adc = GetAdc(TH_TO_LR_ADC,LR_ADC_CHANNEL);
        }
        else;
        
        if(th_adc>TH_ADC_MAX)               //��λ��
          th_adc = TH_ADC_MAX;
        if(dr_adc>DR_ADC_MAX)
          dr_adc = DR_ADC_MAX;
        if(ud_adc>UD_ADC_MAX)
          ud_adc = UD_ADC_MAX;
        if(lr_adc>LR_ADC_MAX)
          lr_adc = LR_ADC_MAX;
        
         if(th_adc<2 && dr_adc>(2700/4096.0*4096.0))//����������
        {
            plane_mod = lock;
            return stb_pre;
        }
        if(th_adc<2 && dr_adc<(1000/4096.0*4096.0))//���������
        {
          unlock_flag = 1;
        }
        if(unlock_flag == 1)
        {
          LcdSS(lcd.scrn_x-c1206,c1206/2*strlen("Mode:experiment "),c1206, " Unlocked", GREEN, BLACK);
          if(start_expriment_flag)//��ʼʵ��
          {
              
              irq_tx_buff[BACKUP1_OFFSET] = 'k'; 
              LcdSS(100,10,c1206, "Start the experiment", YELLOW,BLACK);
              if(expriment_time>1010)
                start_expriment_flag = 0;
          }
          else{
            
            irq_tx_buff[BACKUP1_OFFSET] = 't'; 
            
            irq_tx_buff[TH_ADC_OFFSET] = th_adc>>4;       //16λת����8λ
            irq_tx_buff[DR_ADC_OFFSET] = dr_adc>>4;
            irq_tx_buff[UD_ADC_OFFSET] = ud_adc>>4;
            irq_tx_buff[LR_ADC_OFFSET] = lr_adc>>4;
            LcdSS(100,10,c1206, "Stop the experiment", YELLOW,BLACK);
          }
        }
        else 
          LcdSS(lcd.scrn_x-c1206,c1206/2*strlen("Mode:experiment "),c1206, "   locked", GREEN, BLACK);
      pit_50ms_flag = 0;
    }

      nrf_tx(irq_tx_buff,DATA_PACKET);      //DATA_PACKET

  
      UpdateRmtCtrlStatus(nrf_tx_sta,1,GREEN,BLACK);
      UpdatePlaneMod(plane_mod,GREEN,BLACK);
    if(plane_mod == lock){
  
        return stb_pre;
      }
      return experiment_pro;
}

PROCEDURE TaskInit(PROCEDURE (*_Task[MAX_TASK_NUM])(char))   //����ָ������
{
  assert(stb_pre>=0 && stb_pre<MAX_TASK_NUM);
  _Task[stb_pre] = StbPrep;
  assert(standby>=0 && standby<MAX_TASK_NUM);
  _Task[standby] = Standby;
  assert(att_pre>=0 && att_pre<MAX_TASK_NUM);
  _Task[att_pre] = AttHldPrep;
  assert(att_hld>=0 && att_hld<MAX_TASK_NUM);
  _Task[att_hld] = AttHld;
  assert(set_para_pre>=0 && set_para_pre<MAX_TASK_NUM);
  _Task[set_para_pre] = SetParaPre;
  assert(set_para>=0 && set_para<MAX_TASK_NUM);
  _Task[set_para] = SetParam;
  assert(experiment_pro>=0 && experiment_pro<MAX_TASK_NUM);
  _Task[experiment_pro] = ExperimentProcedure;  
  assert(hei_hld_pre>=0 && hei_hld_pre<MAX_TASK_NUM);
  _Task[hei_hld_pre] = HeiHldPre; 
  assert(hei_hld>=0 && hei_hld<MAX_TASK_NUM);
  _Task[hei_hld] = HeiHld;   
  
    return stb_pre;
}