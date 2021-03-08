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
lcd.c�ļ������˶Եײ�Ĳ�����Ҳ����˵����һ��LCDֻ�����ͷ�ļ�����ʹ��
ɨ�跽��ֻ�ǲ���ϳ���дGRAM��ʱ����Ļ�����ʾ�ķ��򣬺ͺ�����û��ϵ
*/


#ifdef STDOUT_LCD               //���ʹ��LCD��Ϊ��׼��ʽ������豸
#define STD_LCD_CHARACTER_SIZE  c1206   //��ʽ������õ�����
STD_PRINTF_AREA std_area;       //��׼�������printf��ӡ�ķ�Χ
#endif


//����LCD��Ҫ����
//Ĭ��Ϊ����
LCD_DEV lcd;                //LCD �������ýṹ��

//LCDдGRAM
//RGB_Code:��ɫֵ
//void LCD_WriteRAM(u16 RGB_Code)
//{
//  GPIOE->CRL = PxOUTPP;
//GPIOE->CRH = PxOUTPP;
//LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
//}

//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);	
  
  return(rgb);
}

//����
//x,y:����
//POINT_COLOR:�˵����ɫstatic inline 
void LcdDrwPoint(u16 x,u16 y, u16 color)
{
  LCD_Area_Set(x, y, x, y);//���ù��λ��       //������һ��ȫ�֣���ȻLCD�������״̬�Զ�����	
  
  //  LCD_SetCursor(x,y);		//���ù��λ��
  LCD_WR_REG(MemoryWrite);	//��ʼд��GRAM
  LCD_WR_DATA(color); 
}
//Brief:��ָ�����������ָ����ɫ
//  �û�������LCD��Ҳ��Ҫ��亯����
//  �����С:(xend-xsta+1)*(yend-ysta+1),���������յ�ᣨ�ݣ�����С�����ᣨ�ݣ����꣬��ô���Զ��жϴ�С����ζ����������sx,ex��С˳��
//  name:LCD_Fill;���ܲ㺯��
//parameters��
//  sx, ex, sy, ey:x��y����ʼ�ߺ���ֹ�ߣ������߽磨�����������մ�С������ע�⴫��˳��
//  color:Ҫ������ɫ
//Return��
//  void;
//Example:
//    LCD_Fill(34,4,6,12,YELLOW);��x��[4,34]��y��[6,12]�ķ�Χ����ɫ
void LCD_Fill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{//������򳬹�Һ�������ֵ�����ử���������֣����ǲ���С��0����Ϊ���޷������ͣ����������������
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
  //  sx = sx             //�߽��飺�Ƿ���Ҫ�߽����Լ������߽���ʾ���ķ�Ӧ��Ҫ�����ֲ�
  
  AreaFill(sx, ex, sy, ey, color);
}

