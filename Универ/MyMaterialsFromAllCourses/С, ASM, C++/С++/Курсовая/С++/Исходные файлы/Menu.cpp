#include "Menu.h"
#include <limits>
#include <string>
#include <cstdlib>
#include <functional>
#include "InputError.h"
#include "FileError.h"
#include "KeyVerifier.h"
#include "TestSession.h"
#include "StatisticMenu.h"

Menu::Menu() 
{
    tests = Test::loadAllTests();
    sortTests();
}

Menu::~Menu() 
{
    for (auto test : tests) {
        delete test;
    }
    tests.clear();
}

void Menu::displayRoleSelectionMenu() 
{
    char choice = 0;
    cout << "����� ����������!\n";
    do {
        cout << "�������� ���� ����:\n";
        cout << "1. �������/�������������\n";
        cout << "2. ������\n";
        cout << "0. �����\n";

        try {
            choice = getInput("012");
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << endl;
            continue;
        }

        switch (choice) {
        case '1':
            for (;;) {
                if (KeyVerifier::verify()) {
                    displayAdminMenu();
                    break;
                }

                cout << "����������� �� ��������. ��������� ������� ����� � ���������� �����.\n";
                cout << "������� 0 ��� ������ ��� ����� ������ ������� ��� ��������� �������: ";
                string adminChoice;
                getline(cin, adminChoice);
                system("cls");
                if (adminChoice == "0") {
                    break;
                }
            }
            break;
        case '2':
            for (;;) {
                cout << "������� ���� ������� � �������� (������� �.�.) ��� '0' ��� ��������: ";
                string studentName;
                getline(cin, studentName);
                system("cls");
                if (studentName == "0") break;
                try {
                    checkStudentName(studentName);
                    cout << "����� ����������, " << studentName << "!\n";
                    displayStudentMenu(studentName);
                    break;
                }
                catch (InputError& e) {
                    cout << "������: " << e.what() << endl;
                }
            }
            break;  
        }
    } while (choice != '0');
    cout << "����� �� ���������...\n";
}

void Menu::displayAdminMenu() 
{
    char choice = 0;
    do {
        cout << "���� �������/��������������:\n";
        cout << "1. ������ ������\n";
        cout << "2. �������� ����\n";
        cout << "3. ������� ����\n";
        cout << "4. ������������� ����\n";
        cout << "5. �������� ����������\n";
        cout << "0. ��������� � ���� ������ ����\n";

        try {
            choice = getInput("012345");
        }
        catch (const InputError& e) {
            cout << e.what() << endl;
            continue;
        }

        switch (choice) {
        case '1':
            displayTestList();
            break;
        case '2':
            addTest();
            break;
        case '3':
            deleteTest();
            break;
        case '4':
            editTest();
            break;
        case '5':
            {
                auto statMenu = new StatisticMenu();
                statMenu->start();
                delete statMenu;
                break;
            }
        }
    } while (choice != '0');
}

void Menu::displayStudentMenu(const string& studentName) 
{
    char choice = 0;
    do {
        cout << "���� �������:\n";
        cout << "1. ������ ������������\n";
        cout << "0. ��������� � ���� ������ ����\n";

        try {
            choice = getInput("01");
        }
        catch (const InputError& e) {
            cout << e.what() << endl;
            continue;
        }

        switch (choice) {
        case '1':
            startTesting(studentName);
            break;
        }
    } while (choice != '0');
}


