#include "Car.h"

void Car::setName()
{
    std::string new_name;
    std::cout << "Введите новое имя машины: ";
    std::cin >> new_name;
    name_ = new_name;
}
void Car::setSpeed()
{
    double new_speed;
    std::cout << "Введите новую скорость машины: ";
    std::cin >> new_speed;
    speed_ = new_speed;
}

void Car::setCapacity()
{
    int new_capacity;
    std::cout << "Введите новую вместимость машины: ";
    std::cin >> new_capacity;
    capacity_ = new_capacity;
}

//std::string Car::getEngineType() const
//{
//    return engine_type;
//}

void Car::getTravelCost(double distance) const
{
    if (std::strcmp(engine_type.c_str(), "electric") == 0 || (std::strcmp(engine_type.c_str(), "hybrid")) == 0) {
        std::cout << "Стоимость поездки составит: " << distance / cost_per_km * 0.5 << std::endl;
    } else
        std::cout << "Стоимость поездки составит: " << distance / cost_per_km << std::endl;
}