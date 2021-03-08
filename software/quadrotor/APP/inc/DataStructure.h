#ifndef __DATASTRUCTURE_H
#define __DATASTRUCTURE_H

//By BD 2021��1��13��
//#ifdef u32
//#else
//typedef unsigned int  u32;
//#endif
//typedef unsigned short u16;
//typedef unsigned char  u8;

typedef struct sNode
{
  float u16data;        //����                  
  struct sNode * pNext;         //����ָ��
}SNODE,*PSNODE;                //��������

typedef struct dNode
{
  struct dNode * pPre;          //ǰ��ָ��
  unsigned short u16data;                          
  struct dNode * pNext;         //����ָ��
}DNODE,*PDNODE;                //˫������


PSNODE CratSList(unsigned int Node_Number,unsigned char ifcycle);
void FreeSList(PSNODE phead);
PDNODE CratDList(unsigned int Node_Number,unsigned char ifcycle);
void FreeDList(PSNODE phead);
PSNODE SearchNode(PSNODE pnode, unsigned int i);
char AddNode(PSNODE pnode, unsigned int Add_Node_Number);
void UpdateDataToNode(PSNODE pnode, unsigned int i, float data);

#endif