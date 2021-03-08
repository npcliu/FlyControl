#include <stdarg.h>
#include "stm32f10x.h"          //关于底层实现的库，此文件对应的lcd用不同的控制器控制时，只需切换包含的库即可
#include "delay.h"              //关于底层实现的库，此文件对应的lcd用不同的控制器控制时，只需切换包含的库即可

#include "ILI9341.h"
//#include "font.h"
/***********内部变量**************/
//static scan_dir deault_scan_dir = lrud;
//static u16 deft_draw_color;         //默认显示颜色，default draw color
//static u16 deft_back_color;        //默认底色    ，default background color

/*
2017年4月3日14:50:03
写命令（Write command code）；按照写命令的时序规则往“并行数据总线”写一串数据（该数据代表命令）
例如：Lcd_Write_Command(0x11)； 查阅ILI9341手册:0x11命令表示解除睡眠状态
*/
void LCD_Write_Command(u16 order)//static inline 
{
  //  DATAOUT(order);
  GPIOE->CRL = PxOUTPP;//PTB_BASE_PTR->PDDR = 0xffffffff;      //设置PTB所有端口方向为输出
  GPIOE->CRH = PxOUTPP;//PTB_BASE_PTR->PDDR = 0xffffffff;      //设置PTB所有端口方向为输出
  LCD_CS = 0;
  LCD_RS = 0;                           //1-data, 0-command
  LCD_WR = 0;
  DATAOUT(order);
  LCD_WR = 1;
  LCD_CS = 1;
}

//写数据函数
//可以替代LCD_WR_DATAX宏,拿时间换空间.
//data:寄存器值
void LCD_WR_DATAX(u16 data)
{
  LCD_RS = 1;
  LCD_CS = 0;
  LCD_WR = 0;
  DATAOUT(data);
  LCD_WR = 1;
  LCD_CS = 1;
}
/*
2017年4月3日14:58:46
从数据总线读数据：按照读数据的时序规则控制从机往数据总线输出数据供主机读取，
返回读取到的值
*/
u16 LCD_RD_DATA(void)
{
  u32 t;
  GPIOE->CRL = PxIPU;
  GPIOE->CRH = PxIPU;
  //GPIOE->ODR=0XFFFF;
  LCD_CS = 0;
  LCD_RS = 1;
  LCD_RD = 0;
  //  DelayUs(1);
  t=DATAIN;                                             //从PDIR（端口输入）寄存器读取数据总线的“32位”数据
  //  t = (((0xff0000 & t)>>8)|((0xf80 & t)>>4) | ((0xe & t)>>1));  //只是从作为数据线PTB1,2,3, 7,8,9 10 11  16……23端口读16位数据，这些数据散布在32位“数据输入寄存器”的1,2,3, 7,8,9 10 11  16……23位上，该式将这些数据移至低16位
  LCD_RD = 1;
  LCD_CS = 1;
  GPIOE->CRL = PxOUTPP;
  GPIOE->CRH = PxOUTPP;
  
  return (u16)t;
}
//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
  LCD_WR_DATAX(LCD_RegValue);
}
//读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值
u16 LCD_ReadReg(u16 LCD_Reg)
{
  u32 t;
  LCD_CS = 0;
  LCD_RS = 0;
  //  GPIOE->CRL = PxOUTPP;
  //  GPIOE->CRH = PxOUTPP;
  DATAOUT(LCD_Reg);
  LCD_WR = 0;
  DelayUs(10);
  LCD_WR = 1;
  LCD_RS = 1;  //读取数据(读寄存器时,并不需要读2次)
  GPIOE->CRL = PxIPU;
  GPIOE->CRH = PxIPU;
  DelayUs(10);
  LCD_RD = 0;
  LCD_RD = 1;
  LCD_RD = 0;
  t = DATAIN;
  t = (((0xff0000 & t)>>8)|((0xf80 & t)>>4) | ((0xe & t)>>1));;
  LCD_RD = 1;
  LCD_CS = 1;
  GPIOE->CRL = PxOUTPP;
  GPIOE->CRH = PxOUTPP;
  
  return t; 
} 

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)//static inline 
{
  LCD_WR_REG(ColumnAddressSet);
  LCD_WR_DATAX(Xpos>>8);
  LCD_WR_DATAX(Xpos);
  LCD_WR_REG(PageAddressSet);
  LCD_WR_DATAX(Ypos>>8);
  LCD_WR_DATAX(Ypos);
}
//LCD开启显示
void LCD_DisplayOn(void)
{
//  if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)
    LCD_WR_REG(0X29) //开启显示
//  else if(lcddev.id==0X5510)
//    LCD_WR_REG(0X2900)	//开启显示
//  else 
//    LCD_WriteReg(0X07,0x0173); 				 	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{
//  if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)
    LCD_WR_REG(0X28)	//关闭显示
//  else if(lcddev.id==0X5510)
//    LCD_WR_REG(0X2800)	//关闭显示
//  else
//    LCD_WriteReg(0X07,0x0);//关闭显示 
}

