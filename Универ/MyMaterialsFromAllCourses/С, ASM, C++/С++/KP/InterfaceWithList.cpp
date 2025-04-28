#include "InterfaceWithList.h"
#include "Algorithms.cpp"

template <typename T>
void InterfaceWithList<T>::menuForUser()
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
                InterfaceWithList<Student> student;
                student.interactWithTObject();
            }
                break;

            case 2:
            {
                InterfaceWithList<PassengerTransport> transport;
                transport.interactWithTObject();
            }
                break;

            case 3:
            {
                InterfaceWithList<int> num;
                num.interactWithTObject();
            }
                break;

            case 4:
                break;
        }
    }
}

template <typename T>
void InterfaceWithList<T>::interactWithTObject()
{
    int enteredNumber;
    ListWithIterator<T> list;
    typename ListWithIterator<T>::template iterator<T> it;
    FileProcessor<T> file("testing.dat", ios::binary |ios::app);
    T data;
    size_t position;
    while(enteredNumber != 18) {
        cout << "\n\n";
        cout << "Для вызова push_front нажмите 0" << endl;
        cout << "Для вызова push_back нажмите 1" << endl;
        cout << "Для вызова pop_front нажмите 2" << endl;
        cout << "Для вызова pop_back нажмите 3" << endl;
        cout << "Для вывода первого элемента списка нажмите 4" << endl;
        cout << "Для вывода последнего элемента списка нажмите 5" << endl;
        cout << "Для вывода размера списка нажмите 6" << endl;
        cout << "Для замены всех элементов списка определенным набором нажмите 7" << endl;
        cout << "Для проверки пустой ли список нажмите 8" << endl;
        cout << "Для вставки элемента по индексу нажмите 9" << endl;
        cout << "Для просмотра всех элементов списка нажмите 10" << endl;
        cout << "Для очистки всего содержимого списка намжите 11" << endl;
        cout << "Для поиска нужного элемента в списке нажмите 12" << endl;
        cout << "Для поиска элемента по индексу в списке нажмите 13" << endl;
        cout << "Для обмена соседних элементов списка нажмите 14" << endl;
        cout << "Для сохранения файла нажмите 15" << endl;
        cout << "Для просмотра содержимого файла нажмите 16" << endl;
        cout << "Для работы с двунаправленной очередью нажмите 17" << endl;
        cout << "Для выхода - нажмите 18" << endl;

        enteredNumber = checkIntInput();

        switch (enteredNumber) {
            case 0: {
                cout << "Введите объект для записи в список" << endl;
                cin >> data;
                list.push_front(data);
            }
                break;

            case 1: {
                cout << "Введите объект для записи в список" << endl;
                cin >> data;
                list.push_back(data);
            }
                break;

            case 2: {
                list.pop_front();
            }
                break;

            case 3: {
                list.pop_back();
            }
                break;
            case 4:
            {
                cout << list.front();
            }
                break;
            case 5:
            {
                cout << list.back();
            }
                break;
            case 6: {
                list.get_size();
            }
                break;

            case 7: {
                cout << "Введите количество элементов" << endl;
                cin >> position;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем входной буфер
                cout << "Введите элемент, который будет повторяться" << endl;
                cin >> data;
                list.assign(position, data);
                cout << "Измененный список: " << endl;
                list.print();
            }
                break;

            case 8: {
                cout << boolalpha << list.empty() << endl;
            }
                break;
            case 9: {
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

            case 10: {
                print(data);
                list.print();
            }
                break;

            case 11: {
                list.clear();
            }
                break;

            case 12: {
                cout << "Введите элемент для поиска" << endl;
                cin >> data;
                auto result = Algorithms<T>::find(list, data);
                if (result != list.end()) {
                    cout << "Элемент найден под индексом ";
                    int index = 0;
                    for (auto it = list.begin(); it != result; ++it) {
                        ++index;
                    }
                    cout << index << endl;
                } else {
                    cout << "Элемент не найден" << endl;
                }
            }
                break;

            case 13:
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

            case 14:
                cout << "Для обмена узлов списка местами введите индекс первого элемента: " << endl;
                cin >> position;
                try {
                    if(position >= list.size())
                    {
                        throw out_of_range("Index out of range");
                    }

                    list.swap(position);
                    cout<< "Обновленный список:" << endl;
                    print(data);
                    list.print();
                } catch (const exception& ex) {
                    cout << "Попробуйте в другой раз" << endl;
                }
                break;

            case 15: {
                T data = list.front();
                file.writeDataToClearFile(data);
                for (size_t i = 1; i < list.size(); ++i) {
                    data = list[i];
                    cout << "объект записан ";
                    file.writeData(data);
                }
                break;
            }

            case 16:
                print(data);
                for (size_t i = 0; i < list.size(); ++i) {
                    T data;
                    file.readData(i, data);
                    cout << data << endl;
                }
                break;

            case 17:
                InterfaceForQueue();

            case 18:
                break;
        }
    }
}

template <typename T>
void InterfaceWithList<T>::print(const T& element)
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

template <typename T>
void InterfaceWithList<T>::InterfaceForQueue()
{
    int enteredNumber;
    deque<T> dq;
    auto it = dq.begin();
    FileProcessor<T> file("testing.dat", ios::binary |ios::app);
    T data;
    size_t position;
    while(enteredNumber != 15) {
        cout << "\n\n";
        cout << "Для вызова push_front нажмите 0" << endl;
        cout << "Для вызова push_back нажмите 1" << endl;
        cout << "Для вызова pop_front нажмите 2" << endl;
        cout << "Для вызова pop_back нажмите 3" << endl;
        cout << "Для вывода первого элемента списка нажмите 4" << endl;
        cout << "Для вывода последнего элемента списка нажмите 5" << endl;
        cout << "Для вывода размера списка нажмите 6" << endl;
        cout << "Для замены всех элементов списка определенным набором нажмите 7" << endl;
        cout << "Для проверки пустой ли список нажмите 8" << endl;
        cout << "Для вставки элемента по индексу нажмите 9" << endl;
        cout << "Для просмотра всех элементов списка нажмите 10" << endl;
        cout << "Для очистки всего содержимого списка намжите 11" << endl;
        cout << "Для поиска элемента по индексу в списке нажмите 12" << endl;
        cout << "Для сохранения файла нажмите 13" << endl;
        cout << "Для просмотра содержимого файла нажмите 14" << endl;
        cout << "Для выхода - нажмите 15" << endl;

        enteredNumber = checkIntInput();

        switch (enteredNumber) {
            case 0: {
                cout << "Введите объект для записи в список" << endl;
                cin >> data;
                dq.push_front(data);
            }
                break;

            case 1: {
                cout << "Введите объект для записи в список" << endl;
                cin >> data;
                dq.push_back(data);
            }
                break;

            case 2: {
                dq.pop_front();
            }
                break;

            case 3: {
                dq.pop_back();
            }
                break;
            case 4:
            {
                cout << dq.front();
            }
                break;
            case 5:
            {
                cout << dq.back();
            }
                break;
            case 6: {
                dq.size();
            }
                break;

            case 7: {
                cout << "Введите количество элементов" << endl;
                cin >> position;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем входной буфер
                cout << "Введите элемент, который будет повторяться" << endl;
                cin >> data;
                dq.assign(position, data);
                cout << "Измененный список: " << endl;
                printQueue(dq);
            }
                break;

            case 8: {
                cout << boolalpha << dq.empty() << endl;
            }
                break;
            case 9: {
                cout << "Введите индекс" << endl;
                cin >> position;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем входной буфер
                cout << "Введите будущий элемент списка" << endl;
                cin >> data;
                dq.insert(it, position, data);
                cout << "Измененный список: " << endl;
                print(data);
                printQueue(dq);
            }
                break;

            case 10: {
                print(data);
                printQueue(dq);
            }
                break;

            case 11: {
                dq.clear();
            }
                break;

            case 12:
                cout << "Введите индекс для поиска" << endl;
                cin >> position;
                try {
                    if(position >= dq.size())
                    {
                        throw out_of_range("Index out of range");
                    }
                    cout << "Элемент под " << position << " : " << endl;
                    print(data);
                    cout << dq[position] << endl;
                } catch (const exception& ex) {
                    cout << "Попробуйте в другой раз" << endl;
                }
                break;

            case 13: {
                T data = dq.front();
                file.writeDataToClearFile(data);
                for (size_t i = 1; i < dq.size(); ++i) {
                    data = dq[i];
                    cout << "объект записан ";
                    file.writeData(data);
                }
                break;
            }

            case 14:
                print(data);
                for (size_t i = 0; i < dq.size(); ++i) {
                    T data;
                    file.readData(i, data);
                    cout << data << endl;
                }
                break;

            case 15:
                break;
        }
    }
}

template <typename T>
void InterfaceWithList<T>::printQueue(const deque<T>& dq)
{
    for (auto it = dq.begin(); it != dq.end(); it++)
        std::cout << *it << "\t";
    std::cout << std::endl;
}