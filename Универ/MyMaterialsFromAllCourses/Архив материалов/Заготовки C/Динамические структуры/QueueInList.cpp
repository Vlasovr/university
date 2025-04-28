//+-----------------------------------------+
//| Автор: Relrin (c) 2011                  |
//|	Библиотека для работы с динамической    |
//| структурой "Очередь (FIFO)"             | 
//|                                         |
//| ПРИМЕЧАНИЕ: При необходимости заменить  |
//| char на нужный тип данных               |
//+-----------------------------------------+
#include <stdio.h>
#include <stdlib.h>

struct Node 
{ 
   int  data; 
   Node *next, *prev; 
}; 
typedef Node *PNode;  //указатель на очередь

struct Queue
{ 
   PNode head, tail;   //указатель на начало и конец очереди
}; 

//+--------------------------------------+
//| Добавление элемента в конец очереди  |
//| Queue &Q - адрес на структуру        |
//| int x    - заносимый элемент         |
//+--------------------------------------+
void PushTail ( Queue &Q, int x ) 
{ 
   PNode NewNode; 
   NewNode = new Node;     // создать новый узел 
   NewNode->data = x;      // заполнить узел данными  
   NewNode->prev = Q.tail; 
   NewNode->next = NULL; 
   if ( Q.tail )             // добавить узел в конец списка 
     Q.tail->next = NewNode; 
   Q.tail = NewNode; 
   if ( ! Q.head ) Q.head = Q.tail; 
}