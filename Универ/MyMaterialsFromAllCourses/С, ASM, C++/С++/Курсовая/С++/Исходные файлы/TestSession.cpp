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
        std::cout << "���������� ������� ���� � ��� ������������.\n";
        return false;
    }
    std::cout << "����: " << test->getTitle();
    std::cout << "\n���������� ��������: " << test->getQuestionCount() << "\n";
    std::cout << "\n������� ������ ����?\n";
    std::cout << "(������� Y ��� ������������� ��� ����� ������ ������ ��� ������): ";

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
        std::cout << "������ " << number << ": ";
        question->printQuestion();
        switch (question->getType()) {
        case QuestionType::SingleChoice:
            std::cout << "������� ���������� ������� ������ ";
            break;
        case QuestionType::MultipleChoice:
            std::cout << "������� ���������� �������� ������ ";
            break;
        case QuestionType::OpenAnswer:
            std::cout << "������� ����� ";
            break;
        default:
            throw InputError("������ ����� ����������� ���");
        }
        std::cout << "(���� ������� ������ ��� ��������): ";
        std::string input;
        getline(std::cin, input);
        if (input.empty()) {
            std::cout << "\n�� �������, ��� ������ ���������� ������?\n";
            std::cout << "!!! �� �� ������� ��������� � ����� ������� !!!\n";
            std::cout << "(������� Y ��� ������������� ��� ����� ������ ������ ��� ������): ";
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
            std::cout << "������: " << e.what() << std::endl;
        }
    } while (true);
    
}

void TestSession::showResult()
{
    score = (int)std::round(((double)(correctAnswers) / test->getQuestionCount()) * 100.0);

    std::cout << "���� ��������! ��� ���������: " << correctAnswers << " �� "
        << test->getQuestionCount() << " (" << score << "%).\n";
    std::cout << "\n������� ����� ������ ��� ��������...";
    std::string input;
    getline(std::cin, input);
    system("cls");
}
