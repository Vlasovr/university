#pragma once
#include "Statistic.h"
#include "InputError.h"
#include <iostream>
#include <string>

class StatisticMenu {
public:
    StatisticMenu();
    ~StatisticMenu();
    void start();

private:
    void displayStatisticList();
    void deleteStatistic();
    int printStatisticList();
    void saveStatistics();
    void sortStatistics();
    List<Statistic*> statistics;
};
