#pragma once
#include "PassengerTransport.h"

const char* PassengerTransport::getName() const
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
double PassengerTransport::getDistance() const
{
    return distance_;
}

double PassengerTransport::getTravelTime(double distance) const
{
    return distance / speed_;
}

double PassengerTransport::getTravelCost(double distance) const
{
    return distance / cost_per_km;
}

ostream& operator << (ostream& os, const PassengerTransport& transport)
{
    os << setw(14) << transport.getName();
    os << setw(15) << transport.getSpeed() << " км/ч";
    os << setw(13) << transport.getCapacity() << " пассажира";
    os << setw(20) << transport.getTravelCost(transport.getDistance()) << " BYN";
    os << setw(18) << transport.getTravelTime(transport.getDistance()) << " часа(ов)";
    cout << endl;
    return os;
}

istream& operator >> (istream& is, PassengerTransport& transport)
{
    char name[50];
    double speed;
    double cost_per_km;
    int capacity;
    double distance;

    cout << "Введите имя транспорта: ";
    checkStrInput(name);

    cout << "Введите скорость транспорта: ";
    speed = checkIntInput();

    cout << "Введите вместимость транспорта: ";
    capacity = checkIntInput();

    cout << "Введите стоимость за один километр транспорта: ";
    cost_per_km = checkIntInput();

    cout << "Введите необходимое расстояние поездки: ";
    distance = checkIntInput();
    cout << "\n";
    transport = PassengerTransport(name, speed, capacity, cost_per_km, distance);
    return is;
}
