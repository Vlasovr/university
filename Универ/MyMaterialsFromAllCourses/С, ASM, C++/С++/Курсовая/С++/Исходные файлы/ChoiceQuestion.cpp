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
        throw InputError("Вариант ответа не может быть пустым.");
    }

    if (option.size() > MAX_QUESTION_OPTION_LENGTH)
    {
        throw InputError("Вариант ответа превышает максимально допустимую длину (" +
            std::to_string(MAX_QUESTION_OPTION_LENGTH) + " символов).");
    }

    if (option.empty()) {
        throw InputError("Вариант ответа не может быть пустым.");
    }

    if (option.front() == ' ' || option.back() == ' ') {
        throw InputError("Вариант ответа не может начинаться или заканчиваться пробелом.");
    }

    for (size_t i = 0; i < option.size(); ++i) {
        unsigned char ch = option[i];
        if (!Question::isAlpha(ch) && !std::isdigit(ch) && !Question::isSymbol(ch)) {
            throw InputError("Вариант ответа может содержать только русские/английские буквы, пробелы и спец. символы.");
        }

        if (i > 0 && option[i - 1] == ' ' && ch == ' ') {
            throw InputError("Вариант ответа не может содержать два и более пробела подряд.");
        }
    }
    if (index == -1) {
        options.pushBack(option);
    }
    else {
        if (index > options.getSize()) {
            throw InputError("Индекс выходит за пределы доступных вариантов.");
        }
        options.insert(option, index);
    }
    
}

void ChoiceQuestion::removeOption(unsigned int index)
{
    if ((int)index >= options.getSize())
    {
        throw InputError("Индекс выходит за пределы диапазона.");
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