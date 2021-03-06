#include "stm32f10x.h"
#include "define.h"
#include "Mpu6050.h"
#include "math.h"
#include "calculation.h"
#include "MyNRF24L0.h"
#include "info.h"

float angle[3] = {0};
float angle_error_x = 0;
//float last_angle_error_x = 0;
float angle_error_y = 0;                                //累加变量注意清nan
//float iangle_error_x = 0, integer_x1 = 0;               //累加变量注意清nan
//float iangle_error_y = 0, integer_y1 = 0;               //累加变量注意清nan

#define ACC_FILT_LENGTH     5                  //平均滤波长度
//Brief:calculate the calibrated data including acc&gyro and filted acc&gyro data from chip output data
//Parameters:
//  pfilted_acc:pointer of the filted acc data,pfilted_acc[0]should be the acc_x and so on;
//  pfilted_gyro:pointer of the filted gyro data;
//  pfilted_cps:pointer of the filted compass data;
//  pacc:pointer of the calibrated acc data;
//  pgyro:pointer of the calibrated gyro data;
//  pcps:pointer of the calibrated compass data;
//  acc_chip_data:acc data which is given by chip;
//  gyro_chip_data:gyro data which is given by chip;
//  pcps_chip_data:compass data which is given by chip;
//Return:void
//Example:
void CaliFilt(float *pfilted_acc,float *pfilted_gyro,float *pfilted_cps,const PACC pacc,const PGYRO pgyro,const PCOMPASS pcps,short* pacc_chip_data,short* pgyro_chip_data,short* pcps_chip_data)
{
  static short acc_filt_buff[3][ACC_FILT_LENGTH] = {0};             //1个加计
  static int pointer = 0;
  static int acc_sum[6] = {0},i=0;
  
  if(pointer<ACC_FILT_LENGTH-1)
    pointer++;
  else
    pointer = 0;
  
  pacc->x = (short)(pacc->kx*(pacc_chip_data[0] - pacc->x_off));
  pacc->y = (short)(pacc->ky*(pacc_chip_data[1] - pacc->y_off));
  pacc->z = (short)(pacc->kz*(pacc_chip_data[2] - pacc->z_off));
  pgyro->x = pgyro_chip_data[0] - pgyro->x_offset;                      //陀螺仪零偏纠正
  pgyro->y = pgyro_chip_data[1] - pgyro->y_offset;
  pgyro->z = pgyro_chip_data[2] - pgyro->z_offset;
  pcps->x = (short)(pcps->kx*(pcps_chip_data[0] - pcps->x_offset));
  pcps->y = (short)(pcps->ky*(pcps_chip_data[1] - pcps->y_offset));
  pcps->z = (short)(pcps->kz*(pcps_chip_data[2] - pcps->z_offset));
  
  for(i=0;i<3;i++)
  {
    acc_sum[i] -= acc_filt_buff[i][pointer];
    acc_filt_buff[i][pointer] = ((short*)pacc)[i];
    acc_sum[i] += acc_filt_buff[i][pointer];
  }
  pfilted_acc[0] = acc_sum[0]/ACC_FILT_LENGTH;
  pfilted_acc[1] = acc_sum[1]/ACC_FILT_LENGTH;
  pfilted_acc[2] = acc_sum[2]/ACC_FILT_LENGTH;
  pfilted_gyro[0] = pgyro->x;                           //gyro data not filted
  pfilted_gyro[1] = pgyro->y;
  pfilted_gyro[2] = pgyro->z;
  pfilted_cps[0] = pcps->x;                           //compass data not filted
  pfilted_cps[1] = pcps->y;
  pfilted_cps[2] = pcps->z;
}

