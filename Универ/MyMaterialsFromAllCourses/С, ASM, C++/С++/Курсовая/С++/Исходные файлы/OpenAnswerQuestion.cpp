#include "OpenAnswerQuestion.h"
#include "InputError.h"
#include <iostream>
#include <cctype>
#include <string>
#include <locale>
#include <fstream>
#include <algorithm>

OpenAnswerQuestion::OpenAnswerQuestion(const OpenAnswerQuestion& other) : Question(other)
{
    setAnswer(other.correctAnswer);
}

OpenAnswerQuestion::OpenAnswerQuestion(
    const std::string& title,
    const std::string& correctAnswer) : Question(title)
{
    setAnswer(correctAnswer);
}

Question* OpenAnswerQuestion::clone() const
{
    return new OpenAnswerQuestion(*this);
}

OpenAnswerQuestion& OpenAnswerQuestion::operator=(const OpenAnswerQuestion& other)
{
    if (this == &other) {
        return *this;
    }

    Question::operator=(other);

    setAnswer(other.correctAnswer);

    return *this;
}

QuestionType OpenAnswerQuestion::getType() const
{
    return QuestionType::OpenAnswer;
}

bool OpenAnswerQuestion::checkAnswer(const std::string answer) const
{
    if (answer.empty())
    {
        throw InputError("Ответ не может быть пустым.");
    }

    if (answer.size() > MAX_ANSWER_LENGTH)
    {
        throw InputError("Ответ превышает максимально допустимую длину (" +
            std::to_string(MAX_ANSWER_LENGTH) + " символов).");
    }

    for (char ch : answer)
    {
        if (!Question::isAlpha(ch) && !std::isdigit(ch)) {
            throw InputError("Ответ может содержать только русские/английские буквы и цифры");
        }
    }

    std::string lowerAnswer;

    for (unsigned char ch : answer) {
        lowerAnswer += toLower(ch);
    }

    int distance = levenshteinDistance(lowerAnswer, this->correctAnswer);
    int threshold = static_cast<int>(std::round(correctAnswer.size() * 0.2));
    return distance <= threshold;
}


int OpenAnswerQuestion::levenshteinDistance(const std::string& str1, const std::string& str2) const
{
    size_t len1 = str1.size();
    size_t len2 = str2.size();

    List<int> prev;
    List<int> curr;

    for (unsigned int j = 0; j <= len2; ++j)
    {
        prev.pushBack(j);
    }

    for (unsigned int i = 1; i <= len1; ++i)
    {
        curr.clear(); 
        curr.pushBack(i);

        auto str2It = str2.begin();
        for (unsigned int j = 1; j <= len2; ++j, ++str2It)
        {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            curr.pushBack(std::min({prev[j] + 1,
                                    curr[j - 1] + 1,   
                                    prev[j - 1] + cost }));
        }

        prev = curr;
    }

    return prev.back();
}


void OpenAnswerQuestion::printQuestion()
{
    std::cout << title << "\n";
}

void OpenAnswerQuestion::printAnswer()
{
    std::cout << "Правильный ответ: "<< correctAnswer << std::endl;
}

void OpenAnswerQuestion::setAnswer(const std::string answer)
{
    if (answer.empty())
    {
        throw InputError("Ответ не может быть пустым.");
    }

    if (answer.size() > MAX_ANSWER_LENGTH)
    {
        throw InputError("Ответ превышает максимально допустимую длину (" +
            std::to_string(MAX_ANSWER_LENGTH) + " символов).");
    }

    for (char ch : answer)
    {
        if (!Question::isAlpha(ch) && !std::isdigit(ch)) {
            throw InputError("Ответ может содержать только русские/английские буквы и цифры");
        }
    }

    this->correctAnswer = answer;
}

void OpenAnswerQuestion::saveToStream(std::ofstream& out)
{
    int type = static_cast<int>(getType());
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));

    size_t titleSize = title.size();
    out.write(reinterpret_cast<const char*>(&titleSize), sizeof(titleSize));
    out.write(title.data(), titleSize);

    size_t answerSize = correctAnswer.size();
    out.write(reinterpret_cast<const char*>(&answerSize), sizeof(answerSize));
    out.write(correctAnswer.data(), answerSize);
}

OpenAnswerQuestion OpenAnswerQuestion::loadFromStream(std::ifstream& in)
{
    size_t titleSize;
    in.read(reinterpret_cast<char*>(&titleSize), sizeof(titleSize));
    std::string title(titleSize, '\0');
    in.read(&title[0], titleSize);

    size_t answerSize;
    in.read(reinterpret_cast<char*>(&answerSize), sizeof(answerSize));
    std::string correctAnswer(answerSize, '\0');
    in.read(&correctAnswer[0], answerSize);
    return OpenAnswerQuestion(title, correctAnswer);
}

unsigned char OpenAnswerQuestion::toLower(unsigned char ch) const{
    if (ch >= 'А' && ch <= 'Я') {
        return ch + ALPHA_OFFSET;
    }
    else if (ch == 'Ё') {
        return 'ё';
    }
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}