#pragma once
#include "Node.h"
#include <iostream>
#include <exception>

using namespace std;

template <typename T>
class TemplateList
{
private:
    void is_empty();
    Node<T>* getNodeAtIndex(size_t index);
    Node<T>* head;
    Node<T>* tail;
public:
    TemplateList() : head(nullptr), tail(nullptr) {}

    ~TemplateList() {
       clear();
    };

    void push_front(T data);
    void push_back(T data);
    void pop_back();
    void pop_front();
    size_t size();
    size_t find(const T& value);
    T& operator[](size_t index);
    T& getValueAtIndex(size_t index);
    void get_size();
    void assign(size_t count, const T& value);
    bool empty();
    void swap(size_t index);
    void insert(size_t pos, const T& value);
    void clear();
    void print();

};
