//Создать класс для работы со строками.
//В классе перегрузить оператор + для сложения  строк.
//Память под строки отводить динамически.
//Использовать конструктор с параметрами по умолчанию, конструктор копирования.

#include "String.h"

int main()
{

    std::cout << "Введите первую строку: ";
    std::string str1;
    getline(std::cin, str1);
    String s1(str1.c_str());

    std::cout << "Введите вторую строку: ";
    std::string str2;
    getline(std::cin, str2);
    String s2(str2.c_str());

    String s3 = s1 + s2;
    std::cout << "Сконкатенированные строки: " << s3 << std::endl;

    return 0;
}
