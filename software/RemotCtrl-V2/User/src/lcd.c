#include <stdlib.h>
#include "delay.h"
#include "lcd.h"
#include "font.h"
#include "RGB888.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ILI9341.h"
#include <math.h>
 
/*
lcd.c文件屏蔽了对底层的操作，也就是说，换一款LCD只需更换头文件即可使用
扫描方向只是不间断持续写GRAM的时候，屏幕逐点显示的方向，和横竖屏没关系
*/


#ifdef STDOUT_LCD               //如果使用LCD作为标准格式化输出设备
#define STD_LCD_CHARACTER_SIZE  c1206   //格式化输出用的字体
STD_PRINTF_AREA std_area;       //标准输出函数printf打印的范围
#endif


//管理LCD重要参数
//默认为竖屏
LCD_DEV lcd;                //LCD 参数设置结构体

//LCD写GRAM
//RGB_Code:颜色值
//void LCD_WriteRAM(u16 RGB_Code)
//{
//  GPIOE->CRL = PxOUTPP;
//GPIOE->CRH = PxOUTPP;
//LCD_WR_DATA(RGB_Code);//写十六位GRAM
//}

//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);	
  
  return(rgb);
}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色static inline 
void LcdDrwPoint(u16 x,u16 y, u16 color)
{
  LCD_Area_Set(x, y, x, y);//设置光标位置       //必须设一下全局，不然LCD处于填充状态自动换行	
  
  //  LCD_SetCursor(x,y);		//设置光标位置
  LCD_WR_REG(MemoryWrite);	//开始写入GRAM
  LCD_WR_DATA(color); 
}
//Brief:在指定区域内填充指定颜色
//  用户在其他LCD上也需要填充函数，
//  区域大小:(xend-xsta+1)*(yend-ysta+1),如果输入的终点横（纵）坐标小于起点横（纵）坐标，那么将自动判断大小，意味着无需区分sx,ex大小顺序
//  name:LCD_Fill;功能层函数
//parameters：
//  sx, ex, sy, ey:x和y的起始线和终止线，包括边界（无需区分起终大小，但须注意传参顺序）
//  color:要填充的颜色
//Return：
//  void;
//Example:
//    LCD_Fill(34,4,6,12,YELLOW);在x∈[4,34]，y∈[6,12]的范围填充黄色
void LCD_Fill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{//填充区域超过液晶屏最大值将不会画出超出部分，但是不能小于0，因为是无符号类型，负数将变最大正数
  u16 tmp;
  if(ex<sx)
  {
    tmp = sx;
    sx = ex;
    ex = tmp;
  }
  if(ey<sy)
  {
    tmp = sy;
    sy = ey;
    ey = tmp;
  }
  //  sx = sx             //边界检查：是否需要边界检查以及超出边界显示屏的反应需要查阅手册
  
  AreaFill(sx, ex, sy, ey, color);
}

//Brief:在指定区域内填充指定颜色
//  用户在其他LCD上也需要填充函数，
//  区域大小:(xend-xsta+1)*(yend-ysta+1),如果输入的终点横（纵）坐标小于起点横（纵）坐标，那么将自动判断大小，意味着无需区分sx,ex大小顺序
//  name:LCD_Fill;驱动层函数
//parameters：
//  sx, ex, sy, ey:x和y的起始线和终止线，包括边界（无需区分起终大小，但须注意传参顺序）
//  color:要填充的颜色
//Return：
//  void;
//Example:
//    Gpio8080OptLcdFill(34,4,6,12,YELLOW);//在x∈[4,34]，y∈[6,12]的范围填充黄色
void Gpio8080OptLcdFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{//填充区域超过液晶屏最大值将不会画出超出部分，但是不能小于0，因为是无符号类型，负数将变最大正数
  u16 tmp;
  if(ex<sx)
  {
    tmp = sx;
    sx = ex;
    ex = tmp;
  }
  if(ey<sy)
  {
    tmp = sy;
    sy = ey;
    ey = tmp;
  }
  //  sx = sx             //边界检查：是否需要边界检查以及超出边界显示屏的反应需要查阅手册
  
  uint32 total = (ex-sx+1)*(ey-sy+1);
  Gpio8080OptLcdAS(sx, ex, sy, ey);	                //设置areald->y_max-1  
  LCD_RS = 0;//  LCD_WR_REG(MemoryWrite);     			//开始写入GRAM
  DATAOUT(MemoryWrite);
  LCD_WR = 0;
  LCD_WR = 1;
  
  LCD_RS = 1;
  DATAOUT(color);
  while(total--)
  {
    LCD_WR = 0;
    LCD_WR = 1;
  }
  //  AreaFill(sx, ex, sy, ey, color);
}

//Brief:画（斜）直线函数，
//  概要：按照一个横纵坐标的方式，横坐标增加一个像素，纵坐标计算出来，画点，但是横坐标需是增量大的方向，举个极端点的例子，横向两个像素差，纵向8个像素点差，如果按横向走点只画得出2个
//  用户在其他LCD上也需要填充函数，
//  区域大小:(xend-xsta+1)*(yend-ysta+1),如果输入的终点横（纵）坐标小于起点横（纵）坐标，那么将自动判断大小，意味着无需区分sx,ex大小顺序
//  name:LCD_Fill;驱动层函数
//parameters：
//  sx, ex, sy, ey:x和y的起始线和终止线，包括边界（无需区分起终大小，但须注意传参顺序）
//  color:要填充的颜色
//Return：
//  void;
//Example:
//    Gpio8080OptLcdFill(34,4,6,12,YELLOW);//在x∈[4,34]，y∈[6,12]的范围填充黄色
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
  if(x1==x2 || y1==y2)
  {
    Gpio8080OptLcdFill(x1, x2,y1,  y2, color);
    return ;
  }
  u16 t;
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 
  delta_x=x2-x1;                //计算坐标增量 
  delta_y=y2-y1;                
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1;          //设置单步方向 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else{incy=-1;delta_y=-delta_y;} 
  if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )//画线输出 
  {
    LcdDrwPoint(uRow,uCol, color);//画点 
    xerr+=delta_x;
    yerr+=delta_y;
    if(xerr>distance)
    {
      xerr-=distance; 
      uRow+=incx; 
    } 
    if(yerr>distance>>1) 
    { 
      yerr-=distance; 
      uCol+=incy; 
    }
  }
}    

