#pragma once
#include "TemplateList.h"

template <typename T>
void TemplateList<T>::is_empty()
{
    try {
        if (empty()) {
            throw runtime_error("Лист пуст!");
        }
    } catch (exception& ex) {
        throw;
    }
}
template <typename T>
Node<T>* TemplateList<T>::getNodeAtIndex(size_t index)
{
    Node<T>* current = head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

template <typename T>
T& TemplateList<T>::operator[](size_t index)
{
    getValueAtIndex();
}

template <typename T>
T& TemplateList<T>::getValueAtIndex(size_t index)
{
    try {
        is_empty();
        if (index >= size()) {
            throw out_of_range("Выход за границы!");
        }
    } catch (exception& ex) {
        cout << "Индекс выходит за границы списка" << endl;
        throw;
    }

    Node<T>* node = getNodeAtIndex(index);
    return node->data;
}

template <typename T>
size_t TemplateList<T>::find(const T& value)
{
    try {
        is_empty();
    }
    catch (exception& ex)
    {
        return numeric_limits<size_t>::max();
    }
    size_t index = 0;
    for (Node<T>* p = head; p != nullptr; p = p->next)
    {
        if (p->data == value) {
            return index;
        }
        index++;
    }

    cout << "Данный элемент в списке не найден" << endl;
    return numeric_limits<size_t>::max();
}

template <typename T>
void TemplateList<T>::push_front(T data)
{
    Node<T>* newNode = new Node<T>(data);

    if (empty()) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

template <typename T>
void TemplateList<T>::push_back(T data)
{
    Node<T>* newNode = new Node<T>(data);
    if (empty()) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

template <typename T>
void TemplateList<T>::pop_front()
{
    if (empty()) {
        cout << "Лист пуст!" << endl;
        return;
    }

    Node<T>* temp = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        // Если после удаления первого элемента список пустой, обновляем tail
        tail = nullptr;
    }
    delete temp;
    cout << "Объект удален успешно" << endl;
}

template <typename T>
void TemplateList<T>::pop_back()
{
    try {
        is_empty();
    }
    catch (exception& ex)
    {
        return;
    }
    Node<T>* temp = tail;
    tail = tail->prev;
    if (tail) {
        tail->next = nullptr;
    } else {
        // Если после удаления последнего элемента список пустой, обновляем head
        head = nullptr;
    }
    delete temp;
    cout << "Объект удален успешно" << endl;
}

template <typename T>
size_t TemplateList<T>::size()
{
    size_t count = 0;
    for (Node<T>* p = head; p != nullptr; p = p->next)
    {
        ++count;
    }
    return count;
}

template <typename T>
void TemplateList<T>::get_size()
{
    cout << "Размер списка составляет " << size() << " элементов" << endl;
}

template <typename T>
void TemplateList<T>::assign(size_t count, const T& value)
{
    clear();
    for (size_t i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <typename T>
bool TemplateList<T>::empty()
{
    return head == nullptr;
}

template <typename T>
void TemplateList<T>::swap(size_t index)
{
    try {
        is_empty();
        if (index >= size() - 1) {
            throw out_of_range("Выход за границы!");
        }
    } catch (exception& ex) {
        cout << "Индекс выходит за границы списка" << endl;
        throw;
    }

    Node<T>* current = getNodeAtIndex(index);
    Node<T>* nextNode = current->next;

    if (current == head) {
        head = nextNode;
        nextNode->prev = nullptr;
    } else if (nextNode == tail) {
        tail = current;
        current->next = nullptr;
    } else {
        current->prev->next = nextNode;
        nextNode->prev = current->prev;
    }

    current->next = nextNode->next;
    nextNode->next->prev = current;

    current->prev = nextNode;
    nextNode->next = current;

    cout << "Соседние элементы были обменены местами" << endl;
}

template <typename T>
void TemplateList<T>::insert(size_t pos, const T& value)
{
    if (pos > size()) {
        cout << "Неверный индекс!" << endl;
        return;
    }

    if (pos == 0) {
        push_front(value);
    } else if (pos == size()) {
        push_back(value);
    } else {
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
void TemplateList<T>::clear()
{
    while (head != nullptr) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void TemplateList<T>::print()
{
    try {
        is_empty();
    }
    catch (exception& ex)
    {
        return;
    }

    for (Node<T>* p = head; p != nullptr; p = p->next) {
        cout << p->data << ' ';
    }
    cout << endl;
}

