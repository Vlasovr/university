#pragma once
#include "ChoiceQuestion.h"

class MultipleChoiceQuestion : public ChoiceQuestion
{
public:
    MultipleChoiceQuestion(const MultipleChoiceQuestion& other);
    MultipleChoiceQuestion(const std::string& title, const List<std::string>& options, const List<unsigned char>& correctIndices);
    MultipleChoiceQuestion() {}
    ~MultipleChoiceQuestion() {}
    Question* clone() const override;
    MultipleChoiceQuestion& operator=(const MultipleChoiceQuestion& other);
    QuestionType getType() const override;
    bool checkAnswer(const std::string answer) const override;
    void shuffle() override;
    void printQuestion() override;
    void printAnswer() override;
    void saveToStream(std::ofstream& out) override;
    void setCorrectIndices(const List<unsigned char>& options);
    static MultipleChoiceQuestion loadFromStream(std::ifstream& in);
private:
    List<unsigned char> correctIndices;
};
