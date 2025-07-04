//+-----------------------------------------+
//| �����: Relrin (c) 2011                  |
//|	���������� ��� ������ � ������������    |
//| ���������� "���� (LIFO)"                | 
//|                                         |
//| ����������: ��� ������������� ��������  |
//| char �� ������ ��� ������               |
//+-----------------------------------------+
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100

struct stack
{
    char data[MAXSIZE];     //��� ����
	int size;               //������� ������ �����
};

void Push(stack &s,char x); //���������� �������� � ���� 
char Pop (stack &s);        //��������� �������� �� ������� �����

//+--------------------------------------+
//| ���������� �������� � ����           |
//| stack &s - ����� �� ���������        |
//| char x   - ��������� �������         |
//+--------------------------------------+
void Push(stack &s,char x)
{
	//��������� �� ���������������
	if(s.size==MAXSIZE)
	{
		printf("Stack overflow!");
		return;
	}
	//���� ����� ����, �� ��������� ��� �������
	s.data[s.size]=x;
	s.size++;
}

//+--------------------------------------+
//| ��������� �������� �� ������� �����  |
//| stack &s - ����� �� ���������        |
//+--------------------------------------+
char Pop(stack &s)
{
	//��������� �� ������ ����
	if(s.size==0)
	{
		printf("Stack is empty!");
		return;
	}
	//��������� ����������� �������
	s.size--;
	//���������� �������� ��������
	return s.data[s.size];
}