//Brief:attitude calculation method,[ref.大计划四旋翼->静态关系]
//Parameters:
//  pangle:3 attitude angles in degree;
//  pacc:pointer of the acc data,it could be raw data or filted data;
//  pgyro:;
//  pcps:;
//  mod:飞行的形式，十字形0，X形=1；
//Return:void;
//Example:;
void AttCalc(float * pangle,float *pacc,float* pgyro,float *pcps, uint8 mod)
{//  木机架抖动太厉害，并且不是高斯分布的，有一种频率比较低的大幅度成分不知道来源估计是共振，如果能解决此“共振”则可用
  static float tmp_acc_angle[3] = {0};      //attitude angle calculated from accelerometer
  float temp_angle[3] = {0};         //acc_delay_angx:acc平均滤波有较大延迟的角度
  
  float csquar = pacc[2] * pacc[2];
  float sec_partx,sec_party;            //计算公式中的第二项
  if(mod)       //X形状，详见大计划
  {
    sec_partx = pacc[1] + pacc[0];
    sec_partx = sec_partx*sec_partx;
    sec_partx = sec_partx/2.0;
    sec_party = pacc[1] - pacc[0];
    sec_party = sec_party*sec_party;
    sec_party = sec_party/2.0;
  }
  else          //十字形，详见大计划
  {
    sec_partx = pacc[0]*pacc[0];
    sec_party = pacc[1]*pacc[1];
  }
  float tmp = sqrt(csquar + sec_partx);
  if(tmp<0.00001)
    return;
  float cos_tmp_ang_x = pacc[2]/tmp; //cos_delta/sqrt(cos_delta*cos_delta + sin_gama*sin_gama);
  if(cos_tmp_ang_x>1)
    cos_tmp_ang_x = 1;
  else if(cos_tmp_ang_x<-1)
    cos_tmp_ang_x = -1;
  
  if(mod)       //X形状，详见大计划
  {
    if(pacc[0]+pacc[1]>0)                         //cos映射到0~180；通过sin正负映射到0~-180
      temp_angle[0] = acos(cos_tmp_ang_x)/PI_DIV_180;
    else
      temp_angle[0] = -acos(cos_tmp_ang_x)/PI_DIV_180;           //加速度计反算的旋转倾斜角度
  }
  else
  {
    if(pacc[0]>0)                         //cos映射到0~180；通过sin正负映射到0~-180
      temp_angle[0] = acos(cos_tmp_ang_x)/PI_DIV_180;
    else
      temp_angle[0] = -acos(cos_tmp_ang_x)/PI_DIV_180;           //加速度计反算的旋转倾斜角度
  }
  acc_angle[0][0] = tmp_acc_angle[0] = temp_angle[0];
  
  tmp = sqrt(sec_party + csquar);
  if(tmp<0.00001)
    return;
  float cos_tmp_ang_y =  pacc[2]/tmp;//cos_delta/sqrt(cos_delta*cos_delta + sin_ykxr*sin_ykxr);
  if(cos_tmp_ang_y>1)
    cos_tmp_ang_y = 1;
  else if(cos_tmp_ang_y<-1)
    cos_tmp_ang_y = -1;
  if(mod)       //X形状，详见大计划
  {
    if(pacc[0]<pacc[1])                         //cos映射到0~180；通过sin正负映射到0~-180
      temp_angle[0] = acos(cos_tmp_ang_y)/PI_DIV_180;
    else
      temp_angle[0] = -acos(cos_tmp_ang_y)/PI_DIV_180;           //加速度计反算的旋转倾斜角度
  }
  else
  {
    if(pacc[1]>0)
      temp_angle[0] = acos(cos_tmp_ang_y)/PI_DIV_180;
    else
      temp_angle[0] = -acos(cos_tmp_ang_y)/PI_DIV_180;
  }
  acc_angle[0][1] = tmp_acc_angle[1] = temp_angle[0];  //  assert(!isnan(temp_angle[0]));
  
  
  /************************compass data process*****************************/
  
  //  static float z_angle[2] = {0};                        //编译时赋值
  
  //  float hz_divid_gz = (float)pcps->z/pacc[2];//注意：如果加速度计采用了长时延的滤波，而电子罗盘未滤波，则变化过程中由于加计延迟，角度会有较大偏差
  //  float index[2] = {0};
  //  index[0] = pcps->x - pacc[0]*hz_divid_gz;
  //  index[1] = pcps->y - pacc[1]*hz_divid_gz;
  //  float denum = sqrt(index[0]*index[0] + index[1]*index[1]);
  //  if(denum<0.00001)
  //    return;
  //  float tan_fai = index[0]/index[1];
  // gc[DBG_TMP_ANG_WATCH][DBG_COMPASS_TMP_ANG_Z_WATCH] = pacc[0]/10;
  //    p_acc_angle[2] = atan2(index[0],index[1])/PI_DIV_180;
  
  sqrt(pcps[0]*pcps[0] + pcps[1]*pcps[1] + pcps[2]*pcps[2]);
  //  gc[DBG_TMP_ANG_WATCH][DBG_COMPASS_TMP_ANG_Z_WATCH] = (z_angle[0] = atan2(pcps[0],pcps[1])/PI_DIV_180); //计算偏航角度
  angle[2] = angle[2] - (gyro.z)*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//pgyro->z
  //    p_acc_angle[2] = (z_angle[0]+99*temp_angle[1])*0.01;  //  计算x&y轴角度值ang_x & ang_y，单位：度
  //    if(nrf_rciv[TH_ADC_OFFSET]<2)
  //      p_acc_angle[2] = 0;
  //    p_acc_angle[2] = temp_angle[1];  //  计算x&y轴角度值ang_x & ang_y，单位：度
#ifdef WATCH_INTEGRAL_ANGLE             //
  static float gyro_angle[2] = {0};
  if(mod)
  {
    gyro_angle[0] = gyro_angle[0] + ((pgyro[0]-pgyro[1])*0.7071)*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;
    gyro_angle[1] = gyro_angle[1] + ((pgyro[1]+pgyro[0])*0.7071)*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;
  }
  else
  {
    gyro_angle[0] = gyro_angle[0] - (pgyro[1])*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//陀螺仪积分推算旋转倾角;
    gyro_angle[1] = gyro_angle[1] + (pgyro[0])*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//atan((float)(pacc->x)/pacc->z)/PI * 180.0 - Angle0;
  }
  gc[0][0] = gyro_angle[0];
  gc[0][1] = gyro_angle[1];
  if(nrf_rciv[TH_ADC_OFFSET]<=2)                        //油门小于2则认为没震动，角度就用加速度计解算值
  {
    gyro_angle[0] = tmp_acc_angle[0];
    gyro_angle[1] = tmp_acc_angle[1];
  }
#else
#endif
  if(mod)
  {
    temp_angle[0] = pangle[0] + ((pgyro[0]-pgyro[1])*0.7071)*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//陀螺仪积分推算旋转倾角;
    temp_angle[1] = pangle[1] + ((pgyro[1]+pgyro[0])*0.7071)*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//atan((float)(pacc->x)/pacc->z)/PI * 180.0 - Angle0;
  }
  else
  {
    temp_angle[0] = pangle[0] - (pgyro[1])*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//陀螺仪积分推算旋转倾角;
    temp_angle[1] = pangle[1] + (pgyro[0])*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//atan((float)(pacc->x)/pacc->z)/PI * 180.0 - Angle0;
  }
  
  if(nrf_rciv[TH_ADC_OFFSET]<=2)                        //油门小于2则认为没震动，角度就用加速度计解算值
  {
    pangle[0] = tmp_acc_angle[0];
    pangle[1] = tmp_acc_angle[1];
  }
  else
  {
    if(nrf_rciv[GPS_MODE_OFFSET])
    {
      pangle[0] = temp_angle[0] - angx_err;
      pangle[1] =(angy_err+49*temp_angle[1])*0.02;
    }
    else
    {
      pangle[0] = 0.001*tmp_acc_angle[0] + (1-0.001)*temp_angle[0];
      pangle[1] = 0.001*tmp_acc_angle[1] + (1-0.001)*temp_angle[1];//      else
    }
  }
  //  AttitudeEKF(true,false,update,5e-3,z,0,0,0,0,0,0,0, 0,xa_apo,Pa_apo,Rot_matrix,eulerAngle,debugOutput);
  
  if(nrf_rciv[TH_ADC_OFFSET]<2)
  {//offset angle and integerd direction angle must be cleand the same time  
    offset_angle[2] = 0;
    angle[2] = 0;
  }
}