void Menu::startTesting(const string& studentName)
{
    do {
        int maxIndex = printTestList();
        if (maxIndex == 0) {
            cout << "������ ������ ����.\n\n������� ����� ������ ��� ��������...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\n������� ����� ����� ��� �����������, ���� 0 ��� ��������: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input == "0") return;
        try {
            int selectedIndex = getValidatedNumber(input, 1, maxIndex);
            auto session = new TestSession(tests[selectedIndex - 1], studentName);
            if (!session->prepare()) {
                delete session;
                continue;
            }
            session->start();
            delete session;
            return;
        }
        catch (const InputError& e)
        {
            cout << "������: " << e.what() << "\n";
        }
    } while (true);
}

void Menu::displayTestList() 
{
    while (true) {
        int maxIndex = printTestList();
        if (maxIndex == 0) {
            cout << "������ ������ ����.\n\n������� ����� ������ ��� ��������...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\n������� ����� ����� ��� ���������, ���� 0 ��� ��������: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input == "0") return;
        try {
            int selectedIndex = getValidatedNumber(input, 1, maxIndex);
            tests[selectedIndex - 1]->print();
            cout << "\n������� ����� ������ ��� ��������...";
            string input;
            getline(cin, input);
            system("cls");
        }
        catch (const InputError& e)
        {
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::addTest() 
{
    cout << "���������� ������ �����\n";
    do {
        cout << "������� �������� ����� (��� 0 ��� ������): ";
        string testTitle;
        getline(cin, testTitle);
        system("cls");

        if (testTitle == "0") return;
        try {
            Test* newTest = new Test(testTitle);
            do {
                addQuestionsToTest(newTest);
                if (newTest->getQuestionCount() >= MIN_TEST_QUESTIONS) {
                    if (saveTest(newTest)) {
                        tests.pushBack(newTest);
                        sortTests();
                        cout << "���� ������� ��������.\n";
                    }
                    else {
                        delete newTest;
                        cout << "���� �� ��� ��������.\n";
                    }
                    return;
                }
                else {
                    cout << "���� �������� ������������ ��������.\n";
                    cout << "������� ����� ������ ��� ����������� (��� 0 ��� ������ ��� ����������):\n";
                    string userInput;
                    getline(cin, userInput);
                    system("cls");
                    if (userInput == "0") {
                        return;
                    }
                }
            } while (true);
            return;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    } while (true);
}

bool Menu::saveTest(Test* test) 
{
    do {
        try {
            test->saveToFile();
            return true;
        }
        catch (const FileError& e) {
            cout << "������: " << e.what() << endl;
            cout << "������� 0 ��� ������ ��� ���������� ��� ����� ������ ������ ��� ��������� �������: ";
            string input;
            getline(cin, input);
            system("cls");
            if (input == "0") {
                return false;
            }
        }
    } while (true);
}

void Menu::deleteTest() 
{
    while (true) {
        int maxIndex = printTestList();
        if (maxIndex == 0) {
            cout << "��� ������ ��� ��������.\n\n������� ����� ������ ��� ��������...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\n������� ����� ����� ��� �������� (��� 0 ��� ������): ";
        string input;
        getline(cin, input);
        if (input == "0") {
            system("cls");
            return;
        }
        try {
            int selectedIndex = getValidatedNumber(input, 1, maxIndex);

            cout << "�� ������������� ������ ������� ���� \"" << tests[selectedIndex - 1]->getTitle()
                << "\"?\n(������� Y ��� ������������� ��� ����� ������ ������ ��� ������): ";
            getline(cin, input);
            system("cls");

            if (input != "Y" && input != "y") {
                cout << "���� �� ��� �����.\n";
                continue;
            }

            tests[selectedIndex - 1]->deleteTestFile();
            tests.removeAt(selectedIndex - 1);
            cout << "���� ������� �����!\n";
        }
        catch (const InputError& e)
        {
            system("cls");
            cout << "������: " << e.what() << "\n";
        }
        catch (const FileError& e) {
            cout << "������: " << e.what() << "\n";
            return;
        }
    }
}

void Menu::editTest() 
{
    while (true) {
        int maxIndex = printTestList();
        if (maxIndex == 0) {
            cout << "��� ������ ��� ���������.\n\n������� ����� ������ ��� ��������..."; 
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\n������� ����� ����� ��� ��������� (��� 0 ��� ������): ";
        string input;
        getline(cin, input);
        system("cls");
        if (input == "0") return;
        try {
            int selectedIndex = getValidatedNumber(input, 1, maxIndex);
            auto test = tests[selectedIndex - 1];
            editTestDetails(test);
            break;
        }
        catch (const InputError& e)
        {
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::editTestDetails(Test* test)
{
    auto changedTest = test;
    char choice = 0;
    while (true) {
        cout << "�������������� ����� \"" << test->getTitle() << "\":\n";
        cout << "1. �������� �������� �����\n";
        cout << "2. �������� ����� �������\n";
        cout << "3. ������� ������\n";
        cout << "4. �������� ������������ ������\n";
        cout << "0. ��������� � ���� ��������������\n";

        try {
            choice = getInput("01234");
            switch (choice) {
            case '1': {
                changeTestTitle(test);
                break;
            }
            case '2':
                addQuestionsToTest(test);
                break;
            case '3':
                deleteQuestionFromTest(test);
                break;
            case '4':
                editQuestionInTest(test);
                break;
            case '0':
                {
                    if (changedTest->getQuestionCount() >= MIN_TEST_QUESTIONS) {
                        if (saveTest(changedTest)) {
                            test = changedTest;
                            sortTests();
                            cout << "���� ������� ��������.\n";
                        }
                        else {
                            delete changedTest;
                            cout << "���� �� ��� ��������.\n";
                        }
                        return;
                    }
                    else {
                        cout << "���� �������� ������������ ��������.\n";
                        cout << "������� ����� ������ ��� ����������� (��� 0 ��� ������ ��� ����������):\n";
                        string userInput;
                        getline(cin, userInput);
                        system("cls");
                        if (userInput == "0") {
                            return;
                        }
                    }
                    break;
                }   
            }
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::addQuestionsToTest(Test* newTest)
{
    char choice;
    do {
        cout << "���������� ������ �������:\n";
        cout << "1. ������ � ����� ���������� �������\n";
        cout << "2. ������ � ����������� ����������� ��������\n";
        cout << "3. ������ �� ��������� �������\n";
        cout << "0. ��������� ���������� ��������\n";
        try {
            choice = getInput("0123");
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << endl;
            continue;
        }

        switch (choice) {
        case '1': {
            addSingleChoiceQuestion(newTest);
            break;
        }
        case '2': {
            addMultipleChoiceQuestion(newTest);
            break;
        }
        case '3': {
            addOpenAnswerQuestion(newTest);
            break;
        }
        case '0':
            return;
        }
    } while (true);
}

void Menu::addSingleChoiceQuestion(Test* test) 
{
    SingleChoiceQuestion* question = new SingleChoiceQuestion();
    cout << "���������� ������� � ����� ���������� �������\n";
    if (!setQuestionTitle(question) || !setQuestionOptions(question) || !setAnswer(question)) {
        delete question;
        cout << "������ �� ��� ��������!\n";
        return;
    }
    test->addQuestion(question);
    cout << "������ ������� ��������!\n";
}

void Menu::addMultipleChoiceQuestion(Test* test)
{
    MultipleChoiceQuestion* question = new MultipleChoiceQuestion();
    cout << "���������� ������� � ����������� ���������� �������\n";
    if (!setQuestionTitle(question) || !setQuestionOptions(question) || !setAnswer(question)) {
        delete question;
        cout << "������ �� ��� ��������!\n";
        return;
    }
    test->addQuestion(question);
    cout << "������ ������� ��������!\n";
}

void Menu::addOpenAnswerQuestion(Test* test) 
{
    OpenAnswerQuestion* question = new OpenAnswerQuestion();
    cout << "���������� ������� �� ��������� �������\n";
    if (!setQuestionTitle(question) || !setAnswer(question)) {
        delete question;
        cout << "������ �� ��� ��������!\n";
        return;
    }
    test->addQuestion(question);
    cout << "������ ������� ��������!\n";
}

void Menu::changeTestTitle(Test* test) 
{
    while (true) {
        cout << "������� �������� �����: \"" << test->getTitle() << "\"\n";
        cout << "������� ����� �������� ����� (��� 0 ��� ������): ";
        string newTitle;
        getline(cin, newTitle);
        system("cls");
        if (newTitle == "0") return;
        try {
            test->setTitle(newTitle);
            cout << "�������� ����� ������� ��������.\n";
            return;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::deleteQuestionFromTest(Test* test) 
{
    while (true) {
        if (test->getQuestionCount() == 0) {
            cout << "� ������ ����� ��� �������� ��� ��������.\n\n������� ����� ������ ��� ��������...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "������ �������� ����� \"" << test->getTitle() << "\":\n";
        for (int i = 0; i < test->getQuestionCount(); ++i) {
            cout << i + 1 << ". " << test->getQuestion(i)->getTitle() << "\n";
        }

        cout << "\n������� ����� ������� ��� �������� (��� 0 ��� ������): ";
        string input;
        getline(cin, input);
        if (input == "0") {
            system("cls");
            return;
        }

        try {
            int selectedIndex = getValidatedNumber(input, 1, test->getQuestionCount());

            cout << "�� ������������� ������ ������� ������ \"" << test->getQuestion(selectedIndex - 1)->getTitle()
                << "\"?\n(������� Y ��� ������������� ��� ����� ������ ������ ��� ������): ";
            getline(cin, input);
            system("cls");

            if (input != "Y" && input != "y") {
                cout << "������ �� ��� �����.\n";
                continue;
            }

            test->removeQuestion(selectedIndex - 1);
            cout << "������ ������� �����!\n";
        }
        catch (const InputError& e) {
            system("cls");
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::editQuestionInTest(Test* test) 
{
    while (true) {
        if (test->getQuestionCount() == 0) {
            cout << "� ������ ����� ��� �������� ��� ��������������.\n\n������� ����� ������ ��� ��������...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "������ �������� ����� \"" << test->getTitle() << "\":\n";
        for (int i = 0; i < test->getQuestionCount(); ++i) {
            cout << i + 1 << ". " << test->getQuestion(i)->getTitle() << "\n";
        }

        cout << "\n������� ����� ������� ��� �������������� (��� 0 ��� ������): ";
        string input;
        getline(cin, input);
        system("cls");
        if (input == "0") {
            return;
        }
        try {
            int selectedIndex = getValidatedNumber(input, 1, test->getQuestionCount());
            Question* question = test->getQuestion(selectedIndex - 1);
            if (auto singleChoiceQuestion = dynamic_cast<SingleChoiceQuestion*>(question)) {
                changeQuestion(singleChoiceQuestion);
            }
            else if (auto multipleChoiceQuestion = dynamic_cast<MultipleChoiceQuestion*>(question)) {
                changeQuestion(multipleChoiceQuestion);
            }
            else if (auto openAnswerQuestion = dynamic_cast<OpenAnswerQuestion*>(question)) {
                changeQuestion(openAnswerQuestion);
            }
            else {
                cout << "������: ��� ������� �� ��������������.\n";
            }
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::changeQuestion(SingleChoiceQuestion* question)
{
    while (true) {
        cout << "�������������� ������� \"" << question->getTitle() << "\":\n";
        cout << "1. �������� ���������\n";
        cout << "2. �������� �������� ������\n";
        cout << "3. �������� ���������� �����\n";
        cout << "4. �������� ������\n";
        cout << "0. ���������\n";

        char choice = getInput("01234");

        try {
            switch (choice) {
            case '1':
                setQuestionTitle(question);
                break;
            case '2':
            {
                auto changedQuestion = new SingleChoiceQuestion(*question);
                setQuestionOptions(changedQuestion);
                if (changedQuestion->getOptionCount() != question->getOptionCount()) {
                    if (!setAnswer(changedQuestion)) {
                        delete changedQuestion;
                        break;
                    }
                }
                *question = *changedQuestion;
                delete changedQuestion;
                break;
            }
            case '3':
                setAnswer(question);
                break;
            case '4':
                printQuestion(question);
                break;
            case '0':
                return;
            }
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}
void Menu::changeQuestion(MultipleChoiceQuestion* question)
{
    while (true) {
        cout << "�������������� ������� \"" << question->getTitle() << "\":\n";
        cout << "1. �������� ���������\n";
        cout << "2. �������� �������� ������\n";
        cout << "3. �������� ���������� ������\n";
        cout << "4. �������� ������\n";
        cout << "0. ���������\n";

        char choice = getInput("01234");

        try {
            switch (choice) {
            case '1':
                setQuestionTitle(question);
                break;
            case '2':
            {
                auto changedQuestion = new MultipleChoiceQuestion(*question);
                setQuestionOptions(changedQuestion);
                if (changedQuestion->getOptionCount() != question->getOptionCount()) {
                    if (!setAnswer(changedQuestion)) {
                        delete changedQuestion;
                        break;
                    }
                }
                *question = *changedQuestion;
                delete changedQuestion;
                break;
            } 
            case '3':
                setAnswer(question);
                break;
            case '4':
                printQuestion(question);
                break;
            case '0':
                return;
            }
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}

void Menu::changeQuestion(OpenAnswerQuestion* question)
{
    while (true) {
        cout << "�������������� ������� \"" << question->getTitle() << "\":\n";
        cout << "1. �������� ���������\n";
        cout << "2. �������� �����\n";
        cout << "3. �������� ������\n";
        cout << "0. ���������\n";

        char choice = getInput("0123");

        try {
            switch (choice) {
            case '1':
                setQuestionTitle(question);
                break;
            case '2':
                setAnswer(question);
                break;
            case '3':
                printQuestion(question);
                break;
            case '0':
                return;
            }
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}

char Menu::getInput(const string& validOptions) {
    string input;
    cout << "������� �����: ";
    getline(cin, input);
    system("cls");
    if (input.length() == 1) {
        if (validOptions.find(tolower(input[0])) == string::npos) throw InputError("������������ ����. �������� (" + input + ") �� ����������.");
        return input[0]; 
    }

    throw InputError("������������ ����. ������� ���� ������.");
}

void Menu::checkStudentName(const string& studentName) {
    if (studentName.size() < MIN_FULL_NAME_LENGTH || studentName.size() > MAX_FULL_NAME_LENGTH) {
        throw InputError("������������ ������ ���. ����� ������ ���� �� "
            + std::to_string(MIN_FULL_NAME_LENGTH) + " �� "
            + std::to_string(MAX_FULL_NAME_LENGTH)
            + " ��������. ������: ������ �.�.");
    }

    size_t spacePos = studentName.find(' ');
    if (spacePos == string::npos || spacePos == 0 || spacePos == studentName.size() - 1) {
        throw InputError("������������ ������ ���. ������: ������ �.�.");
    }

    string lastName = studentName.substr(0, spacePos);
    string initials = studentName.substr(spacePos + 1);

    if (lastName.empty() || ((lastName[0] < '�' || lastName[0] > '�') && lastName[0] != '�')) {
        throw InputError("������� ������ ���������� � ��������� �����.");
    }

    if (lastName.size() < MIN_LAST_NAME_LENGTH) {
        throw InputError("������� ������� ��������.");
    }

    for (size_t i = 1; i < lastName.size(); ++i) {
        if ((lastName[i] < '�' || lastName[i] > '�') && lastName[i] != '�') {
            throw InputError("������� ������ ��������� ������ ������� ����� � ��������������� ������� \"�������\"");
        }
    }

    if (initials.size() != INITIALS_LENGTH || initials[1] != '.' || initials[3] != '.' ||
        ((initials[0] < '�' || initials[0] > '�') && initials[0] != '�') ||
        ((initials[2] < '�' || initials[2] > '�') && initials[2] != '�')) {
        throw InputError("�������� ������ ���� � ������� �.�. � ���������� �������� �������.");
    }
}

bool Menu::setQuestionTitle(Question* question) {
    auto currentTitle = question->getTitle();
    do {
        if (!currentTitle.empty()) {
            cout << "������� ���������: \"" << question->getTitle() << "\"\n";
        }
        cout << "������� " << (!currentTitle.empty() ? "����� " : "") << "��������� ������� (��� 0 ��� ������): ";
        string questionText;
        getline(cin, questionText);
        system("cls");
        if (questionText == "0") {
            return false;
        }
        try {
            question->setTitle(questionText);
            return true;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    } while (true);
}

bool Menu::setQuestionOptions(ChoiceQuestion* question) {
    char option = 'A';

    while (option <= 'Z') {
        int index = option - 'A';
        if (index < question->getOptionCount()) {
            if (!handleExistingOption(question, option, index)) {
                return true;
            }
        }
        else {
            int startCount = question->getOptionCount();
            if (!handleNewOption(question, option)) {
                return false;
            }
            if (startCount == question->getOptionCount()) {
                return true;
            }
        }
    }
    return true;
}

bool Menu::handleExistingOption(ChoiceQuestion* question, char& option, unsigned int index) {
    while (true) {
        cout << "������� ������� ������ " << option << ": " << question->getOption(index) << "\n";
        cout << "�������� ��������:\n";
        cout << "1. ��������\n";
        cout << "2. �������\n";
        cout << "3. ����������\n";
        cout << "0. ��������� ��������� ���������\n";
        char choice;
        try {
            choice = getInput("0123");
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
            continue;
        }
        switch (choice) {
        case '1':
            if (replaceOption(question, option, index)) {
                cout << "������� ������ " << option << " ������� �������.\n";
                option++;
                return true;
            } 
            continue;
        case '2':
            question->removeOption(index);
            cout << "������� ������ " << option << " ������� �����.\n";
            return true;
        case '3':
            cout << "������� ������ " << option << " ��������.\n";
            option++;
            return true;
        case '0':
            return false;
        }
    }
}

bool Menu::replaceOption(ChoiceQuestion* question, char option, unsigned int index) {
    while (true) {
        cout << "������� ����� ����� ��� �������� " << option << " (��� �������� ������ ��� ������): ";
        string newOptionText;
        getline(cin, newOptionText);
        system("cls");
        if (newOptionText.empty()) {
            return false;
        }
        try {
            question->addOption(newOptionText, index);
            question->removeOption(index + 1);
            return true;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}


bool Menu::handleNewOption(ChoiceQuestion* question, char& option) {
    while (true) {
        cout << "������� ������� ������ " << option << " (��� �������� ������ ��� ����������): ";
        string optionText;
        getline(cin, optionText);
        system("cls");

        if (optionText.empty()) {
            if (question->getOptionCount() < 2) {
                cout << "������: ������� ��� �������� ������ �����������.\n";
                cout << "������� ����� ������ ��� ����������� (��� 0 ��� ������ ��� ����������): ";
                string userInput;
                getline(cin, userInput);
                system("cls");
                if (userInput == "0") {
                    return false;
                }
                continue;
            }
            else {
                return true;
            }
        }
        try {
            question->addOption(optionText);
            option++;
            return true;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
}

bool Menu::setAnswer(SingleChoiceQuestion* question) {
    while (true) {
        cout << "������: ";
        question->printQuestion();
        cout << "������� ���������� ������� ������ (A-" << static_cast<char>('A' + question->getOptionCount() - 1)
            << ") ��� 0 ��� ������: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input.size() != 1) {
            cout << "������: ����� ������ �������� �� ������ �������\n";
        }
        if (input == "0") {
            return false;
        }
        try {
            char correctOption = input[0];
            question->setCorrectIndex(correctOption);
            break;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
    return true;
}

bool Menu::setAnswer(MultipleChoiceQuestion* question) {
    while (true) {
        cout << "������: ";
        question->printQuestion();
        cout << "������� ���������� �������� ������ (A-" << static_cast<char>('A' + question->getOptionCount() - 1)
            << "), �������� \"AB\", ��� 0 ��� ������: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input == "0") {
            return false;
        }
        List<unsigned char> correctOptions;
        try {
            for (unsigned char ch : input) {
                correctOptions.pushBack(toupper(ch));
            }
            question->setCorrectIndices(correctOptions);
            break;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
    return true;
}

bool Menu::setAnswer(OpenAnswerQuestion* question) {
    while (true) {
        cout << "������: ";
        question->printQuestion();
        cout << "������� ���������� �����, ��� �������� ������ ��� ������: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input.empty()) {
            return false;
        }
        try {
            question->setAnswer(input);
            break;
        }
        catch (const InputError& e) {
            cout << "������: " << e.what() << "\n";
        }
    }
    return true;
}

int Menu::getValidatedNumber(const std::string& input, int min = 0, int max = 100) {
    try {
        int number = std::stoi(input);

        if (number < min || number > max) {
            throw InputError("����� ������ ���� � ��������� �� " + std::to_string(min) + " �� " + std::to_string(max) + ".");
        }

        return number;
    }
    catch (const std::invalid_argument&) {
        throw InputError("������������ ������ �����.");
    }
    catch (const std::out_of_range&) {
        throw InputError("�������� ����� ������� ������.");
    }
}

int Menu::printTestList() {
    if (tests.isEmpty()) {
        return 0;
    }
    cout << "������ ������:\n";
    int  maxIndex = 0;
    for (auto test : tests) {
        cout << ++maxIndex << ". " << test->getTitle() << "\n";
    }
    return maxIndex;
}


void Menu::printQuestion(Question* question) {
    cout << "������: ";
    question->printQuestion();;
    question->printAnswer();
    cout << "\n������� ����� ������ ��� ��������...";
    string input;
    getline(cin, input);
    system("cls");
}

void Menu::sortTests() {
    int n = tests.getSize();

    std::function<void(int, int)> heapify;

    heapify = [&](int n, int i) {
        int largest = i;
        int left = 2 * i + 1; 
        int right = 2 * i + 2;

        if (left < n && tests[left]->getTitle() > tests[largest]->getTitle()) {
            largest = left;
        }

        if (right < n && tests[right]->getTitle() > tests[largest]->getTitle()) {
            largest = right;
        }

        if (largest != i) {
            std::swap(tests[i], tests[largest]);
            heapify(n, largest);
        }
    };

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(n, i);
    }

    for (int i = n - 1; i > 0; --i) {
        std::swap(tests[0], tests[i]);
        heapify(i, 0);
    }
}
