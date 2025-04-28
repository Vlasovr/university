#pragma once
#include "ChoiceQuestion.h"

class SingleChoiceQuestion : public ChoiceQuestion
{
public:
    SingleChoiceQuestion(const SingleChoiceQuestion& other);
    SingleChoiceQuestion(const std::string& title, const List<std::string>& options, char correctIndex);
    SingleChoiceQuestion() {}
    ~SingleChoiceQuestion() {}
    Question* clone() const override;
    SingleChoiceQuestion& operator=(const SingleChoiceQuestion& other);
    QuestionType getType() const override;
    bool checkAnswer(const std::string answer) const override;
    void saveToStream(std::ofstream& out) override;
    void shuffle() override;
    void printQuestion() override;
    void printAnswer() override;
    void setCorrectIndex(unsigned char correctIndex);
    static SingleChoiceQuestion loadFromStream(std::ifstream& in);
private:
    char correctIndex = 'A';
};