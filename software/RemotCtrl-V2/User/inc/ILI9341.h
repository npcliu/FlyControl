#ifndef __ILI9341_H
#define __ILI9341_H		

#define STDOUT_LCD      //if use this LCD as standard output（printf） facility?

/***********************LCD GPIO Definition*************************/
#define	LCD_RST_CLR	PD8_OUT = 0    //1数据/命令0 		PC8	   
#define LCD_CS_CLR      PB11_OUT = 0    //片选端口  		PC9
#define	LCD_RS_CLR	PB12_OUT = 0    //1数据/命令0 		PC8	   
#define	LCD_WR_CLR	PB13_OUT = 0    //写数据		PC7
#define	LCD_RD_CLR	PB14_OUT = 0    //读数据		PC6

#define	LCD_RST_SET	PD8_OUT = 1    //1数据/命令0 		PC8	   
#define	LCD_CS_SET      PB11_OUT = 1    //片选端口  		PC9
#define	LCD_RS_SET	PB12_OUT = 1    //1数据/命令0 		PC8	   
#define	LCD_WR_SET	PB13_OUT = 1    //写数据		PC7
#define	LCD_RD_SET	PB14_OUT = 1    //读数据		PC6

#define	LCD_RST_PIN     PD8
#define	LCD_CS_PIN      PB11     //片选端口  		PC9
#define	LCD_DC_PIN	PB12     //数据/命令		PC8	   
#define	LCD_WR_PIN	PB13     //写数据		PC7
#define	LCD_RD_PIN	PB14     //读数据		PC6   
#define	LCD_CS  PB11_OUT     //片选端口  		PC9
#define	LCD_RS	PB12_OUT     //数据/命令		PC8	   
#define	LCD_WR	PB13_OUT     //写数据		PC7
#define	LCD_RD	PB14_OUT     //读数据		PC6   

//PTB123 789 10 11  16……23作为数据线  PT_DWORD(B,PDOR)是32位的输出寄存器，欲将16位数据正确输出到对应的端口，须:高8个数左移8位，中间5个数左移4位，最低3个数左移1位，这样就空出最低位（0位）正好对应未使用的PTB0
#define DATAOUT(x)      GPIOE->ODR=(x)//GPIOB->ODR=x; //数据输出 32位输出数据寄存器 PSOR
#define DATAIN          (GPIOE->IDR)                         //STM32:GPIOB->IDR;   //数据输入 32位输入数据寄存器	

/***********************LCD parameter Definition*************************/
#define LCD_WIDTH       240     //仅仅从外形上看的长和宽，和坐标、显示方向没有仍任何关系
#define LCD_LENGTH      320
#define TOTAL_POINT     76800

//扫描方向定义，正对屏幕，排线的那一边朝下
//扫描方向由MemoryAccessControl寄存器又名MADCTL（0x36）的D5,D6,D7（第5,6,7）位决定的，Manual P127
//屏幕的坐标系又是由上述寄存器的D6,D7（第6,7）位决定的，Manual P207，所以扫描方向确定了，坐标就确定了
//因为想要快速直接填充像素，必须确定扫描方向
typedef enum
{
  lrud = 0, //从左到右,从上到下，分别是x和y
  lrdu, //从左到右,从下到上，分别是x和y
  rlud, //从右到左,从上到下，分别是x和y
  rldu, //从右到左,从下到上，分别是x和y
   udlr, //从上到下,从左到右，分别是x和y
  udrl, //从上到下,从右到左，分别是x和y
  dulr, //从下到上,从左到右，分别是x和y
  durl, //从下到上,从右到左，分别是x和y
}ScanDir;              //该参数在程序中会改变，及动态改变显示方向，所以不能用宏定义死，最好用枚举封装

#ifdef STDOUT_LCD
typedef struct
{
  uint16 sx;     //start x position
  uint16 ex;     //end x position
  uint16 sy;
  uint16 ey;
  uint16 x_now;         //where the new location has reached,if something printed on screen,the position should be followed
  uint16 y_now;
}STD_PRINTF_AREA;       //标准输出函数printf打印的范围
#endif 
/***********************ILI9341 Reg Definition*************************/
//说明：宏名称将就复制手册中的，还未来得及大写

#define ReadDisplayMADCTL       0x0B


#define ColumnAddressSet        0x2A
#define PageAddressSet          0x2B
#define MemoryWrite             0x2C
#define ColorSet                0x2D

#define MemoryAccessControl     0x36//又叫MADCTL Manual P127 P207;确定扫描方向和坐标系

#define VerticalRefreshOrderSHIFT       4
#define HorizontalRefreshORDERSHIFT     2

uint16 LCD_Init(u8 dir,u16*px,u16*py, uint16 default_draw_color, uint16 default_back_color,...);	//初始化
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_Write_Command(u16 order);
u16  LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_WR_DATAX(u16 data);
u16 LCD_RD_DATA(void);
void LCD_SetCursor(u16 Xpos, u16 Ypos);//static inline 
void AreaFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color);

void LCD_Area_Set(u16 x0, u16 y0, u16 x1, u16 y1);
void Gpio8080OptLcdAS(u16 x0, u16 x1, u16 y0, u16 y1);

//void LCD_Scan_Dir(PLCD_DEV);							//设置屏扫描方向
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//设置窗口	
//void	TFT_Init_Ili9341(void);

//写数据函数
#define LCD_WR_DATA(data){\
LCD_RS = 1;\
LCD_CS = 0;\
DATAOUT(data);\
LCD_WR = 0;\
LCD_WR = 1;\
LCD_CS = 1;\
}
//写地址 
#define LCD_WR_REG(data){\
LCD_RS = 0;\
LCD_CS = 0;\
DATAOUT(data);\
LCD_WR = 0;\
LCD_WR = 1;\
LCD_CS = 1;\
}

#define LCDDWP(x,y,color)      {\
        LCD_WR_REG(0x2A);\
        LCD_WR_DATA(x>>8);\
        LCD_WR_DATA(x);\
        LCD_WR_REG(0x2B);\
        LCD_WR_DATA(y>>8);\
        LCD_WR_DATA(y);\
        LCD_WR_REG(0x2C);\
        LCD_WR_DATA(color);\
}
//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)





#endif