//Brief:��ָ�����������ָ����ɫ
//  �û�������LCD��Ҳ��Ҫ��亯����
//  �����С:(xend-xsta+1)*(yend-ysta+1),���������յ�ᣨ�ݣ�����С�����ᣨ�ݣ����꣬��ô���Զ��жϴ�С����ζ����������sx,ex��С˳��
//  name:LCD_Fill;�����㺯��
//parameters��
//  sx, ex, sy, ey:x��y����ʼ�ߺ���ֹ�ߣ������߽磨�����������մ�С������ע�⴫��˳��
//  color:Ҫ������ɫ
//Return��
//  void;
//Example:
//    Gpio8080OptLcdFill(34,4,6,12,YELLOW);//��x��[4,34]��y��[6,12]�ķ�Χ����ɫ
void Gpio8080OptLcdFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{//������򳬹�Һ�������ֵ�����ử���������֣����ǲ���С��0����Ϊ���޷������ͣ����������������
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
  //  sx = sx             //�߽��飺�Ƿ���Ҫ�߽����Լ������߽���ʾ���ķ�Ӧ��Ҫ�����ֲ�
  
  uint32 total = (ex-sx+1)*(ey-sy+1);
  Gpio8080OptLcdAS(sx, ex, sy, ey);	                //����areald->y_max-1  
  LCD_RS = 0;//  LCD_WR_REG(MemoryWrite);     			//��ʼд��GRAM
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

//Brief:����б��ֱ�ߺ�����
//  ��Ҫ������һ����������ķ�ʽ������������һ�����أ������������������㣬���Ǻ���������������ķ��򣬾ٸ����˵�����ӣ������������ز����8�����ص�����������ߵ�ֻ���ó�2��
//  �û�������LCD��Ҳ��Ҫ��亯����
//  �����С:(xend-xsta+1)*(yend-ysta+1),���������յ�ᣨ�ݣ�����С�����ᣨ�ݣ����꣬��ô���Զ��жϴ�С����ζ����������sx,ex��С˳��
//  name:LCD_Fill;�����㺯��
//parameters��
//  sx, ex, sy, ey:x��y����ʼ�ߺ���ֹ�ߣ������߽磨�����������մ�С������ע�⴫��˳��
//  color:Ҫ������ɫ
//Return��
//  void;
//Example:
//    Gpio8080OptLcdFill(34,4,6,12,YELLOW);//��x��[4,34]��y��[6,12]�ķ�Χ����ɫ
//x1,y1:�������
//x2,y2:�յ�����  
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
  delta_x=x2-x1;                //������������ 
  delta_y=y2-y1;                
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1;          //���õ������� 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else{incy=-1;delta_y=-delta_y;} 
  if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )//������� 
  {
    LcdDrwPoint(uRow,uCol, color);//���� 
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

//�������ܣ�������
//������
//x��ʼ������
//y:��ʼ������
//c��X/Y ����/���򣨴�Сд���ޣ�
//len0���ܳ���
//len1�����߳���
//len2���հ׳���
//color��������ɫ
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

//Brief:��������������,�������������
//  name:DrawTh(throttle)Line,
//  ����ģ���ʵ�֣�
//Parameters:
//  line_info��
//      x,y:������꣬x�������������ƣ�y������Ԥ�����õĿ��(size����)��x,y�Կ���ԭ���Ϊʼ,΢���Գ����߽�����
//      x_len:������¼�ϴεĳ��ȣ�ֻ��һ����������ʵ�������÷���
//      y_len:���������
//      color����ɫ;
//  dir:����,0:x+; 1:y+; 2:x-; 3:y-;
//Return:
//  void��
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
      if(line_info->x_len * len<=0)//���ʱ������ԭ�ȷ�Χ�ڵ��߶�
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
      if(line_info->y_len * len<=0)//���ʱ������ԭ�ȷ�Χ�ڵ��߶�
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

//�����ο�	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
  Gpio8080OptLcdFill(x1,x2,y1,y1, color);
  Gpio8080OptLcdFill(x1,x1,y1,y2, color);
  Gpio8080OptLcdFill(x1,x2,y2,y2, color);
  Gpio8080OptLcdFill(x2,x2,y1,y2, color);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 color)
{
  int a,b;
  int di;
  a=0;b=r;	  
  di=3-(r<<1);             //�ж��¸���λ�õı�־
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
    //ʹ��Bresenham�㷨��Բ     
    if(di<0)di +=4*a+6;	  
    else
    {
      di+=10+4*(a-b);   
      b--;
    } 						    
  }
} 									  

