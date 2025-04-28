#include "String.h"

String::String(const char* str)
{
    str_length = strlen(str);
    inputStr = new char[str_length + 1];
    strcpy(inputStr, str);
}

String::String(const String& other)
{
    str_length = other.str_length;
    inputStr = new char[str_length + 1];
    strcpy(inputStr, other.inputStr);
}

String::~String()
{
    delete[] inputStr;
}

String String::operator+(const String& other) const
{
    if (other.str_length == 0 && str_length == 0)
    {
        return "Ошибка! Вы ввели пустые строки";
    }

    int total_length = str_length + other.str_length;
    String obj;
    obj.inputStr = new char[total_length + 1];
    strcpy(obj.inputStr, inputStr);
    strcat(obj.inputStr + str_length, other.inputStr);
    obj.str_length = total_length;
    return obj;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
    os << str.inputStr;
    return os;
}
