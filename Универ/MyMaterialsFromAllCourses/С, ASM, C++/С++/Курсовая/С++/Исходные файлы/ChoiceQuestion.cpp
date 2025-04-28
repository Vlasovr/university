#include "ChoiceQuestion.h"
#include "InputError.h"
#include <string>
#include <cctype>

ChoiceQuestion::ChoiceQuestion(const ChoiceQuestion& other) : Question(other) 
{ 
    for (auto option : other.options) {
        addOption(option);
    }
}

ChoiceQuestion::ChoiceQuestion(const std::string& title, const List<std::string>& options) : Question(title)
{
    for (auto option : options) {
        addOption(option);
    }
}

ChoiceQuestion& ChoiceQuestion::operator=(const ChoiceQuestion& other)
{
    if (this == &other) {
        return *this; 
    }

    Question::operator=(other);

    options.clear();
    for (auto option : other.options) {
        addOption(option);
    }

    return *this;
}

void ChoiceQuestion::addOption(const std::string& option, int index)
{
    if (option.empty())
    {
        throw InputError("������� ������ �� ����� ���� ������.");
    }

    if (option.size() > MAX_QUESTION_OPTION_LENGTH)
    {
        throw InputError("������� ������ ��������� ����������� ���������� ����� (" +
            std::to_string(MAX_QUESTION_OPTION_LENGTH) + " ��������).");
    }

    if (option.empty()) {
        throw InputError("������� ������ �� ����� ���� ������.");
    }

    if (option.front() == ' ' || option.back() == ' ') {
        throw InputError("������� ������ �� ����� ���������� ��� ������������� ��������.");
    }

    for (size_t i = 0; i < option.size(); ++i) {
        unsigned char ch = option[i];
        if (!Question::isAlpha(ch) && !std::isdigit(ch) && !Question::isSymbol(ch)) {
            throw InputError("������� ������ ����� ��������� ������ �������/���������� �����, ������� � ����. �������.");
        }

        if (i > 0 && option[i - 1] == ' ' && ch == ' ') {
            throw InputError("������� ������ �� ����� ��������� ��� � ����� ������� ������.");
        }
    }
    if (index == -1) {
        options.pushBack(option);
    }
    else {
        if (index > options.getSize()) {
            throw InputError("������ ������� �� ������� ��������� ���������.");
        }
        options.insert(option, index);
    }
    
}

void ChoiceQuestion::removeOption(unsigned int index)
{
    if ((int)index >= options.getSize())
    {
        throw InputError("������ ������� �� ������� ���������.");
    }

    options.removeAt(index);
}

int ChoiceQuestion::getOptionCount() const {
    return options.getSize();
}

List<std::string> ChoiceQuestion::getOptions() const
{
    auto newOptions = options;
    return newOptions;
}

std::string ChoiceQuestion::getOption(unsigned int index) {
    return options[index];
}