//Brief:��ָ��꣬������
//  name:StdLcdSC=standard LCD Show Char,��׼�ģ�������������ƽ̨���õģ���ָ��λ����ʾһ���ַ�
//  ����ģ���ʵ�֣�ʱ���Ż��ٶȿ�,����鳬��LCD�߽磬��Ҫ�û�ע�⣻���û�����
//Parameters:
//  x,y:ָ�������꣬x��y�����ƣ�x�����߽��Զ����У�y�����߽�ص�0
//  size:length of the pointer ��|<��|��,�������ĳ���
//  point_direction��������ָ����0:x+; 1:y+; 2:x-; 3:y-;�����˳�
//  color��������ɫ;
//Return:
//  void��
//Example:
//    //
void LCDDrawPointer(u16 x,u16 y, u16 size,char point_direction,u16 color)
{
  assert(size>0);
  size -= 1;    //��Ϊ���������β�����䣬�ȳ��ȶ�һ�������Լ�1
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
//  name:Std(standard)LcdS(Show)C(Char),��׼�ģ�������������ƽ̨���õģ���ָ��λ����ʾһ���ַ�
//  ����ģ���ʵ�֣�ʱ���Ż��ٶȿ�,����鳬��LCD�߽磬��Ҫ�û�ע�⣻���û�����
//Parameters:
//  x,y:������꣬Ӧ�����ַ��������϶��㣻x��y�����ƣ�x�����߽��Զ����У�y�����߽�ص�0
//  size:�����С������鿴CHAR_SIZEö�٣�
//  num:�ַ�����ָ��("")���������κ���ʾ
//  color��������ɫ;
//  bcolor:����ɫ;
//Return:
//  0:No problems;
//  1:No Font;
//Example:
//    StdLcdSC(10, 20,c1206, 'A',CYAN,BLACK);//��10,10�����ַ����Ͻ�λ�ã���CYAN��BLACK����ʾA
char StdLcdSC(u16 x,u16 y, CHAR_SIZE size, u8 num, u16 color, u16 bcolor)
{
  signed char t;
  u8 csize;	//�õ�����һ���ַ���Ӧ������ռ���ֽ���
  num = num - ' ';                                //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
  
  const unsigned char *pfont;   //ָ���ֿ�ĳ���ַ���ָ�� 
  switch (size)
  {
  case c1206: 	//����1206����
    {
      csize = 12;               //һ���ַ���Ӧ������ռ���ֽ���
      pfont=asc2_1206[num];
    }break;
  case c1608:	//����1608����
    {
      csize = 16;               //һ���ַ���Ӧ������ռ���ֽ���
      pfont=asc2_1608[num];
    }break;
  case c2412:	//����2412����
    {
      csize = 36;               //һ���ַ���Ӧ������ռ���ֽ���
      pfont=asc2_2412[num];
    }break;
  default:return 1;
  }
  
  uint16 k;
  u8 r = 0;
  
  for(t=0;t<csize;t++)
  {
    num = pfont[t];                     //num��ʱ��Ϊ��ʱ�������ˣ�
    k=8;
    while(k--)
    {
      if(num&0x80)
        LcdDrwPoint(x+r,y,color);
      else
        LcdDrwPoint(x+r,y,bcolor);
      num<<=1;
      r++;
      if(r>=size)        //�ж��Ƿ񵽴��˵ײ�����1206��һ��ֻ��12�����أ���6�У����������ֽڴ棬�ڶ����ֽڵĺ���λ�����õ�
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
//  name:Gpio8080Opt��optimized��LcdS��Show��C(Char)=���GPIOģ��8080ʱ���Ż�������ַ���ʾ����,
//  ר�õ�������ʵ�֣�����LCDԤ�ú��������ʱ�����߽��Զ������ص㣬�Լ���������ʱ����м��٣���ָ��λ����ʾһ���ַ���
//  ����鳬��LCD�߽磬��Ҫ�û�ע�⣻���û�����
//Parameters:
//  x,y:������꣬Ӧ�����ַ��������϶��㣻x��y�����ƣ�x�����߽��Զ����У�y�����߽�ص�0
//  size:�����С������鿴CHAR_SIZEö�٣�
//  num:�ַ�����ָ��("")���������κ���ʾ
//  color��������ɫ;
//  bcolor:����ɫ;
//Return:
//  0:No problems;
//  1:No Font;
//Example:
//      Gpio8080OptLcdSC(10, 20,c1206, 'A',CYAN,BLACK);//��10,10�����ַ����Ͻ�λ�ã���CYAN��BLACK����ʾA
char Gpio8080OptLcdSC(u16 x,u16 y, CHAR_SIZE size, u8 num, u16 color, u16 bcolor)
{
  signed char t;
  u8 csize;	//�õ�����һ���ַ���Ӧ������ռ���ֽ���
  num = num - ' ';                                //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
  
  const unsigned char *pfont;   //ָ���ֿ�ĳ���ַ���ָ�� 
  switch (size)
  {
  case c1206: 	//����1206����
    {
      csize = 12;               //һ���ַ���Ӧ������ռ���ֽ���
      pfont=asc2_1206[num];
    }break;
  case c1608:	//����1608����
    {
      csize = 16;               //һ���ַ���Ӧ������ռ���ֽ���
      pfont=asc2_1608[num];
    }break;
  case c2412:	//����2412����
    {
      csize = 36;               //һ���ַ���Ӧ������ռ���ֽ���
      pfont=asc2_2412[num];
    }break;
  default:return 1;
  }
  Gpio8080OptLcdAS(x, x+size-1, y, y+size/2-1);//���ù��λ�� 	
  LCD_RS = 0;//  LCD_WR_REG(MemoryWrite);     			//��ʼд��GRAM
  DATAOUT(MemoryWrite);
  LCD_WR = 0;
  LCD_WR = 1;
  
  x = 0;
  LCD_RS = 1;
  for(t=0;t<csize;t++)
  {
    num = pfont[t];                     //num��ʱ��Ϊ��ʱ�������ˣ�
    y=8;
    while(y--)
    {
      if(num&(0x80))
        DATAOUT(color); //��ɫ��ͬ�Ļ���д�˿ڶ�������
      else
        DATAOUT(bcolor);
      LCD_WR = 0;
      num<<=1;
      x++;
      LCD_WR = 1;
      if(x-size>=0)        //�ж��Ƿ񵽴��˵ײ�����1206��һ��ֻ��12�����أ���6�У����������ֽڴ棬�ڶ����ֽڵĺ���λ�����õ�
      {
        x=0;
        break;
      }
    }
  }
  LCD_CS = 1;
  return 0;
}


//Brief:��ʾ�ַ������Զ����У���������У��Զ�������ʼ�ַ���
//  name:LcdS(Show)S(String);
//Parameters:
//  x,y:������꣬Ӧ�����ַ��������϶��㣻x��y�����ƣ�x�����߽��Զ����У�y�����߽�ص�0
//  size:�����С��
//  mode��1���Ǹ��Ƿ�ʽ��0�����Ƿ�ʽ���Ǹ����ٶȽϿ�
//  *p:�ַ�������ָ��("")���������κ���ʾ
//  color��������ɫ;
//  bcolor:����ɫ;
//Return:
//  void;
//Example:
//    LcdSS(2, 3, c1206, "abc", RED, BLACK); //��2,3λ������1206���壬�����ǵķ�ʽ�����ֺڵ���ʾabc
//char mmm = 0;
char LcdSS(u16 x, u16 y, CHAR_SIZE size, const char *p, u16 color, u16 bcolor)
{//x��y�ǻ�ÿ���ַ�������
  //  uint16 point[2]={x,y};                //��ÿ�����ص������
  //  u16 y_start;
  //  u8 csize;	//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
  //  u8 ch;
  //  u8 temp,t1,t;
  //  const unsigned char *pfont;   //ָ���ֿ�ĳ���ַ���ָ�� 
  
  if(y>lcd.scrn_y - size/2)//��Ȼ������������ʾ�������ôӶ��ܿ�����Գ�����ʾ�߽�ļ�飬���ǻ�����ʾ���أ����Ի��Ǽ��
    y = 0;                              //y�����߽�����Ϊ0������Ҳ�ǰ�0����
  u16 y_start = y;
  while((*p<='~')&&(*p>=' '))           //�ж��ǲ��ǷǷ��ַ�!�������Ƿ��ַ�������ʾ
  {
    if(y>lcd.scrn_y - size/2)         //����y��Χ
    {
      y = y_start;                             //������ʼ�ַ���
      x+=size;                          //����
    }
    if(x>lcd.scrn_x-size)            //�޷������Ͳ��ü�鸺����y������Χ��0
    {
      x = 0;
    }
    Gpio8080OptLcdSC(x, y, size, *p,color, bcolor);
    y+=size/2;
    p++;
  }
  return 0;
}

//Brief:����Ļ��ָ��λ��(x,y�������ݽ�����λ��Ҫ����δ����ʼ�����ص�)����ָ����������ɫ�͵�λ��ɫ��ʾһ������λ�����ݣ����������Σ������Ǹ����ͣ������е�λҲ���ޣ��޵�λ��ָ����λΪ""
//  ������ƿ��ǣ��ض���������Ļ�ϵ���ʾλ��һ���ǲ���ģ��ýṹ����Լ��ٶ��βε�ѹջ������������ʾ������
//  �����Ƕ������ͣ���������һ�ֽṹ������ֵ�������3���βδ���;���ݴӼ��ٱ�ɼ�ʮ�����������Ƚ������仯����������������������Ƕ���
//  ������ʾӦ���������Ҳ�Ϊ��׼�����Ӽ���ɼ�ʮ��λ���ڵ���ʾλ�ò�Ӧ�ñ䣬��λ�ӱ仯Ҳ��������λ�ã���Ȼ�������εú�
//  name:LCDShowVar(Variable);
//Parameters:
//  pd_p:��ʾ����Ϣ�����꣬��С����ɫ��;
//  value:��ֵ��֮������float��intתfloat��ʧ���ȣ�doubleתfloat��Ȼ��ʧ���ȣ�����ֻ��ʾ��Ӱ��;
//Return:
//  void;
//Example:
//    LCDShowVar(&bty_votg_lcd_info,bat_voltage,0,"V");//�ṹ������ʾ��Ϣ����ʾ��ص�ѹ��С�����0λ������������λΪV
void LCDShowVar(PLCD_VAR_INFO pd_p, float value)
{
  char str[16] = {0};
  char tmp[16] = "";
  snprintf(str,16,"%.*f",pd_p->acr_desm,value);
  uint8 len = strlen(str);
  short derta = pd_p->last_len - len;
  if(derta>0)                   //���ε����ݳ��ȱ���ǰ��С
  {
    len += derta;               //����ֱ��ʡ�Զ�������ʹ�õ�len����last_length����Ϊ��ʼʱlast_length������0������
    while(derta--)
      strcat(tmp," ");          //��ǰ�油�ɿո�
  }
  strcat(tmp,str);
  LcdSS(pd_p->x, pd_p->y-len*pd_p->size/2, pd_p->size,tmp, pd_p->data_colr,lcd.bcolr);
  LcdSS(pd_p->x, pd_p->y+1, pd_p->size,pd_p->unit, pd_p->unit_colr, lcd.bcolr);
  pd_p->last_len = strlen(str);
}

//#define FDATA_LEN 300
//short DATA_STORAGED[FDATA_LEN], DELTA_DATA[FDATA_LEN];      //������ʾ5������160�����ݵĺ���,DELTA_DATA���ڲ�����ص����ݴ洢
//Brief:
//  ������ģʽ��һ���ǴӸ���������������е�������ʾ�ɺ������ߣ�����֧�����鳤��С�ڵ��ں����������ص�������������ʱ�ú�����������Ԫ��ƽ���ֿ��ں����귶Χ��
//  �ڶ�����ɨ��ģʽ������������������һά��ʱ��(����Ԫ��Ҳ����)�����ģʽ,ɨ��ģʽ������ģʽ����ͨ�ã���Ϊ����ģʽֻ��Ҫ����ڵ���ڴ棬��ɨ��ģʽ��Ҫ����������ʾ����ȵ��ڴ�
//  �������ߵ���ʾ������ͼ�������õ����䷶Χ����ʾ�������������߼���ͼ����ʱ���Ҫ���������ܣ�
//  ��һ��������ʾ������scanģʽ����������ʾ�͹��������ݵ�;
//  û��Ҫ���������鶼���������������Žڵ���ʾ������ȫֻ��Ҫ����ʾ��˼�����������ɰ�����������һ�£�˭������ô��һ�����͹�����ʾ��
//  ���ձ���ڵĲ���һ��һ�����͹�����ʾ�𣿼�ʹ�û�����һ������������������ǲ��Ǹ��ŷ���һ������ռ��أ���Ȼ���ǣ�ֻ��Ҫ����ʾ�ĵ��ס����
//  ��Ҫ���ǵ�һ�����û������������ݵ���ܺ�ϡ�裬�����ò�ֱ�ߵİ취�������ģ����Ǽ�¼�ĵ����϶�������Ļ�����ĵ�����
//  ��ʾ�߿���ʾ��ֵ����ʾ�����ߵ�
//  name:Statc(static)ShowF(function)();��̬�ĺ���������ʾ��������һ���������ʾ��Ϊ��̬��
//sx,ex,sy,ey:��ʾ�����򣬲������߽磬����̫�������һ���ؼ�
//y:��ʾ��ʼ������
//parr������������
//hang����ʾ��������������ĵڼ���
//compress_index��������ѹ����
//slice���Ƿ���ֱ��������ɢ��
//  ��1������ֱ�����ӡ���0����������
//color����ʾ��ɫ
char JstShowF(PLCD_CRV_INFO crv)
{
  if(false==crv->pan.iffil)
    LCD_DrawRectangle(crv->pan.sx,crv->pan.sy,crv->pan.sx+crv->pan.x_len-(crv->pan.x_len>0)+(crv->pan.x_len<0),crv->pan.sy+crv->pan.y_len-(crv->pan.y_len>0)+(crv->pan.y_len<0),crv->pan.colr);
  else
    Gpio8080OptLcdFill(crv->pan.sx,crv->pan.sx+crv->pan.x_len-(crv->pan.x_len>0)+(crv->pan.x_len<0),crv->pan.sy,crv->pan.sy+crv->pan.y_len-(crv->pan.y_len>0)+(crv->pan.y_len<0),crv->pan.colr);
  //  uint16 idx = 0;  //�������������±�ƫ����ʱ����
  
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
  //      if(idx)   //�ӵڶ����㿪ʼ�����ߣ�����֮ǰ�����ֱ�ߣ��ػ����ڵ����ߣ���֮ǰ�����굽���ڵ�����
  //      {
  //        LCD_DrawLine(last_stor_dat, lasty, crv->stor_dat[idx], y, lcd.bcolr);
  //        LCD_DrawLine(crv->stor_dat[idx-1], lasty, tmp_ops, y, crv->crvcolr);
  //      }
  //      last_stor_dat = crv->stor_dat[idx];//����������㣬��һ�ε�stor_dat�Ѿ��ڳ�����󱻸����ˣ�����Ҫȡ��������
  //      lasty = y;
  //    }
  //    else
  //    {
  //    if(crv->stor_dat[idx]>crv->pan.sx && crv->stor_dat[idx]<(crv->pan.sx+crv->pan.x_len-1))
  //      LcdDrwPoint(crv->stor_dat[idx], y, lcd.bcolr);
  //    if(tmp_ops>crv->pan.sx && tmp_ops<(crv->pan.sx+crv->pan.x_len-1))
  //      LcdDrwPoint(tmp_ops, y, crv->crvcolr);    //������������Ϊ���Ժ��е��Ǹ�λ�û���һ���㣬����Ҫ�����ݼ�¼�����ģ���Ȼ��ȫֱ�ߵ�ʱ���û������
  //    }
  //    crv->stor_dat[idx] = tmp_ops;
  //  }
  return 0;
}

//Brief:����Ļָ�����򻭶�̬�������ߣ���̬֮��Ϊ�����ݽ�׷��������ĩβ,ע�������ǰ������ص���е�
//  name:ScanShowF(function),ɨ���Եػ��ƺ������ߣ�
//  ����ģʽ:��ģʽ(������ɢ��),��ͨģʽ(��ֱ�߽�����ɢ��������)���ڽṹ�����������
//  �ú����Ǻ�����Ļ�ֱ�ߣ�һ��ֱ�����յ����⣬��˺�����ָ���߿򣬲��ô����ڸú����ڴ���ǳ����ӣ��ڻ�ֱ�ߺ����ڴ�����ֻ��������������ĸ�����
//  �˺������յ�������һ�����ص㣬��ˣ�������д��ֱ��
//  ����ֱ�ߵ����������߽粻��
//  ����ģ���ʵ�֣�
//Parameters:
//  crv:����������ؽṹ��ָ��
//  data:���ε����ݣ����ᱻ��ʾ����Ļ�ϲ��Ҽ�¼�ڽṹ��������
//Return:
//  void��
//Example:
//  ScanShowF(p_thcrv,(u16)(((float)th_adc/TH_ADC_MAX)*255));//
void ScanShowF(PSCAN_CRV_INFO crv,short data)
{
  static u16 last_data;//last_stor_dat������֮ǰ�����߶ε��׶˵������꣬last_data����һ����ʾ�ĵ����꣬��������ε�ֱ�ߵ�
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
    if(crv->last_n->pNext!=crv->sdatli->pNext)   //�����Ǵ�kλ�õ㵽k+1λ�õģ��ֳ�2���߶Σ��κ�һ�γ�����Χ���������߽�Ϊֹ
    {
      tmp1 = crv->last_n->u16data;                //k���ص����λ��
      tmp3 = crv->last_n->pNext->u16data;          //k+1���ص���λ��
      tmp2 = (tmp1+tmp3)>>1;                       //�м�λ��
      if(tmp1>rag_l && tmp1<rag_h)   //��һ�Σ�kλ�ã��߶Σ��˵�1�ڷ�Χ�ڣ�
      {
        if(tmp2<=rag_l)           //�˵�1�ڷ�Χ������Ҫ�鿴�˵�2�Ƿ��ޣ���������λ
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos,crv->pos, lcd.bcolr);   //��һ���߶�
      }
      else if(tmp2>rag_l && tmp2<rag_h)   //��һ�Σ�kλ�ã��߶Σ��˵�2�ڷ�Χ�ڣ�
      {
        if(tmp1<=rag_l)
          tmp1 = rag_l+1;
        else if(tmp1>=rag_h)
          tmp1 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos,crv->pos, lcd.bcolr);
      }
      else;//������ڷ�Χ���򲻻���
      if(tmp2>rag_l && tmp2<rag_h)   //�ڶ��Σ�k+1λ�ã��߶Σ��˵�1�ڷ�Χ�ڣ�
      {
        if(tmp3<=rag_l)
          tmp3 = rag_l+1;
        else if(tmp3>=rag_h)
          tmp3 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos+1,crv->pos+1, lcd.bcolr);
      }
      else if(tmp3>rag_l && tmp3<rag_h)   //�ڶ��Σ�k+1λ�ã��߶Σ��˵�2�ڷ�Χ�ڣ�
      {
        if(tmp2<=rag_l)
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos+1,crv->pos+1, lcd.bcolr);
      }
      else;//������ڷ�Χ���򲻻���
    }
    if(crv->last_n!=crv->sdatli->pNext)   //�ӵڶ����㿪ʼ�����ߣ��ػ����ڵ����ߣ���֮ǰ�����굽���ڵ�����
    {
      tmp1 = last_data;
      tmp3 = tmp_ops;
      tmp2 = (tmp1+tmp3)>>1;
      if(tmp1>rag_l && tmp1<rag_h)
      {
        if(tmp2<=rag_l)
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos-1,crv->pos-1, crv->crv_ifo.crvcolr);
      }
      else if(tmp2>rag_l && tmp2<rag_h)
      {
        if(tmp1<=rag_l)
          tmp1 = rag_l+1;
        else if(tmp1>=rag_h)
          tmp1 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp1,tmp2,crv->pos-1,crv->pos-1, crv->crv_ifo.crvcolr);
      }
      else;//������ڷ�Χ���򲻻���
      if(tmp2>rag_l && tmp2<rag_h)
      {
        if(tmp3<=rag_l)
          tmp3 = rag_l+1;
        else if(tmp3>=rag_h)
          tmp3 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos,crv->pos, crv->crv_ifo.crvcolr);
      }
      else if(tmp3>rag_l && tmp3<rag_h)
      {
        if(tmp2<=rag_l)
          tmp2 = rag_l+1;
        else if(tmp2>=rag_h)
          tmp2 = rag_l+abs(crv->crv_ifo.pan.x_len)-2;            //Ҫ��ܴ�ֵ���ܱ����Сֵ���߸��������򽫻�������ֱ��
        Gpio8080OptLcdFill(tmp2,tmp3,crv->pos,crv->pos, crv->crv_ifo.crvcolr);
      }
      else;//������ڷ�Χ���򲻻���
    }
    last_data = tmp_ops;//�����������л�������
  }
  else
  {
    if(crv->last_n->u16data>crv->crv_ifo.pan.sx && crv->last_n->u16data<(crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0)))
      LcdDrwPoint(crv->last_n->u16data, crv->pos, lcd.bcolr);
    if(tmp_ops>crv->crv_ifo.pan.sx && tmp_ops<(crv->crv_ifo.pan.sx+crv->crv_ifo.pan.x_len-(crv->crv_ifo.pan.x_len>0)+(crv->crv_ifo.pan.x_len<0)))
      LcdDrwPoint(tmp_ops, crv->pos, crv->crv_ifo.crvcolr);    //������������Ϊ���Ժ��е��Ǹ�λ�û���һ���㣬����Ҫ�����ݼ�¼�����ģ���Ȼ��ȫֱ�ߵ�ʱ���û������
  }
  if(crv->pos<crv->crv_ifo.pan.sy+abs(crv->crv_ifo.pan.y_len)-2)//�Ƿ񳬹��߽�
    crv->pos++;
  else
    crv->pos = crv->crv_ifo.pan.sy+1;
  crv->last_n->u16data = tmp_ops;       //��¼�ýڵ��������������
  crv->last_n = crv->last_n->pNext;     //������ѭ���ľ�ֱ��ָ����һ��λ��
}

