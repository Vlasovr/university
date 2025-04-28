#include <iostream>
#include <cstring>
#pragma once

class PassengerTransport
{
public:
    PassengerTransport(const std::string& name, double speed, int capacity, double cost_per_km)
         : name_(name), speed_(speed), capacity_(capacity), cost_per_km(cost_per_km) {}
    virtual ~PassengerTransport() = default;

    std::string getName() const;
    virtual void setName(){};
    virtual double getSpeed() const;
    virtual void setSpeed(){};
    virtual int getCapacity() const;
    virtual void setCapacity(){};;

    virtual void getTravelTime(double distance) const;
    virtual void getTravelCost(double distance) const;

protected:
    double cost_per_km;
    std::string name_;
    double speed_;
    int capacity_;
};
