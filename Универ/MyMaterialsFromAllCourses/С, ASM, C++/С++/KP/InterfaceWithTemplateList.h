#pragma once
#include "TemplateList.cpp"

template <typename T>
class InterfaceWithTemplateList
{
public:
    InterfaceWithTemplateList() = default;
    ~InterfaceWithTemplateList() = default;

    void menuForUser();
    void interactWithTObject();
    void print(const T& element);
};