//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//x_max,y_max:窗口宽度和高度,必须大于0!!
//窗体大小:x_max*y_max.
void LCD_Set_Window(u16 sx,u16 sy,u16 x_max,u16 y_max)
{
//  u8 hsareg,heareg,vsareg,veareg;
//  u16 hsaval,heaval,vsaval,veaval; 
  u16 twidth,theight;
  twidth=sx+x_max-1;
  theight=sy+y_max-1;
  //    GPIOE->CRL = PxOUTPP;
  //GPIOE->CRH = PxOUTPP;
//  if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804||(lcddev.dir==1&&lcddev.id==0X1963))
//  {
    LCD_WR_REG(ColumnAddressSet); 
    LCD_WR_DATA(sx>>8); 
    LCD_WR_DATA(sx&0XFF);	 
    LCD_WR_DATA(twidth>>8); 
    LCD_WR_DATA(twidth&0XFF);  
    LCD_WR_REG(PageAddressSet); 
    LCD_WR_DATA(sy>>8); 
    LCD_WR_DATA(sy&0XFF); 
    LCD_WR_DATA(theight>>8); 
    LCD_WR_DATA(theight&0XFF); 
//  }
//  else if(lcddev.id==0X1963)//1963竖屏特殊处理
//  {
//    sx=lcddev.x_max-x_max-sx; 
//    y_max=sy+y_max-1; 
//    LCD_WR_REG(ColumnAddressSet); 
//    LCD_WR_DATA(sx>>8); 
//    LCD_WR_DATA(sx&0XFF);	 
//    LCD_WR_DATA((sx+x_max-1)>>8); 
//    LCD_WR_DATA((sx+x_max-1)&0XFF);  
//    LCD_WR_REG(PageAddressSet); 
//    LCD_WR_DATA(sy>>8); 
//    LCD_WR_DATA(sy&0XFF); 
//    LCD_WR_DATA(y_max>>8); 
//    LCD_WR_DATA(y_max&0XFF); 		
//  }else if(lcddev.id==0X5510)
//  {
//    LCD_WR_REG(ColumnAddressSet);LCD_WR_DATA(sx>>8);  
//    LCD_WR_REG(ColumnAddressSet+1);LCD_WR_DATA(sx&0XFF);	  
//    LCD_WR_REG(ColumnAddressSet+2);LCD_WR_DATA(twidth>>8);   
//    LCD_WR_REG(ColumnAddressSet+3);LCD_WR_DATA(twidth&0XFF);   
//    LCD_WR_REG(PageAddressSet);LCD_WR_DATA(sy>>8);   
//    LCD_WR_REG(PageAddressSet+1);LCD_WR_DATA(sy&0XFF);  
//    LCD_WR_REG(PageAddressSet+2);LCD_WR_DATA(theight>>8);   
//    LCD_WR_REG(PageAddressSet+3);LCD_WR_DATA(theight&0XFF);  
//  }else	//其他驱动IC
//  {
//    if(lcddev.dir==1)//横屏
//    {
//      //窗口值
//      hsaval=sy;				
//      heaval=theight;
    //      vsaval=lcddev.x_max-twidth-1;
    //      veaval=lcddev.x_max-sx-1;				
    //    }else
    //    { 
    //      hsaval=sx;				
    //      heaval=twidth;
    //      vsaval=sy;
    //      veaval=theight;
    //    } 
    //    hsareg=0X50;heareg=0X51;//水平方向窗口寄存器
    //    vsareg=0X52;veareg=0X53;//垂直方向窗口寄存器	   							  
    //    //设置寄存器值
    //    LCD_WriteReg(hsareg,hsaval);
    //    LCD_WriteReg(heareg,heaval);
    //    LCD_WriteReg(vsareg,vsaval);
    //    LCD_WriteReg(veareg,veaval);		
    //    LCD_SetCursor(sx,sy);	//设置光标位置
    //  }
}


