#pragma once
#include "InterfaceWithList.h"
#include "typeinfo"
using namespace std;

template <typename T>
class InterfaceWithFile
{
public:
    InterfaceWithFile() = default;
    ~InterfaceWithFile() = default;
    void menuForUser();
    void interactWithTObject();
    void print(const T& element);
};

template <typename T>
void InterfaceWithFile<T>::print(const T& element)
{
    switch(checkAsciiSum(element))
    {
        case 2014:
            cout << setw(15) << "Имя транспорта " << setw(6) << "Скорость транспорта " << setw(6) << "Вместимость транспорта " << setw(6) << "Стоимость поездки составит "<< setw(6) << "Время поездки составит " << endl;
            break;
        case 798:
            cout << setw(15) << "Имя студента " << setw(6) << "Средняя оценка  " << setw(6) << "Возраст " << endl;
            break;
        default:
            cout << "Числа: ";
            break;
    }
}

template <typename T>
void InterfaceWithFile<T>::menuForUser()
{
    int enteredNumber;
    while(enteredNumber != 4)
    {
        cout << "Для работы в бинарном файле со структурой Student нажмите 1" << endl;
        cout << "Для работы в бинарном файле с классом PassengerTransport нажмите 2" << endl;
        cout << "Для работы в бинарном файле с классом int нажмите 3" << endl;
        cout << "Конец - нажмите 4" << endl;

        enteredNumber = checkIntInput();

        switch(enteredNumber)
        {
            case 1:
            {
                InterfaceWithFile<Student> student;
                student.interactWithTObject();
            }
                break;

            case 2:
            {
                InterfaceWithFile<PassengerTransport> transport;
                transport.interactWithTObject();
            }

            case 3:
            {
                InterfaceWithFile<int> num;
                num.interactWithTObject();
            }
                break;

            case 4:
                break;
        }
    }
}

template <typename T>
void InterfaceWithFile<T>::interactWithTObject()
{
    FileProcessor<T> file("test.dat", ios::out | ios::binary | ios::trunc);
    T element;
    int enteredNumber;

    cout << "Введите желаемое количество элементов в файле" << endl;
    unsigned int arr_size = checkArraySize();

    while(enteredNumber != 5) {
        cout << "\n" << "Для записи элементов в файл нажмите 1" << endl;
        cout << "Чтобы показать имеющиеся элементы в файле нажмите 2" << endl;
        cout << "Чтобы показать количество элементов в файле нажмите 3" << endl;
        cout << "Для замены местами предпоследнего и последнего элементов в файле нажмите 4" << endl;
        cout << "Для выхода - нажмите 5" << endl;
        enteredNumber = checkIntInput();

        switch(enteredNumber)
        {
            case 1:
            {
                for(int i = 0; i < arr_size; i++)
                {
                    cout << "Введите элемент" << endl;
                    cin >> element;
                    file.writeData(element);
                }
            }
                break;

            case 2:
            {
                print(element);
                for(int i = 0; i < arr_size; i++)
                {
                    file.readData(i, element);
                    cout << element;
                }
            }
                break;

            case 3:
            {
                cout <<"Количество элементов в файле: "<< file.getNumData() << "\n\n";
            }
                break;

            case 4:
            {
                file.swapLastTwoData();
                cout << "Замена произведена успешно" << endl;
            }
                break;

            case 5:
                break;
        }
    }
}

