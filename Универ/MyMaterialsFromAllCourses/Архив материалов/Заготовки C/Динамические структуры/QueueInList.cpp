//+-----------------------------------------+
//| �����: Relrin (c) 2011                  |
//|	���������� ��� ������ � ������������    |
//| ���������� "������� (FIFO)"             | 
//|                                         |
//| ����������: ��� ������������� ��������  |
//| char �� ������ ��� ������               |
//+-----------------------------------------+
#include <stdio.h>
#include <stdlib.h>

struct Node 
{ 
   int  data; 
   Node *next, *prev; 
}; 
typedef Node *PNode;  //��������� �� �������

struct Queue
{ 
   PNode head, tail;   //��������� �� ������ � ����� �������
}; 

//+--------------------------------------+
//| ���������� �������� � ����� �������  |
//| Queue &Q - ����� �� ���������        |
//| int x    - ��������� �������         |
//+--------------------------------------+
void PushTail ( Queue &Q, int x ) 
{ 
   PNode NewNode; 
   NewNode = new Node;     // ������� ����� ���� 
   NewNode->data = x;      // ��������� ���� �������  
   NewNode->prev = Q.tail; 
   NewNode->next = NULL; 
   if ( Q.tail )             // �������� ���� � ����� ������ 
     Q.tail->next = NewNode; 
   Q.tail = NewNode; 
   if ( ! Q.head ) Q.head = Q.tail; 
}