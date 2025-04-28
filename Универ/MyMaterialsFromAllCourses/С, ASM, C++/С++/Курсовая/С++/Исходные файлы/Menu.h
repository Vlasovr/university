#pragma once
#include <iostream>
#include "Test.h"
#include "SingleChoiceQuestion.h"
#include "OpenAnswerQuestion.h"
#include "MultipleChoiceQuestion.h"

#define MIN_FULL_NAME_LENGTH 7
#define MAX_FULL_NAME_LENGTH 25
#define MIN_LAST_NAME_LENGTH 2
#define INITIALS_LENGTH 4
#define MIN_TEST_QUESTIONS 3

using namespace std;

class Menu {
public:
    Menu();
    ~Menu();
    void displayRoleSelectionMenu();
    static int getValidatedNumber(const std::string& input, int min, int max);
    static char getInput(const string& validOptions);
private:
    void displayAdminMenu();
    void displayStudentMenu(const string& studentName);
    void checkStudentName(const string& studentName);
    void startTesting(const string& studentName);
    void displayTestList();
    void addTest();
    void deleteTest();
    void editTest();
    void editTestDetails(Test* test);
    bool saveTest(Test* test);
    void addQuestionsToTest(Test* newTest);
    void addSingleChoiceQuestion(Test* test);
    void addMultipleChoiceQuestion(Test* test);
    void addOpenAnswerQuestion(Test* test);
    void changeQuestion(SingleChoiceQuestion* question);
    void changeQuestion(MultipleChoiceQuestion* question);
    void changeQuestion(OpenAnswerQuestion* question);
    bool setQuestionTitle(Question* question);
    bool setQuestionOptions(ChoiceQuestion* question);
    bool handleExistingOption(ChoiceQuestion* question, char& option, unsigned int index);
    bool replaceOption(ChoiceQuestion* question, char option, unsigned int index);
    bool handleNewOption(ChoiceQuestion* question, char& option);
    bool setAnswer(SingleChoiceQuestion* question);
    bool setAnswer(MultipleChoiceQuestion* question);
    bool setAnswer(OpenAnswerQuestion* question);
    void changeTestTitle(Test* test);
    void deleteQuestionFromTest(Test* test);
    void editQuestionInTest(Test* test);
    int printTestList();
    void printQuestion(Question* question);
    void sortTests();

    List<Test*> tests;
};