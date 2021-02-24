#include <stdio.h>
#include <stdlib.h>
#include "DataStructure.h"

//Brief:创建单向循环可选链表
//  name:Crat(Creat)S(single direction)List;
//parameters：
//  Node_Number:节点个数（不包含头结点）
//  ifcycle:是否是循环链表;1=是;0=不是
//Return：
//  PDNODE:单向链表头结点指针;
//  NULL:创建失败;
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
  //头结点数据初始化
  pHead->u16data = Node_Number;			//头节点放链表长度
//  pHead->pPre = NULL;
  pHead->pNext = NULL;
  pLast = pHead;				//因为最后头结点是要返回的，直到return不能变，所以用一个新变量
  
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
    pLast->pNext = pNew;                //上一个接新的
//    pNew->pPre = pLast;
      pNew->pNext = NULL;             //单链表末尾放0
    pLast = pNew;
  }
  if(ifcycle)                         //新节点尾巴连接
  {
    pLast->pNext = pHead->pNext;           //回头
//    pHead->pNext->pPre = pLast;           //咬尾
  }
  return pHead;
}

//搜索pnode链表的第i个结点
PSNODE SearchNode(PSNODE pnode, unsigned int i){
  
  for(int j = 0;j<i;j++)
  {
    pnode = pnode->pNext;
  }
  return pnode;
}

//为pnode添加结点
//Add_Node_Number：要添加的个数
char AddNode(PSNODE pnode, unsigned int Add_Node_Number){
  PSNODE pNew;
  PSNODE pLast;

  pLast = SearchNode(pnode, (unsigned short)pnode->u16data);            //找到pnode链表的最后一个结点
  pnode->u16data += Add_Node_Number;			//头节点放链表长度 
  while(Add_Node_Number--)
  {
    pNew = (PSNODE)malloc(sizeof(SNODE));
    if(NULL==pNew)
    {
      return -1;
         
    }
    pNew->u16data = 0;
    pLast->pNext = pNew;                //上一个接新的
//    pNew->pPre = pLast;
      pNew->pNext = NULL;             //单链表末尾放0
    pLast = pNew;
  }
  return 0;
}

//修改链表中的第i个结点的数据为data
void UpdateDataToNode(PSNODE pnode, unsigned int i, float data){
  pnode = SearchNode(pnode, i);
  pnode -> u16data = data;
}

//Brief:释放单向循环可选链表，头结点的u16data必须存放链表长度（不包括头结点）
//  name:FreeS(single)List;
//parameters：
//  phead:头结点;
//Return：
//  void:单向链表头结点指针;
//Example:
//    FreeSList(PSNODE指针);//创建10个节点的循环单向链表
void FreeSList(PSNODE phead)
{
  PSNODE tp = phead;
  unsigned short len = (unsigned short)phead->u16data;
  phead = phead->pNext;
  free(tp);     //释放头结点
  while(len--)
  {
    tp = phead;
    phead = phead->pNext;
    free(tp);
  }
}

//Brief:创建双向循环可选链表
//  name:Crat(Creat)D(double direction)List;
//parameters：
//  Node_Number:节点个数（不包含头结点）
//  ifcycle:是否是循环链表;1=是;0=不是
//Return：
//  PDNODE:双向链表头结点指针;
//  NULL:创建失败;
//Example:
//    CratDList(10,true);//创建10个节点的循环双向链表
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
  //头结点数据初始化
  pHead->u16data = Node_Number;			//头节点放链表长度
  pHead->pPre = NULL;
  pHead->pNext = NULL;
  pLast = pHead;				//因为最后头结点是要返回的，直到return不能变，所以用一个新变量
  
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
    pLast->pNext = pNew;                //上一个接新的
    pNew->pPre = pLast;
      pNew->pNext = NULL;             //单链表末尾放0
    pLast = pNew;
  }
  if(ifcycle)                         //新节点尾巴连接
  {
    pLast->pNext = pHead->pNext;           //回头
    pHead->pNext->pPre = pLast;           //咬尾
  }
  return pHead;
}
//Brief:释放双向循环可选链表，头结点的u16data必须存放链表长度（不包括头结点）
//  name:FreeD(double direction)List;
//parameters：
//  phead:头结点;
//Return：
//  void:单向链表头结点指针;
//Example:
//    FreeDList(PSNODE指针);//
void FreeDList(PSNODE phead)
{
  PSNODE tp = phead;
  unsigned short len = (unsigned short)phead->u16data;
  phead = phead->pNext;
  free(tp);     //释放头结点
  while(len--)
  {
    tp = phead;
    phead = phead->pNext;
    free(tp);
  }
}

