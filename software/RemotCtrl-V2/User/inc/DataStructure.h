#ifndef __DATASTRUCTURE_H
#define __DATASTRUCTURE_H

//By BD 2021年1月13日
//#ifdef u32
//#else
//typedef unsigned int  u32;
//#endif
//typedef unsigned short u16;
//typedef unsigned char  u8;

typedef struct sNode
{
  unsigned short u16data;        //数据                  
  struct sNode * pNext;         //后向指针
}SNODE,*PSNODE;                //单向链表

typedef struct dNode
{
  struct dNode * pPre;          //前向指针
  unsigned short u16data;                          
  struct dNode * pNext;         //后向指针
}DNODE,*PDNODE;                //双向链表


PSNODE CratSList(unsigned int Node_Number,unsigned char ifcycle);
void FreeSList(PSNODE phead);
PDNODE CratDList(unsigned int Node_Number,unsigned char ifcycle);
void FreeDList(PSNODE phead);


#endif