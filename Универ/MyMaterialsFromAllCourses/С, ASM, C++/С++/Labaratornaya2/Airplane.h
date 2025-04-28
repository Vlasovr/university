#include "PassengerTransport.h"
#include <cstring>

class Airplane : public PassengerTransport
{
public:
    std::string ticket_type;

    Airplane(const std::string& name, double speed, int capacity, double cost_per_km, const std::string& ticket_type)
            : PassengerTransport(name, speed, capacity, cost_per_km), ticket_type(ticket_type) {}

    ~Airplane() override {
        std::cout << "Вызван деструктор класса Airplane";
    }

    void setName() override;
    void setSpeed() override;
    void setCapacity() override;

    void getTravelCost(double distance) const override;

};