//函数功能：画虚线
//参数：
//x起始横坐标
//y:起始纵坐标
//c：X/Y 横向/纵向（大小写不限）
//len0：总长度
//len1：画线长度
//len2：空白长度
//color：画线颜色
void LcdDrwDotLine(uint16 x, uint16 y, char c, int len0, int len1,int len2, uint16 color)
{
  int i, j;
  switch(c)
  {
    case'X':;
  case 'x':
    {
      for(j=0;j<len0;j+=i)
      {
        for(i=0;i<len1;++i)
          LcdDrwPoint(x+i+j, y, color);
        i+=len2;
      }
    }break;
    case'Y':;
  case 'y':
    {
      for(j=0;j<len0;j+=i)
      {
        for(i=0;i<len1;++i)
          LcdDrwPoint(x, y+i+j, color);
        i+=len2;
      }
    }break;default:break;
  }
}

//Brief:画油量升降曲线,更换方向待测试
//  name:DrawTh(throttle)Line,
//  功能模块层实现，
//Parameters:
//  line_info：
//      x,y:起点坐标，x长度由油量控制，y长度由预先设置的宽度(size决定)，x,y以靠近原点端为始,微测试超过边界的情况
//      x_len:用来记录上次的长度，只是一个名不符其实的特殊用法；
//      y_len:油量条宽度
//      color：颜色;
//  dir:方向,0:x+; 1:y+; 2:x-; 3:y-;
//Return:
//  void；
//Example:
//    //
void DrawThLine(PLCD_RCTG_INFO line_info, short len, char dir)
{
  short derta[2];
  uint16 dcolr;
//  u16 bcolr;
  switch (dir)
  {
  case 1:
  case 3:
    {
      if(line_info->x_len < len)
      {
        derta[0] = line_info->sx - line_info->x_len;
        derta[1] = line_info->sx - (len-1);
                if(len>0)
          dcolr = line_info->colr;
        else
          dcolr = lcd.bcolr;

      }
      else if(line_info->x_len > len)
      {
        derta[0] = line_info->sx - (line_info->x_len-1);
        derta[1] = line_info->sx - (len);
        if(len>0)
          dcolr = lcd.bcolr;
        else
          dcolr = line_info->colr;
      }
      else
        return;
      if(line_info->x_len * len<=0)//跨界时消隐掉原先范围内的线段
      {
        Gpio8080OptLcdFill(line_info->sx,line_info->sx-line_info->x_len,line_info->sy,line_info->sy+line_info->y_len-(line_info->y_len>0)+(line_info->y_len<0),lcd.bcolr);
        Gpio8080OptLcdFill(line_info->sx,derta[1],line_info->sy,line_info->sy+line_info->y_len-(line_info->y_len>0)+(line_info->y_len<0),dcolr);
      }
      else
        Gpio8080OptLcdFill(derta[0],derta[1],line_info->sy,line_info->sy+line_info->y_len-(line_info->y_len>0)+(line_info->y_len<0),dcolr);
      line_info->x_len = len;
    }break;
  case 0:
  case 2:
    {
      if(line_info->y_len < len)
      {
        derta[0] = line_info->sy - line_info->y_len;
        derta[1] = line_info->sy - (len-1);
        if(len>0)
          dcolr = line_info->colr;
        else
          dcolr = lcd.bcolr;
      }
      else if(line_info->y_len > len)
      {
        derta[0] = line_info->sy - (line_info->y_len-(line_info->y_len>0)+(line_info->y_len<0));
        derta[1] = line_info->sy - (len);
        if(len>0)
          dcolr = lcd.bcolr;
        else
          dcolr = line_info->colr;
      }
      else
        return;
      if(line_info->y_len * len<=0)//跨界时消隐掉原先范围内的线段
      {
        Gpio8080OptLcdFill(line_info->sx,line_info->sx+line_info->x_len-(line_info->x_len>0)+(line_info->x_len<0),line_info->sy,line_info->sy-line_info->y_len,lcd.bcolr);
        Gpio8080OptLcdFill(line_info->sx,line_info->sx+line_info->x_len-(line_info->x_len>0)+(line_info->x_len<0),line_info->sy,derta[1],dcolr);
      }
      else
        Gpio8080OptLcdFill(line_info->sx,line_info->sx+line_info->x_len-(line_info->x_len>0)+(line_info->x_len<0),derta[0],derta[1],dcolr);
      line_info->y_len = len;
    }break;
  default:break;
  }
}

//画矩形框	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
  Gpio8080OptLcdFill(x1,x2,y1,y1, color);
  Gpio8080OptLcdFill(x1,x1,y1,y2, color);
  Gpio8080OptLcdFill(x1,x2,y2,y2, color);
  Gpio8080OptLcdFill(x2,x2,y1,y2, color);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 color)
{
  int a,b;
  int di;
  a=0;b=r;	  
  di=3-(r<<1);             //判断下个点位置的标志
  while(a<=b)
  {
    LcdDrwPoint(x0+a,y0-b, color);             //5
    LcdDrwPoint(x0+b,y0-a, color);             //0           
    LcdDrwPoint(x0+b,y0+a, color);             //4               
    LcdDrwPoint(x0+a,y0+b, color);             //6 
    LcdDrwPoint(x0-a,y0+b, color);             //1       
    LcdDrwPoint(x0-b,y0+a, color);             
    LcdDrwPoint(x0-a,y0-b, color);             //2             
    LcdDrwPoint(x0-b,y0-a, color);             //7     	         
    a++;
    //使用Bresenham算法画圆     
    if(di<0)di +=4*a+6;	  
    else
    {
      di+=10+4*(a-b);   
      b--;
    } 						    
  }
} 									  

