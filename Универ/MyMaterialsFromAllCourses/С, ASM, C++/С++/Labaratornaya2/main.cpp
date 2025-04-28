//Создать базовый класс «Пассажироперевозчик» и производные классы «Самолёт», «Поезд», «Автомобиль».
// Определить время и стоимость передвижения. Классы должны содержать методы доступа и изменения всех полей.

#include "Airplane.h"
#include "Train.h"
#include "Car.h"
#include <vector>

int main()
{
    int array_size = 3;
    std::vector<Car> arr;

    for(int i = 0; i < array_size; i++) {
        std::string name;
        double speed;
        double cost_per_km;
        int capacity;
        std::string engine_type;

        std::cout << "Name: ";
        std::getline(std::cin, name);

        std::cout << "Speed: ";
        std::cin >> speed;

        std::cout << "Capacity: ";
        std::cin >> capacity;

        std::cout << "Cost per km: ";
        std::cin >> cost_per_km;

        std::cout << "Engine type: ";
        std::cin >> engine_type;
        arr.push_back(Car(name, speed, capacity, cost_per_km, engine_type));
    }
    for(int i = 0; i < array_size; i++) {
        std::cout << "Введенное имя: " << arr[i].getName() << std::endl;
        std::cout << "Введенная скорость: " << arr[i].getSpeed() << std::endl;
        std::cout << "Введенная вместимость: " << arr[i].getCapacity() << std::endl;
       // std::cout << "Введенный тип двигателя: " << arr[i].get << std::endl;
    }

    return 0;
}

