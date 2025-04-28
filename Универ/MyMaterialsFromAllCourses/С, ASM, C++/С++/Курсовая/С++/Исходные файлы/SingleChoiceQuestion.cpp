#include "SingleChoiceQuestion.h"
#include "InputError.h"
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <random>

SingleChoiceQuestion::SingleChoiceQuestion(
    const SingleChoiceQuestion& other) : ChoiceQuestion(other)
{
    setCorrectIndex(other.correctIndex);
}

SingleChoiceQuestion::SingleChoiceQuestion(
    const std::string& title,
    const List<std::string>& options,
    char correctIndex) : ChoiceQuestion(title, options)
{
    setCorrectIndex(correctIndex);
}

Question* SingleChoiceQuestion::clone() const
{
    return new SingleChoiceQuestion(*this);
}

SingleChoiceQuestion& SingleChoiceQuestion::operator=(const SingleChoiceQuestion& other) {
    if (this == &other) {
        return *this;
    }

    ChoiceQuestion::operator=(other);

    setCorrectIndex(other.correctIndex);

    return *this;
}

QuestionType SingleChoiceQuestion::getType() const
{
    return QuestionType::SingleChoice;
}

void SingleChoiceQuestion::printQuestion()
{
    std::cout << title << "\n";
    std::cout << "Варианты ответа:\n";
    for (int i = 0; i < options.getSize(); ++i) {
        std::cout << static_cast<char>('A' + i) << ") " << options[i] << "\n";
    }
}

void SingleChoiceQuestion::printAnswer()
{
    std::cout << "Правильный ответ: " << correctIndex << std::endl;
}

bool SingleChoiceQuestion::checkAnswer(const std::string answer) const
{
    if (answer.size() != 1)
    {
        throw InputError("Введите один символ.");
    }

    unsigned char userAnswer = std::toupper(answer[0]);

    if (!std::isalpha(userAnswer))
    {
        throw InputError("Ответом может быть только буква латинского алфавита (A-Z).");
    }

    if (userAnswer < 'A' || userAnswer >= 'A' + options.getSize())
    {
        throw InputError("Ответ выходит за пределы допустимых вариантов (A-" +
            std::string(1, 'A' + options.getSize() - 1) + ").");
    }

    return userAnswer == correctIndex;
}

void SingleChoiceQuestion::setCorrectIndex(unsigned char correctIndex)
{
    correctIndex = std::toupper(correctIndex);

    if (!std::isalpha(correctIndex))
    {
        throw InputError("Правильный ответ должен быть буквой латинского алфавита.");
    }

    if (correctIndex < 'A' || correctIndex >= 'A' + options.getSize())
    {
        throw InputError("Правильный ответ выходит за пределы допустимых вариантов (A-" +
            std::string(1, 'A' + options.getSize() - 1) + ").");
    }

    this->correctIndex = correctIndex;
}

void SingleChoiceQuestion::saveToStream(std::ofstream& out)
{
    int type = static_cast<int>(getType());
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));

    size_t titleSize = title.size();
    out.write(reinterpret_cast<const char*>(&titleSize), sizeof(titleSize));
    out.write(title.data(), titleSize);

    size_t optionsSize = options.getSize();
    out.write(reinterpret_cast<const char*>(&optionsSize), sizeof(optionsSize));

    for (const auto& option : options)
    {
        size_t optSize = option.size();
        out.write(reinterpret_cast<const char*>(&optSize), sizeof(optSize));
        out.write(option.data(), optSize);
    }

    out.write(reinterpret_cast<const char*>(&correctIndex), sizeof(correctIndex));
}

void SingleChoiceQuestion::shuffle()
{
    std::vector<int> indices(options.getSize());
    for (int i = 0; i < options.getSize(); ++i) {
        indices[i] = i;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(indices.begin(), indices.end(), gen);

    List<std::string> shuffledOptions;
    unsigned char newCorrectIndex = 'A';

    for (int newIndex = 0; newIndex < (int)indices.size(); ++newIndex) {
        shuffledOptions.pushBack(options[indices[newIndex]]);

        unsigned char originalLetter = ('A' + indices[newIndex]);
        if (originalLetter == correctIndex) {
            newCorrectIndex = 'A' + newIndex;
        }
    }

    options = shuffledOptions;
    correctIndex = newCorrectIndex;
}

SingleChoiceQuestion SingleChoiceQuestion::loadFromStream(std::ifstream& in)
{
    size_t titleSize;
    in.read(reinterpret_cast<char*>(&titleSize), sizeof(titleSize));
    std::string title(titleSize, '\0');
    in.read(&title[0], titleSize);

    size_t optionsSize;
    in.read(reinterpret_cast<char*>(&optionsSize), sizeof(optionsSize));
    List<std::string> options;
    for (size_t i = 0; i < optionsSize; ++i)
    {
        size_t optSize;
        in.read(reinterpret_cast<char*>(&optSize), sizeof(optSize));
        std::string option(optSize, '\0');
        in.read(&option[0], optSize);
        options.pushBack(option);
    }

    char correctIndex;
    in.read(reinterpret_cast<char*>(&correctIndex), sizeof(correctIndex));

    return SingleChoiceQuestion(title, options, correctIndex);
}