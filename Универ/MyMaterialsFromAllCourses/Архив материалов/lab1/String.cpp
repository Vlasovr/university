#include "String.h"

String::String(const char* str)
{
	m_length = strlen(str);
	m_str = new char[m_length + 1];
	strcpy_s(m_str, m_length + 1, str);
}

String::String(const String& other)
{
	m_length = other.m_length;
	m_str = new char[m_length + 1];
	strcpy_s(m_str, m_length + 1, other.m_str);
}

String::~String()
{
	delete[] m_str;
}

String String::operator +(const String& other) {
	int total_length = m_length + other.m_length + 1;
	String obj;
	obj.m_str = new char[total_length];
	strcpy_s(obj.m_str, m_length + 1, m_str);
	strcpy_s(obj.m_str, m_length + 1, other.m_str);
	return obj;
}