//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
//u16 LCD_ReadPoint(u16 x,u16 y, u8 *R, u8 *G, u8 *B)
//{
//  u32 _32bit_data;                                     //because we doctted useed pin from 1 to 31,and the data_in register is 32 bit
//  if(x>=lcddev.scrn_x_max || y>=lcddev.scrn_y_max)return 0;	//超过了范围,直接返回
//  LCD_SetCursor(x, y);
//  //LCD_WR_REG(lcddev.wramcmd);	//开始写入GRAM
//  //LCD_WR_DATA(GREEN);
//  //GPIOE->CRL = PxOUTPP;
//  //GPIOE->CRH = PxOUTPP;
//  LCD_WR_REG(0X2E);//9341/6804/3510/1963 发送读GRAM指令
//  GPIOE->CRL = PxIPU;
//  GPIOE->CRH = PxIPU;
//  LCD_RS = 1;
//  LCD_CS = 0;
//  LCD_RD = 0;
//  DelayUs(2);									//延时		
//  //  r=DATAIN;
//  //  r = (((0xff0000 & r)>>8)|((0xf80 & r)>>4) | ((0xe & r)>>1));        //第一次读取的是假颜色 
//  LCD_RD = 1;
//  LCD_RD = 0;
//  DelayUs(2);//延时
//  _32bit_data = DATAIN;  	//实际坐标颜色
//  //  _32bit_data = PORT_IN_DATA_TRANSFER(_32bit_data);
//  *G = (_32bit_data & 0XFF) >> 2;               //对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
//  *R = _32bit_data >> 11;
//  LCD_RD = 1;
//  LCD_RD = 0;
//  DelayUs(2);//延时
//  _32bit_data = DATAIN;//读取蓝色值
//  //  _32bit_data = PORT_IN_DATA_TRANSFER(_32bit_data);          //b在16位的高八位
//  *B = _32bit_data >> 11;
//  LCD_RD = 1;
//  LCD_CS = 1;
//  GPIOE->CRL = PxOUTPP;
//  GPIOE->CRH = PxOUTPP;
//  
//  return (u16)((*R<<11) | (*G<<5) | *B);//ILI9341/NT35310/NT35510需要公式转换一下,8位的数据也是可以移动不小于8次的
//}
void LCD_Area_Set(u16 x0, u16 y0, u16 x1, u16 y1)
{
  LCD_WR_REG(ColumnAddressSet);   //Column Address Set (2Ah)
  LCD_WR_DATAX(x0>>8);
  LCD_WR_DATAX(x0);
  LCD_WR_DATAX(x1>>8);
  LCD_WR_DATAX(x1);
  LCD_WR_REG(PageAddressSet);   //Page Address Set (2Bh)
  LCD_WR_DATAX(y0>>8);
  LCD_WR_DATAX(y0);
  LCD_WR_DATAX(y1>>8);
  LCD_WR_DATAX(y1);
}
//Brief:
//  name:Gpio8080OptLcdAS（optimized）LcdA(Area)C(Set)=针对GPIO模拟8080时序优化过后的区域设置程序,
//  专用的驱动层实现，以及利用冗余时序进行加速，
//Parameters:
//  x0,y0,x1,y1:起、终点坐标，x，y无限制，用户自查
//Return:
//  void;
//Example:
//    //
void Gpio8080OptLcdAS(u16 x0, u16 x1, u16 y0, u16 y1)
{
  LCD_RS = 0;
  LCD_CS = 0;
  DATAOUT(ColumnAddressSet);
  LCD_WR = 0;
  LCD_WR = 1;
  
  LCD_RS = 1;
  LCD_WR = 0;
  DATAOUT(x0>>8);
  LCD_WR = 1;
  LCD_WR = 0;
  DATAOUT(x0);
  LCD_WR = 1;
  LCD_WR = 0;
  DATAOUT(x1>>8);
  LCD_WR = 1;
  LCD_WR = 0;
  DATAOUT(x1);
  LCD_WR = 1;
  
  LCD_RS = 0;
  DATAOUT(PageAddressSet);
  LCD_WR = 0;
  LCD_WR = 1;
  
  LCD_RS = 1;
  LCD_WR = 0;
  DATAOUT(y0>>8);
  LCD_WR = 1;
  LCD_WR = 0;
  DATAOUT(y0);
  LCD_WR = 1;
  LCD_WR = 0;
  DATAOUT(y1>>8);
  LCD_WR = 1;
  LCD_WR = 0;
  DATAOUT(y1);
  LCD_WR = 1;
}
//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void LCD_SSD_BackLightSet(u8 pwm)
{
  LCD_WR_REG(0xBE);	//配置PWM输出
  LCD_WR_DATA(0x05);	//1设置PWM频率
  LCD_WR_DATA((u8)(pwm * 2.55));//2设置PWM占空比
  LCD_WR_DATA(0x01);	//3设置C
  LCD_WR_DATA(0xFF);	//4设置D
  LCD_WR_DATA(0x00);	//5设置E
  LCD_WR_DATA(0x00);	//6设置F
}

