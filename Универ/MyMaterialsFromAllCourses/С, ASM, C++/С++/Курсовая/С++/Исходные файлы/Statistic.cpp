#include "Statistic.h"
#include "FileError.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>

Statistic::Statistic(const std::string name, Test* test)
    : name(name) {
    this->test = new Test(*test);
    setCurrentTime();
    answers = {};
    score = 0;
    correctAnswersCount = 0;
}

void Statistic::setAnswers(const List<std::string>& answers)
{
    this->answers = answers;
}

void Statistic::setCorrectAnswersCount(int count)
{
    correctAnswersCount = count;
    score = (int)std::round(((double)(correctAnswersCount) / test->getQuestionCount()) * 100.0);
}

void Statistic::setTime(const std::string time)
{
    this->time = time;
}

void Statistic::setCurrentTime() 
{
    auto now = std::chrono::system_clock::now();
    auto nowTimeT = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &nowTimeT);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%d.%m.%y %H:%M");
    this->time = oss.str();
}

std::string Statistic::getTestTitle()
{
    return test->getTitle();
}
std::string Statistic::getUserName()
{
    return name;
}
std::string Statistic::getTime()
{
    return time;
}

void Statistic::print() 
{
    std::cout << "Имя тестируемого: " << name << "\n";
    std::cout << "Название теста: " << test->getTitle() << "\n";
    std::cout << "Дата и время прохождения: "<< time << "\n";
    std::cout << "===============================";
    for (int i = 0; i < answers.getSize(); ++i) {
        std::cout << "\nВопрос " << (i + 1) << ": ";
        test->getQuestion(i)->printQuestion();
        if (answers[i] == SKIP) {
            std::cout << "Вопрос был пропущен\n";
            test->getQuestion(i)->printAnswer();
        }
        else {
            std::cout << "Ответ: " << answers[i];
            if (test->getQuestion(i)->checkAnswer(answers[i])) {
                std::cout << " [V]\n";
            }
            else {
                std::cout << " [X]\n";
                test->getQuestion(i)->printAnswer();
            }
        }
           
    }
    std::cout << "===============================\n";
    std::cout << "Результат: " << correctAnswersCount << " из "
        << test->getQuestionCount() << " (" << score << "%).\n";
    std::cout << "\nВведите любой символ для возврата...";
    std::string input;
    getline(std::cin, input);
    system("cls");
}

void Statistic::saveToFile()
{
    std::ofstream out(STAT_FILE, std::ios::binary | std::ios::app);
    if (!out.is_open()) {
        std::string errorTest = "Не удалось открыть файл для записи: ";
        throw FileError(errorTest + STAT_FILE);
    }

    test->saveToStream(out);

    size_t nameSize = name.size();
    out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    out.write(name.data(), nameSize);

    size_t timeSize = time.size();
    out.write(reinterpret_cast<const char*>(&timeSize), sizeof(timeSize));
    out.write(time.data(), timeSize);

    int answersCount = answers.getSize();
    out.write(reinterpret_cast<const char*>(&answersCount), sizeof(answersCount));
    for (int i = 0; i < answersCount; ++i) {
        size_t answerSize = answers[i].size();
        out.write(reinterpret_cast<const char*>(&answerSize), sizeof(answerSize));
        out.write(answers[i].data(), answerSize);
    }

    out.write(reinterpret_cast<const char*>(&correctAnswersCount), sizeof(correctAnswersCount));

    out.close();
}

List<Statistic*> Statistic::loadAllFromFile()
{
    List<Statistic*>* statistics = new List<Statistic*>();
    std::ifstream in(STAT_FILE, std::ios::binary);
    if (!in.is_open()) return *statistics;


    while (in.peek() != EOF) {
        try {
            Statistic* stat = Statistic::loadFromStream(in);
            statistics->pushBack(stat);
        }
        catch (const std::exception&) {
            throw FileError("Ошибка: Не удалось прочитать файл статистики");
        }
    }

    in.close();
    return *statistics;
}


Statistic* Statistic::loadFromStream(std::ifstream& in) 
{
    Test* test = Test::loadFromStream(in);
    Statistic* stat = new Statistic("", test);

    size_t nameSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    stat->name.resize(nameSize);
    in.read(&stat->name[0], nameSize);

    size_t timeSize;
    in.read(reinterpret_cast<char*>(&timeSize), sizeof(timeSize));
    stat->time.resize(timeSize);
    in.read(&stat->time[0], timeSize);

    int answersCount;
    in.read(reinterpret_cast<char*>(&answersCount), sizeof(answersCount));
    stat->answers.clear();
    for (int i = 0; i < answersCount; ++i) {
        size_t answerSize;
        in.read(reinterpret_cast<char*>(&answerSize), sizeof(answerSize));
        std::string answer(answerSize, '\0');
        in.read(&answer[0], answerSize);
        stat->answers.pushBack(answer);
    }

    int count;
    in.read(reinterpret_cast<char*>(&count), sizeof(stat->correctAnswersCount));
    stat->setCorrectAnswersCount(count);
    return stat;
}
