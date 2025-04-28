#pragma once
#include "Question.h"
#define ALPHA_OFFSET 32
class OpenAnswerQuestion : public Question
{
public:
    OpenAnswerQuestion(const OpenAnswerQuestion& other);
    OpenAnswerQuestion(const std::string& title, const std::string& correctAnswer);
    OpenAnswerQuestion() {}
    ~OpenAnswerQuestion() {}
    Question* clone() const override;
    OpenAnswerQuestion& operator=(const OpenAnswerQuestion& other);
    QuestionType getType() const override;
    void printQuestion() override;
    void printAnswer() override;
    bool checkAnswer(const std::string answer) const override;
    void setAnswer(const std::string answer);
    void saveToStream(std::ofstream& out) override;
    static OpenAnswerQuestion loadFromStream(std::ifstream& in);
private:
    unsigned char toLower(unsigned char) const;
    int levenshteinDistance(const std::string& str1, const std::string& str2) const;
    std::string correctAnswer;
}; 
