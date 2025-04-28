#include "String.h"

String::String(const char* str)
{
	m_length = strlen(str);
	m_str = new char[strlen(str) + 1];
	strcpy_s(m_str, strlen(str) + 1, str);
}

String::String(const String& other)
{
	m_length = strlen(other.m_str);
	m_str = new char[strlen(other.m_str) + 1];
	strcpy_s(m_str, strlen(other.m_str) + 1, other.m_str);
}

String::~String()
{
	delete[] m_str;
}

String String::operator +(const String& other) {
	int total_length = strlen(m_str) + strlen(other.m_str) + 1;
	String obj;
	obj.m_str = new char[total_length];
	strcpy_s(obj.m_str, strlen(m_str) + 1, m_str);
	strcat_s(obj.m_str, strlen(other.m_str)+ strlen(m_str) + 1, other.m_str);
	return obj;
}