#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

inline void checkStrInput(char* str)
{
    string input;
    bool valid = false;
    while (!valid) {
        cout << "Введите строку: ";
        getline(cin, input);

        // Проверка на наличие цифр в строке
        bool containsDigit = false;
        for (char c : input) {
            if (isdigit(c)) {
                containsDigit = true;
                break;
            }
        }

        if (containsDigit) {
            cout << "Ошибка: Строка должна содержать только буквы. Пожалуйста, попробуйте снова." << endl;
        } else {
            valid = true;
            strncpy(str, input.c_str(), sizeof(str) - 1);  // Копирование строки в массив char
            str[sizeof(str) - 1] = '\0';  // Установка нулевого символа в конце массива
        }
    }
}


inline int checkIntInput()
{
    int num;
    string input;
    bool valid = false;
    while (!valid) {
        cout << "Введите число: ";
        getline(cin, input);
        try
        {
            num = std::stoi(input);
            valid = true;
        }
        catch (const std::exception& e)
        {
            cout << "Ошибка: Некорректный ввод" << endl;
        }
    }
    return num;
}

inline unsigned int checkArraySize()
{
    int num;
    string input;
    bool valid = false;
    while (!valid) {
        cout << "Введите число: ";
        getline(cin, input);
        try
        {
           if (((num = std::stoi(input)) < 5))
            valid = true;
        }
        catch (const std::exception& e) {
            cout << "Ошибка: Некорректный ввод" << endl;
        }
    }
    return num;
}

template <typename T>
inline int checkAsciiSum(const T& element)
{
    string str = typeid(element).name();
    int *ascii_str = new int[str.length()];
    int ascii_sum = 0;

    for (int i = 0; i < str.length(); i++)
    {
        ascii_str[i] = str[i];
        ascii_sum += ascii_str[i];
    }

    return ascii_sum;
}