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

/* 矩阵转置
  m:转置前矩阵行数。n:转置前矩阵列数
  *sour[5]:转置前5列的矩阵指针
  (*dest)[3]:转置后3列的矩阵指针
*/
void Transpose(float **sour, float **dest, int m, int n)
{
  int i, j;
  for (i = 0; i<n; i++)//转置后的每行
  {
    for (j = 0; j<m; j++)//转置后的每列
    {
      *(*(dest + i) + j) = *(*(sour + j) + i);
      //相等于:后矩阵[i][j] = 前矩阵[j][i]
    }
  }
}