//Brief:画指向标，待完善
//  name:StdLcdSC=standard LCD Show Char,标准的（可轻松在其他平台重用的）在指定位置显示一个字符
//  功能模块层实现，时序优化速度快,不检查超出LCD边界，需要用户注意；供用户调用
//Parameters:
//  x,y:指针尖端坐标，x，y无限制，x超过边界自动换行，y超过边界回到0
//  size:length of the pointer →|<□|←,整个光标的长度
//  point_direction：三角形指向方向，0:x+; 1:y+; 2:x-; 3:y-;其他退出
//  color：箭标颜色;
//Return:
//  void；
//Example:
//    //
void LCDDrawPointer(u16 x,u16 y, u16 size,char point_direction,u16 color)
{
  assert(size>0);
  size -= 1;    //因为坐标包含首尾闭区间，比长度多一个，所以减1
  uint16 wide = (u16)(.35*size);
  uint16 i = 0,j = 0;
  
  switch (point_direction)
  {
  case 1:
    for(i=0;i<wide;i++)         //y+
    {
      for(i=0;i<wide;i++)
      {
        j = (i<<1)+1;
        while(j--)
          LcdDrwPoint(x-i+j,y-i,color);
      }
      LCD_Fill(x-wide, x+wide, y-wide, y-size,color);
    }	  
    break;
  case 3:
    {
      for(i=0;i<wide;i++)       //y-
      {
        j = (i<<1)+1;
        while(j--)
          LcdDrwPoint(x-i+j,y+i,color);
      }
      LCD_Fill(x-wide, x+wide, y+wide, y+size,color);
    }break;
  case 0:
    for(i=0;i<wide;i++)         //x+
    {
      for(i=0;i<wide;i++)
      {
        j = (i<<1)+1;
        while(j--)
          LcdDrwPoint(x-i,y-i+j,color);
      }
      LCD_Fill(x-wide, x-size, y-wide, y+wide,color);
    }	  
    break;
  case 2:
    {
      for(i=0;i<wide;i++)       //x-
      {
        j = (i<<1)+1;
        while(j--)
          LcdDrwPoint(x+i,y+i-j,color);
      }
      LCD_Fill(x+wide, x+size, y-wide, y+wide,color);
    }break;
  default:break;
  };
}

//Brief:
//  name:Std(standard)LcdS(Show)C(Char),标准的（可轻松在其他平台重用的）在指定位置显示一个字符
//  功能模块层实现，时序优化速度快,不检查超出LCD边界，需要用户注意；供用户调用
//Parameters:
//  x,y:起点坐标，应该是字符串的左上顶点；x，y无限制，x超过边界自动换行，y超过边界回到0
//  size:字体大小，具体查看CHAR_SIZE枚举；
//  num:字符；空指针("")将不会作任何显示
//  color：字体颜色;
//  bcolor:背景色;
//Return:
//  0:No problems;
//  1:No Font;
//Example:
//    StdLcdSC(10, 20,c1206, 'A',CYAN,BLACK);//在10,10处（字符左上角位置）用CYAN字BLACK底显示A
char StdLcdSC(u16 x,u16 y, CHAR_SIZE size, u8 num, u16 color, u16 bcolor)
{
  signed char t;
  u8 csize;	//得到字体一个字符对应点阵集所占的字节数
  num = num - ' ';                                //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
  
  const unsigned char *pfont;   //指向字库某个字符的指针 
  switch (size)
  {
  case c1206: 	//调用1206字体
    {
      csize = 12;               //一个字符对应点阵集所占的字节数
      pfont=asc2_1206[num];
    }break;
  case c1608:	//调用1608字体
    {
      csize = 16;               //一个字符对应点阵集所占的字节数
      pfont=asc2_1608[num];
    }break;
  case c2412:	//调用2412字体
    {
      csize = 36;               //一个字符对应点阵集所占的字节数
      pfont=asc2_2412[num];
    }break;
  default:return 1;
  }
  
  uint16 k;
  u8 r = 0;
  
  for(t=0;t<csize;t++)
  {
    num = pfont[t];                     //num这时作为临时变量用了，
    k=8;
    while(k--)
    {
      if(num&0x80)
        LcdDrwPoint(x+r,y,color);
      else
        LcdDrwPoint(x+r,y,bcolor);
      num<<=1;
      r++;
      if(r>=size)        //判断是否到达了底部，如1206，一列只有12个像素（共6列），用两个字节存，第二个字节的后四位是无用的
      {
        r=0;
        y++;
        break;
      }
    }
  }
  return 0;
}

//Brief:
//  name:Gpio8080Opt（optimized）LcdS（Show）C(Char)=针对GPIO模拟8080时序优化过后的字符显示程序,
//  专用的驱动层实现，利用LCD预置好区域，填充时遇到边界自动换行特点，以及利用冗余时序进行加速，在指定位置显示一个字符，
//  不检查超出LCD边界，需要用户注意；供用户调用
//Parameters:
//  x,y:起点坐标，应该是字符串的左上顶点；x，y无限制，x超过边界自动换行，y超过边界回到0
//  size:字体大小，具体查看CHAR_SIZE枚举；
//  num:字符；空指针("")将不会作任何显示
//  color：字体颜色;
//  bcolor:背景色;
//Return:
//  0:No problems;
//  1:No Font;
//Example:
//      Gpio8080OptLcdSC(10, 20,c1206, 'A',CYAN,BLACK);//在10,10处（字符左上角位置）用CYAN字BLACK底显示A
char Gpio8080OptLcdSC(u16 x,u16 y, CHAR_SIZE size, u8 num, u16 color, u16 bcolor)
{
  signed char t;
  u8 csize;	//得到字体一个字符对应点阵集所占的字节数
  num = num - ' ';                                //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
  
  const unsigned char *pfont;   //指向字库某个字符的指针 
  switch (size)
  {
  case c1206: 	//调用1206字体
    {
      csize = 12;               //一个字符对应点阵集所占的字节数
      pfont=asc2_1206[num];
    }break;
  case c1608:	//调用1608字体
    {
      csize = 16;               //一个字符对应点阵集所占的字节数
      pfont=asc2_1608[num];
    }break;
  case c2412:	//调用2412字体
    {
      csize = 36;               //一个字符对应点阵集所占的字节数
      pfont=asc2_2412[num];
    }break;
  default:return 1;
  }
  Gpio8080OptLcdAS(x, x+size-1, y, y+size/2-1);//设置光标位置 	
  LCD_RS = 0;//  LCD_WR_REG(MemoryWrite);     			//开始写入GRAM
  DATAOUT(MemoryWrite);
  LCD_WR = 0;
  LCD_WR = 1;
  
  x = 0;
  LCD_RS = 1;
  for(t=0;t<csize;t++)
  {
    num = pfont[t];                     //num这时作为临时变量用了，
    y=8;
    while(y--)
    {
      if(num&(0x80))
        DATAOUT(color); //颜色相同的话，写端口都不必了
      else
        DATAOUT(bcolor);
      LCD_WR = 0;
      num<<=1;
      x++;
      LCD_WR = 1;
      if(x-size>=0)        //判断是否到达了底部，如1206，一列只有12个像素（共6列），用两个字节存，第二个字节的后四位是无用的
      {
        x=0;
        break;
      }
    }
  }
  LCD_CS = 1;
  return 0;
}


