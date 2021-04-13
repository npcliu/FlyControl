#include "stm32f10x.h"
#include "define.h"
#include "Mpu6050.h"
#include "math.h"
#include "calculation.h"
#include "MyNRF24L0.h"
#include "info.h"
#include "filter.h"
float change_th_flag = 0;//油门改变标志位
float yaw_init = 0;//初始偏航角，起飞前记录一下机头与正北方向的夹角,起飞以后要减去这个偏置角
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
//
void AttCalc(float * pangle,float *pacc,float* pgyro,float *pcps, uint8 mod)
{//  木机架抖动太厉害，并且不是高斯分布的，有一种频率比较低的大幅度成分不知道来源估计是共振，如果能解决此“共振”则可用
  static float tmp_acc_angle[3] = {0};      //attitude angle calculated from accelerometer
  float a = pacc[0], b = pacc[1], c = pacc[2], f = pcps[0], h = pcps[1], m = pcps[2];//加负号是为了和加速度计在一个坐标系
  float temp_angle[3] = {0};         //acc_delay_angx:acc平均滤波有较大延迟的角度
  
  float csquar = c * c;//c^2
  float asquar,bsquar;//a^2,b^2            //计算公式中的第二项
  if(mod)       //X形状，详见大计划
  {
    asquar = b + a;
    asquar = asquar*asquar;
    asquar = asquar/2.0;
    bsquar = b - a;
    bsquar = bsquar*bsquar;
    bsquar = bsquar/2.0;
  }
  else          //十字形，详见大计划
  {
    asquar = a*a;
    bsquar = b*b;
  }
  float asquar_plus_csquar = asquar + csquar;//a^2 + b^2
  float abs_d = sqrt(asquar_plus_csquar);//向量d的模
  if(abs_d<0.00001)
    return;
  float cos_alpha = c/abs_d; //y轴旋转角的绝对值
  if(cos_alpha>1)
    cos_alpha = 1;
  else if(cos_alpha<-1)
    cos_alpha = -1;
  
  if(mod)       //X形状，详见大计划
  {
    if(a+b>0)                         //cos映射到0~180；通过sin正负映射到0~-180
      tmp_acc_angle[0] = acos(cos_alpha)*R2D;
    else
    {
      tmp_acc_angle[0] = -acos(cos_alpha)*R2D;           //加速度计反算的旋转倾斜角度

    }
  }
  else
  {
    if(a>0)                         //cos映射到0~180；通过sin正负映射到0~-180
      tmp_acc_angle[0] = acos(cos_alpha)*R2D;
    
    else
    {
      tmp_acc_angle[0] = -acos(cos_alpha)*R2D;           //加速度计反算的旋转倾斜角度

    }
  }
  acc_angle[0][0] = tmp_acc_angle[0];
  
  
  float abs_e = sqrt(asquar*bsquar + asquar_plus_csquar*asquar_plus_csquar + bsquar*csquar);//向量e的模
  if(abs_e<0.00001)
    return;
  float cos_beta =  asquar_plus_csquar/abs_e;//绕x轴旋转角的余弦的绝对值
  if(cos_beta>1)
    cos_beta = 1;
  else if(cos_beta<-1)
    cos_beta = -1;
  if(mod)       //X形状，详见大计划
  {
    if(a<b)                         //cos映射到0~180；通过sin正负映射到0~-180
      tmp_acc_angle[1] = acos(cos_beta)*R2D;
    else
    {
      tmp_acc_angle[1] = -acos(cos_beta)*R2D;           //加速度计反算的旋转倾斜角度
    
    }
  }
  else
  {
    if(b>0)
      tmp_acc_angle[1] = acos(cos_beta)*R2D;
    else
    {
      
      tmp_acc_angle[1] = -acos(cos_beta)*R2D;
    
    }
  }
  acc_angle[0][1] = tmp_acc_angle[1];  //  assert(!isnan(temp_angle[0]));
  
  /************************compass data process*****************************/
  float gamma = 0;
  static float last_gamma = 0;
  
  
  float sin_alpha = sin(pangle[0]*D2R);
  float sin_beta = sin(pangle[1]*D2R);
  cos_alpha = cos(pangle[0]*D2R);
  cos_beta = cos(pangle[1]*D2R);

  float earth_magnetic_in_d = f * cos_alpha - m * sin_alpha;//地球磁场的在水平面x轴（也就是向量d）上的分量
  
  float earth_magnetic_in_e = h * cos_beta - (f * sin_alpha + m * cos_alpha) * sin_beta;//地球磁场在水平面y轴（也就是向量e）上的分量

  if(earth_magnetic_in_e>=0)
  {
    if(earth_magnetic_in_d>0)//0到90度
      gamma = R2D*atan(earth_magnetic_in_e / earth_magnetic_in_d);//从上往下看，逆时针转，数据为0到90度，转换为实际角度
    else//90到180度
      gamma = 180 + R2D*atan(earth_magnetic_in_e / earth_magnetic_in_d);//数据为-90到0度，转换为实际角度
  }
  else
  {
    if(earth_magnetic_in_d>0)//270到360度
      gamma = 360 + R2D*atan(earth_magnetic_in_e / earth_magnetic_in_d);//数据为-90到0度，转换为实际角度
    else//180到270度    
      gamma = 180 + R2D*atan(earth_magnetic_in_e / earth_magnetic_in_d);//数据为0到90度，转换为实际角度
  }
  acc_angle[0][2] = gamma;// - yaw_init;    
   
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
  //偏航角互补滤波
  temp_angle[2] = pangle[2] - (gyro.z)*MPU6050GYRO_SCALE_DEG * INTERUPT_CYC_IN_MS/1000.0;//z轴陀螺仪积分得到另一种偏航角
  
  if(nrf_rciv[TH_ADC_OFFSET]<3)                        //油门小于2则认为没震动，角度就用加速度计和地磁计解算值
  {
    pangle[0] = tmp_acc_angle[0];
    pangle[1] = tmp_acc_angle[1];
    //offset angle and integerd direction angle must be cleand the same time  
    
    yaw_init = gamma;
    pangle[2] = yaw_init;
    offset_angle[2] = yaw_init;
  }
  else
  {
//    if(nrf_rciv[GPS_MODE_OFFSET])
//    {
//      pangle[0] = temp_angle[0] - angx_err;
//      pangle[1] =(angy_err+49*temp_angle[1])*0.02;
//    }
      pangle[0] = 0.0015*tmp_acc_angle[0] + (1-0.0015)*temp_angle[0];
      pangle[1] = 0.0015*tmp_acc_angle[1] + (1-0.0015)*temp_angle[1];//      else
      //偏航角互补滤波
      if(gamma-last_gamma<345&&(gamma-last_gamma>-345))
        pangle[2] = 0.0015*gamma + (1-0.0015)*temp_angle[2];
      else
        pangle[2] = gamma;
      last_gamma = gamma;
  }
}

  float height_acc_p = 40;
  float height_acc_i = 0;
  float height_acc_d = 8;
