#pragma once
#include "List.h"

#define MIN_TITLE_LENGTH 6
#define MAX_QUESTION_TITLE_LENGTH 128
#define MAX_QUESTION_OPTION_LENGTH 64
#define MAX_QUESTION_ANSWER_LENGTH 20
#define MAX_ANSWER_LENGTH 32

enum class QuestionType
{
    SingleChoice = 1,
    MultipleChoice = 2,
    OpenAnswer = 3
};

class Question
{
public:
    Question(const Question& other);
    Question(const std::string& title);
    Question() {}
    Question& operator=(const Question& other);
    virtual ~Question() = default;
    virtual Question* clone() const = 0;
    virtual QuestionType getType() const = 0;
    virtual void printQuestion() = 0;
    virtual void printAnswer() = 0;
    virtual bool checkAnswer(const std::string answer) const = 0;
    virtual void saveToStream(std::ofstream& out) = 0;
    std::string getTitle() const;
    void setTitle(const std::string& title);
    static Question* loadFromStream(std::ifstream& in);
    static bool isAlpha(unsigned char ch);
    static bool isSymbol(unsigned char ch);
protected:
    std::string title = "";
};