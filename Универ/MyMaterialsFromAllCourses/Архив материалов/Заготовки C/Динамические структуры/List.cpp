//+-----------------------------------------+
//| Автор: Relrin (c) 2011                  |
//|	Библиотека для работы с динамической    |
//| структурой "Односвязный cписок"         | 
//|                                         |
//| ПРИМЕЧАНИЕ: При необходимости заменить  |
//| char на нужный тип данных               |
//+-----------------------------------------+
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node 
{ 
	char  word[40];  // область данных 
    int   count; 
    Node  *next;     // ссылка на следующий узел      
}; 

typedef Node *PNode;  // тип данных: указатель на узел 
PNode   Head = NULL;  // указатель на начало списка
  
//+--------------------------------------+
//| Добавление элемента в список         |
//| NewWord[] - вносимое слово           |
//+--------------------------------------+
PNode CreateNode(char NewWord[]) 
{ 
    PNode NewNode = new Node;       // указатель на новый узел 
    strcpy(NewNode->word, NewWord); // записать слово 
    NewNode->count = 1;             // счетчик слов = 1 
    NewNode->next = NULL;           // следующего узла нет 
    return NewNode;                 // результат функции – адрес узла 
}

//+--------------------------------------+
//| Добавление элемента в начало списока |
//| PNode &Head   - ук. на начало списка |
//| PNode NewNode - ук. на новый узел    |
//+--------------------------------------+
void AddFirst (PNode &Head, PNode NewNode) 
{ 
    NewNode->next = Head; 
    Head = NewNode; 
}

//+--------------------------------------+
//| Добавление узла после заданного эл-а |
//| PNode p - указатель на элемент, после|
//| которого нужно вставить              |
//| PNode NewNode - ук. на новый узел    |
//+--------------------------------------+
void AddAfter (PNode p, PNode NewNode) 
{ 
   NewNode->next = p->next; 
   p->next = NewNode; 
} 

//+--------------------------------------+
//| Добавление узла после заданного эл-а |
//| PNode &Head   - ук. на начало списка |
//| PNode p - указатель на элемент, после|
//| которого нужно вставить              |
//| PNode NewNode - ук. на новый узел    |
//+--------------------------------------+
void AddBefore(PNode &Head, PNode p, PNode NewNode) 
{ 
	PNode q = Head; 
	if (Head == p) 
	{             
		AddFirst(Head, NewNode); //вставка перед первым узлом 
	    return; 
    } 
    while (q && q->next!=p)     //ищем узел, за которым следует p  
    q = q->next; 
    if (q)                      //если нашли такой узел, 
     AddAfter(q, NewNode);      //добавить новый после него 
}

//+--------------------------------------+
//| Добавление узла в конец списка       |
//| PNode &Head   - ук. на начало списка |
//| PNode NewNode - ук. на новый узел    |
//+--------------------------------------+
void AddLast(PNode &Head, PNode NewNode) 
{ 
	PNode q = Head; 
	if (Head == NULL) 
	{  
		//если список пуст, то вставляем первый элемент 
		AddFirst(Head, NewNode); 
	    return; 
    }  
	//ищем последний элемент 
	while (q->next) q = q->next; 
	  AddAfter(q, NewNode); 
} 

//+--------------------------------------+
//| Поиск по списку                      |
//| PNode &Head   - ук. на начало списка |
//| NewWord[]     - искомое слово        |
//+--------------------------------------+
PNode Find (PNode Head, char NewWord[]) 
{ 
   PNode q = Head; 
   while (q && strcmp(q->word, NewWord))  
      q = q->next; 
   return q; 

   /* 
   Если необходимо вернуть адрес узла, перед которым надо вставить новое слово

   PNode q = Head; 
   while (q && (strcmp(q->word, NewWord) > 0))  
     q = q->next; 
   return q; 

   */
} 

//+--------------------------------------+
//| Удаление узла                        |
//| PNode &Head   - ук. на начало списка |
//| PNode OldNode - удаляемый элемент    |
//+--------------------------------------+
void DeleteNode(PNode &Head, PNode OldNode) 
{ 
	PNode q=Head; 
	if (Head==OldNode)  
     Head=OldNode->next;     //удаляем первый элемент 
    else 
    { 
		while(q&&q->next!=OldNode) //ищем элемент 
          q=q->next; 
        if (q==NULL)return; //если не нашли, выход 
        q->next=OldNode->next; 
    } 
	delete OldNode;         //освобождаем память 
}

