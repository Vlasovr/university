#include "Airplane.h"

void Airplane::setName()
{
    std::string new_name;
    std::cout << "Введите новое имя самолета: ";
    std::cin >> new_name;
    name_ = new_name;
}
void Airplane::setSpeed()
{
    double new_speed;
    std::cout << "Введите новую скорость самолета: ";
    std::cin >> new_speed;
    speed_ = new_speed;
}

void Airplane::setCapacity()
{
    int new_capacity;
    std::cout << "Введите новую вместимость самолета: ";
    std::cin >> new_capacity;
    capacity_ = new_capacity;
}

void Airplane::getTravelCost(double distance) const
{
    if(std::strcmp(ticket_type.c_str(), "business") == 0)
        std::cout << "Стоимость поездки составит: " << distance / cost_per_km * 2  << std::endl;
    else if (std::strcmp(ticket_type.c_str(), "standard" ) == 0)
        std::cout << "Стоимость поездки составит: " << distance / cost_per_km / 2  << std::endl;
    else
    std::cout << "Стоимость поездки составит: " << distance / cost_per_km << std::endl;
}

