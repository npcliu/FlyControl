#include "stm32f10x.h"
#include "delay.h"
#include "define.h"
#include "sys_init.h"
#include "ISR.h"
#include "MyIIC.h"              //IIC PORT initial
#include "Mpu6050.h"
#include "QMC5883.h"
#include "stm32f10x_MY_spi.h"
#include "LSM303.h"
#include "stm32f10x_MY_Flash.h"
#include "MyNRF24L0.h"
#include "L3GD20.h"
#include "Task.h"
#include "interact.h"           //uart initial
#include "bmp180.h"
#include "ms5611.h"
#include "math.h"
#include "matrix.h"

/* ����ת��
  m:ת��ǰ����������n:ת��ǰ��������
  *sour[5]:ת��ǰ5�еľ���ָ��
  (*dest)[3]:ת�ú�3�еľ���ָ��
*/
void Transpose(float **sour, float **dest, int m, int n)
{
  int i, j;
  for (i = 0; i<n; i++)//ת�ú��ÿ��
  {
    for (j = 0; j<m; j++)//ת�ú��ÿ��
    {
      *(*(dest + i) + j) = *(*(sour + j) + i);
      //�����:�����[i][j] = ǰ����[j][i]
    }
  }
}
