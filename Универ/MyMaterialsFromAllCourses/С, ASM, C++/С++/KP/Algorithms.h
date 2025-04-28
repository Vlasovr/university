#pragma once
#include "ListWithIterator.cpp"

using namespace std;

template <typename T>
class Algorithms {
public:
    typedef typename ListWithIterator<T>::template iterator<T> iterator;

    static iterator find(ListWithIterator<T>& list, const T& value)
    {
        for (iterator it = list.begin(); it != list.end(); ++it)
        {
            if (*it == value)
                return it;
        }

        return list.end();
    }

    static void removeValue(ListWithIterator<T>& list, const T& value)
    {
        iterator it = find(list, value);
        if (it != list.end())
        {
            list.erase(it);
            cout << "Элемент удален успешно" << endl;
        } else {
            cout << "Элемент не найден" << endl;
        }
    }

    static iterator clear(ListWithIterator<T>& list)
    {
        for (iterator it = list.begin(); it != list.end(); ++it)
        {
            list.pop_front();
        }
    }

};
