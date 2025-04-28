#include "ListWithIterator.h"
template <typename T>
void ListWithIterator<T>::is_empty()
{
    try {
        if (empty())
        {
            throw runtime_error("Лист пуст!");
        }
    } catch (exception& ex)
    {
        cout << "Лист пуст!" << endl;
        throw;
    }
}

template <typename T>
Node<T>* ListWithIterator<T>::getNodeAtIndex(size_t index)
{
    Node<T>* current = head;

    for (size_t i = 0; i < index; i++)
    {
        current = current->next;
    }

    return current;
}

template <typename T>
T& ListWithIterator<T>::operator[](size_t index)
{
    return getValueAtIndex(index);
}

template <typename T>
T& ListWithIterator<T>::getValueAtIndex(size_t index)
{
    try{
        is_empty();
        if (index >= size())
        {
            throw out_of_range("Выход за границы!");
        }
    } catch (exception& ex)
    {
        cout << "Индекс выходит за границы списка" << endl;
        throw;
    }

    Node<T>* node = getNodeAtIndex(index);
    return node->data;
}

template <typename T>
void ListWithIterator<T>::push_front(T data)
{
    Node<T>* newNode = new Node<T>(data);

    if (empty())
    {
        head = newNode;
        tail = newNode;
    } else
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

template <typename T>
void ListWithIterator<T>::push_back(T data)
{
    Node<T>* newNode = new Node<T>(data);
    if (empty())
    {
        head = newNode;
        tail = newNode;
    } else
    {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

template <typename T>
void ListWithIterator<T>::pop_front()
{
    if (empty())
    {
        cout << "Лист пуст!" << endl;
        return;
    }

    Node<T>* temp = head;
    head = head->next;

    if (head)
    {
        head->prev = nullptr;
    } else
    {
        // Если после удаления первого элемента список пустой, обновляем tail
        tail = nullptr;
    }

    delete temp;
    cout << "Объект удален успешно" << endl;
}

template <typename T>
void ListWithIterator<T>::pop_back()
{
    try{
        is_empty();
    } catch (exception& ex)
    {
        return;
    }

    Node<T>* temp = tail;
    tail = tail->prev;

    if (tail)
    {
        tail->next = nullptr;
    } else
    {
        // Если после удаления последнего элемента список пустой, обновляем head
        head = nullptr;
    }
    delete temp;
    cout << "Объект удален успешно" << endl;
}

template <typename T>
size_t ListWithIterator<T>::size()
{
    size_t count = 0;

    for (auto it = begin(); it != end(); ++it)
    {
        ++count;
    }

    return count;
}

template <typename T>
void ListWithIterator<T>::get_size()
{
    cout << "Размер списка составляет " << size() << " элементов" << endl;
}

template <typename T>
void ListWithIterator<T>::assign(size_t count, const T& value)
{
    clear();
    for (size_t i = 0; i < count; ++i)
    {
        push_back(value);
    }
}

template <typename T>
bool ListWithIterator<T>::empty()
{
    return head == nullptr;
}

template <typename T>
void ListWithIterator<T>::swap(size_t index)
{
    try {
        is_empty();
        if (index >= size())
        {
            throw out_of_range("Выход за границы!");
        }
    } catch (exception &ex)
    {
        cout << "Индекс выходит за границы списка" << endl;
        throw;
    }

    Node<T> *current = getNodeAtIndex(index);
    Node<T> *nextNode = current->next;

    if (current == nullptr || nextNode == nullptr)
    {
        cout << "Ошибка: один из элементов равен nullptr" << endl;
        return;
    }

    if (current == head)
    {
        head = nextNode;
        nextNode->prev = nullptr;
    } else
    {
        current->prev->next = nextNode;
        nextNode->prev = current->prev;
    }

    if (nextNode == tail)
    {
        tail = current;
        current->next = nullptr;
    } else
    {
        current->next = nextNode->next;
        nextNode->next->prev = current;
    }

    current->prev = nextNode;
    nextNode->next = current;

    cout << "Соседние элементы были обменены местами" << endl;
}

template <typename T>
void ListWithIterator<T>::insert(size_t pos, const T& value)
{
    if (pos > size())
    {
        cout << "Неверный индекс!" << endl;
        return;
    }

    if (pos == 0)
    {
        push_front(value);
    } else if (pos == size())
    {
        push_back(value);
    } else
    {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* current = getNodeAtIndex(pos - 1);
        Node<T>* nextNode = current->next;

        newNode->prev = current;
        newNode->next = nextNode;
        current->next = newNode;
        nextNode->prev = newNode;
    }
}

template <typename T>
void ListWithIterator<T>::print()
{
    try {
        is_empty();
    }
    catch (exception& ex)
    {
        return;
    }
    for (auto it = begin(); it != end(); ++it)
    {
        cout << *it << ' ';
    }
    cout << endl;
}

template <typename T>
void ListWithIterator<T>::clear()
{
    while (head != nullptr)
    {
        pop_front();
    }
}

template <typename T>
T ListWithIterator<T>::front()
{
    return head->data;
}

template <typename T>
T ListWithIterator<T>::back()
{
    return tail->data;
}

template <typename T>
typename ListWithIterator<T>::template iterator<T> ListWithIterator<T>::begin()
{
    return iterator<T>(head);
}

template <typename T>
typename ListWithIterator<T>::template iterator<T> ListWithIterator<T>::end()
{
    return iterator<T>(nullptr);
}
template <typename T>
void ListWithIterator<T>::erase(iterator<T> it)
{
    if (it == end()) {
        return;
    }

    Node<T>* node = it.node;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }

    delete node;
}

//template <typename T>
//typename ListWithIterator<T>::template iterator<T> ListWithIterator<T>::find(const T& value)
//{
//    for (iterator<T> it = begin(); it != end(); ++it)
//    {
//        if (*it == value)
//            return it;
//    }
//
//    return end();
//}
