#ifndef __LCD_H
#define __LCD_H		

//#include "define.h"
#include "ILI9341.h"
#include <DataStructure.h>

//LCD��Ҫ������,��ʾ�����ǹ������������ǳ����궨��Ҳ��ʡ���˶���ʱ�䣬�������оƬ��Ϊ�˽������������һ������Ӧ������Ӧ�ýṹ��
typedef struct  
{
  u16 scrn_x;		//LCD ���:screen x max
  u16 scrn_y;		//LCD �߶�
  u16 scrn_id;			//LCD ID
  ScanDir  scan_dir;		//���������������ƣ�0��������1��������//����dir=1��0�������Ͻ�Ϊ����x(0~319);��y(0~239)	,
  
  u16 pcolr;            //��ʾɫ
  u16 bcolr;            //����ɫ
}LCD_DEV, *PLCD_DEV;
//LCD����
extern LCD_DEV lcd;	//����LCD��Ҫ����

typedef enum
{
  c1206 = 12,
  c1608 = 16,
  c2412 = 24,
}CHAR_SIZE;

typedef struct
{
  u16 x;                //��Ļ�ϵ���ʾλ��
  u16 y;                //���ݣ���������λ�����һλ�Ľ���λ��
  uint8 last_length;    //��һ��data�ĳ��ȣ�������Ӧ������
  CHAR_SIZE size;       //�����С
  u16 data_color;       //������ɫ
  u16 unit_color;       //��λ��ɫ
}DATA_ON_LCD, *PDATA_ON_LCD;            //��������ʾ���ϵ���ʾ��Ϣ�ṹ�壬�������κ����͵����ݰ�������������...

typedef struct
{
  u16 sx;                //��Ļ�ϵ����λ��
  u16 sy;                //��Ļ�ϵ����λ��
  short x_len;           //��С,����ʾ��sx������С�ķ�����Ϊ�����εı߳�
  short y_len;           //��С
  char iffil;           //if fill �Ƿ����������Ȼ�����䣬��ɫ����colr
  u16 colr;            //��ʾ��ɫ
}LCD_RCTG_INFO,*PLCD_RCTG_INFO;//���г���ɫ���ʵ�lcd��ʾ��,Ҳ�����Ϊ����color������ɫ
extern LCD_RCTG_INFO th_lcd_info;
extern LCD_RCTG_INFO lr_lcd_info;
extern LCD_RCTG_INFO ud_lcd_info;
extern LCD_RCTG_INFO dr_lcd_info;

typedef struct
{
  u16 x;                //��Ļ�ϵ���ʾλ��
  u16 y;                //���ݣ���������λ�����һλ�Ľ���λ��
  uint8 last_len;    //��һ��data�ĳ���last_len��������Ӧ������
  CHAR_SIZE size;       //�����С
  u16 data_colr;       //������ɫ
  u16 unit_colr;       //��λ��ɫ
  uint8 acr_desm;       //Ҫ��ʾ��С��������λ��0~n;
  char unit[10];       //��λ�ַ�������Ҫ����10���ַ�
}LCD_VAR_INFO, *PLCD_VAR_INFO;            //LCD��variable information������λ�ģ���������ʾ���ϵ���ʾ��Ϣ�ṹ�壬�������κ����͵����ݰ�������������...

typedef struct
{
  LCD_RCTG_INFO pan;                                //��ʾ���һ���Ĺ��ҳ�֮Ϊ��pan��pannel
  LCD_VAR_INFO pan_dat;                 //panneld dat,�������ʾ��������Ϣ������x��y��ʾ�����������Ͻǵ�ƫ��λ��
}PANNELD_DATA,*PPANNELD_DATA;//������һ��ɫ���������ʾ��,���ҳ�Ϊ�̣������˵���������и����ݣ�
extern LCD_VAR_INFO bty_votg_lcd_info;//battery voltage show information like show bit��unit��position on the screen..
extern LCD_VAR_INFO nrf_send_times;