#ifdef STDOUT_LCD                               //���ʹ��LCD��Ϊ��׼��ʽ������豸
static PLCDROW p_now;
static PLCDROW p_ssnow;       //show start    //����ʱ������
#endif //#ifdef STDOUT_LCD

//Brief:ָ����ɫ����������ģ��㺯�������ָ����LCD��Ϊ��׼����豸�������ڵĴ�ӡλ������
//  name:LCD_Fill;
//parameters��
//  color:Ҫ������ɫ
//Return��
//  void;
//Example:
//    LCD_Clear(YELLOW);�û�ɫ����
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
#ifdef STDOUT_LCD                               //���ʹ��LCD��Ϊ��׼��ʽ������豸
  std_area.x_now = std_area.sx;
  std_area.y_now = std_area.sy;
  p_now->length = 0;
  p_ssnow = p_now;
#endif //#ifdef STDOUT_LCD
}

//Brief:ָ����ɫ����������ģ��㺯��,���ָ����LCD��Ϊ��׼����豸�������ڵĴ�ӡλ������
//  ������ʵ�֣�����LCDԤ�ú��������ʱ�����߽��Զ������ص㣬�Լ���������ʱ����м���
//  name:Gpio8080OptLcdClr;
//parameters��
//  color:Ҫ������ɫ
//Return��
//  void;
//Example:
//    LCD_Clear(YELLOW);�û�ɫ����
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
  Gpio8080OptLcdAS(0, end_x, 0, end_y);	                //����areald->y_max-1  
  LCD_RS = 0;//  LCD_WR_REG(MemoryWrite);     			//��ʼд��GRAM
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
#ifdef STDOUT_LCD                               //���ʹ��LCD��Ϊ��׼��ʽ������豸
  std_area.x_now = std_area.sx;
  std_area.y_now = std_area.sy;
  p_now->length = 0;
  p_ssnow = p_now;
