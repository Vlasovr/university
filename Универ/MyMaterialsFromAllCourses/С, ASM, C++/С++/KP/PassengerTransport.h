#pragma once
#include "Functions.h"

using namespace std;

class PassengerTransport
{
public:
    PassengerTransport() : name_(""), speed_(0), capacity_(0), cost_per_km(0), distance_(0) {}
    PassengerTransport(const char name[50], double speed, int capacity, double cost_per_km, double distance)
            : speed_(speed), capacity_(capacity), cost_per_km(cost_per_km), distance_(distance) {
        strncpy(name_, name, 50);
        name_[49] = '\0';
    }
    virtual ~PassengerTransport() = default;

    const char* getName() const;
    double getSpeed() const;
    int getCapacity() const;
    double getDistance() const;
    double getTravelTime(double distance) const;
    double getTravelCost(double distance) const;

    bool operator==(const PassengerTransport& other) const {
        return name_ == other.name_ && speed_ == other.speed_ && capacity_ == other.capacity_
               && cost_per_km == other.cost_per_km && distance_ == other.distance_;
    }

protected:
    double cost_per_km;
    char name_[50];
    double speed_;
    int capacity_;
    double distance_;
    friend istream& operator >> (istream& is, PassengerTransport& transport);
    friend ostream& operator << (ostream& os, const PassengerTransport& transport);
};