/*******************************************PWM值计算*******************************************/
uint16 cali = 0;
float offset_angle[3] = {0};
float x_b = 0,y_b = 0;          //basic banlance param of 2 axis
float z_p = 0,z_d = 0;          //yaw control parameters
float x_p_o;                     //x(x axis)_p(proportion)_o(out loop)_t(ten shape fly)x轴十形或者X形飞行控制参数
float x_p_i;
float x_d_i;
float y_p_o;
float y_p_i;
float y_d_i;

int Coef = 20;                          //derivative Coefficient
int pwm[4] = {0};      //
float xcq = 0,ycq = 0;                          //X,Y angle Control quantity
float pwm_of_dir = 0;                           //yaw  Control quantity
//float ycq = 0;
//Brief:name-PWMCalc(Calculate),4个电机占空比计算函数
//  计算之PWM值存于pwm
//Parameters:
//  mod:飞行的形式，十字形0，X形=1；
//return:void
//Example:
//  PWMCalc(0);   //十字形飞行模式下4个电机的转速(PWM)计算
void PWMCalc(uint8 mod)
{
  static float dir_angle_error[2] = {0};
  static float throttle = 0;
  static float omega_e = 0;             //desier rotation speed
  static float _omega_error = 0;
#ifdef WATCH_DIFF_COEF
  static float last_omega_error_x,last_omega_error_y;
#else
#endif
  
  //用信号框图表示的话，这里写反了
  angle_error_x = angle[0]-(nrf_rciv[LR_ADC_OFFSET]-129)*ANG_CTRL_RATE + offset_angle[0]; //
  angle_error_y = (nrf_rciv[UD_ADC_OFFSET]-127)*ANG_CTRL_RATE-angle[1] + offset_angle[1];
  static float filter_coef_state_x = 0,filter_coef_state_y = 0;              //微分状态变量
  float deriv_out = 0;
  if(mod)//X形飞行
  {
    omega_e = x_p_o*angle_error_x;
    _omega_error = omega_e - (gyro.y-gyro.x)*0.7071;
    deriv_out = Coef*(x_d_i*_omega_error-filter_coef_state_x);//FilterCoefficient，不加滤波，也可以飞，但是电调会因为尖峰而响应失败，导致炸机
    xcq = x_p_i*_omega_error + deriv_out;
    filter_coef_state_x += deriv_out*INTERUPT_CYC_IN_MS*1e-3;
#ifdef WATCH_DIFF_COEF
  gc[1][0] = (_omega_error - last_omega_error_x)/INTERUPT_CYC_IN_MS*1e3;
  last_omega_error_x = _omega_error;
  gc[1][2] = deriv_out/x_d_i;
#else
#endif
    
    omega_e = y_p_o*angle_error_y;  
    _omega_error = omega_e - (gyro.x+gyro.y)*0.7071;
#ifdef WATCH_DIFF_COEF
  gc[1][1] = (_omega_error - last_omega_error_y)/INTERUPT_CYC_IN_MS*1e3;
  last_omega_error_y = _omega_error;
#else
#endif
    deriv_out = Coef*(y_d_i*_omega_error-filter_coef_state_y);//FilterCoefficient[0]
    ycq = y_p_i*_omega_error + deriv_out;
    filter_coef_state_y += deriv_out*INTERUPT_CYC_IN_MS*1e-3;
  }
  else//十形飞行
  {
//    if(ctrl[0]>-1 && ctrl[0]<1)
//        x1[0] = x_p_o*angle_error_x;
    omega_e = x_p_o*angle_error_x;
    _omega_error = omega_e - gyro.y;
    deriv_out = Coef*(x_d_i*_omega_error-filter_coef_state_x);//FilterCoefficient[0]
    xcq = x_b*sin(angle[0]*D2R) + x_p_i*_omega_error + deriv_out;
    filter_coef_state_x += deriv_out*INTERUPT_CYC_IN_MS*1e-3;
    
    omega_e = y_p_o*angle_error_y;  
    _omega_error = omega_e-gyro.x;
    deriv_out = Coef*(y_d_i*_omega_error-filter_coef_state_y);//FilterCoefficient[0]
    ycq = y_b*sin(angle[1]*D2R) + y_p_i*_omega_error + deriv_out;
    filter_coef_state_y += deriv_out*INTERUPT_CYC_IN_MS*1e-3;
  }
  dir_angle_error[0] = offset_angle[2] - angle[2];
  pwm_of_dir = z_p *dir_angle_error[0] + z_d*(gyro.z);

#ifdef MY_WOOD                  //my wood plane
  throttle = 1.0*nrf_rciv[TH_ADC_OFFSET]+500;      //1.3的比例操作非常灵敏,油门比例需要配合电调的油门行程
#elif CARBON_FIBRE              //other plane
  throttle = 1.2*nrf_rciv[TH_ADC_OFFSET]+500;      //
#endif
  if(mod)
  {
    //    pwm[x_n] = (int)(throttle + xcq);//
    //    pwm[x_p] = (int)(throttle - xcq);//
    //    pwm[y_n] = (int)(throttle + xcq);
    //    pwm[y_p] = (int)(throttle - xcq);
    //    pwm[x_n] = (int)(throttle - ycq);//
    //    pwm[x_p] = (int)(throttle + ycq);//
    //    pwm[y_n] = (int)(throttle + ycq);
    //    pwm[y_p] = (int)(throttle - ycq);
    //    pwm[x_n] = (int)(throttle + xcq - ycq);//
    //    pwm[x_p] = (int)(throttle - xcq + ycq);//
    //    pwm[y_n] = (int)(throttle + xcq + ycq);
    //    pwm[y_p] = (int)(throttle - xcq - ycq);
    pwm[x_n] = (int)(throttle + xcq + ycq - pwm_of_dir);//
    pwm[x_p] = (int)(throttle - xcq - ycq - pwm_of_dir);//
    pwm[y_n] = (int)(throttle + xcq - ycq + pwm_of_dir);
    pwm[y_p] = (int)(throttle - xcq + ycq + pwm_of_dir);
  }
  else
  {
    pwm[x_n] = (int)(throttle + xcq - pwm_of_dir);//
    pwm[x_p] = (int)(throttle - xcq - pwm_of_dir);//
    pwm[y_n] = (int)(throttle - ycq + pwm_of_dir);
    pwm[y_p] = (int)(throttle + ycq + pwm_of_dir);
  }
}

