#include <stdio.h>
#include <stdlib.h>
#include "DataStructure.h"

//Brief:��������ѭ����ѡ����
//  name:Crat(Creat)S(single direction)List;
//parameters��
//  Node_Number:�ڵ������������ͷ��㣩
//  ifcycle:�Ƿ���ѭ������;1=��;0=����
//Return��
//  PDNODE:��������ͷ���ָ��;
//  NULL:����ʧ��;
//Example:
//    CratSList(10,true);//
PSNODE CratSList(unsigned int Node_Number,unsigned char ifcycle)
{
  PSNODE pNew;
  PSNODE pHead;
  PSNODE pLast;
  
  pHead = (PSNODE)malloc(sizeof(SNODE));
  if(NULL==pHead)
  {
//    printf("malloc failed!\n");
    return NULL;
  }
  //ͷ������ݳ�ʼ��
  pHead->u16data = Node_Number;			//ͷ�ڵ��������
//  pHead->pPre = NULL;
  pHead->pNext = NULL;
  pLast = pHead;				//��Ϊ���ͷ�����Ҫ���صģ�ֱ��return���ܱ䣬������һ���±���
  
  while(Node_Number--)
  {
    pNew = (PSNODE)malloc(sizeof(SNODE));
    if(NULL==pNew)
    {
//      printf("malloc failed!\n");
//      exit(0);
          return NULL;
    }
    pNew->u16data = 0;
    pLast->pNext = pNew;                //��һ�����µ�
//    pNew->pPre = pLast;
      pNew->pNext = NULL;             //������ĩβ��0
    pLast = pNew;
  }
  if(ifcycle)                         //�½ڵ�β������
  {
    pLast->pNext = pHead->pNext;           //��ͷ
//    pHead->pNext->pPre = pLast;           //ҧβ
  }
  return pHead;
}

//����pnode����ĵ�i�����
PSNODE SearchNode(PSNODE pnode, unsigned int i){
  
  for(int j = 0;j<i;j++)
  {
    pnode = pnode->pNext;
  }
  return pnode;
}

//Ϊpnode��ӽ��
//Add_Node_Number��Ҫ��ӵĸ���
char AddNode(PSNODE pnode, unsigned int Add_Node_Number){
  PSNODE pNew;
  PSNODE pLast;

  pLast = SearchNode(pnode, (unsigned short)pnode->u16data);            //�ҵ�pnode��������һ�����
  pnode->u16data += Add_Node_Number;			//ͷ�ڵ�������� 
  while(Add_Node_Number--)
  {
    pNew = (PSNODE)malloc(sizeof(SNODE));
    if(NULL==pNew)
    {
      return -1;
         
    }
    pNew->u16data = 0;
    pLast->pNext = pNew;                //��һ�����µ�
//    pNew->pPre = pLast;
      pNew->pNext = NULL;             //������ĩβ��0
    pLast = pNew;
  }
  return 0;
}

//�޸������еĵ�i����������Ϊdata
void UpdateDataToNode(PSNODE pnode, unsigned int i, float data){
  pnode = SearchNode(pnode, i);
  pnode -> u16data = data;
}

//Brief:�ͷŵ���ѭ����ѡ����ͷ����u16data�����������ȣ�������ͷ��㣩
//  name:FreeS(single)List;
//parameters��
//  phead:ͷ���;
//Return��
//  void:��������ͷ���ָ��;
//Example:
//    FreeSList(PSNODEָ��);//����10���ڵ��ѭ����������
void FreeSList(PSNODE phead)
{
  PSNODE tp = phead;
  unsigned short len = (unsigned short)phead->u16data;
  phead = phead->pNext;
  free(tp);     //�ͷ�ͷ���
  while(len--)
  {
    tp = phead;
    phead = phead->pNext;
    free(tp);
  }
}

//Brief:����˫��ѭ����ѡ����
//  name:Crat(Creat)D(double direction)List;
//parameters��
//  Node_Number:�ڵ������������ͷ��㣩
//  ifcycle:�Ƿ���ѭ������;1=��;0=����
//Return��
//  PDNODE:˫������ͷ���ָ��;
//  NULL:����ʧ��;
//Example:
//    CratDList(10,true);//����10���ڵ��ѭ��˫������
PDNODE CratDList(unsigned int Node_Number,unsigned char ifcycle)
{
  PDNODE pNew;
  PDNODE pHead;
  PDNODE pLast;
  
  pHead = (PDNODE)malloc(sizeof(DNODE));
  if(NULL==pHead)
  {
//    printf("malloc failed!\n");
//    exit(0);
    return NULL;
  }
  //ͷ������ݳ�ʼ��
  pHead->u16data = Node_Number;			//ͷ�ڵ��������
  pHead->pPre = NULL;
  pHead->pNext = NULL;
  pLast = pHead;				//��Ϊ���ͷ�����Ҫ���صģ�ֱ��return���ܱ䣬������һ���±���
  
  while(Node_Number--)
  {
    pNew = (PDNODE)malloc(sizeof(DNODE));
    if(NULL==pNew)
    {
//      printf("malloc failed!\n");
//      exit(0);
    return NULL;
    }
    pNew->u16data = 0;
    pLast->pNext = pNew;                //��һ�����µ�
    pNew->pPre = pLast;
      pNew->pNext = NULL;             //������ĩβ��0
    pLast = pNew;
  }
  if(ifcycle)                         //�½ڵ�β������
  {
    pLast->pNext = pHead->pNext;           //��ͷ
    pHead->pNext->pPre = pLast;           //ҧβ
  }
  return pHead;
}
//Brief:�ͷ�˫��ѭ����ѡ����ͷ����u16data�����������ȣ�������ͷ��㣩
//  name:FreeD(double direction)List;
//parameters��
//  phead:ͷ���;
//Return��
//  void:��������ͷ���ָ��;
//Example:
//    FreeDList(PSNODEָ��);//
void FreeDList(PSNODE phead)
{
  PSNODE tp = phead;
  unsigned short len = (unsigned short)phead->u16data;
  phead = phead->pNext;
  free(tp);     //�ͷ�ͷ���
  while(len--)
  {
    tp = phead;
    phead = phead->pNext;
    free(tp);
  }
}