#endif //#ifdef STDOUT_LCD
}

#ifdef STDOUT_LCD                               //���ʹ��LCD��Ϊ��׼��ʽ������豸
#include <stdlib.h>

PLCDROW plcd_row;
static PLCDROW p_roll_start;                  //roll ʱ��ʼ�Ľڵ�
static PLCDROW p_last_roll_start;             //����������

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
  //ͷ������ݳ�ʼ��
  pHead->str[0] = '\0';			//��ʱ����֪���ô��ʲô����
  pHead->pPre = NULL;
  pHead->pNext = NULL;
  pLast = pHead;				//��Ϊ���ͷ�����Ҫ���صģ�ֱ��return���ܱ䣬������һ���±���
  
  Node_Number = NUMOFSHOWROW;
  //	printf("������ڵ����:");
  //	scanf("%d", &Node_Number);
  //	if(0==Node_Number)
  //	{
  //		printf("������Ϊ��!\n");
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
    pNew->pNext = NULL;     //ѭ������ĩβ�ſ�ͷ
    pLast->pNext = pNew;
    pNew->pPre = pLast;
    
    pLast = pNew;
  }
  if(ifcycle)                         //�½ڵ�β������
  {
    pLast->pNext = pHead->pNext;           //��ͷ
    pHead->pNext->pPre = pLast;           //ҧβ
  }
  
  return pHead;
}

