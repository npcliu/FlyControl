#include <stdarg.h>
#include "stm32f10x.h"          //���ڵײ�ʵ�ֵĿ⣬���ļ���Ӧ��lcd�ò�ͬ�Ŀ���������ʱ��ֻ���л������Ŀ⼴��
#include "delay.h"              //���ڵײ�ʵ�ֵĿ⣬���ļ���Ӧ��lcd�ò�ͬ�Ŀ���������ʱ��ֻ���л������Ŀ⼴��

#include "ILI9341.h"
//#include "font.h"
/***********�ڲ�����**************/
//static scan_dir deault_scan_dir = lrud;
//static u16 deft_draw_color;         //Ĭ����ʾ��ɫ��default draw color
//static u16 deft_back_color;        //Ĭ�ϵ�ɫ    ��default background color

/*
2017��4��3��14:50:03
д���Write command code��������д�����ʱ��������������������ߡ�дһ�����ݣ������ݴ������
���磺Lcd_Write_Command(0x11)�� ����ILI9341�ֲ�:0x11�����ʾ���˯��״̬
*/
void LCD_Write_Command(u16 order)//static inline 
{
  //  DATAOUT(order);
  GPIOE->CRL = PxOUTPP;//PTB_BASE_PTR->PDDR = 0xffffffff;      //����PTB���ж˿ڷ���Ϊ���
  GPIOE->CRH = PxOUTPP;//PTB_BASE_PTR->PDDR = 0xffffffff;      //����PTB���ж˿ڷ���Ϊ���
  LCD_CS = 0;
  LCD_RS = 0;                           //1-data, 0-command
  LCD_WR = 0;
  DATAOUT(order);
  LCD_WR = 1;
  LCD_CS = 1;
}

//д���ݺ���
//�������LCD_WR_DATAX��,��ʱ�任�ռ�.
//data:�Ĵ���ֵ
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
2017��4��3��14:58:46
���������߶����ݣ����ն����ݵ�ʱ�������ƴӻ�����������������ݹ�������ȡ��
���ض�ȡ����ֵ
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
  t=DATAIN;                                             //��PDIR���˿����룩�Ĵ�����ȡ�������ߵġ�32λ������
  //  t = (((0xff0000 & t)>>8)|((0xf80 & t)>>4) | ((0xe & t)>>1));  //ֻ�Ǵ���Ϊ������PTB1,2,3, 7,8,9 10 11  16����23�˿ڶ�16λ���ݣ���Щ����ɢ����32λ����������Ĵ�������1,2,3, 7,8,9 10 11  16����23λ�ϣ���ʽ����Щ����������16λ
  LCD_RD = 1;
  LCD_CS = 1;
  GPIOE->CRL = PxOUTPP;
  GPIOE->CRH = PxOUTPP;
  
  return (u16)t;
}
//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
  LCD_WR_DATAX(LCD_RegValue);
}
//���Ĵ���
//LCD_Reg:�Ĵ������
//����ֵ:������ֵ
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
  LCD_RS = 1;  //��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
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

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)//static inline 
{
  LCD_WR_REG(ColumnAddressSet);
  LCD_WR_DATAX(Xpos>>8);
  LCD_WR_DATAX(Xpos);
  LCD_WR_REG(PageAddressSet);
  LCD_WR_DATAX(Ypos>>8);
  LCD_WR_DATAX(Ypos);
}
//LCD������ʾ
void LCD_DisplayOn(void)
{
//  if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)
    LCD_WR_REG(0X29) //������ʾ
//  else if(lcddev.id==0X5510)
//    LCD_WR_REG(0X2900)	//������ʾ
//  else 
//    LCD_WriteReg(0X07,0x0173); 				 	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{
//  if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)
    LCD_WR_REG(0X28)	//�ر���ʾ
//  else if(lcddev.id==0X5510)
//    LCD_WR_REG(0X2800)	//�ر���ʾ
//  else
//    LCD_WriteReg(0X07,0x0);//�ر���ʾ 
}

