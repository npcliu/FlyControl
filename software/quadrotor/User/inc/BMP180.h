#ifndef __BMP180_H
#define __BMP180_H
//#include "myiic.h"
#include "define.h"

typedef struct __BMP180
{
	short AC1;
	short AC2;
	short AC3;
	unsigned short AC4;
	unsigned short AC5;
	unsigned short AC6;
	short B1;
	short B2;
	short MB;
	short MC;
	short MD;
	long UT;
	long UP;
	long X1;
	long X2;
	long X3;
	long B3;
	unsigned long B4;
	long B5;
	long B6;
	long B7;
	long p;
	long Temp;//����10�õ���ʵ�¶�
	float altitude;
        float altitude_init;//��¼���ǰ�ĳ�ʼ�߶�
}_bmp180;

extern   unsigned int ut;
extern unsigned long up;

void BMP_ReadCalibrationData(void);
void BMP_UncompemstatedToTrue(void);
#endif