//Brief:显示字符串；自动换行，（如果换行）自动对齐起始字符，
//  name:LcdS(Show)S(String);
//Parameters:
//  x,y:起点坐标，应该是字符串的左上顶点；x，y无限制，x超过边界自动换行，y超过边界回到0
//  size:字体大小；
//  mode：1：非覆盖方式，0：覆盖方式；非覆盖速度较快
//  *p:字符串；空指针("")将不会作任何显示
//  color：字体颜色;
//  bcolor:背景色;
//Return:
//  void;
//Example:
//    LcdSS(2, 3, c1206, "abc", RED, BLACK); //在2,3位置上用1206字体，不覆盖的方式，红字黑底显示abc
//char mmm = 0;
char LcdSS(u16 x, u16 y, CHAR_SIZE size, const char *p, u16 color, u16 bcolor)
{//x，y是画每个字符的坐标
  //  uint16 point[2]={x,y};                //画每个像素点的坐标
  //  u16 y_start;
  //  u8 csize;	//得到字体一个字符对应点阵集所占的字节数	
  //  u8 ch;
  //  u8 temp,t1,t;
  //  const unsigned char *pfont;   //指向字库某个字符的指针 
  
  if(y>lcd.scrn_y - size/2)//虽然可以依赖对显示屏的设置从而避开程序对超出显示边界的检查，但是换种显示屏呢？所以还是检查
    y = 0;                              //y超过边界设置为0；后面也是按0对齐
  u16 y_start = y;
  while((*p<='~')&&(*p>=' '))           //判断是不是非法字符!，遇到非法字符结束显示
  {
    if(y>lcd.scrn_y - size/2)         //超出y范围
    {
      y = y_start;                             //对齐起始字符串
      x+=size;                          //换行
    }
    if(x>lcd.scrn_x-size)            //无符号类型不用检查负数，y超出范围置0
    {
      x = 0;
    }
    Gpio8080OptLcdSC(x, y, size, *p,color, bcolor);
    y+=size/2;
    p++;
  }
  return 0;
}

//Brief:在屏幕上指定位置(x,y描述数据结束单位正要（还未）开始的像素点)，用指定的数据颜色和单位颜色显示一个带单位的数据，可以是整形，可以是浮点型，可以有单位也可无，无单位请指定单位为""
//  函数设计考虑：特定数据在屏幕上的显示位置一般是不变的，用结构体可以减少多形参的压栈次数，但是显示的数据
//  可以是多种类型，不可以用一种结构体存放数值，因此用3个形参传递;数据从几百变成几十甚至几传唱度将发生变化，如果不清屏看不出数字是多少
//  而且显示应该以数据右侧为基准，即从几变成几十个位所在的显示位置不应该变，单位从变化也不变数据位置，不然看起来晃得很
//  name:LCDShowVar(Variable);
//Parameters:
//  pd_p:显示的信息，坐标，大小，颜色等;
//  value:数值，之所以用float，int转float不失精度，double转float虽然丢失精度，但是只显示不影响;
//Return:
//  void;
//Example:
//    LCDShowVar(&bty_votg_lcd_info,bat_voltage,0,"V");//结构体是显示信息，显示电池电压，小数点后0位（整数），单位为V
void LCDShowVar(PLCD_VAR_INFO pd_p, float value)
{
  char str[16] = {0};
  char tmp[16] = "";
  snprintf(str,16,"%.*f",pd_p->acr_desm,value);
  uint8 len = strlen(str);
  short derta = pd_p->last_len - len;
  if(derta>0)                   //本次的数据长度比先前的小
  {
    len += derta;               //不能直接省略而将后面使用的len换成last_length，因为初始时last_length可能是0，会乱
    while(derta--)
      strcat(tmp," ");          //将前面补成空格
  }
  strcat(tmp,str);
  LcdSS(pd_p->x, pd_p->y-len*pd_p->size/2, pd_p->size,tmp, pd_p->data_colr,lcd.bcolr);
  LcdSS(pd_p->x, pd_p->y+1, pd_p->size,pd_p->unit, pd_p->unit_colr, lcd.bcolr);
  pd_p->last_len = strlen(str);
}

