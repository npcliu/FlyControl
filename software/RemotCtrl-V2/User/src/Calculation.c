#include "include.h"

float pwm_of_dir = 0;
//float angle_error_x = 0;
//float last_angle_error_x = 0;
//float angle_error_y = 0;                                //�ۼӱ���ע����nan
//float iangle_error_x = 0, integer_x1 = 0;               //�ۼӱ���ע����nan
//float iangle_error_y = 0, integer_y1 = 0;               //�ۼӱ���ע����nan
//float x1[2] = {0}, x2 = 0;
//float y1[2] = {0}, y2 = 0;
//float acc_ang_buff[ACC_FILT_ORDER] = {0};
//PACC_ANG p_acc_angx_list = NULL;
//PACC_ANG p_acc_angy_list = NULL;
//PACC_ANG pnow = NULL;       

//void Filt()  // һ�׻�����lvbo
//{  //  ��Ϊ����̫���������Ҳ��Ǹ�˹�ֲ��ģ���һ��Ƶ�ʱȽϵ͵Ĵ���ȳɷֲ�֪����Դ�����ǹ�������ܽ���ˡ����������
//  static float z_angle[2] = {0};                        //����ʱ��ֵ
//  double compass_x = 0,compass_y = 0;      //ȥ����ƫ�������
//  float temp_angle[3] = {0};         //acc_delay_angx:accƽ���˲��нϴ��ӳٵĽǶ�
//
//  
//  w_y = Gyro_ADC.y - Gyro_ADC.y_offset;
//  w_x = Gyro_ADC.x - Gyro_ADC.x_offset;
//  w_z = Gyro_ADC.z - Gyro_ADC.z_offset;
//  
//  float csquar = Acc_ADC.z*Acc_ADC.z;
////  float acc_mold = 0;//  gc[DBG_ACC_COMPASS_WATCH][DBG_ACC_M_WATCH] = (acc_mold = sqrt(Acc_ADC.x*Acc_ADC.x+Acc_ADC.y*Acc_ADC.y+Acc_ADC.z*Acc_ADC.z))-2085;
////  if(0==acc_mold)
////    acc_mold = 1;//�п���acc_mold��0������0��������
////  
////  float cos_delta = Acc_ADC.z/acc_mold;
////  float sin_gama = Acc_ADC.x/acc_mold;
//  float cos_tmp_ang_x = Acc_ADC.z/sqrt(Acc_ADC.x*Acc_ADC.x + csquar);//cos_delta/sqrt(cos_delta*cos_delta + sin_gama*sin_gama);
//  if(Acc_ADC.x>0)        //cosӳ�䵽0~180��ͨ��sin����ӳ�䵽0~-180
//    gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_X_WATCH] = temp_angle[0] = acos(cos_tmp_ang_x)/PI_DIV_180;
//  else
//    gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_X_WATCH] = temp_angle[0] = -acos(cos_tmp_ang_x)/PI_DIV_180;
//  gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_X_WATCH] = temp_angle[1] = ang_x - w_y/16.4 * INTERUPT_CYC_IN_MS/1000.0;//atan((float)(Acc_ADC.x)/Acc_ADC.z)/PI * 180.0 - Angle0;
//  if(nrf_rciv[TH_ADC]<=0)
//    ang_x = temp_angle[0];
//  else
//  {
//    if(nrf_rciv[GPS_MODE])
//      ang_x = temp_angle[1]-angx_err;
//    else
//      ang_x = temp_angle[1];
//  }
//  //    ang_x = (temp_angle[0]+49*temp_angle[1])*0.02;   //�����˲�
//  
////  float sin_ykxr = Acc_ADC.y/acc_mold;
//  float cos_tmp_ang_y =  Acc_ADC.z/sqrt(Acc_ADC.y*Acc_ADC.y + csquar);//cos_delta/sqrt(cos_delta*cos_delta + sin_ykxr*sin_ykxr);
//  if(Acc_ADC.y>0)
//    gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_Y_WATCH] = temp_angle[0] = acos(cos_tmp_ang_y)/PI_DIV_180;
//  else
//    gc[DBG_TMP_ANG_WATCH][DBG_ACC_TMP_ANG_Y_WATCH] = temp_angle[0] = -acos(cos_tmp_ang_y)/PI_DIV_180;
//  gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_Y_WATCH] = temp_angle[1] = ang_y + w_x/16.4 * INTERUPT_CYC_IN_MS/1000.0;//atan((float)(Acc_ADC.x)/Acc_ADC.z)/PI * 180.0 - Angle0;
//  assert(!isnan(temp_angle[0]));
//  if(nrf_rciv[TH_ADC]<=0)
//    ang_y = temp_angle[0];
//  else
//  {
//    if(nrf_rciv[GPS_MODE])
//      ang_y =(angy_err+49*temp_angle[1])*0.02;
//    else
//      ang_y = temp_angle[1];
//  }
//  //      ang_y = (temp_angle[0]+49*temp_angle[1])*0.02;
//  
//  /************************compass data process*****************************/
//  float compass_z = 0;
//  gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_X_WATCH] = compass_x = Compass.x - Compass.x_offset;
//  gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_Y_WATCH] = compass_y = Compass.y - Compass.y_offset;
//  gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_Z_WATCH] = compass_z = Compass.z - Compass.z_offset;
//  gc[DBG_ACC_COMPASS_WATCH][DBG_COMPASS_M_WATCH] = sqrt(compass_x*compass_x + compass_y*compass_y + compass_z*compass_z);
//  gc[DBG_TMP_ANG_WATCH][DBG_COMPASS_TMP_ANG_Z_WATCH] = z_angle[0] = atan2(compass_x,compass_y)/PI_DIV_180; //����ƫ���Ƕ�
//  gc[DBG_TMP_ANG_WATCH][DBG_GYRO_TMP_ANG_Z_WATCH] = temp_angle[1] = ang_z + w_z/16.4*INTERUPT_CYC_IN_MS/1000.0;
//  ang_z = (z_angle[0]+99*temp_angle[1])*0.01;  //  ����x&y��Ƕ�ֵang_x & ang_y����λ����
//}
//
///*******************************************PWMֵ���㺯��*******************************************/
//uint16 cali = 0;
//void PWM_Calculation(void)
//{
//  static float dir_angle_error[2] = {0};
//  static uint8 time1 = 0;
//  ++time1;
//  
//  angle_error_x = ang_x+(nrf_rciv[LR_ADC]-127)*ANG_CTRL_RATE; //ƫת45������
//  iangle_error_x += angle_error_x;
//  if(iangle_error_x>500)
//    iangle_error_x = 500;
//  else if(iangle_error_x<-500)
//    iangle_error_x = -500;
//  else if(isnan(iangle_error_x))
//    iangle_error_x = 0;
//  
//  x1[0] = basic*sin(ang_x/180.0*PI) + x_p1*angle_error_x - x_h1*w_y + x_i1*iangle_error_x; //- x_a2*alfa_y;
//  integer_x1 += x1[0];
//  if(integer_x1>1000)
//    integer_x1 = 1000;
//  else if(integer_x1<-1000)
//    integer_x1 = -1000;
//  else if(isnan(integer_x1))
//    integer_x1 = 0;
//  gc[DBG_PWM_WATCH][DBG_PWM_X_WATCH] = x2 = x_p2*x1[0] + x_d2*(x1[0]-x1[1]) + x_i2*integer_x1;
//  
//  angle_error_y = (nrf_rciv[UB_ADC]-127)*ANG_CTRL_RATE - ang_y + offset_angle_y;
//  iangle_error_y += angle_error_y;
//  if(iangle_error_y>500)
//    iangle_error_y = 500;
//  else if(iangle_error_y<-500)
//    iangle_error_y = -500;
//  else if(isnan(iangle_error_y))
//    iangle_error_y = 0;
//  y1[0] = y_p1*angle_error_y - y_h1*w_x + y_i1*iangle_error_y;// - y_d2*alfa_x;
//  integer_y1 += y1[0];
//  if(integer_y1>1000)
//    integer_y1 = 1000;
//  else if(integer_y1<-1000)
//    integer_y1 = -1000;
//  else if(isnan(integer_y1))
//    integer_y1 = 0;
//  gc[DBG_PWM_WATCH][DBG_PWM_Y_WATCH] = y2 = y_p2*y1[0] + y_d2*(y1[0]-y1[1]) + y_i2*integer_y1;
//  
//  dir_angle_error[0] = ang_z-offset_angle_z;
//  pwm_of_dir = z_p1 *dir_angle_error[0] + z_d1*(w_z);
//  dir_angle_error[1] = dir_angle_error[0];
//  if('f'==nrf_rciv[PLANE_MODE])              //��������ģʽ
//  {
//    if(nrf_rciv[TH_ADC]>1)
//    {
//      TIM_SetCompare1(TIM2,(int)(1.5*nrf_rciv[TH_ADC]+500 + (int)y2 - pwm_of_dir));  //X1
//      TIM_SetCompare2(TIM2,(int)(1.5*nrf_rciv[TH_ADC]+500 - (int)y2 - pwm_of_dir));  //X2
//      TIM_SetCompare3(TIM2,(int)(1.5*nrf_rciv[TH_ADC]+500 + (int)x2 + pwm_of_dir));  //Y1
//      TIM_SetCompare4(TIM2,(int)(1.5*nrf_rciv[TH_ADC]+500 - (int)x2 + pwm_of_dir));  //Y2
//    }    else
//    {
//      TIM_SetCompare1(TIM2,STOP_PWM);      
//      TIM_SetCompare2(TIM2,STOP_PWM);
//      TIM_SetCompare3(TIM2,STOP_PWM);
//      TIM_SetCompare4(TIM2,STOP_PWM);
//    }
//  }
//  else
//  {
//    TIM_SetCompare1(TIM2,cali);      
//    TIM_SetCompare2(TIM2,cali);
//    TIM_SetCompare3(TIM2,cali);
//    TIM_SetCompare4(TIM2,cali);
//  }
//  
//  last_angle_error_x = angle_error_x;
//  if(time1>=2)
//  {
//    x1[1] = x1[0];
//    y1[1] = y1[0];
//    time1 = 0;
//  }
//}
//
//#define CALL_PERIOD_S     1       //GPSCal�������ڵ�λs
//float angx_err = 0;
//float angy_err = 0;
////info:GPS������
////th_vs_mg;���ر�
//void GPSCal(nmeaINFO *info, uint32 th_vs_mg)
//{
//  static float last_speed = 0, previous_dir = 0;            //�ϴβ������ٶȺ���ǰ�����򣬲������ԣ����β����ٶȲ�Ϊ0���Ƚ�С�����Է������ȷ������ʱGPS�ͻ����0ֵ
//  static float acc = 0;
//  float theta = 0,fi = 0;  //���������plane_inclination
//  float sin_theta = 0,sin_fi = 0;
//  
//  float ang_delta = info->direction-previous_dir;
//  
//  //���GPS����o��0ֵ��Ҫ�؄e�Pע��
//  if(info->direction>-0.1 && info->direction<0.1 && info->speed>0.5 && last_speed>0.5)
//    fi = previous_dir*PI_DIV_180;
//  else                  //ֻҪGPS�����˴���0�ķ���ֵ����ô��ֵ����
//  {
//    fi = info->direction*PI_DIV_180;
//    previous_dir = info->direction;
//  }
//  
//  /************����ƽ�����*************/
////  acc = (info->speed - last_speed)/CALL_PERIOD_S/3.6;
//    acc = ctrl[1];//(info->speed - last_speed)/CALL_PERIOD/3.6;
//  last_speed = (info->speed);
//  sin_theta = acc/(th_vs_mg*9.8);
//  if(sin_theta>0.99)
//    sin_theta = 0.99;
//  theta = asin(sin_theta);         //in rad
//  if(ctrl[2]<360)
//    ctrl[2]++;
//  else
//    ctrl[2] = 0;
//    fi = ctrl[2]*PI_DIV_180;//info->declination*PI_DIV_180;
//  sin_fi = sin(fi);
//  if(ctrl[3])
//  {
//    angy_err = atan(-sin_theta * cos(fi) / cos(theta))/PI_DIV_180;
//  angx_err = -asin(sin_theta*sin_fi)/PI_DIV_180;
//  }
//  else
//  {
//    float cos_fi = cos(fi);
//    float squre_sin_theta = sin_theta*sin_theta;
//    float squre_cos_fi = cos_fi*cos_fi;
//    angy_err = acos(cos(theta)*sqrt((1-squre_sin_theta*squre_cos_fi)/(squre_sin_theta*squre_sin_theta*squre_cos_fi-2*squre_sin_theta*squre_cos_fi+1))) / PI_DIV_180;
//  angx_err = asin(sin_theta*cos(fi))/PI_DIV_180;
//  }
//}
////len:������
//PACC_ANG CreatList(uint32 len)
//{
//  int i = 0;
//  //	int len = 0;
//  //	int val = 0;
//  PACC_ANG pLast;
//  PACC_ANG pNow;
//  
//  PACC_ANG pHead = (PACC_ANG)malloc(sizeof(ACC_ANG));
//  if(NULL==pHead)
//  {
//    //		printf("�ڴ����ʧ��!");
//    exit(-1);
//  }
//  pLast = pHead;
//  pLast->p_next = NULL;
//  
//  if(len>=0)
//  {
//    for(i=0; i<len; ++i)
//    {
//      pNow = (PACC_ANG)malloc(sizeof(ACC_ANG));
//      if(NULL==pNow)
//      {
//        //				printf("�ڴ����ʧ��!");
//        exit(-1);
//      }
//      //			printf("�������%d���ڵ��ֵ:\n", i+1);
//      //			scanf("%d", &val);
//      pNow->data = 0;
//      pNow->p_next = NULL;
//      pLast->p_next = pNow;
//      pLast = pNow;
//    }
//  }
//  else;
//  pLast->p_next = pHead->p_next;        //ѭ������
//  
//  return pHead;
//}
