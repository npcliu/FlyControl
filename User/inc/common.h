#ifndef __COMMON_H
#define __COMMON_H

typedef union
{
  unsigned int  DW;
  unsigned short  W[2];
  unsigned char   B[4];
  struct                      //four bit filed
  {
    unsigned int fb0: 4;
    unsigned int fb1: 4;
    unsigned int fb2: 4;
    unsigned int fb3: 4;
    unsigned int fb4: 4;
    unsigned int fb5: 4;
    unsigned int fb6: 4;
    unsigned int fb7: 4;
  };
  struct              //double bit
  {
    unsigned int db0: 2;
    unsigned int db1: 2;
    unsigned int db2: 2;
    unsigned int db3: 2;
    unsigned int db4: 2;
    unsigned int db5: 2;
    unsigned int db6: 2;
    unsigned int db7: 2;
    unsigned int db8: 2;
    unsigned int db9: 2;
    unsigned int db10: 2;
    unsigned int db11: 2;
    unsigned int db12: 2;
    unsigned int db13: 2;
    unsigned int db14: 2;
    unsigned int db15: 2;
  };
  struct
  {
    unsigned int b0: 1;
    unsigned int b1: 1;
    unsigned int b2: 1;
    unsigned int b3: 1;
    unsigned int b4: 1;
    unsigned int b5: 1;
    unsigned int b6: 1;
    unsigned int b7: 1;
    unsigned int b8: 1;
    unsigned int b9: 1;
    unsigned int b10: 1;
    unsigned int b11: 1;
    unsigned int b12: 1;
    unsigned int b13: 1;
    unsigned int b14: 1;
    unsigned int b15: 1;
    unsigned int b16: 1;
    unsigned int b17: 1;
    unsigned int b18: 1;
    unsigned int b19: 1;
    unsigned int b20: 1;
    unsigned int b21: 1;
    unsigned int b22: 1;
    unsigned int b23: 1;
    unsigned int b24: 1;
    unsigned int b25: 1;
    unsigned int b26: 1;
    unsigned int b27: 1;
    unsigned int b28: 1;
    unsigned int b29: 1;
    unsigned int b30: 1;
    unsigned int b31: 1;
  };
} _32type;    //sizeof(Dtype) Ϊ 4


typedef struct
{
  unsigned char b0:1;
  unsigned char b1:1;
  unsigned char b2:1;
  unsigned char b3:1;
  unsigned char b4:1;
  unsigned char b5:1;
  unsigned char b6:1;
  unsigned char b7:1;
}b;
//}bit_char;
typedef union
{
  unsigned char c;
  b bitc;
}unionb;

typedef enum
{
  false,
  true
}BOOL;

#endif