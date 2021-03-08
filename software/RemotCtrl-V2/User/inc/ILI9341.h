#ifndef __ILI9341_H
#define __ILI9341_H		

#define STDOUT_LCD      //if use this LCD as standard output��printf�� facility?

/***********************LCD GPIO Definition*************************/
#define	LCD_RST_CLR	PD8_OUT = 0    //1����/����0 		PC8	   
#define LCD_CS_CLR      PB11_OUT = 0    //Ƭѡ�˿�  		PC9
#define	LCD_RS_CLR	PB12_OUT = 0    //1����/����0 		PC8	   
#define	LCD_WR_CLR	PB13_OUT = 0    //д����		PC7
#define	LCD_RD_CLR	PB14_OUT = 0    //������		PC6

#define	LCD_RST_SET	PD8_OUT = 1    //1����/����0 		PC8	   
#define	LCD_CS_SET      PB11_OUT = 1    //Ƭѡ�˿�  		PC9
#define	LCD_RS_SET	PB12_OUT = 1    //1����/����0 		PC8	   
#define	LCD_WR_SET	PB13_OUT = 1    //д����		PC7
#define	LCD_RD_SET	PB14_OUT = 1    //������		PC6

#define	LCD_RST_PIN     PD8
#define	LCD_CS_PIN      PB11     //Ƭѡ�˿�  		PC9
#define	LCD_DC_PIN	PB12     //����/����		PC8	   
#define	LCD_WR_PIN	PB13     //д����		PC7
#define	LCD_RD_PIN	PB14     //������		PC6   
#define	LCD_CS  PB11_OUT     //Ƭѡ�˿�  		PC9
#define	LCD_RS	PB12_OUT     //����/����		PC8	   
#define	LCD_WR	PB13_OUT     //д����		PC7
#define	LCD_RD	PB14_OUT     //������		PC6   

//PTB123 789 10 11  16����23��Ϊ������  PT_DWORD(B,PDOR)��32λ������Ĵ���������16λ������ȷ�������Ӧ�Ķ˿ڣ���:��8��������8λ���м�5��������4λ�����3��������1λ�������Ϳճ����λ��0λ�����ö�Ӧδʹ�õ�PTB0
#define DATAOUT(x)      GPIOE->ODR=(x)//GPIOB->ODR=x; //������� 32λ������ݼĴ��� PSOR
#define DATAIN          (GPIOE->IDR)                         //STM32:GPIOB->IDR;   //�������� 32λ�������ݼĴ���	

/***********************LCD parameter Definition*************************/
#define LCD_WIDTH       240     //�����������Ͽ��ĳ��Ϳ������ꡢ��ʾ����û�����κι�ϵ
#define LCD_LENGTH      320
#define TOTAL_POINT     76800

//ɨ�跽���壬������Ļ�����ߵ���һ�߳���
//ɨ�跽����MemoryAccessControl�Ĵ�������MADCTL��0x36����D5,D6,D7����5,6,7��λ�����ģ�Manual P127
//��Ļ������ϵ�����������Ĵ�����D6,D7����6,7��λ�����ģ�Manual P207������ɨ�跽��ȷ���ˣ������ȷ����
//��Ϊ��Ҫ����ֱ��������أ�����ȷ��ɨ�跽��
typedef enum
{
  lrud = 0, //������,���ϵ��£��ֱ���x��y
  lrdu, //������,���µ��ϣ��ֱ���x��y
  rlud, //���ҵ���,���ϵ��£��ֱ���x��y
  rldu, //���ҵ���,���µ��ϣ��ֱ���x��y
   udlr, //���ϵ���,�����ң��ֱ���x��y
  udrl, //���ϵ���,���ҵ��󣬷ֱ���x��y
  dulr, //���µ���,�����ң��ֱ���x��y
  durl, //���µ���,���ҵ��󣬷ֱ���x��y
}ScanDir;              //�ò����ڳ����л�ı䣬����̬�ı���ʾ�������Բ����ú궨�����������ö�ٷ�װ

#ifdef STDOUT_LCD
typedef struct
{
  uint16 sx;     //start x position
  uint16 ex;     //end x position
  uint16 sy;
  uint16 ey;
  uint16 x_now;         //where the new location has reached,if something printed on screen,the position should be followed
  uint16 y_now;
}STD_PRINTF_AREA;       //��׼�������printf��ӡ�ķ�Χ
#endif 
/***********************ILI9341 Reg Definition*************************/
//˵���������ƽ��͸����ֲ��еģ���δ���ü���д

#define ReadDisplayMADCTL       0x0B


#define ColumnAddressSet        0x2A
#define PageAddressSet          0x2B
#define MemoryWrite             0x2C
#define ColorSet                0x2D

#define MemoryAccessControl     0x36//�ֽ�MADCTL Manual P127 P207;ȷ��ɨ�跽�������ϵ

#define VerticalRefreshOrderSHIFT       4
#define HorizontalRefreshORDERSHIFT     2

uint16 LCD_Init(u8 dir,u16*px,u16*py, uint16 default_draw_color, uint16 default_back_color,...);	//��ʼ��
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

//void LCD_Scan_Dir(PLCD_DEV);							//������ɨ�跽��
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//���ô���	
//void	TFT_Init_Ili9341(void);

//д���ݺ���
#define LCD_WR_DATA(data){\
LCD_RS = 1;\
LCD_CS = 0;\
DATAOUT(data);\
LCD_WR = 0;\
LCD_WR = 1;\
LCD_CS = 1;\
}
//д��ַ 
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
//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��
//���¼����������Զ�����
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)





#endif































