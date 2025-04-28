#include <iostream>


class String
{
public:
    String(const char* str="");
    String(const String& other);
    ~String();

    String operator+(const String& other) const;

    friend std::ostream& operator<<(std::ostream& os, const String& str);

private:
    char* inputStr;
    int str_length;
};