//#define FDATA_LEN 300
//short DATA_STORAGED[FDATA_LEN], DELTA_DATA[FDATA_LEN];      //可以显示5个带有160个数据的函数,DELTA_DATA用于补线相关的数据存储
//Brief:
//  有两种模式，一种是从给定的数组把数组中的数据显示成函数曲线，现在支持数组长度小于等于横向区间像素点个数的情况，此时该函数会把数组的元素平均分开在横坐标范围内
//  第二种是扫描模式，当传过来的数组是一维的时候(单个元素也可以)进入该模式,扫描模式和立显模式不能通用，因为立显模式只需要分配节点个内存，而扫描模式需要分配整个显示区宽度的内存
//  函数曲线的显示功能蓝图：在设置得区间范围内显示给定的数据曲线即画图，这时候就要有伸缩功能，
//  二一个是类似示波器的scan模式，即立即显示送过来的数据点;
//  没必要把整个数组都存下来，假设跳着节点显示，那完全只需要把显示的思安记下来即可啊，况且再想一下，谁会用那么大一数组送过来显示呢
//  更普遍存在的不是一个一个点送过来显示吗？即使用户送了一个大数组过来，那我是不是跟着分配一个超大空间呢，显然不是，只需要把显示的点记住即可
//  还要考虑的一点是用户传过来的数据点可能很稀疏，我是用补直线的办法画出来的，这是记录的点数肯定少于屏幕画出的点数的
//  显示边框，显示数值，显示格子线等
//  name:Statc(static)ShowF(function)();静态的函数曲线显示函数，给一个数组就显示是为静态，
//sx,ex,sy,ey:显示的区域，不包括边界，参数太多可以来一个控件
//y:显示起始纵坐标
//parr：数据数组名
//hang：显示的数据所在数组的第几行
//compress_index：纵坐标压缩率
//slice：是否用直线连接离散点
//  ‘1’：用直线连接。‘0’：不连接
//color：显示颜色
char JstShowF(PLCD_CRV_INFO crv)
{
  if(false==crv->pan.iffil)
    LCD_DrawRectangle(crv->pan.sx,crv->pan.sy,crv->pan.sx+crv->pan.x_len-(crv->pan.x_len>0)+(crv->pan.x_len<0),crv->pan.sy+crv->pan.y_len-(crv->pan.y_len>0)+(crv->pan.y_len<0),crv->pan.colr);
  else
    Gpio8080OptLcdFill(crv->pan.sx,crv->pan.sx+crv->pan.x_len-(crv->pan.x_len>0)+(crv->pan.x_len<0),crv->pan.sy,crv->pan.sy+crv->pan.y_len-(crv->pan.y_len>0)+(crv->pan.y_len<0),crv->pan.colr);
  //  uint16 idx = 0;  //传进来的数组下标偏移临时变量
  
  //  uint16 step;
  //  if(1==crv->show_len)
  //    return 1;           //
  //
  //    step = (crv->pan.y_len-3)/(crv->show_len-1);
  //  uint16 y = crv->pan.sy + 1;
  //  u16 last_stor_dat,lasty = 0;
  //  float k = (crv->pan.x_len-3)/((float)(crv->min-crv->max));
  //  u16 tmp_ops;
  //  
  //  for(idx = 0;idx<crv->show_len;y+=step,idx++)
  //  {
  //    tmp_ops = (u16)(k*(crv->show_buf[idx]-crv->min) + crv->pan.sx+crv->pan.x_len-2);
  //    if(true==crv->ifslc)
  //    {
  //      if(idx)   //从第二个点开始才有线，消隐之前补足的直线，重画现在的曲线，从之前的坐标到现在的坐标
  //      {
  //        LCD_DrawLine(last_stor_dat, lasty, crv->stor_dat[idx], y, lcd.bcolr);
  //        LCD_DrawLine(crv->stor_dat[idx-1], lasty, tmp_ops, y, crv->crvcolr);
  //      }
  //      last_stor_dat = crv->stor_dat[idx];//到现在这个点，上一次的stor_dat已经在程序最后被更新了，所以要取出来备用
  //      lasty = y;
  //    }
  //    else
  //    {
  //    if(crv->stor_dat[idx]>crv->pan.sx && crv->stor_dat[idx]<(crv->pan.sx+crv->pan.x_len-1))
  //      LcdDrwPoint(crv->stor_dat[idx], y, lcd.bcolr);
  //    if(tmp_ops>crv->pan.sx && tmp_ops<(crv->pan.sx+crv->pan.x_len-1))
  //      LcdDrwPoint(tmp_ops, y, crv->crvcolr);    //否则你可以理解为在脑海中的那个位置画了一个点，还是要把数据记录下来的，不然补全直线的时候就没数据了
  //    }
  //    crv->stor_dat[idx] = tmp_ops;
  //  }
  return 0;
}

