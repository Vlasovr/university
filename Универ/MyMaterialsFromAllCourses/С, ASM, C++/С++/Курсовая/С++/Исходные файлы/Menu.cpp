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
    cout << "Добро пожаловать!\n";
    do {
        cout << "Выберите вашу роль:\n";
        cout << "1. Учитель/Администратор\n";
        cout << "2. Ученик\n";
        cout << "0. Выход\n";

        try {
            choice = getInput("012");
        }
        catch (const InputError& e) {
            cout << "Ошибка: " << e.what() << endl;
            continue;
        }

        switch (choice) {
        case '1':
            for (;;) {
                if (KeyVerifier::verify()) {
                    displayAdminMenu();
                    break;
                }

                cout << "Верификация не пройдена. Проверьте наличие ключа и попробуйте снова.\n";
                cout << "Введите 0 для выхода или любую другую клавишу для повторной попытки: ";
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
                cout << "Введите вашу фамилию и инициалы (Фамилия И.О.) или '0' для возврата: ";
                string studentName;
                getline(cin, studentName);
                system("cls");
                if (studentName == "0") break;
                try {
                    checkStudentName(studentName);
                    cout << "Добро пожаловать, " << studentName << "!\n";
                    displayStudentMenu(studentName);
                    break;
                }
                catch (InputError& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
            break;  
        }
    } while (choice != '0');
    cout << "Выход из программы...\n";
}

void Menu::displayAdminMenu() 
{
    char choice = 0;
    do {
        cout << "Меню учителя/администратора:\n";
        cout << "1. Список тестов\n";
        cout << "2. Добавить тест\n";
        cout << "3. Удалить тест\n";
        cout << "4. Редактировать тест\n";
        cout << "5. Просмотр статистики\n";
        cout << "0. Вернуться в меню выбора роли\n";

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
        cout << "Меню ученика:\n";
        cout << "1. Начать тестирование\n";
        cout << "0. Вернуться в меню выбора роли\n";

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
            cout << "Список тестов пуст.\n\nВведите любой символ для возврата...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\nВведите номер теста для продолжения, либо 0 для возврата: ";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    } while (true);
}

void Menu::displayTestList() 
{
    while (true) {
        int maxIndex = printTestList();
        if (maxIndex == 0) {
            cout << "Список тестов пуст.\n\nВведите любой символ для возврата...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\nВведите номер теста для просмотра, либо 0 для возврата: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input == "0") return;
        try {
            int selectedIndex = getValidatedNumber(input, 1, maxIndex);
            tests[selectedIndex - 1]->print();
            cout << "\nВведите любой символ для возврата...";
            string input;
            getline(cin, input);
            system("cls");
        }
        catch (const InputError& e)
        {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::addTest() 
{
    cout << "Добавление нового теста\n";
    do {
        cout << "Введите название теста (или 0 для отмены): ";
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
                        cout << "Тест успешно сохранен.\n";
                    }
                    else {
                        delete newTest;
                        cout << "Тест не был сохранен.\n";
                    }
                    return;
                }
                else {
                    cout << "Тест содержит недостаточно вопросов.\n";
                    cout << "Введите любой символ для продолжения (или 0 для выхода без сохранения):\n";
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
            cout << "Ошибка: " << e.what() << "\n";
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
            cout << "Ошибка: " << e.what() << endl;
            cout << "Введите 0 для выхода без сохранения или любой другой символ для повторной попытки: ";
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
            cout << "Нет тестов для удаления.\n\nВведите любой символ для возврата...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\nВведите номер теста для удаления (или 0 для отмены): ";
        string input;
        getline(cin, input);
        if (input == "0") {
            system("cls");
            return;
        }
        try {
            int selectedIndex = getValidatedNumber(input, 1, maxIndex);

            cout << "Вы действительно хотите удалить тест \"" << tests[selectedIndex - 1]->getTitle()
                << "\"?\n(Введите Y для подтверждения или любой другой символ для отмены): ";
            getline(cin, input);
            system("cls");

            if (input != "Y" && input != "y") {
                cout << "Тест не был удалён.\n";
                continue;
            }

            tests[selectedIndex - 1]->deleteTestFile();
            tests.removeAt(selectedIndex - 1);
            cout << "Тест успешно удалён!\n";
        }
        catch (const InputError& e)
        {
            system("cls");
            cout << "Ошибка: " << e.what() << "\n";
        }
        catch (const FileError& e) {
            cout << "Ошибка: " << e.what() << "\n";
            return;
        }
    }
}

void Menu::editTest() 
{
    while (true) {
        int maxIndex = printTestList();
        if (maxIndex == 0) {
            cout << "Нет тестов для изменения.\n\nВведите любой символ для возврата..."; 
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "\nВведите номер теста для изменения (или 0 для отмены): ";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::editTestDetails(Test* test)
{
    auto changedTest = test;
    char choice = 0;
    while (true) {
        cout << "Редактирование теста \"" << test->getTitle() << "\":\n";
        cout << "1. Изменить название теста\n";
        cout << "2. Добавить новые вопросы\n";
        cout << "3. Удалить вопрос\n";
        cout << "4. Изменить существующий вопрос\n";
        cout << "0. Вернуться в меню администратора\n";

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
                            cout << "Тест успешно сохранен.\n";
                        }
                        else {
                            delete changedTest;
                            cout << "Тест не был сохранен.\n";
                        }
                        return;
                    }
                    else {
                        cout << "Тест содержит недостаточно вопросов.\n";
                        cout << "Введите любой символ для продолжения (или 0 для выхода без сохранения):\n";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::addQuestionsToTest(Test* newTest)
{
    char choice;
    do {
        cout << "Добавление нового вопроса:\n";
        cout << "1. Вопрос с одним правильным ответом\n";
        cout << "2. Вопрос с несколькими правильными ответами\n";
        cout << "3. Вопрос со свободным ответом\n";
        cout << "0. Завершить добавление вопросов\n";
        try {
            choice = getInput("0123");
        }
        catch (const InputError& e) {
            cout << "Ошибка: " << e.what() << endl;
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
    cout << "Добавление вопроса с одним правильным ответом\n";
    if (!setQuestionTitle(question) || !setQuestionOptions(question) || !setAnswer(question)) {
        delete question;
        cout << "Вопрос не был добавлен!\n";
        return;
    }
    test->addQuestion(question);
    cout << "Вопрос успешно добавлен!\n";
}

void Menu::addMultipleChoiceQuestion(Test* test)
{
    MultipleChoiceQuestion* question = new MultipleChoiceQuestion();
    cout << "Добавление вопроса с несколькими правильным ответом\n";
    if (!setQuestionTitle(question) || !setQuestionOptions(question) || !setAnswer(question)) {
        delete question;
        cout << "Вопрос не был добавлен!\n";
        return;
    }
    test->addQuestion(question);
    cout << "Вопрос успешно добавлен!\n";
}

void Menu::addOpenAnswerQuestion(Test* test) 
{
    OpenAnswerQuestion* question = new OpenAnswerQuestion();
    cout << "Добавление вопроса со свободным ответом\n";
    if (!setQuestionTitle(question) || !setAnswer(question)) {
        delete question;
        cout << "Вопрос не был добавлен!\n";
        return;
    }
    test->addQuestion(question);
    cout << "Вопрос успешно добавлен!\n";
}

void Menu::changeTestTitle(Test* test) 
{
    while (true) {
        cout << "Текущее название теста: \"" << test->getTitle() << "\"\n";
        cout << "Введите новое название теста (или 0 для отмены): ";
        string newTitle;
        getline(cin, newTitle);
        system("cls");
        if (newTitle == "0") return;
        try {
            test->setTitle(newTitle);
            cout << "Название теста успешно изменено.\n";
            return;
        }
        catch (const InputError& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::deleteQuestionFromTest(Test* test) 
{
    while (true) {
        if (test->getQuestionCount() == 0) {
            cout << "В данном тесте нет вопросов для удаления.\n\nВведите любой символ для возврата...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "Список вопросов теста \"" << test->getTitle() << "\":\n";
        for (int i = 0; i < test->getQuestionCount(); ++i) {
            cout << i + 1 << ". " << test->getQuestion(i)->getTitle() << "\n";
        }

        cout << "\nВведите номер вопроса для удаления (или 0 для отмены): ";
        string input;
        getline(cin, input);
        if (input == "0") {
            system("cls");
            return;
        }

        try {
            int selectedIndex = getValidatedNumber(input, 1, test->getQuestionCount());

            cout << "Вы действительно хотите удалить вопрос \"" << test->getQuestion(selectedIndex - 1)->getTitle()
                << "\"?\n(Введите Y для подтверждения или любой другой символ для отмены): ";
            getline(cin, input);
            system("cls");

            if (input != "Y" && input != "y") {
                cout << "Вопрос не был удалён.\n";
                continue;
            }

            test->removeQuestion(selectedIndex - 1);
            cout << "Вопрос успешно удалён!\n";
        }
        catch (const InputError& e) {
            system("cls");
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::editQuestionInTest(Test* test) 
{
    while (true) {
        if (test->getQuestionCount() == 0) {
            cout << "В данном тесте нет вопросов для редактирования.\n\nВведите любой символ для возврата...";
            string input;
            getline(cin, input);
            system("cls");
            return;
        }

        cout << "Список вопросов теста \"" << test->getTitle() << "\":\n";
        for (int i = 0; i < test->getQuestionCount(); ++i) {
            cout << i + 1 << ". " << test->getQuestion(i)->getTitle() << "\n";
        }

        cout << "\nВведите номер вопроса для редактирования (или 0 для отмены): ";
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
                cout << "Ошибка: тип вопроса не поддерживается.\n";
            }
        }
        catch (const InputError& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::changeQuestion(SingleChoiceQuestion* question)
{
    while (true) {
        cout << "Редактирование вопроса \"" << question->getTitle() << "\":\n";
        cout << "1. Изменить заголовок\n";
        cout << "2. Изменить варианты ответа\n";
        cout << "3. Изменить правильный ответ\n";
        cout << "4. Показать вопрос\n";
        cout << "0. Вернуться\n";

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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}
void Menu::changeQuestion(MultipleChoiceQuestion* question)
{
    while (true) {
        cout << "Редактирование вопроса \"" << question->getTitle() << "\":\n";
        cout << "1. Изменить заголовок\n";
        cout << "2. Изменить варианты ответа\n";
        cout << "3. Изменить правильные ответы\n";
        cout << "4. Показать вопрос\n";
        cout << "0. Вернуться\n";

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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void Menu::changeQuestion(OpenAnswerQuestion* question)
{
    while (true) {
        cout << "Редактирование вопроса \"" << question->getTitle() << "\":\n";
        cout << "1. Изменить заголовок\n";
        cout << "2. Изменить ответ\n";
        cout << "3. Показать вопрос\n";
        cout << "0. Вернуться\n";

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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

char Menu::getInput(const string& validOptions) {
    string input;
    cout << "Введите выбор: ";
    getline(cin, input);
    system("cls");
    if (input.length() == 1) {
        if (validOptions.find(tolower(input[0])) == string::npos) throw InputError("Некорректный ввод. Операции (" + input + ") не существует.");
        return input[0]; 
    }

    throw InputError("Некорректный ввод. Введите один символ.");
}

void Menu::checkStudentName(const string& studentName) {
    if (studentName.size() < MIN_FULL_NAME_LENGTH || studentName.size() > MAX_FULL_NAME_LENGTH) {
        throw InputError("Некорректный формат ФИО. Длина должна быть от "
            + std::to_string(MIN_FULL_NAME_LENGTH) + " до "
            + std::to_string(MAX_FULL_NAME_LENGTH)
            + " символов. Пример: Иванов И.И.");
    }

    size_t spacePos = studentName.find(' ');
    if (spacePos == string::npos || spacePos == 0 || spacePos == studentName.size() - 1) {
        throw InputError("Некорректный формат ФИО. Пример: Иванов И.И.");
    }

    string lastName = studentName.substr(0, spacePos);
    string initials = studentName.substr(spacePos + 1);

    if (lastName.empty() || ((lastName[0] < 'А' || lastName[0] > 'Я') && lastName[0] != 'Ё')) {
        throw InputError("Фамилия должна начинаться с заглавной буквы.");
    }

    if (lastName.size() < MIN_LAST_NAME_LENGTH) {
        throw InputError("Фамилия слишком короткая.");
    }

    for (size_t i = 1; i < lastName.size(); ++i) {
        if ((lastName[i] < 'а' || lastName[i] > 'я') && lastName[i] != 'ё') {
            throw InputError("Фамилия должна содержать только русские буквы и соответствовать формату \"Фамилия\"");
        }
    }

    if (initials.size() != INITIALS_LENGTH || initials[1] != '.' || initials[3] != '.' ||
        ((initials[0] < 'А' || initials[0] > 'Я') && initials[0] != 'Ё') ||
        ((initials[2] < 'А' || initials[2] > 'Я') && initials[2] != 'Ё')) {
        throw InputError("Инициалы должны быть в формате И.О. с заглавными русскими буквами.");
    }
}

bool Menu::setQuestionTitle(Question* question) {
    auto currentTitle = question->getTitle();
    do {
        if (!currentTitle.empty()) {
            cout << "Текущий заголовок: \"" << question->getTitle() << "\"\n";
        }
        cout << "Введите " << (!currentTitle.empty() ? "новый " : "") << "заголовок вопроса (или 0 для отмены): ";
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
            cout << "Ошибка: " << e.what() << "\n";
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
        cout << "Текущий вариант ответа " << option << ": " << question->getOption(index) << "\n";
        cout << "Выберите действие:\n";
        cout << "1. Заменить\n";
        cout << "2. Удалить\n";
        cout << "3. Пропустить\n";
        cout << "0. Завершить изменение вариантов\n";
        char choice;
        try {
            choice = getInput("0123");
        }
        catch (const InputError& e) {
            cout << "Ошибка: " << e.what() << "\n";
            continue;
        }
        switch (choice) {
        case '1':
            if (replaceOption(question, option, index)) {
                cout << "Вариант ответа " << option << " успешно изменен.\n";
                option++;
                return true;
            } 
            continue;
        case '2':
            question->removeOption(index);
            cout << "Вариант ответа " << option << " успешно удалён.\n";
            return true;
        case '3':
            cout << "Вариант ответа " << option << " пропущен.\n";
            option++;
            return true;
        case '0':
            return false;
        }
    }
}

bool Menu::replaceOption(ChoiceQuestion* question, char option, unsigned int index) {
    while (true) {
        cout << "Введите новый текст для варианта " << option << " (или оставьте пустым для отмены): ";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}


bool Menu::handleNewOption(ChoiceQuestion* question, char& option) {
    while (true) {
        cout << "Введите вариант ответа " << option << " (или оставьте пустым для завершения): ";
        string optionText;
        getline(cin, optionText);
        system("cls");

        if (optionText.empty()) {
            if (question->getOptionCount() < 2) {
                cout << "Ошибка: Минимум два варианта ответа обязательно.\n";
                cout << "Введите любой символ для продолжения (или 0 для выхода без сохранения): ";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

bool Menu::setAnswer(SingleChoiceQuestion* question) {
    while (true) {
        cout << "Вопрос: ";
        question->printQuestion();
        cout << "Введите правильный вариант ответа (A-" << static_cast<char>('A' + question->getOptionCount() - 1)
            << ") или 0 для отмены: ";
        string input;
        getline(cin, input);
        system("cls");
        if (input.size() != 1) {
            cout << "Ошибка: Ответ должен состоять из одного символа\n";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return true;
}

bool Menu::setAnswer(MultipleChoiceQuestion* question) {
    while (true) {
        cout << "Вопрос: ";
        question->printQuestion();
        cout << "Введите правильные варианты ответа (A-" << static_cast<char>('A' + question->getOptionCount() - 1)
            << "), например \"AB\", или 0 для отмены: ";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return true;
}

bool Menu::setAnswer(OpenAnswerQuestion* question) {
    while (true) {
        cout << "Вопрос: ";
        question->printQuestion();
        cout << "Введите правильный ответ, или оставьте пустым для отмены: ";
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
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return true;
}

int Menu::getValidatedNumber(const std::string& input, int min = 0, int max = 100) {
    try {
        int number = std::stoi(input);

        if (number < min || number > max) {
            throw InputError("Число должно быть в диапазоне от " + std::to_string(min) + " до " + std::to_string(max) + ".");
        }

        return number;
    }
    catch (const std::invalid_argument&) {
        throw InputError("Некорректный формат числа.");
    }
    catch (const std::out_of_range&) {
        throw InputError("Введённое число слишком велико.");
    }
}

int Menu::printTestList() {
    if (tests.isEmpty()) {
        return 0;
    }
    cout << "Список тестов:\n";
    int  maxIndex = 0;
    for (auto test : tests) {
        cout << ++maxIndex << ". " << test->getTitle() << "\n";
    }
    return maxIndex;
}


void Menu::printQuestion(Question* question) {
    cout << "Вопрос: ";
    question->printQuestion();;
    question->printAnswer();
    cout << "\nВведите любой символ для возврата...";
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
