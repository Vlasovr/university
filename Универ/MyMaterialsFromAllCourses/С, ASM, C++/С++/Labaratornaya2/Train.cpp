#include "Train.h"

void Train::setName()
{
    std::string new_name;
    std::cout << "Введите новое имя поезда: ";
    std::cin >> new_name;
    name_ = new_name;
}
void Train::setSpeed()
{
    double new_speed;
    std::cout << "Введите новую скорость поезда: ";
    std::cin >> new_speed;
    speed_ = new_speed;
}

void Train::setCapacity()
{
    int new_capacity;
    std::cout << "Введите новую вместимость поезда: ";
    std::cin >> new_capacity;
    capacity_ = new_capacity;
}

void Train::getTravelCost(double distance) const
{
    std::cout << "Введите сколько остановок собираетесь проехать: ";
    int stops_in_road;
    std::cin >> stops_in_road;
    std::cout << "Стоимость поездки составит: " << distance / cost_per_km * (stops_quantity - stops_in_road)  << std::endl;
}