//Brief:在屏幕指定区域画动态函数曲线，动态之意为新数据将追画在曲线末尾,注意曲线是挨个像素点进行的
//  name:ScanShowF(function),扫描性地绘制函数曲线，
//  两种模式:点模式(绘制离散点),连通模式(用直线将相邻散点连起来)，在结构体参数中设置
//  该函数是很特殊的画直线，一般直线起终点任意，像此函数有指定边框，不好处理（在该函数内处理非常复杂，在画直线函数内处理吧又会增加其他函数的负担）
//  此函数起终点横坐标差一个像素点，因此，单独重写此直线
//  不补直线的情况：拆除边界不画
//  功能模块层实现，
//Parameters:
//  crv:函数曲线相关结构体指针
//  data:本次的数据；将会被显示在屏幕上并且记录在结构体链表中
//Return:
//  void；
//Example:
//  ScanShowF(p_thcrv,(u16)(((float)th_adc/TH_ADC_MAX)*255));//
void ScanShowF(PSCAN_CRV_INFO crv,short data)
{
  static u16 last_data;//last_stor_dat是消隐之前画的线段的首端点纵坐标，last_data是上一次显示的点坐标，用来画这次的直线的
  if(false==crv->crv_ifo.pan.iffil)
    LCD_DrawRectangle(crv->crv_ifo.pan.sx,crv->crv_ifo.pan.sy,crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0),crv->crv_ifo.pan.sy+crv->crv_ifo.pan.y_len-(crv->crv_ifo.pan.y_len>0)+(crv->crv_ifo.pan.y_len<0),crv->crv_ifo.pan.colr);
  else
    Gpio8080OptLcdFill(crv->crv_ifo.pan.sx,crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0),crv->crv_ifo.pan.sy,crv->crv_ifo.pan.sy+crv->crv_ifo.pan.y_len-(crv->crv_ifo.pan.y_len>0)+(crv->crv_ifo.pan.y_len<0),crv->crv_ifo.pan.colr);
  LCDShowVar(&crv->crv_ifo.min_label,crv->crv_ifo.min);
  LCDShowVar(&crv->crv_ifo.max_label,crv->crv_ifo.max);
  float k = (abs(crv->crv_ifo.pan.x_len)-3)/((float)(crv->crv_ifo.min-crv->crv_ifo.max));
  u16 tmp_ops,tmp1,tmp2,tmp3,rag_l = crv->crv_ifo.pan.sx,rag_h=crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0);
  tmp_ops = (u16)(k*(data-crv->crv_ifo.min) + crv->crv_ifo.pan.sx+abs(crv->crv_ifo.pan.x_len)-2);
  if(true==crv->crv_ifo.ifslc)
  {
    if(crv->last_n->pNext!=crv->sdatli->pNext)   //消隐是从k位置点到k+1位置的，分成2段线段，任何一段超出范围都将画到边界为止
    {
      tmp1 = crv->last_n->u16data;                //k像素点的纵位置
      tmp3 = crv->last_n->pNext->u16data;          //k+1像素的纵位置
      tmp2 = (tmp1+tmp3)>>1;                       //中间位置
      if(tmp1>rag_l && tmp1<rag_h)   //第一段（k位置）线段，端点1在范围内？
      {
        if(tmp2<=rag_l)           //端点1在范围内则需要验看端点2是否超限，超限则限位
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos,crv->pos, lcd.bcolr);   //第一段线段
      }
      else if(tmp2>rag_l && tmp2<rag_h)   //第一段（k位置）线段，端点2在范围内？
      {
        if(tmp1<=rag_l)
          tmp1 = rag_l+1;
        else if(tmp1>=rag_h)
          tmp1 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos,crv->pos, lcd.bcolr);
      }
      else;//两点均在范围外则不画线
      if(tmp2>rag_l && tmp2<rag_h)   //第二段（k+1位置）线段，端点1在范围内？
      {
        if(tmp3<=rag_l)
          tmp3 = rag_l+1;
        else if(tmp3>=rag_h)
          tmp3 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos+1,crv->pos+1, lcd.bcolr);
      }
      else if(tmp3>rag_l && tmp3<rag_h)   //第二段（k+1位置）线段，端点2在范围内？
      {
        if(tmp2<=rag_l)
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos+1,crv->pos+1, lcd.bcolr);
      }
      else;//两点均在范围外则不画线
    }
    if(crv->last_n!=crv->sdatli->pNext)   //从第二个点开始才有线，重画现在的曲线，从之前的坐标到现在的坐标
    {
      tmp1 = last_data;
      tmp3 = tmp_ops;
      tmp2 = (tmp1+tmp3)>>1;
      if(tmp1>rag_l && tmp1<rag_h)
      {
        if(tmp2<=rag_l)
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos-1,crv->pos-1, crv->crv_ifo.crvcolr);
      }
      else if(tmp2>rag_l && tmp2<rag_h)
      {
        if(tmp1<=rag_l)
          tmp1 = rag_l+1;
        else if(tmp1>=rag_h)
          tmp1 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos-1,crv->pos-1, crv->crv_ifo.crvcolr);
      }
      else;//两点均在范围外则不画线
      if(tmp2>rag_l && tmp2<rag_h)
      {
        if(tmp3<=rag_l)
          tmp3 = rag_l+1;
        else if(tmp3>=rag_h)
          tmp3 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos,crv->pos, crv->crv_ifo.crvcolr);
      }
      else if(tmp3>rag_l && tmp3<rag_h)
      {
        if(tmp2<=rag_l)
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //要求很大值不能变回最小值或者负数，否则将画反方向直线
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos,crv->pos, crv->crv_ifo.crvcolr);
      }
      else;//两点均在范围外则不画线
    }
    last_data = tmp_ops;//单向链表不具有回溯能力
  }
  else
  {
    if(crv->last_n->u16data>crv->crv_ifo.pan.sx && crv->last_n->u16data<(crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0)))
      LcdDrwPoint(crv->last_n->u16data, crv->pos, lcd.bcolr);
    if(tmp_ops>crv->crv_ifo.pan.sx && tmp_ops<(crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0)))
      LcdDrwPoint(tmp_ops, crv->pos, crv->crv_ifo.crvcolr);    //否则你可以理解为在脑海中的那个位置画了一个点，还是要把数据记录下来的，不然补全直线的时候就没数据了
  }
  if(crv->pos<crv->crv_ifo.pan.sy+abs(crv->crv_ifo.pan.y_len)-2)//是否超过边界
    crv->pos++;
  else
    crv->pos = crv->crv_ifo.pan.sy+1;
  crv->last_n->u16data = tmp_ops;       //记录该节点的数据于链表中
  crv->last_n = crv->last_n->pNext;     //链表是循环的就直接指向下一个位置
}

#ifdef STDOUT_LCD                               //如果使用LCD作为标准格式化输出设备
static PLCDROW p_now;
static PLCDROW p_ssnow;       //show start    //滚屏时起作用
#endif //#ifdef STDOUT_LCD

//Brief:指定颜色清屏，功能模块层函数；如果指定该LCD作为标准输出设备，则将现在的打印位置清零
//  name:LCD_Fill;
//parameters：
//  color:要填充的颜色
//Return：
//  void;
//Example:
//    LCD_Clear(YELLOW);用黄色清屏
void LCD_Clear(uint16 color)
{
  //  u32 index;
  uint16 end_x,end_y;
  if(lcd.scan_dir<=rldu)
  {
    end_x = LCD_WIDTH-1;
    end_y = LCD_LENGTH-1;
  }
  else if(lcd.scan_dir<=durl)
  {
    end_x = LCD_LENGTH-1;
    end_y = LCD_WIDTH-1;
  }
  else;
  AreaFill(0, end_x, 0, end_y, color);
#ifdef STDOUT_LCD                               //如果使用LCD作为标准格式化输出设备
  std_area.x_now = std_area.sx;
  std_area.y_now = std_area.sy;
  p_now->length = 0;
  p_ssnow = p_now;
#endif //#ifdef STDOUT_LCD
}

