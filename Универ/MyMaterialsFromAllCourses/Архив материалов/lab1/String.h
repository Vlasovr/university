#pragma once
#include <string.h>
#include <iostream>
using namespace std;
class String
{
public:
	String(const char* str = "");
	String(const String& other);
	~String();
	String operator+(const String& other);
	const char* getString() const 
	{
		return m_str;
	}
private:
	char* m_str;
	int m_length;
};

