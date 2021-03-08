
double BMP085_Get_Altitude(long Pressure)
{
  double altitude;
  altitude=44330.0*(1-pow((double)Pressure/101325,1/5.255));  
  
  return altitude;
}
void bmp085_init(void)
{
  //  gpio_init(EOC_PIN,IN_FLOATING,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  //  gpio_init(XCLR_PIN,Out_PP,n_interupt,GPIO_Speed_50MHz,1);						 //PB.5 ???
  ac1 = twobmp085_read(0xAA);
  ac2 = twobmp085_read(0xAC);
  ac3 = twobmp085_read(0xAE);
  ac4 = twobmp085_read(0xB0);
  ac5 = twobmp085_read(0xB2);
  ac6 = twobmp085_read(0xB4);
  b1 =  twobmp085_read(0xB6);
  b2 =  twobmp085_read(0xB8);
  mb =  twobmp085_read(0xBA);
  mc =  twobmp085_read(0xBC);
  md =  twobmp085_read(0xBE);
}

short ac1, ac2,ac3,b1, b2,mb,mc,md;
unsigned short ac4,ac5,ac6;


//long x1, x2, b5, b6, x3, b3, p, b4, b7;
void bmp085Convert()
{
  unsigned int ut;
  unsigned long up;
  long x1, x2, b5, b6, x3, b3, p;
  unsigned long b4, b7;
  
    ut = bmp085ReadTemp();	   
    ut = bmp085ReadTemp();	   
  up = bmp085ReadPressure();
  x1 = ((ut - ac6)*ac5) >> 15;
  x2 = (mc << 11) / (x1 + md);
  b5 = x1 + x2;
  temperature = ((b5 + 8) >> 4);
  
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15; 
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1; 
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  pressure = p+((x1 + x2 + 3791)>>4);
}