//Brief:驱动层实现的区域填充函数，因为有的lcd设置区域后不需要写入GRAM命令，和ILI9341不一样，为了提供同样的接口构造此函数
//  不检查超出边界，需用户自查
//  name:AreaFill
//Parameters:
//  sx,ex,sy,ey:起、终区域，无限制，需用户自查
//Return:
//  void;
//Example:
//    //
void AreaFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{
    u32 i, j = (ex - sx +1) * (ey - sy + 1);//仍然比横纵循环快
  LCD_Area_Set(sx, sy, ex, ey);//设置光标位置 	
  LCD_WR_REG(MemoryWrite);     			//开始写入GRAM
  for(i=0;i<j;i++)
    LCD_WR_DATA(color);	
}

void LCD_GPIO_Init()
{
  PTXn_e pin;                                   //PTB1,2,3,7,8,9,10,11,16……23作为16位数据总线
  for(pin=PE0; pin<=PE15; ++pin)              //数据总线初始化；即使有些数据位没有对应的端口，任然全部写一遍，否则你挨个儿写初始化给我看看？
    gpio_init(pin,Out_PP,n_interupt,GPIO_Speed_50MHz,0);        //默认Out_PP模式，读（IPU模式）完数据需要立即切回Out_PP模式；
  //8080控制总线（RES复位;DC数据/命令;CS片选;WR写;RD读）端口+背光端口初始化  
  
  gpio_init(LCD_RST_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_CS_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_DC_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_WR_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_RD_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  TIM1_PWM_Init(1000,10000,2000);                       //背光亮度控制引脚
}

//初始化lcd，不同LCD初始化差异较多，因此将初始化部分放到驱动层，便不能用LCD文件中定义的结构体传参了，只能一个一个传
//Parameters:
//  dir:显示屏的显示方向，详细看Lcd_ILI9341_Parameter_Init初始化
uint16 LCD_Init(u8 dir,u16*px,u16*py, uint16 default_draw_color, uint16 default_back_color,...)
{
  LCD_GPIO_Init();  
  LCD_RST_CLR;                                  //实验表明:必须复位
  DelayMs(50);
  LCD_RST_SET;
  DelayMs(50);
  LCD_Write_Command(0xD3);                       //读取控制芯片的ID命令; 参看ILI94341的手册
  LCD_RD_DATA();                                 //前两次读取是无用的数，参看ILI94341的手册
  LCD_RD_DATA();                                 //前两次读取是无用的数，参看ILI94341的手册
  uint16 tmp = (LCD_RD_DATA()<<8) | LCD_RD_DATA();//ID;后面的“两次“读取分别是93和41，参看ILI9341的手册
  if(0x9341!=tmp)
    return tmp;           //出错了
    
//  if(lcddev.id==0X9341)	//9341已识别，否则为成功识别LCD返回假
  {
    LCD_WR_REG(0xCF);   //Power control B (CFh)
    LCD_WR_DATAX(0x00);
    LCD_WR_DATAX(0xC1);
    LCD_WR_DATAX(0X30);
    LCD_WR_REG(0xED);   //Power on sequence control (EDh)
    LCD_WR_DATAX(0x64); 
    LCD_WR_DATAX(0x03); 
    LCD_WR_DATAX(0X12); 
    LCD_WR_DATAX(0X81); 
    LCD_WR_REG(0xE8);   //Driver timing control A (E8h)
    LCD_WR_DATAX(0x85); 
    LCD_WR_DATAX(0x10);
    LCD_WR_DATAX(0x7A); 
    LCD_WR_REG(0xCB);   //Power control A (CBh)
    LCD_WR_DATAX(0x39); 
    LCD_WR_DATAX(0x2C); 
    LCD_WR_DATAX(0x00); 
    LCD_WR_DATAX(0x34);
    LCD_WR_DATAX(0x02); 
    LCD_WR_REG(0xF7);   //Pump ratio control (F7h)
    LCD_WR_DATAX(0x20); 
    LCD_WR_REG(0xEA);   //Driver timing control B (EAh)
    LCD_WR_DATAX(0x00); 
    LCD_WR_DATAX(0x00); 
    LCD_WR_REG(0xC0);    //Power Control 1 (C0h)
    LCD_WR_DATAX(0x1B);  //VRH[5:0] 
    LCD_WR_REG(0xC1);    //Power control 2 (C1h)
    LCD_WR_DATAX(0x01); //SAP[2:0];BT[3:0] 
    LCD_WR_REG(0xC5);    //VCOM Control 1(C5h)
    LCD_WR_DATAX(0x30); //3F
    LCD_WR_DATAX(0x30); //3C
    LCD_WR_REG(0xC7);    //VCOM Control 2(C7h)
    LCD_WR_DATAX(0XB7);
    LCD_WR_REG(0x36);    //Memory Access Control (36h)
    LCD_WR_DATAX(0x48); 
    LCD_WR_REG(0x3A);    //COLMOD: Pixel Format Set (3Ah)
    LCD_WR_DATAX(0x55); 
    LCD_WR_REG(0xB1);    //Frame Rate Control (In Normal Mode/Full Colors) (B1h)
    LCD_WR_DATAX(0x00);   
    LCD_WR_DATAX(0x1A); 
    LCD_WR_REG(0xB6);    //Display Function Control (B6h)
    LCD_WR_DATAX(0x0A); 
    LCD_WR_DATAX(0xA2);
    LCD_WR_REG(0xF2);    //Enable 3 gamma (F2h)( 3Gamma Function Disable )
    LCD_WR_DATAX(0x00); 
    LCD_WR_REG(0x26);    //Gamma Set (26h)      (Gamma curve selected )
    LCD_WR_DATAX(0x01); 
    LCD_WR_REG(0xE0);    //Positive Gamma Correction (E0h)      (Set Gamma )
    LCD_WR_DATAX(0x0F); 
    LCD_WR_DATAX(0x2A);
    LCD_WR_DATAX(0x28);
    LCD_WR_DATAX(0x08);
    LCD_WR_DATAX(0x0E);
    LCD_WR_DATAX(0x08);
    LCD_WR_DATAX(0x54);
    LCD_WR_DATAX(0XA9);
    LCD_WR_DATAX(0x43);
    LCD_WR_DATAX(0x0A);
    LCD_WR_DATAX(0x0F);
    LCD_WR_DATAX(0x00);
    LCD_WR_DATAX(0x00);
    LCD_WR_DATAX(0x00);
    LCD_WR_DATAX(0x00);
    LCD_WR_REG(0XE1);    //Negative Gamma Correction (E1h)      (Set Gamma)
    LCD_WR_DATAX(0x00); 
    LCD_WR_DATAX(0x15);
    LCD_WR_DATAX(0x17);
    LCD_WR_DATAX(0x07);
    LCD_WR_DATAX(0x11);
    LCD_WR_DATAX(0x06);
    LCD_WR_DATAX(0x2B);
    LCD_WR_DATAX(0x56);
    LCD_WR_DATAX(0x3C);
    LCD_WR_DATAX(0x05);
    LCD_WR_DATAX(0x10);
    LCD_WR_DATAX(0x0F);
    LCD_WR_DATAX(0x3F);
    LCD_WR_DATAX(0x3F);
    LCD_WR_DATAX(0x0F); 
    LCD_WR_REG(0x11);   //Exit Sleep
    DelayMs(120);
    LCD_WR_REG(0x29);   //display on	
  }
//  else
//    return false;
//    Lcd_ILI9341_Parameter_Init(dir, &lcddev);   //把lcddev变量里的参数与ILI9341对应起来,0-竖屏a；3竖屏b；5-横屏a；6-横屏b
tmp = 0;
  switch(dir)
  {
  case lrud:
    tmp|=(0<<7)|(0<<6)|(0<<5);break;
  case lrdu:
    tmp|=(1<<7)|(0<<6)|(0<<5);break;
  case rlud:
    tmp|=(0<<7)|(1<<6)|(0<<5);break;
  case rldu:
    tmp|=(1<<7)|(1<<6)|(0<<5);break;
  case udlr:
    tmp|=(0<<7)|(0<<6)|(1<<5);break;
  case udrl:
    tmp|=(0<<7)|(1<<6)|(1<<5);break;
  case dulr:
    tmp|=(1<<7)|(0<<6)|(1<<5);break;
  case durl:
    tmp|=(1<<7)|(1<<6)|(1<<5);break;
  }
  tmp|=0x08;                                 //需要BGR
  
  LCD_WriteReg(MemoryAccessControl,tmp);

    if(dir<=rldu)
    {
      *px = LCD_WIDTH;
      *py = LCD_LENGTH;
    }
    else if(dir<=durl)
    {
      *px = LCD_LENGTH;
      *py = LCD_WIDTH;
    }
    else
      return 1;
//  ld->dir = direction;
  //  ld->setxcmd=0x2A;                     //列地址设置指令，参数详情参看手册
  //  ld->setycmd=0x2B;                     //页地址设置指令
  //  ld->wramcmd=0x2C;                     //写GRAM指令，参数就是颜色数据
//  deft_draw_color = default_draw_color;         //默认显示颜色，default draw color
//  deft_back_color = default_back_color;
  
  
//  LCD_Clear(default_back_color);
  LCD_SSD_BackLightSet(20);
    
  return tmp;
}