//Brief:�����׼��ӡ����printf����ʾ����
//  name:Clear;
//parameters��
//  color����ɫ
//Return��
//  void;
//Example:
//    
void Clear(u16 color)
{//����д�ɸ���plcd_row������Ϣ��������������ص����׼���٣��ٶȸ���
  Gpio8080OptLcdFill(std_area.sx,std_area.ex,std_area.sy,std_area.ey,color);
  std_area.x_now = std_area.sx;
  std_area.y_now = std_area.sy;
}

//Brief:���ñ�׼��ӡ����printf����ʾ����
//  һ����˵�����������ʱ�����˻�������ʱ�򣬲���Ҫ�����ԣ�����д�ɺ�
//  name:SetStdPrintfArea;
//parameters��
//  sx, ex, sy, ey:x��y����ʼ�ߺ���ֹ�ߣ�ע�⴫��˳���Լ���Ҫ�����߽�
//Return��
//  void;
//Example:
//    
void SetStdPrintfArea(u16 sx,u16 ex,u16 sy,u16 ey)         //���ñ�׼���������
{
  std_area.sx = sx;       //��׼�������printf��ӡ�ķ�Χ
  std_area.ex = ex;       //��׼�������printf��ӡ�ķ�Χ
  std_area.sy = sy;       //��׼�������printf��ӡ�ķ�Χ
  std_area.ey = ey;       //��׼�������printf��ӡ�ķ�Χ
}
int scrn_p;
//uint32 std_printf_row = 0;      //��ӡ���������
//Brief:fputc��C���Ա�׼�������printf����õ��Ӻ�����printf��һ���ַ������嵽ÿ���ַ��Ĵ�������õ�fputc������
//  ��ң����ʹ�õ�����豸��LCD��Ҫ�����ʹ��printfֻ���fputc��LCD��ʵ�ּ���
//  λ������������ģ��������printfһ��
//  ��������STD_LCD_CHARACTER_SIZE�궨���
//  ��ɫ�������������ַ�������ʾ��������lcd�ṹ�嶨���
//  �ú����ǿ��Թ�����ʾ�ģ�������ʾ�������һ��ʱ��֮ǰ�����ݽ������ƶ���������������ʾ�����һ��
//  name:fputc;
//parameters��
//  ch����׼printf���ݽ������ַ�������\n��һ���ַ���
//  f:�۲����
//Return��
//  �ݲ����
//Example:
//    
int fputc(int ch, FILE* f)
{
  //  if(scrn_p)
  //    return 0;
  if(ch == '\n') //���д���
  {
    std_area.x_now += STD_LCD_CHARACTER_SIZE; //...
    std_area.y_now = std_area.sy; //...
    
    p_now->str[p_now->length] = '\0';
    p_now = p_now->pNext;
    p_now->length = 0;
    return ch;
  }
  if(ch & 0x80) //���Ĵ���
  {  }
  else
  {
    if(std_area.y_now > std_area.ey-STD_LCD_CHARACTER_SIZE/2) //�ж��Ƿ���Ҫ����
    {
      std_area.x_now += STD_LCD_CHARACTER_SIZE; //���в���...
      std_area.y_now = std_area.sy; //...
      
      p_now->str[p_now->length] = '\0';
      p_now = p_now->pNext;
      p_now->length = 0;
    }
    uint16 tmp = std_area.ex-STD_LCD_CHARACTER_SIZE;
    if(std_area.x_now > tmp) //���������Ĳ���
    {
      uint32 last_length = 0;     //��һ���ж����ַ�
      std_area.x_now -= STD_LCD_CHARACTER_SIZE;
      uint16 i = std_area.sx;
      tmp -= STD_LCD_CHARACTER_SIZE;//���һ�β��ܻ����������δ����֪
      p_now = p_ssnow;
      while(1)          //��֮ǰ��ʾ����Ļ�ϵ���������һ����ʾ
      {
        if(i>tmp)
          break;
        last_length = p_now->length;
        p_now=p_now->pNext;
        LcdSS(i,std_area.sy,STD_LCD_CHARACTER_SIZE,p_now->str,lcd.pcolr,lcd.bcolr);
        if(last_length>p_now->length)//��һ����ʾ��λ�õ��ַ���������Ҫȥ���������ʾ
          Gpio8080OptLcdFill(i,i+STD_LCD_CHARACTER_SIZE,p_now->length*STD_LCD_CHARACTER_SIZE/2+std_area.sy,last_length*STD_LCD_CHARACTER_SIZE/2+std_area.sy,lcd.bcolr);
        i += STD_LCD_CHARACTER_SIZE;
      }
      p_now = p_now->pNext;
      Gpio8080OptLcdFill(i,i+STD_LCD_CHARACTER_SIZE,std_area.sy,std_area.ey,lcd.bcolr);
      p_ssnow = p_ssnow->pNext;         //��ʾβ�ڵ���ǰ�ƶ�һ��
      p_roll_start = p_ssnow;           //������
      p_last_roll_start = p_ssnow;
      //    ��ʾ�ս���������ַ�
      Gpio8080OptLcdSC(std_area.x_now, std_area.y_now,STD_LCD_CHARACTER_SIZE, ch,lcd.pcolr,lcd.bcolr);
      std_area.y_now += STD_LCD_CHARACTER_SIZE/2;
      p_now->str[p_now->length++] = ch;
    }
    else
    {
      Gpio8080OptLcdSC(std_area.x_now, std_area.y_now,STD_LCD_CHARACTER_SIZE, ch,lcd.pcolr,lcd.bcolr);
      std_area.y_now += STD_LCD_CHARACTER_SIZE/2;
      p_now->str[p_now->length++] = ch;         //�Ѹ��ַ������ַ�����
    }
  }
  return ch;
}


//  static PLCDROW origin;
//Brief:�û���ʼ�ٿع��������뿴��ǰ�����ݣ�������ѭ���б���ʱ�����ٸ��´�ӡ������������ǰ��ȫ�����ݣ����������ݣ�

//  name:Roll;
//parameters��
//  
//Return��
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
    //      if(p_last_roll_start->length>ptmp->length)//��һ����ʾ��λ�õ��ַ���������Ҫȥ���������ʾ
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
  if(sx>LCD_LENGTH-1 || ex>LCD_LENGTH-1 || sy>LCD_LENGTH-1 || ey>LCD_LENGTH-1)//�������²��Ƿ����������û�д�����ʹ��Ĭ��ֵ
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
  
  plcd_row = CratLcdRowList(NUMOFSHOWROW,true);      //ѭ������������ʾ����
  p_now = plcd_row->pNext;
  p_ssnow = plcd_row->pNext;
#endif //#ifdef STDOUT_LCD
  return 0;
}