//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//x_max,y_max:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:x_max*y_max.
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
//  else if(lcddev.id==0X1963)//1963�������⴦��
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
//  }else	//��������IC
//  {
//    if(lcddev.dir==1)//����
//    {
//      //����ֵ
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
    //    hsareg=0X50;heareg=0X51;//ˮƽ���򴰿ڼĴ���
    //    vsareg=0X52;veareg=0X53;//��ֱ���򴰿ڼĴ���	   							  
    //    //���üĴ���ֵ
    //    LCD_WriteReg(hsareg,hsaval);
    //    LCD_WriteReg(heareg,heaval);
    //    LCD_WriteReg(vsareg,vsaval);
    //    LCD_WriteReg(veareg,veaval);		
    //    LCD_SetCursor(sx,sy);	//���ù��λ��
    //  }
}


//��ȡ��ĳ�����ɫֵ
//x,y:����
//����ֵ:�˵����ɫ
//u16 LCD_ReadPoint(u16 x,u16 y, u8 *R, u8 *G, u8 *B)
//{
//  u32 _32bit_data;                                     //because we doctted useed pin from 1 to 31,and the data_in register is 32 bit
//  if(x>=lcddev.scrn_x_max || y>=lcddev.scrn_y_max)return 0;	//�����˷�Χ,ֱ�ӷ���
//  LCD_SetCursor(x, y);
//  //LCD_WR_REG(lcddev.wramcmd);	//��ʼд��GRAM
//  //LCD_WR_DATA(GREEN);
//  //GPIOE->CRL = PxOUTPP;
//  //GPIOE->CRH = PxOUTPP;
//  LCD_WR_REG(0X2E);//9341/6804/3510/1963 ���Ͷ�GRAMָ��
//  GPIOE->CRL = PxIPU;
//  GPIOE->CRH = PxIPU;
//  LCD_RS = 1;
//  LCD_CS = 0;
//  LCD_RD = 0;
//  DelayUs(2);									//��ʱ		
//  //  r=DATAIN;
//  //  r = (((0xff0000 & r)>>8)|((0xf80 & r)>>4) | ((0xe & r)>>1));        //��һ�ζ�ȡ���Ǽ���ɫ 
//  LCD_RD = 1;
//  LCD_RD = 0;
//  DelayUs(2);//��ʱ
//  _32bit_data = DATAIN;  	//ʵ��������ɫ
//  //  _32bit_data = PORT_IN_DATA_TRANSFER(_32bit_data);
//  *G = (_32bit_data & 0XFF) >> 2;               //����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
//  *R = _32bit_data >> 11;
//  LCD_RD = 1;
//  LCD_RD = 0;
//  DelayUs(2);//��ʱ
//  _32bit_data = DATAIN;//��ȡ��ɫֵ
//  //  _32bit_data = PORT_IN_DATA_TRANSFER(_32bit_data);          //b��16λ�ĸ߰�λ
//  *B = _32bit_data >> 11;
//  LCD_RD = 1;
//  LCD_CS = 1;
//  GPIOE->CRL = PxOUTPP;
//  GPIOE->CRH = PxOUTPP;
//  
//  return (u16)((*R<<11) | (*G<<5) | *B);//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��,8λ������Ҳ�ǿ����ƶ���С��8�ε�
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
//  name:Gpio8080OptLcdAS��optimized��LcdA(Area)C(Set)=���GPIOģ��8080ʱ���Ż�������������ó���,
//  ר�õ�������ʵ�֣��Լ���������ʱ����м��٣�
//Parameters:
//  x0,y0,x1,y1:���յ����꣬x��y�����ƣ��û��Բ�
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
//SSD1963 ��������
//pwm:����ȼ�,0~100.Խ��Խ��.
void LCD_SSD_BackLightSet(u8 pwm)
{
  LCD_WR_REG(0xBE);	//����PWM���
  LCD_WR_DATA(0x05);	//1����PWMƵ��
  LCD_WR_DATA((u8)(pwm * 2.55));//2����PWMռ�ձ�
  LCD_WR_DATA(0x01);	//3����C
  LCD_WR_DATA(0xFF);	//4����D
  LCD_WR_DATA(0x00);	//5����E
  LCD_WR_DATA(0x00);	//6����F
}

