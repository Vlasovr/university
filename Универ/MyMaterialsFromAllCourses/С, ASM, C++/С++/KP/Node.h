#pragma once
template <typename T>
struct Node
{
    T data;
    Node* next;
    Node* prev;

    Node(T data = T(), Node<T>* next = nullptr, Node<T>* prev = nullptr)
    {
        this->data = data;
        this->next = next;
        this->prev = prev;
    }
};
