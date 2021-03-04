#ifndef __LCD_H
#define __LCD_H		

//#include "define.h"
#include "ILI9341.h"
#include <DataStructure.h>

//LCD重要参数集,显示屏虽是固连件，单考虑长宽都宏定义也节省不了多少时间，不如更换芯片，为了将来像电脑切屏一样自适应，还是应用结构体
typedef struct  
{
  u16 scrn_x;		//LCD 宽度:screen x max
  u16 scrn_y;		//LCD 高度
  u16 scrn_id;			//LCD ID
  ScanDir  scan_dir;		//横屏还是竖屏控制：0，竖屏；1，横屏。//横屏dir=1和0都是左上角为起点→x(0~319);↓y(0~239)	,
  
  u16 pcolr;            //显示色
  u16 bcolr;            //背景色
}LCD_DEV, *PLCD_DEV;
//LCD参数
extern LCD_DEV lcd;	//管理LCD重要参数

typedef enum
{
  c1206 = 12,
  c1608 = 16,
  c2412 = 24,
}CHAR_SIZE;

typedef struct
{
  u16 x;                //屏幕上的显示位置
  u16 y;                //数据（不包含单位）最后一位的结束位置
  uint8 last_length;    //上一次data的长度，如果变短应该清屏
  CHAR_SIZE size;       //字体大小
  u16 data_color;       //数字颜色
  u16 unit_color;       //单位颜色
}DATA_ON_LCD, *PDATA_ON_LCD;            //数据在显示屏上的显示信息结构体，可用于任何类型的数据包括浮点数整数...

typedef struct
{
  u16 sx;                //屏幕上的起点位置
  u16 sy;                //屏幕上的起点位置
  short x_len;           //大小,负表示从sx起往减小的方向作为长方形的边长
  short y_len;           //大小
  char iffil;           //if fill 是否填充它，当然如果填充，颜色就是colr
  u16 colr;            //显示颜色
}LCD_RCTG_INFO,*PLCD_RCTG_INFO;//具有长宽色性质的lcd显示件,也可理解为矩形color定义颜色
extern LCD_RCTG_INFO th_lcd_info;
extern LCD_RCTG_INFO lr_lcd_info;
extern LCD_RCTG_INFO ud_lcd_info;
extern LCD_RCTG_INFO dr_lcd_info;

typedef struct
{
  u16 x;                //屏幕上的显示位置
  u16 y;                //数据（不包含单位）最后一位的结束位置
  uint8 last_len;    //上一次data的长度last_len，如果变短应该清屏
  CHAR_SIZE size;       //字体大小
  u16 data_colr;       //数字颜色
  u16 unit_colr;       //单位颜色
  uint8 acr_desm;       //要显示的小数点后面的位数0~n;
  char unit[10];       //单位字符串，不要超过10个字符
}LCD_VAR_INFO, *PLCD_VAR_INFO;            //LCD的variable information，带单位的，数据在显示屏上的显示信息结构体，可用于任何类型的数据包括浮点数整数...

typedef struct
{
  LCD_RCTG_INFO pan;                                //显示面板一样的姑且称之为（pan）pannel
  LCD_VAR_INFO pan_dat;                 //panneld dat,用面板显示的数据信息，其中x，y表示相对于面板左上角的偏移位置
}PANNELD_DATA,*PPANNELD_DATA;//背后有一个色板的数据显示件,姑且称为盘，形象的说就是盘上有个数据；
extern LCD_VAR_INFO bty_votg_lcd_info;//battery voltage show information like show bit、unit、position on the screen..
extern LCD_VAR_INFO nrf_send_times;

typedef struct
{
  LCD_RCTG_INFO pan;            //显示面板一样的姑且称之为（pan）pannel，就是曲线的显示范围
  LCD_VAR_INFO min_label;       //最小值的纵坐标标签
  LCD_VAR_INFO max_label;       //
    int min;                       //这个就和显示屏关系不密切了，是原始数据的显示最大值和最小值，如果给定的数据中有超出此区间范围，显示会超出边界，不予显示
  int max;
  u16 axis_colr;                //坐标系的颜色
  u16 crvcolr;                    //curve color,曲线的颜色
  char ifslc;                    //if slice 是否在节点之间补充直线
}LCD_CRV_INFO,*PLCD_CRV_INFO;//curve lcd information，一个函数曲线在LCD上的显示信息

typedef struct
{
  LCD_CRV_INFO crv_ifo;   //curve info
  SNODE *sdatli;          //stored data list存上一帧显示的数据，方便消除上一次画线,这个显示的数据是记录屏幕上点相对于sx的位置，不会存负数，不是存的原始数据
  SNODE *last_n;          //现在的节点指针
  u16 pos;                //单向链表不好知道上一个的值，所以保存一下
}SCAN_CRV_INFO,*PSCAN_CRV_INFO;


void LCD_DisplayOn(void);							//开显示
void LCD_DisplayOff(void);							//关显示
void LCD_Clear(uint16 color);
void Gpio8080OptLcdClr(uint16 color);


void LCD_Draw_A_Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);
void LCD_Draw_Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color);

void LcdDrwPoint(u16 x,u16 y, u16 color);						//画点
u16  LCD_ReadPoint(u16 x,u16 y, u8 *R, u8 *G, u8 *B); 						//读点 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 color);					//画圆
//char ShowF(PLCD_CRV_INFO crv);
void ScanShowF(PSCAN_CRV_INFO crv,short data);
void LcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);				//画线
void DrawThLine(PLCD_RCTG_INFO line_info, short len, char dir);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);		   		//画矩形
void LCD_Fill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color);		   		//填充单色
void Gpio8080OptLcdFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color);
void LCDDrawPointer(u16 x,u16 y, u16 size,char point_direction,u16 color);
void LCDShowVar(PLCD_VAR_INFO pd_p, float value);
//char LCD_ShowString(u16 x,u16 y, CHAR_SIZE size, u8 mode,const char *p, u16 color, u16 bcolor);		//显示一个字符串,12/16字体
char LcdSS(u16 x, u16 y, CHAR_SIZE size, const char *p, u16 color, u16 bcolor);
int LcdParamInit();

#ifdef STDOUT_LCD                               //如果使用LCD作为标准格式化输出设备
#define NUMOFSHOWROW  (LCD_LENGTH/c1206+2)                       //显示屏滚动显示存储的行数 ,保险起见+2                         
typedef struct Node
{
  struct Node * pPre;
  char str[LCD_LENGTH*2/c1206+2];        //不论横纵一行最多能显示的字符个数，为保险起见多加2个
  uint16 length;                          //存这一行的字符数，用于快速清屏
  struct Node * pNext;
}LCDROW,*PLCDROW;                //每一行的显示信息存储在此，滚屏时需要

PLCDROW CratLcdRowList(uint32 Node_Number, u8 ifcycle);
void Clear(u16 color);
void SetStdPrintfArea(u16 sx,u16 ex,u16 sy,u16 ey);         //设置标准输出的区域
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