//Brief:������ʵ�ֵ�������亯������Ϊ�е�lcd�����������Ҫд��GRAM�����ILI9341��һ����Ϊ���ṩͬ���Ľӿڹ���˺���
//  ����鳬���߽磬���û��Բ�
//  name:AreaFill
//Parameters:
//  sx,ex,sy,ey:�������������ƣ����û��Բ�
//Return:
//  void;
//Example:
//    //
void AreaFill(u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{
    u32 i, j = (ex - sx +1) * (ey - sy + 1);//��Ȼ�Ⱥ���ѭ����
  LCD_Area_Set(sx, sy, ex, ey);//���ù��λ�� 	
  LCD_WR_REG(MemoryWrite);     			//��ʼд��GRAM
  for(i=0;i<j;i++)
    LCD_WR_DATA(color);	
}

void LCD_GPIO_Init()
{
  PTXn_e pin;                                   //PTB1,2,3,7,8,9,10,11,16����23��Ϊ16λ��������
  for(pin=PE0; pin<=PE15; ++pin)              //�������߳�ʼ������ʹ��Щ����λû�ж�Ӧ�Ķ˿ڣ���Ȼȫ��дһ�飬�����㰤����д��ʼ�����ҿ�����
    gpio_init(pin,Out_PP,n_interupt,GPIO_Speed_50MHz,0);        //Ĭ��Out_PPģʽ������IPUģʽ����������Ҫ�����л�Out_PPģʽ��
  //8080�������ߣ�RES��λ;DC����/����;CSƬѡ;WRд;RD�����˿�+����˿ڳ�ʼ��  
  
  gpio_init(LCD_RST_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_CS_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_DC_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_WR_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  gpio_init(LCD_RD_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);
  TIM1_PWM_Init(1000,10000,2000);                       //�������ȿ�������
}

//��ʼ��lcd����ͬLCD��ʼ������϶࣬��˽���ʼ�����ַŵ������㣬�㲻����LCD�ļ��ж���Ľṹ�崫���ˣ�ֻ��һ��һ����
//Parameters:
//  dir:��ʾ������ʾ������ϸ��Lcd_ILI9341_Parameter_Init��ʼ��
uint16 LCD_Init(u8 dir,u16*px,u16*py, uint16 default_draw_color, uint16 default_back_color,...)
{
  LCD_GPIO_Init();  
  LCD_RST_CLR;                                  //ʵ�����:���븴λ
  DelayMs(50);
  LCD_RST_SET;
  DelayMs(50);
  LCD_Write_Command(0xD3);                       //��ȡ����оƬ��ID����; �ο�ILI94341���ֲ�
  LCD_RD_DATA();                                 //ǰ���ζ�ȡ�����õ������ο�ILI94341���ֲ�
  LCD_RD_DATA();                                 //ǰ���ζ�ȡ�����õ������ο�ILI94341���ֲ�
  uint16 tmp = (LCD_RD_DATA()<<8) | LCD_RD_DATA();//ID;����ġ����Ρ���ȡ�ֱ���93��41���ο�ILI9341���ֲ�
  if(0x9341!=tmp)
    return tmp;           //������
    
//  if(lcddev.id==0X9341)	//9341��ʶ�𣬷���Ϊ�ɹ�ʶ��LCD���ؼ�
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
//    Lcd_ILI9341_Parameter_Init(dir, &lcddev);   //��lcddev������Ĳ�����ILI9341��Ӧ����,0-����a��3����b��5-����a��6-����b
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
  tmp|=0x08;                                 //��ҪBGR
  
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
  //  ld->setxcmd=0x2A;                     //�е�ַ����ָ���������ο��ֲ�
  //  ld->setycmd=0x2B;                     //ҳ��ַ����ָ��
  //  ld->wramcmd=0x2C;                     //дGRAMָ�����������ɫ����
//  deft_draw_color = default_draw_color;         //Ĭ����ʾ��ɫ��default draw color
//  deft_back_color = default_back_color;
  
  
//  LCD_Clear(default_back_color);
  LCD_SSD_BackLightSet(20);
    
  return tmp;
}

