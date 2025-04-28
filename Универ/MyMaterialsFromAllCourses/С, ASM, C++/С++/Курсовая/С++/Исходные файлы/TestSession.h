#pragma once
#include "Test.h"
#include "InputError.h"
#include <iostream>
#include <string>
#include <cstdlib>

class TestSession {
public:
    TestSession(Test* test, std::string name);
    void start();
    bool prepare();
private:
    void askQuestion(Question* question, int number);
    void showResult();

    Test* test;
    std::string name;
    List<std::string> userAnswers;
    int correctAnswers;
    int score;
};
