
#include "InterfaceWithTemplateList.h"
#include "InterfaceWithFile.h"

template <typename T>
void InterfaceWithTemplateList<T>::menuForUser()
{
    int enteredNumber;
    while(enteredNumber != 4)
    {
        cout << "Для работы с классом Student нажмите 1" << endl;
        cout << "Для работы с классом PassengerTransport нажмите 2" << endl;
        cout << "Для работы с классом int нажмите 3" << endl;
        cout << "Конец - нажмите 4" << endl;

        enteredNumber = checkIntInput();

        switch(enteredNumber)
        {
            case 1:
            {
                InterfaceWithTemplateList<Student> student;
                student.interactWithTObject();
            }
                break;

            case 2:
            {
                InterfaceWithTemplateList<PassengerTransport> transport;
                transport.interactWithTObject();
            }
                break;

            case 3:
            {
                InterfaceWithTemplateList<int> num;
                num.interactWithTObject();
            }
                break;

            case 4:
                break;
        }
    }
}

template <typename T>
void InterfaceWithTemplateList<T>::interactWithTObject()
{
    int enteredNumber;
    TemplateList<T> list;
    T data;
    size_t position;
    while(enteredNumber != 13)
    {
        cout << "\n\n";
        cout << "Для вызова push_front нажмите 0" << endl;
        cout << "Для вызова push_back нажмите 1" << endl;
        cout << "Для вызова pop_front нажмите 2" << endl;
        cout << "Для вызова pop_back нажмите 3" << endl;
        cout << "Для вывода размера списка нажмите 4" << endl;
        cout << "Для замены всех элементов списка определенным набором нажмите 5" << endl;
        cout << "Для проверки пустой ли список нажмите 6" << endl;
        cout << "Для вставки элемента по индексу нажмите 7" << endl;
        cout << "Для просмотра всех элементов списка нажмите 8" << endl;
        cout << "Для очистки всего содержимого списка намжите 9" << endl;
        cout << "Для поиска нужного элемента в списке нажмите 10" << endl;
        cout << "Для поиска элемента по индексу в списке нажмите 11" << endl;
        cout << "Для обмена соседних элементов списка нажмите 12" << endl;
        cout << "Для выхода - нажмите 13" << endl;

        enteredNumber = checkIntInput();

        switch (enteredNumber)
        {
            case 0:
            {
                cout << "Введите объект для записи в список" << endl;
                cin >> data;
                list.push_front(data);

            }
                break;

            case 1:
            {
                cout << "Введите объект для записи в список" << endl;
                cin >> data;
                list.push_back(data);
            }
                break;

            case 2:
            {
                list.pop_front();
            }
                break;

            case 3:
            {
                list.pop_back();
            }
                break;
            case 4:
            {
                list.get_size();
            }
                break;
            case 5:
            {
                cout << "Введите количество элементов" << endl;
                cin >> position;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем входной буфер
                cout << "Введите элемент, который будет повторяться" << endl;
                cin >> data;
                list.assign(position, data);
                cout << "Измененный список: " << endl;
                list.print();
            }
                break;

            case 6:
            {
                cout << boolalpha << list.empty() << endl;
            }
                break;

            case 7:
            {
                cout << "Введите индекс" << endl;
                cin >> position;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем входной буфер
                cout << "Введите будущий элемент списка" << endl;
                cin >> data;
                list.insert(position, data);
                cout << "Измененный список: " << endl;
                print(data);
                list.print();
            }
                break;

            case 8:
            {
                print(data);
                list.print();
            }
                break;

            case 9:
            {
                list.clear();
            }
                break;

            case 10:
            {
                cout << "Введите элемент, который хотели бы найти в списке" << endl;
                cin >> data;
                cout << "Элемент найден под индексом " << list.find(data) << endl;
            }
                break;

            case 11:
                cout << "Введите индекс для поиска" << endl;
                cin >> position;
                try {
                    if(position >= list.size())
                    {
                        throw out_of_range("Index out of range");
                    }
                    cout << "Элемент под " << position << " : " << endl;
                    print(data);
                    cout << list.getValueAtIndex(position) << endl;
                } catch (const exception& ex) {
                    cout << "Попробуйте в другой раз" << endl;
                }
                break;

            case 12:
                cout << "Для обмена узлов списка местами введите индекс первого элемента: " << endl;
                cin >> position;
                try {
                    if(position >= list.size())
                    {
                        throw out_of_range("Index out of range");
                    }
                    list.swap(position - 1);
                    cout<< "Обновленный список:" << endl;
                    print(data);
                    list.print();
                } catch (const exception& ex) {
                    cout << "Попробуйте в другой раз" << endl;
                }
                break;

            case 13:
                break;
        }
    }
}

template <typename T>
void InterfaceWithTemplateList<T>::print(const T& element)
{
    switch(checkAsciiSum(element))
    {
        case 2014:
            cout << setw(15) << "Имя транспорта " << setw(6) << "Скорость транспорта " << setw(6) << "Вместимость транспорта " << setw(6) << "Стоимость поездки составит "<< setw(6) << "Время поездки составит " << endl;
            break;
        case 798:
            cout << setw(15) << "Имя студента " << setw(6) << "Средняя оценка  " << setw(6) << "Возраст " << endl;
            break;
        case 105:
            cout << "Числа: ";
            break;
        default:
            break;
    }
}