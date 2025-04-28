#pragma once
#include "Node.h"
#include <iostream>
#include <exception>


using namespace std;

template <typename T>
class ListWithIterator
{
public:
    template<typename U>
    class iterator
    {
    public:
        Node<T>* node;
        iterator(Node<U>* node) : node(node) {}
        iterator() : node(nullptr) {}
        iterator(const iterator &other) : node(other.node) {}

        iterator& operator++()
        {
            node = node->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator temp(*this);
            ++(*this);
            return temp;
        }

        iterator& operator--()
        {
            node = node->prev;
            return *this;
        }

        iterator operator--(int)
        {
            iterator temp(*this);
            --(*this);
            return temp;
        }

        bool operator==(const iterator& other) const
        {
            return node == other.node;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

        T& operator*()
        {
            return node->data;
        }
    };
private:
    Node<T>* head;
    Node<T>* tail;
    void is_empty();
    Node<T>* getNodeAtIndex(size_t index);
public:
    ListWithIterator() : head(nullptr), tail(nullptr) {}

    ~ListWithIterator()
    {
        clear();
    }
    void push_front(T data);
    void push_back(T data);
    void pop_back();
    void pop_front();
    size_t size();
    T& operator[](size_t index);
    T& getValueAtIndex(size_t index);
    void get_size();
    void assign(size_t count, const T& value);
    bool empty();
    void swap(size_t index);
    void insert(size_t pos, const T& value);
    void clear();
    void erase(iterator<T>it);
    void print();
    T front();
    T back();

    iterator<T> begin();
    iterator<T> end();
   // iterator<T> find(const T& value);

};

