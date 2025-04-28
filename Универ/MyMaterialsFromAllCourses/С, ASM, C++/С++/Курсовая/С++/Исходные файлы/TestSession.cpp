#include "TestSession.h"
#include "Statistic.h"

TestSession::TestSession(Test* test, std::string name): name(name) 
{
    this->test = new Test(*test);
    userAnswers = {};
    correctAnswers = 0;
    score = 0;
}

bool TestSession::prepare() 
{
    if (!test || name.empty()) {
        std::cout << "Необходимо указать тест и имя тестируемого.\n";
        return false;
    }
    std::cout << "Тест: " << test->getTitle();
    std::cout << "\nКоличество вопросов: " << test->getQuestionCount() << "\n";
    std::cout << "\nЖелаете начать тест?\n";
    std::cout << "(Введите Y для подтверждения или любой другой символ для отмены): ";

    std::string input;
    getline(std::cin, input);
    system("cls");
    if (input != "Y" && input != "y") {
        return false;
    }

    test->shuffleQuestions();
    correctAnswers = 0;
    userAnswers.clear();
    return true;
}

void TestSession::start()
{
    auto stat = new Statistic(name, test);
    int max = test->getQuestionCount();
    for (int i = 0; i < max; ++i) {
        Question* question = test->getQuestion(i);
        askQuestion(question, i + 1);
    }
    stat->setAnswers(userAnswers);
    stat->setCorrectAnswersCount(correctAnswers);
    stat->saveToFile();
    delete stat;
    showResult();
}

void TestSession::askQuestion(Question* question, int number)
{
    do {
        std::cout << "Вопрос " << number << ": ";
        question->printQuestion();
        switch (question->getType()) {
        case QuestionType::SingleChoice:
            std::cout << "Введите правильный вариант ответа ";
            break;
        case QuestionType::MultipleChoice:
            std::cout << "Введите правильные варианты ответа ";
            break;
        case QuestionType::OpenAnswer:
            std::cout << "Введите ответ ";
            break;
        default:
            throw InputError("Вопрос имеет неизвестный тип");
        }
        std::cout << "(либо введите пустым для пропуска): ";
        std::string input;
        getline(std::cin, input);
        if (input.empty()) {
            std::cout << "\nВы уверены, что хотите пропустить вопрос?\n";
            std::cout << "!!! Вы не сможете вернуться к этому вопросу !!!\n";
            std::cout << "(Введите Y для подтверждения или любой другой символ для отмены): ";
            getline(std::cin, input);
            system("cls");
            if (input != "Y" && input != "y") continue;
            userAnswers.pushBack(SKIP);
            return;
        }
        system("cls");
        try {
            bool result = question->checkAnswer(input);
            if (result) correctAnswers++;
            userAnswers.pushBack(input);
            return;
        }
        catch (const InputError& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    } while (true);
    
}

void TestSession::showResult()
{
    score = (int)std::round(((double)(correctAnswers) / test->getQuestionCount()) * 100.0);

    std::cout << "Тест завершён! Ваш результат: " << correctAnswers << " из "
        << test->getQuestionCount() << " (" << score << "%).\n";
    std::cout << "\nВведите любой символ для возврата...";
    std::string input;
    getline(std::cin, input);
    system("cls");
}
