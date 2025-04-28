#include "PassengerTransport.h"


std::string PassengerTransport::getName() const
{
    return name_;
}

double PassengerTransport::getSpeed() const
{
    return speed_;
}

int PassengerTransport::getCapacity() const
{
    return capacity_;
}

void PassengerTransport::getTravelTime(double distance) const {
    std::cout << "Время поездки составит: " << distance / speed_ << std::endl;
}

void PassengerTransport::getTravelCost(double distance) const
{
    std::cout << "Стоимость поездки составит: " << distance / cost_per_km << std::endl;
}