//高度控制
float AltitudeControl(float * pacc)
{
  //printf("%f,%f,%f\r\n",_ohm,_a2,_b0);


   float height_acc = sqrt(0.0000234256*pacc[0]*pacc[0]+0.0000238144*pacc[1]*pacc[1]+0.0000228484*pacc[2]*pacc[2]) - 9.82;//运动时高度轴方向上的加速度，所以减去重力加速度
   //printf("%f\r\n",height_acc);
   gc[3][1] = height_acc;
   float filted_height_acc = 0;
       
   static float last_height_acc = 0, last_filted_height_acc = 0;
   
   float height_acc_err = 0;
   float height_diff_acc_err = 0,z_pid_out = 0;
   static float last_height_acc_err = 0, sum_of_heoght_acc_err = 0;
   
   float filted_height_acc_pid = 0;
   static float last_height_acc_pid = 0;
   static float last_filted_height_acc_pid = 0;
   float z_pid_out1 = 0;

   static LPFParam LPFParam_of_height_acc;//高度方向上加速度的滤波器参数，里面某些成员需要静态变量

   extern KalmanInfo kalmanFilter_of_height_acc;
  if(nrf_rciv[TH_ADC_OFFSET]>20)
  {
    //LPFParam_of_height_acc.fd=4;//滤波器截止频率
    //LPFParam_of_height_acc.input=height_acc;
    //filted_height_acc=SecondOrderLPF(&LPFParam_of_height_acc);
    
    filted_height_acc = KalmanFilter(&kalmanFilter_of_height_acc, height_acc);
    //    //把加速度过一个一阶低通滤波器，其采样频率为200HZ，截止频率为1HZ
    //filted_height_acc = (19.73 * (height_acc + last_height_acc) + 1236.27 * last_filted_height_acc) / 1275.73;
    //last_height_acc = height_acc;
    //last_filted_height_acc = filted_height_acc;
    gc[3][2] =filted_height_acc;
    float expect_acc = 0;
 //   if(nrf_rciv[TH_ADC_OFFSET]<97)
 //   {
 //     expect_acc = -0.8;
 //   }
 //   else if(nrf_rciv[TH_ADC_OFFSET]>=97&&(nrf_rciv[TH_ADC_OFFSET]<157))
 //   {
 //     expect_acc = 0;
 //   }
 //   else if(nrf_rciv[TH_ADC_OFFSET]>=157)
 //   {
 //     expect_acc = 0.8;
 //   }
 //   else//如果执行这一个程序肯定出问题了
 //   {
 //     expect_acc = 0;
 //   }
    expect_acc = 0;
    
     // height_acc_err = (expect_acc - height_acc);
    gc[3][0] = height_acc_err = (expect_acc - filted_height_acc);

    height_diff_acc_err = height_acc_err - last_height_acc_err;
    last_height_acc_err = height_acc_err;
    sum_of_heoght_acc_err += height_acc_err;
    z_pid_out = height_acc_p * height_acc_err + height_acc_i * sum_of_heoght_acc_err + height_acc_d * height_diff_acc_err;
    gc[3][0] = z_pid_out;
      
    if(z_pid_out>250)z_pid_out = 250;
    else if(z_pid_out<-250)z_pid_out = -250;
    
//    if(z_pid_out>0)
//      z_pid_out1 = 5*sqrt(z_pid_out);
//    else 
//      z_pid_out1 = -5*sqrt(-z_pid_out); 
//    z_pid_out1 = z_pid_out;
    
    z_pid_out1 = 250 * (1 - exp(-0.02 * z_pid_out)) / (1 + exp(-0.02 * z_pid_out));

    if(z_pid_out1>300)z_pid_out1 = 300;
    else if(z_pid_out1<-300)z_pid_out1 = -300;
    
  }
  else
  {
    gc[3][1] = z_pid_out1 = 0;
    gc[3][0] = filted_height_acc_pid = 0;
    last_height_acc = 0;
    last_filted_height_acc = 0;
    last_height_acc_err = 0;
    sum_of_heoght_acc_err = 0;
    z_pid_out = 0;
    
    last_filted_height_acc_pid = 0;
    last_height_acc_pid = 0;
   
  }
//  static float v_of_height_in_acc_inf = 0;//加速度计积分得到当前高度方向上的速度
//  gc[2][0] = v_of_height_in_acc_inf += (filted_height_acc * INTERUPT_CYC_IN_MS*1e-3);//加速度积分
//
//  static float height_in_acc_inf= 0;//加速度计两次积分得到的高度
//  gc[2][1] = height_in_acc_inf += v_of_height_in_acc_inf*INTERUPT_CYC_IN_MS*1e-3;

  return z_pid_out;  
}
/*******************************************PWM值计算*******************************************/
uint16 cali = 0;
float offset_angle[3] = {0};
float x_b = 0,y_b = 0;          //basic banlance param of 2 axis
float z_p = 0,z_d = 0;          //yaw control parameters
float x_p_o;                     //x(x axis)_p(proportion)_o(out loop)，x轴十形或者X形飞行控制参数，初始化为不同值
float x_p_i;
float x_d_i;
float y_p_o;
float y_p_i;
float y_d_i;