//Brief:指定颜色清屏，功能模块层函数,如果指定该LCD作为标准输出设备，则将现在的打印位置清零
//  驱动层实现，利用LCD预置好区域，填充时遇到边界自动换行特点，以及利用冗余时序进行加速
//  name:Gpio8080OptLcdClr;
//parameters：
//  color:要填充的颜色
//Return：
//  void;
//Example:
//    LCD_Clear(YELLOW);用黄色清屏
void Gpio8080OptLcdClr(uint16 color)
{
  uint16 end_x,end_y;
  if(lcd.scan_dir<=rldu)
  {
    end_x = LCD_WIDTH-1;
    end_y = LCD_LENGTH-1;
  }
  else if(lcd.scan_dir<=durl)
  {
    end_x = LCD_LENGTH-1;
    end_y = LCD_WIDTH-1;
  }
  else;
  Gpio8080OptLcdAS(0, end_x, 0, end_y);	                //设置areald->y_max-1  
  LCD_RS = 0;//  LCD_WR_REG(MemoryWrite);     			//开始写入GRAM
  DATAOUT(MemoryWrite);
  LCD_WR = 0;
  LCD_WR = 1;
  
  //  for(index=0; index<TOTAL_POINT; index++)
  LCD_RS = 1;
  u32 index = TOTAL_POINT;
  DATAOUT(color);
  while(index--)
  {
    LCD_WR = 0;
    LCD_WR = 1;
  }
#ifdef STDOUT_LCD                               //如果使用LCD作为标准格式化输出设备
  std_area.x_now = std_area.sx;
  std_area.y_now = std_area.sy;
  p_now->length = 0;
  p_ssnow = p_now;
#endif //#ifdef STDOUT_LCD
}

#ifdef STDOUT_LCD                               //如果使用LCD作为标准格式化输出设备
#include <stdlib.h>

PLCDROW plcd_row;
static PLCDROW p_roll_start;                  //roll 时起始的节点
static PLCDROW p_last_roll_start;             //快速清屏用

PLCDROW CratLcdRowList(uint32 Node_Number, u8 ifcycle)
{
  struct Node * pNew;
  struct Node * pHead;
  struct Node * pLast;
  int i;
  
  pHead = (struct Node *)malloc(sizeof(struct Node));
  if(NULL==pHead)
  {
    printf("malloc failed!\n");
    exit(0);
  }
  //头结点数据初始化
  pHead->str[0] = '\0';			//暂时还不知道该存放什么数据
  pHead->pPre = NULL;
  pHead->pNext = NULL;
  pLast = pHead;				//因为最后头结点是要返回的，直到return不能变，所以用一个新变量
  
  Node_Number = NUMOFSHOWROW;
  //	printf("请输入节点个数:");
  //	scanf("%d", &Node_Number);
  //	if(0==Node_Number)
  //	{
  //		printf("该链表为空!\n");
  //		return pHead;
  //	}
  //	else
  //	{
  for(i=0; i<Node_Number; ++i)
  {
    pNew = (struct Node *)malloc(sizeof(struct Node));
    if(NULL==pNew)
    {
      printf("malloc failed!\n");
      exit(0);
    }
    pNew->length = 0;
    pNew->pNext = NULL;     //循环链表末尾放开头
    pLast->pNext = pNew;
    pNew->pPre = pLast;
    
    pLast = pNew;
  }
  if(ifcycle)                         //新节点尾巴连接
  {
    pLast->pNext = pHead->pNext;           //回头
    pHead->pNext->pPre = pLast;           //咬尾
  }
  
  return pHead;
}

//Brief:清除标准打印函数printf的显示区域
//  name:Clear;
//parameters：
//  color：颜色
//Return：
//  void;
//Example:
//    
void Clear(u16 color)
{//可以写成根据plcd_row链表信息清屏，清除的像素点更精准更少，速度更快
  Gpio8080OptLcdFill(std_area.sx,std_area.ex,std_area.sy,std_area.ey,color);
  std_area.x_now = std_area.sx;
  std_area.y_now = std_area.sy;
}

