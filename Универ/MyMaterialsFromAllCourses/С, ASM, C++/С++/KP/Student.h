#pragma once
#include "FileProcessor.h"
#include "Functions.h"
using namespace std;

struct Student
{
    char name[50];
    float averageMark;
    int age;

    friend inline ostream &operator<<(ostream &os, const Student &student)
    {
        os << setw(12) << student.name;
        os << setw(8) << student.averageMark << " баллов";
        os << setw(5) << student.age << " лет" << endl;
        return os;
    }

    friend inline istream& operator>>(istream& is, Student& student)
    {

        cout << "Введите имя студента: " << endl;
        checkStrInput(student.name);

        cout << "Введите среднюю оценку студента: " << endl;
        student.averageMark = checkIntInput();

        cout << "Введите возраст студента: " << endl;
        student.age = checkIntInput();

        return is;
    }

    bool operator==(const Student& other) const {
        return strcmp(name, other.name) == 0 && averageMark == other.averageMark && age == other.age;
    }

    Student& operator=(const Student& other) {
        if (this == &other) {
            return *this;
        }
        strcpy(name, other.name);
        averageMark = other.averageMark;
        age = other.age;
        return *this;
    }

};