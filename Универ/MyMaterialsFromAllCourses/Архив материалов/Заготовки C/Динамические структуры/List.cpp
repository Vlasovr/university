//+-----------------------------------------+
//| �����: Relrin (c) 2011                  |
//|	���������� ��� ������ � ������������    |
//| ���������� "����������� c�����"         | 
//|                                         |
//| ����������: ��� ������������� ��������  |
//| char �� ������ ��� ������               |
//+-----------------------------------------+
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node 
{ 
	char  word[40];  // ������� ������ 
    int   count; 
    Node  *next;     // ������ �� ��������� ����      
}; 

typedef Node *PNode;  // ��� ������: ��������� �� ���� 
PNode   Head = NULL;  // ��������� �� ������ ������
  
//+--------------------------------------+
//| ���������� �������� � ������         |
//| NewWord[] - �������� �����           |
//+--------------------------------------+
PNode CreateNode(char NewWord[]) 
{ 
    PNode NewNode = new Node;       // ��������� �� ����� ���� 
    strcpy(NewNode->word, NewWord); // �������� ����� 
    NewNode->count = 1;             // ������� ���� = 1 
    NewNode->next = NULL;           // ���������� ���� ��� 
    return NewNode;                 // ��������� ������� � ����� ���� 
}

//+--------------------------------------+
//| ���������� �������� � ������ ������� |
//| PNode &Head   - ��. �� ������ ������ |
//| PNode NewNode - ��. �� ����� ����    |
//+--------------------------------------+
void AddFirst (PNode &Head, PNode NewNode) 
{ 
    NewNode->next = Head; 
    Head = NewNode; 
}

//+--------------------------------------+
//| ���������� ���� ����� ��������� ��-� |
//| PNode p - ��������� �� �������, �����|
//| �������� ����� ��������              |
//| PNode NewNode - ��. �� ����� ����    |
//+--------------------------------------+
void AddAfter (PNode p, PNode NewNode) 
{ 
   NewNode->next = p->next; 
   p->next = NewNode; 
} 

//+--------------------------------------+
//| ���������� ���� ����� ��������� ��-� |
//| PNode &Head   - ��. �� ������ ������ |
//| PNode p - ��������� �� �������, �����|
//| �������� ����� ��������              |
//| PNode NewNode - ��. �� ����� ����    |
//+--------------------------------------+
void AddBefore(PNode &Head, PNode p, PNode NewNode) 
{ 
	PNode q = Head; 
	if (Head == p) 
	{             
		AddFirst(Head, NewNode); //������� ����� ������ ����� 
	    return; 
    } 
    while (q && q->next!=p)     //���� ����, �� ������� ������� p  
    q = q->next; 
    if (q)                      //���� ����� ����� ����, 
     AddAfter(q, NewNode);      //�������� ����� ����� ���� 
}

//+--------------------------------------+
//| ���������� ���� � ����� ������       |
//| PNode &Head   - ��. �� ������ ������ |
//| PNode NewNode - ��. �� ����� ����    |
//+--------------------------------------+
void AddLast(PNode &Head, PNode NewNode) 
{ 
	PNode q = Head; 
	if (Head == NULL) 
	{  
		//���� ������ ����, �� ��������� ������ ������� 
		AddFirst(Head, NewNode); 
	    return; 
    }  
	//���� ��������� ������� 
	while (q->next) q = q->next; 
	  AddAfter(q, NewNode); 
} 

//+--------------------------------------+
//| ����� �� ������                      |
//| PNode &Head   - ��. �� ������ ������ |
//| NewWord[]     - ������� �����        |
//+--------------------------------------+
PNode Find (PNode Head, char NewWord[]) 
{ 
   PNode q = Head; 
   while (q && strcmp(q->word, NewWord))  
      q = q->next; 
   return q; 

   /* 
   ���� ���������� ������� ����� ����, ����� ������� ���� �������� ����� �����

   PNode q = Head; 
   while (q && (strcmp(q->word, NewWord) > 0))  
     q = q->next; 
   return q; 

   */
} 

//+--------------------------------------+
//| �������� ����                        |
//| PNode &Head   - ��. �� ������ ������ |
//| PNode OldNode - ��������� �������    |
//+--------------------------------------+
void DeleteNode(PNode &Head, PNode OldNode) 
{ 
	PNode q=Head; 
	if (Head==OldNode)  
     Head=OldNode->next;     //������� ������ ������� 
    else 
    { 
		while(q&&q->next!=OldNode) //���� ������� 
          q=q->next; 
        if (q==NULL)return; //���� �� �����, ����� 
        q->next=OldNode->next; 
    } 
	delete OldNode;         //����������� ������ 
}

