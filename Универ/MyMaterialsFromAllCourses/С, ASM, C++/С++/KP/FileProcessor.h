#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include "PassengerTransport.h"

using namespace std;

template<typename T>
class FileProcessor {
public:
    FileProcessor(const char* filename, ios::openmode mode) : filename_(filename)
    {
        file.open(filename, mode);
        if (!file.is_open()) {
            throw runtime_error("Такого файла нет!");
        }
    }

    bool writeData(const T& data);
    bool writeDataToClearFile(const T& data);
    bool readData(int dataIndex, T& data);
    bool swapLastTwoData();
    size_t getNumData();

private:
    const char* filename_;
    fstream file;
};


template<typename T>
bool FileProcessor<T>::writeData(const T& data) {
    try {

        ofstream output(filename_, ios::binary | ios::app);
        if (!output) {
            throw runtime_error("Ошибка в открытии файла!");
        }

        // Записываем данные в файл как массив байтов
        output.write(reinterpret_cast<const char*>(&data), sizeof(T));
        output.close();

        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка в записи файла: " << e.what() << endl;
        return false;
    }
}

template<typename T>
bool FileProcessor<T>::writeDataToClearFile(const T& data)
{
    try {
        ofstream output(filename_, ios::out | ios::binary | ios::trunc);
        if (!output) {
            throw runtime_error("Ошибка в открытии файла!");
        }
        output.write(reinterpret_cast<const char*>(&data), sizeof(T));
        output.close();

        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка в записи файла: " << e.what() << endl;
        return false;
    }
}

template<typename T>
bool FileProcessor<T>::readData(int dataIndex, T& data) {
    try {
        ifstream input(filename_, ios::binary);
        if (!input) {
            throw runtime_error("Ошибка в открытии файла!");
        }

        // Получаем размер файла в байтах
        streamsize fileSize = input.seekg(0, ios::end).tellg();

        // Проверяем, что индекс данных не выходит за границы файла
        if (fileSize < (dataIndex + 1) * sizeof(T))
        {
            throw out_of_range("Выход за границы массива!");
        }

        // Перемещаемся к нужному смещению от начала файла
        input.seekg(dataIndex * sizeof(T), ios::beg);

        // Читаем данные из файла как массив байтов
        input.read(reinterpret_cast<char*>(&data), sizeof(T));
        input.close();

        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка в чтении файла: " << e.what() << endl;
        return false;
    }
}

template<typename T>
bool FileProcessor<T>::swapLastTwoData() {
    try {
        fstream file(filename_, ios::binary | ios::in | ios::out);
        if (!file) {
            throw runtime_error("Ошибка в открытии файла!");
        }

        // Перемещаемся к последнему элементу данных в файле
        file.seekg(-1 * sizeof(T), ios::end);

        // Читаем последний элемент данных из файла как массив байтов
        T lastData;
        file.read(reinterpret_cast<char*>(&lastData), sizeof(T));

        // Читаем предпоследний элемент данных из файла как массив байтов
        T secondToLastData;
        file.seekg(-2 * sizeof(T), ios::cur);
        file.read(reinterpret_cast<char*>(&secondToLastData), sizeof(T));

        // Перемещаемся к предпоследнему элементу данных в файле и записываем туда последний элемент данных как массив байтов
        file.seekg(-2 * sizeof(T), ios::end);
        file.write(reinterpret_cast<const char*>(&lastData), sizeof(T));

        // Записываем предпоследний элемент данных как массив байтов после последнего элемента данных
        file.write(reinterpret_cast<const char*>(&secondToLastData), sizeof(T));

        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка в swap: " << e.what() << endl;
        return false;
    }
}

template<typename T>
size_t FileProcessor<T>::getNumData() {
    try
    {
        ifstream file(filename_, ios::binary);

        if (!file) {
            throw runtime_error("Ошибка в открытии файла!");
        }

        streamsize fileSize = file.seekg(0, ios::end).tellg();
        file.close();

        return fileSize / sizeof(T);
    }
    catch (const exception &e)
    {
        cerr << "Ошибка!: " << e.what() << endl;
        return 0;
    }
}
