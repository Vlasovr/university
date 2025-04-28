#pragma once
#include "Test.h"
#include <string>
#include <vector>
#include <chrono>

#define SKIP "Пропущен"
#define STAT_FILE "stat.dat"

class Statistic {
public:
    Statistic(const std::string name, Test* test);
    void setCurrentTime();
    void setAnswers(const List<std::string>& answers);
    void setTime(const std::string time);
    void setCorrectAnswersCount(int count);
    std::string getTestTitle();
    std::string getUserName();
    std::string getTime();
    void print();
    void saveToFile();
    static Statistic* loadFromStream(std::ifstream& in);
    static List<Statistic*> loadAllFromFile();
private:
    std::string name;
    std::string time;
    List<std::string> answers;
    int correctAnswersCount;
    int score;
    Test* test;
};