#define CALL_PERIOD_S     1       //GPSCal调用周期单位s
float angx_err = 0;
float angy_err = 0;
//info:GPS测量量
//th_vs_mg;推重比
void GPSCal(nmeaINFO *info, uint32 th_vs_mg)
{
  static float last_speed = 0, previous_dir = 0;            //上次测量的速度和以前！方向，测量特性：本次测量速度不为0，比较小，所以方向很难确定，这时GPS就会给出0值
  static float _acc = 0;
  float theta = 0,fi = 0;  //机体面倾角plane_inclination
  float sin_theta = 0,sin_fi = 0;
  
  //如果GPS方向給出0值就要特別關注了
  if(info->direction>-0.1 && info->direction<0.1 && info->speed>0.5 && last_speed>0.5)
    fi = previous_dir*PI_DIV_180;
  else                  //只要GPS给出了大于0的方向值，那么该值可信
  {
    fi = info->direction*PI_DIV_180;
    previous_dir = info->direction;
  }
  
  /************计算平面倾角*************/
  last_speed = (info->speed);
  sin_theta = _acc/(th_vs_mg*9.8);
  if(sin_theta>0.99)
    sin_theta = 0.99;
  theta = asin(sin_theta);         //in rad
  sin_fi = sin(fi);
  if(ctrl[3])
  {
    angy_err = atan(-sin_theta * cos(fi) / cos(theta))/PI_DIV_180;
    angx_err = -asin(sin_theta*sin_fi)/PI_DIV_180;
  }
  else
  {
    float cos_fi = cos(fi);
    float squre_sin_theta = sin_theta*sin_theta;
    float squre_cos_fi = cos_fi*cos_fi;
    angy_err = acos(cos(theta)*sqrt((1-squre_sin_theta*squre_cos_fi)/(squre_sin_theta*squre_sin_theta*squre_cos_fi-2*squre_sin_theta*squre_cos_fi+1))) / PI_DIV_180;
    angx_err = asin(sin_theta*cos(fi))/PI_DIV_180;
  }
}