//Brief:设置标准打印函数printf的显示区域
//  一般来说，设置区域的时候是人机交互的时候，不需要快速性，不必写成宏
//  name:SetStdPrintfArea;
//parameters：
//  sx, ex, sy, ey:x和y的起始线和终止线，注意传参顺序，以及不要超出边界
//Return：
//  void;
//Example:
//    
void SetStdPrintfArea(u16 sx,u16 ex,u16 sy,u16 ey)         //设置标准输出的区域
{
  std_area.sx = sx;       //标准输出函数printf打印的范围
  std_area.ex = ex;       //标准输出函数printf打印的范围
  std_area.sy = sy;       //标准输出函数printf打印的范围
  std_area.ey = ey;       //标准输出函数printf打印的范围
}
int scrn_p;
//uint32 std_printf_row = 0;      //打印区域的行数
//Brief:fputc是C语言标准输出函数printf会调用的子函数，printf是一串字符，具体到每个字符的处理就是用的fputc函数，
//  在遥控中使用的输出设备是LCD，要想可以使用printf只需把fputc在LCD上实现即可
//  位置是连续输出的，像电脑上printf一样
//  字体是由STD_LCD_CHARACTER_SIZE宏定义的
//  颜色（包括背景和字符）和显示方向是由lcd结构体定义的
//  该函数是可以滚动显示的，即当显示至最底下一行时，之前的内容将往上移动，新来的内容显示在最后一行
//  name:fputc;
//parameters：
//  ch：标准printf传递进来的字符，包括\n（一个字符）
//  f:咱不清楚
//Return：
//  暂不清楚
//Example:
//    
int fputc(int ch, FILE* f)
{
  //  if(scrn_p)
  //    return 0;
  if(ch == '\n') //换行处理
  {
    std_area.x_now += STD_LCD_CHARACTER_SIZE; //...
    std_area.y_now = std_area.sy; //...
    
    p_now->str[p_now->length] = '\0';
    p_now = p_now->pNext;
    p_now->length = 0;
    return ch;
  }
  if(ch & 0x80) //中文处理
  {  }
  else
  {
    if(std_area.y_now > std_area.ey-STD_LCD_CHARACTER_SIZE/2) //判断是否需要换行
    {
      std_area.x_now += STD_LCD_CHARACTER_SIZE; //换行操作...
      std_area.y_now = std_area.sy; //...
      
      p_now->str[p_now->length] = '\0';
      p_now = p_now->pNext;
      p_now->length = 0;
    }
    uint16 tmp = std_area.ex-STD_LCD_CHARACTER_SIZE;
    if(std_area.x_now > tmp) //超出行数的操作
    {
      uint32 last_length = 0;     //上一行有多少字符
      std_area.x_now -= STD_LCD_CHARACTER_SIZE;
      uint16 i = std_area.sx;
      tmp -= STD_LCD_CHARACTER_SIZE;//最后一次不能画，否则就是未卜先知
      p_now = p_ssnow;
      while(1)          //把之前显示在屏幕上的内容上移一行显示
      {
        if(i>tmp)
          break;
        last_length = p_now->length;
        p_now=p_now->pNext;
        LcdSS(i,std_area.sy,STD_LCD_CHARACTER_SIZE,p_now->str,lcd.pcolr,lcd.bcolr);
        if(last_length>p_now->length)//上一次显示的位置的字符更长，就要去除多余的显示
          Gpio8080OptLcdFill(i,i+STD_LCD_CHARACTER_SIZE,p_now->length*STD_LCD_CHARACTER_SIZE/2+std_area.sy,last_length*STD_LCD_CHARACTER_SIZE/2+std_area.sy,lcd.bcolr);
        i += STD_LCD_CHARACTER_SIZE;
      }
      p_now = p_now->pNext;
      Gpio8080OptLcdFill(i,i+STD_LCD_CHARACTER_SIZE,std_area.sy,std_area.ey,lcd.bcolr);
      p_ssnow = p_ssnow->pNext;         //显示尾节点向前移动一个
      p_roll_start = p_ssnow;           //看定义
      p_last_roll_start = p_ssnow;
      //    显示刚进来的这个字符
      Gpio8080OptLcdSC(std_area.x_now, std_area.y_now,STD_LCD_CHARACTER_SIZE, ch,lcd.pcolr,lcd.bcolr);
      std_area.y_now += STD_LCD_CHARACTER_SIZE/2;
      p_now->str[p_now->length++] = ch;
    }
    else
    {
      Gpio8080OptLcdSC(std_area.x_now, std_area.y_now,STD_LCD_CHARACTER_SIZE, ch,lcd.pcolr,lcd.bcolr);
      std_area.y_now += STD_LCD_CHARACTER_SIZE/2;
      p_now->str[p_now->length++] = ch;         //把该字符存在字符串中
    }
  }
  return ch;
}


//  static PLCDROW origin;
//Brief:用户开始操控滚动条，想看以前的数据，由于是循环列表，这时不能再更新打印缓冲区保留以前的全部数据，丢弃新数据，

//  name:Roll;
//parameters：
//  
//Return：
//  void;
//Example:
//    
void Roll(int increase)
{
  //  static PLCDROW last_pos;
  scrn_p += increase;
  if(scrn_p>0 || NULL==p_roll_start)
  {
    scrn_p = 0;
    return;
  }
  if(increase<0)
    while(increase++)
    {
      if((p_roll_start == p_ssnow && p_last_roll_start!=p_ssnow))
        return ;
      p_roll_start=p_roll_start->pPre;
    }
  else
    while(increase--)
      p_roll_start=p_roll_start->pNext;
  
  Clear(lcd.bcolr);
  PLCDROW ptmp = p_roll_start;
  uint16 tmp = std_area.ex-STD_LCD_CHARACTER_SIZE;
  uint16 i = std_area.sx;
  while(1)
  {
    if(i>tmp)
      break;
    LcdSS(i,std_area.sy,STD_LCD_CHARACTER_SIZE,ptmp->str,lcd.pcolr,lcd.bcolr);
    //      if(p_last_roll_start->length>ptmp->length)//上一次显示的位置的字符更长，就要去除多余的显示
    //        Gpio8080OptLcdFill(i,i+STD_LCD_CHARACTER_SIZE,ptmp->length*STD_LCD_CHARACTER_SIZE/2+std_area.sy,p_last_roll_start->length*STD_LCD_CHARACTER_SIZE/2+std_area.sy,lcd.bcolr);
    ptmp = ptmp->pNext;
    //    p_last_roll_start = p_last_roll_start->pNext;
    i += STD_LCD_CHARACTER_SIZE;
  }
  p_last_roll_start = p_roll_start;
}

#endif //#ifdef STDOUT_LCD

int LcdParamInit()
{
  lcd.pcolr = CYAN;
  
#ifdef STDOUT_LCD
  uint16 sx=c1206+6,sy = c1206,ex = lcd.scrn_x-c1206,ey = lcd.scrn_y-c1206;
  if(sx>LCD_LENGTH-1 || ex>LCD_LENGTH-1 || sy>LCD_LENGTH-1 || ey>LCD_LENGTH-1)//这样来猜测是否输入参数，没有传入则使用默认值
  {
    sx = 12;sy = 5;
    if(lcd.scan_dir<=rldu)
    {
      ex = LCD_WIDTH-12;
      ey = LCD_LENGTH-5;
    }
    else if(lcd.scan_dir<=durl)
    {
      ex = LCD_LENGTH-12;
      ey = LCD_WIDTH-5;
    }
    else
      return 1;
  }
  SetStdPrintfArea(sx, ex, sy, ey);
  //  Clear(PALEGOLDENROD);
  
  //  std_printf_row = (std_area.ex - std_area.sx+1)/STD_LCD_CHARACTER_SIZE;
  //  scrn_p = 0;
  
  plcd_row = CratLcdRowList(NUMOFSHOWROW,true);      //循环链表，储存显示内容
  p_now = plcd_row->pNext;
  p_ssnow = plcd_row->pNext;
#endif //#ifdef STDOUT_LCD
  return 0;
}

