#include "MultipleChoiceQuestion.h"
#include "InputError.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

MultipleChoiceQuestion::MultipleChoiceQuestion(
    const MultipleChoiceQuestion& other) : ChoiceQuestion(other)
{
    setCorrectIndices(other.correctIndices);
}

MultipleChoiceQuestion::MultipleChoiceQuestion(
    const std::string& title, 
    const List<std::string>& options,
    const List<unsigned char>& correctIndices) : ChoiceQuestion(title, options)
{
    setCorrectIndices(correctIndices);
}

Question* MultipleChoiceQuestion::clone() const
{
    return new MultipleChoiceQuestion(*this);
}

MultipleChoiceQuestion& MultipleChoiceQuestion::operator=(const MultipleChoiceQuestion& other)
{
    if (this == &other) {
        return *this;
    }

    ChoiceQuestion::operator=(other);

    setCorrectIndices(other.correctIndices);

    return *this;
}

QuestionType MultipleChoiceQuestion::getType() const
{
    return QuestionType::MultipleChoice;
}

void MultipleChoiceQuestion::printQuestion()
{
    std::cout << title << "\n";
    std::cout << "Варианты ответа:\n";
    for (int i = 0; i < options.getSize(); ++i) {
        std::cout << static_cast<char>('A' + i) << "] " << options[i] << "\n";
    }
}

void MultipleChoiceQuestion::printAnswer()
{
    std::cout << "Правильные ответы: ";
    for (unsigned char index : correctIndices) {
        std::cout << index;
    }
    std::cout << std::endl;
}

bool MultipleChoiceQuestion::checkAnswer(const std::string answer) const
{
    List<char> userIndices;

    std::istringstream iss(answer);
    unsigned char index;

    while (iss >> index)
    {
        if (!std::isalpha(index))
        {
            throw InputError("Ответами могут быть только буквы латинского алфавита (A-Z).");
        }

        char userChar = std::toupper(index);

        if (userChar < 'A' || userChar >= 'A' + options.getSize())
        {
            throw InputError("Правильный ответ \"" + std::string(1, userChar) +
                "\" выходит за пределы допустимых вариантов (A-" +
                std::string(1, 'A' + options.getSize() - 1) + ").");
        }

        bool inserted = false;
        for (int i = 0; i < userIndices.getSize(); ++i) {
            if (userChar == userIndices[i]) {
                throw InputError("Правильный ответ \"" + std::string(1, userChar) +
                    "\" указан более одного раза.");
            }
            if (userIndices[i] > userChar) {
                userIndices.insert(userChar, i);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            userIndices.pushBack(userChar);
        }
    }

    if (userIndices.getSize() != correctIndices.getSize()) return false;

    for (auto it = correctIndices.begin(); it != correctIndices.end(); ++it)
    {
        if (!userIndices.contains(*it)) return false;
    }

    return true;
}

void MultipleChoiceQuestion::shuffle()
{
    std::vector<int> indices(options.getSize());
    for (int i = 0; i < options.getSize(); ++i) {
        indices[i] = i;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(indices.begin(), indices.end(), gen);

    List<std::string> shuffledOptions;
    List<unsigned char> newCorrectIndices;

    for (int newIndex = 0; newIndex < (int)indices.size(); ++newIndex) {
        shuffledOptions.pushBack(options[indices[newIndex]]);

        unsigned char originalLetter = ('A' + indices[newIndex]);
        if (correctIndices.contains(originalLetter)) {
            newCorrectIndices.pushBack('A' + newIndex);
        }
    }

    options = shuffledOptions;
    correctIndices = newCorrectIndices;
}


void MultipleChoiceQuestion::setCorrectIndices(const List<unsigned char>& correctIndices)
{
    if (correctIndices.isEmpty())
    {
        throw InputError("Необходимо указать хотя бы один правильный ответ.");
    }

    List<unsigned char> indices = {};

    for (unsigned char index : correctIndices)
    {
        unsigned char upperIndex = std::toupper(index);

        if (!std::isalpha(upperIndex))
        {
            throw InputError("Каждый правильный ответ должен быть буквой латинского алфавита.");
        }

        if (upperIndex < 'A' || upperIndex >= 'A' + options.getSize())
        {
            throw InputError("Правильный ответ \"" + std::string(1, upperIndex) +
                "\" выходит за пределы допустимых вариантов (A-" +
                std::string(1, 'A' + options.getSize() - 1) + ").");
        }

        bool inserted = false;
        for (int i = 0; i < indices.getSize(); ++i) {
            if (upperIndex == indices[i]) {
                throw InputError("Правильный ответ \"" + std::string(1, upperIndex) +
                    "\" указан более одного раза.");
            }
            if (indices[i] > upperIndex) {
                indices.insert(upperIndex, i);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            indices.pushBack(upperIndex);
        }
    }

    this->correctIndices = indices;
}


void MultipleChoiceQuestion::saveToStream(std::ofstream& out)
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

    size_t correctSize = correctIndices.getSize();
    out.write(reinterpret_cast<const char*>(&correctSize), sizeof(correctSize));
    for (const char& index : correctIndices)
    {
        out.write(reinterpret_cast<const char*>(&index), sizeof(index));
    }
}

MultipleChoiceQuestion MultipleChoiceQuestion::loadFromStream(std::ifstream& in)
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
    size_t correctSize;
    in.read(reinterpret_cast<char*>(&correctSize), sizeof(correctSize));
    List<unsigned char> correctIndices;
    for (size_t i = 0; i < correctSize; ++i)
    {
        char index;
        in.read(reinterpret_cast<char*>(&index), sizeof(index));
        correctIndices.pushBack(index);
    }
    return MultipleChoiceQuestion(title, options, correctIndices);
}

