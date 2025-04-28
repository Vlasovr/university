#include "PassengerTransport.h"

class Train : public PassengerTransport
{
public:
    int stops_quantity;
    Train(const std::string& name, double speed, int capacity, double cost_per_km, int stops_quantity)
            : PassengerTransport(name, speed, capacity, cost_per_km), stops_quantity(stops_quantity) {}
    ~Train() override {
        std::cout << "Вызван деструктор класса Train";
    }

    void setName() override;
    void setSpeed() override;
    void setCapacity() override;

    void getTravelCost(double distance) const override;
};

