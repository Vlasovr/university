#pragma once
#include "fstream"
#include "Student.h"
#include "PassengerTransport.h"
#include "Algorithms.cpp"
#include "Functions.h"
#include <deque>

template <typename T>
class InterfaceWithList
{
public:
    InterfaceWithList() = default;
    ~InterfaceWithList() = default;

    void menuForUser();
    void interactWithTObject();
    void print(const T& element);
    void InterfaceForQueue();
    void printQueue(const deque<T>& dq);
};
