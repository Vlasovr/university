#include "Question.h"
#include "SingleChoiceQuestion.h"
#include "MultipleChoiceQuestion.h"
#include "OpenAnswerQuestion.h"
#include "InputError.h"
#include "FileError.h"
#include <string>
#include <memory> 
#include <fstream>

Question::Question(const Question& other) { setTitle(other.title); }

Question::Question(const std::string& title) { setTitle(title); }

Question& Question::operator=(const Question& other)
{
    if (this == &other) {
        return *this; 
    }

    setTitle(other.title);

    return *this;
}

std::string Question::getTitle() const
{
    return title;
}

bool Question::isAlpha(unsigned char ch)
{
    return std::isalpha(ch) ||
        (ch >= 0xC0 && ch <= 0xFF) ||
        (ch == 0xB8 || ch == 0xA8);
}

bool Question::isSymbol(unsigned char ch) {
    static const std::string validSymbols = "-+=?!;%� $()*&|#@:/.,><~_[]{}^'\"\\";
    return validSymbols.find(ch) != std::string::npos;
}

void Question::setTitle(const std::string& title)
{
    if (title.size() > MAX_QUESTION_TITLE_LENGTH) {
        throw InputError("��������� ��������� ����������� ���������� ����� (" +
            std::to_string(MAX_QUESTION_TITLE_LENGTH) + " ��������).");
    }
    if (title.size() < MIN_TITLE_LENGTH) {
        throw InputError("��������� ������ �������� ������� " +
            std::to_string(MIN_TITLE_LENGTH) + " ��������.");
    }

    if (title.empty()) {
        throw InputError("��������� �� ����� ���� ������.");
    }

    if (title.front() == ' ' || title.back() == ' ') {
        throw InputError("��������� ����� ���������� ��� ������������� ��������.");
    }

    if (!Question::isAlpha(title.front())) {
        throw InputError("��������� ������ ���������� � �����");
    }

    for (size_t i = 0; i < title.size(); ++i) {
        unsigned char ch = title[i];
        if (!Question::isAlpha(ch) && !std::isdigit(ch) && !Question::isSymbol(ch)) {
            throw InputError("��������� ����� ��������� ������ �������/���������� �����, ������� � ����. �������.");
        }

        if (i > 0 && title[i - 1] == ' ' && ch == ' ') {
            throw InputError("��������� �� ����� ��������� ��� � ����� ������� ������.");
        }
    }

    this->title = title;
}

Question* Question::loadFromStream(std::ifstream& in)
{
    int typeInt;
    in.read(reinterpret_cast<char*>(&typeInt), sizeof(typeInt));
    QuestionType type = static_cast<QuestionType>(typeInt);

    switch (type)
    {
    case QuestionType::SingleChoice:
        return new SingleChoiceQuestion(SingleChoiceQuestion::loadFromStream(in));
    case QuestionType::MultipleChoice:
        return new MultipleChoiceQuestion(MultipleChoiceQuestion::loadFromStream(in));
    case QuestionType::OpenAnswer:
        return new OpenAnswerQuestion(OpenAnswerQuestion::loadFromStream(in));
    default:
        throw FileError("����������� ��� �������.");
    }
}