typedef struct
{
  LCD_RCTG_INFO pan;            //��ʾ���һ���Ĺ��ҳ�֮Ϊ��pan��pannel���������ߵ���ʾ��Χ
  LCD_VAR_INFO min_label;       //��Сֵ���������ǩ
  LCD_VAR_INFO max_label;       //
    int min;                       //����ͺ���ʾ����ϵ�������ˣ���ԭʼ���ݵ���ʾ���ֵ����Сֵ������������������г��������䷶Χ����ʾ�ᳬ���߽磬������ʾ
  int max;
  u16 axis_colr;                //����ϵ����ɫ
  u16 crvcolr;                    //curve color,���ߵ���ɫ
  char ifslc;                    //if slice �Ƿ��ڽڵ�֮�䲹��ֱ��
}LCD_CRV_INFO,*PLCD_CRV_INFO;//curve lcd information��һ������������LCD�ϵ���ʾ��Ϣ

typedef struct
{
  LCD_CRV_INFO crv_ifo;   //curve info
  SNODE *sdatli;          //stored data list����һ֡��ʾ�����ݣ�����������һ�λ���,�����ʾ�������Ǽ�¼��Ļ�ϵ������sx��λ�ã�����渺�������Ǵ��ԭʼ����
  SNODE *last_n;          //���ڵĽڵ�ָ��
  u16 pos;                //����������֪����һ����ֵ�����Ա���һ��
}SCAN_CRV_INFO,*PSCAN_CRV_INFO;


void LCD_DisplayOn(void);							//����ʾ
void LCD_DisplayOff(void);							//����ʾ
void LCD_Clear(uint16 color);
void Gpio8080OptLcdClr(uint16 color);


void LCD_Draw_A_Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);
void LCD_Draw_Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);

void LcdDrwPoint(u16 x,u16 y, u16 color);						//����
u16  LCD_ReadPoint(u16 x,u16 y, u8 *R, u8 *G, u8 *B); 						//���� 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 color);					//��Բ
//char ShowF(PLCD_CRV_INFO crv);
void ScanShowF(PSCAN_CRV_INFO crv,short data);
void LcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);				//����
void DrawThLine(PLCD_RCTG_INFO line_info, short len, char dir);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);		   		//������
void LCD_Fill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color);		   		//��䵥ɫ
void Gpio8080OptLcdFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color);
void LCDDrawPointer(u16 x,u16 y, u16 size,char point_direction,u16 color);
void LCDShowVar(PLCD_VAR_INFO pd_p, float value);
//char LCD_ShowString(u16 x,u16 y, CHAR_SIZE size, u8 mode,const char *p, u16 color, u16 bcolor);		//��ʾһ���ַ���,12/16����
char LcdSS(u16 x, u16 y, CHAR_SIZE size, const char *p, u16 color, u16 bcolor);
int LcdParamInit();

#ifdef STDOUT_LCD                               //���ʹ��LCD��Ϊ��׼��ʽ������豸
#define NUMOFSHOWROW  (LCD_LENGTH/c1206+2)                       //��ʾ��������ʾ�洢������ ,�������+2                         
typedef struct Node
{
  struct Node * pPre;
  char str[LCD_LENGTH*2/c1206+2];        //���ۺ���һ���������ʾ���ַ�������Ϊ����������2��
  uint16 length;                          //����һ�е��ַ��������ڿ�������
  struct Node * pNext;
}LCDROW,*PLCDROW;                //ÿһ�е���ʾ��Ϣ�洢�ڴˣ�����ʱ��Ҫ

PLCDROW CratLcdRowList(uint32 Node_Number, u8 ifcycle);
void Clear(u16 color);
void SetStdPrintfArea(u16 sx,u16 ex,u16 sy,u16 ey);         //���ñ�׼���������
#endif //#ifdef STDOUT_LCD

//typedef struct {
//	uint8 	*Array;
//	uint8	Xsize;
//	uint8	Ysize;
//	uint8	CharPitch;
//	uint8	LinePitch;
//	uint8	IndexOfs;
//	} FONT;
//#define		Rbits			0		// Red bits position
//#define		Gbits			5		// Green bits position
//#define		Bbits			11		// Blue bits position
//#define		RGB(R, G, B)	(((R >> 3) << Rbits) | ((G >> 2) << Gbits) | ((B >> 3) << Bbits))
#define RGB(r,g,b)              (uint16)((((uint16)r & 0xF8) << 8) | (((uint16)g & 0xFC) << 3) | ((uint16)b >> 3))

typedef struct
{
  unsigned int DW[2];
}_96bit;
typedef union
{
  _96bit font1206;
}_96type;
//typedef _8type Ctype;


#endif