int Coef = 15;                          //derivative Coefficient
int pwm[4] = {0};      //
float xcq = 0,ycq = 0;                          //X,Y angle Control quantity
float pwm_of_dir = 0;                           //yaw  Control quantity
extern float filted_acc[2][3];
//Brief:name-PWMCalc(Calculate),4个电机占空比计算函数
//  计算之PWM值存于pwm
//Parameters:
//  mod:飞行的形式，十字形0，X形=1；
//return:void
//Example:
//  PWMCalc(0);   //十字形飞行模式下4个电机的转速(PWM)计算
void PWMCalc(uint8 mod)
{
  float dir_angle_error[2] = {0};
  static float throttle = 0;
  static float omega_e = 0;             //desier rotation speed
  static float _omega_error = 0;
#ifdef WATCH_DIFF_COEF
  static float last_omega_error_x,last_omega_error_y;
#else
#endif
  
  angle_error_x = angle[0]-(nrf_rciv[LR_ADC_OFFSET]-129)*ANG_CTRL_RATE + offset_angle[0]; //
  gc[1][2] = angle_error_y = (nrf_rciv[UD_ADC_OFFSET]-127)*ANG_CTRL_RATE-angle[1] + offset_angle[1];
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
  //gc[1][1] = (_omega_error - last_omega_error_y)/INTERUPT_CYC_IN_MS*1e3;
  last_omega_error_y = _omega_error;
#else
#endif
    deriv_out = Coef*(y_d_i*_omega_error-filter_coef_state_y);//FilterCoefficient[0]
    ycq = y_p_i*_omega_error + deriv_out;
    filter_coef_state_y += deriv_out*INTERUPT_CYC_IN_MS*1e-3;
  }
  else//十形飞行
  {
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
  if(dir_angle_error[0]>200)//误差大于270度说明经过了0和360度之间的间断点
    dir_angle_error[0] -= 360;
  else if(dir_angle_error[0]<-200)
    dir_angle_error[0] += 360;
  pwm_of_dir = z_p *dir_angle_error[0] + z_d*(gyro.z);
  
  extern char Filter_init_flag;
  static float expect_throttle = 0;
  if('f'==nrf_rciv[PLANE_MODE_OFFSET])
  {
     #ifdef MY_WOOD                  //my wood plane
       expect_throttle = (1.0*nrf_rciv[TH_ADC_OFFSET]);      //1.3的比例操作非常灵敏,油门比例需要配合电调的油门行程
     #elif CARBON_FIBRE              //other plane
       
       expect_throttle = sqrt(800*nrf_rciv[TH_ADC_OFFSET]);      //
       //expect_throttle = nrf_rciv[TH_ADC_OFFSET]; 
     #endif
       throttle = expect_throttle;
  }
  else if('h'==nrf_rciv[PLANE_MODE_OFFSET]&&(Filter_init_flag==1))
  {
    throttle = AltitudeControl(filted_acc[0]) + expect_throttle;

  }
  if(mod)
  {
    pwm[x_n] = (int)sqrt(throttle + xcq + ycq - pwm_of_dir)*20 + 500;//
    pwm[x_p] = (int)sqrt(throttle - xcq - ycq - pwm_of_dir)*20 + 500;//
    pwm[y_n] = (int)sqrt(throttle + xcq - ycq + pwm_of_dir)*20 + 500;
    pwm[y_p] = (int)sqrt(throttle - xcq + ycq + pwm_of_dir)*20 + 500;
  }
  else
  {
    
    pwm[x_n] = (int)(throttle + xcq - pwm_of_dir) + 500;//
    pwm[x_p] = (int)(throttle - xcq - pwm_of_dir) + 500;//
    pwm[y_n] = (int)(throttle - ycq + pwm_of_dir) + 500;
    pwm[y_p] = (int)(throttle + ycq + pwm_of_dir) + 500;
    
//    pwm[x_n] = (int)sqrt(500 * (throttle + xcq - pwm_of_dir)) + 500;//
//    pwm[x_p] = (int)sqrt(500 * (throttle - xcq - pwm_of_dir)) + 500;//
//    pwm[y_n] = (int)sqrt(500 * (throttle - ycq + pwm_of_dir)) + 500;
//    pwm[y_p] = (int)sqrt(500 * (throttle + ycq + pwm_of_dir)) + 500;
    

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
