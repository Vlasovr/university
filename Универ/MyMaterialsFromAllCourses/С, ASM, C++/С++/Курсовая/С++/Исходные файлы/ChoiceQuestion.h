#pragma once
#include "Question.h"

class ChoiceQuestion : public Question
{
public:
    ChoiceQuestion(const ChoiceQuestion& other);
    ChoiceQuestion(const std::string& title, const List<std::string>& options);
    ChoiceQuestion(): Question() {}
    ChoiceQuestion& operator=(const ChoiceQuestion& other);
    virtual ~ChoiceQuestion() = default;
    virtual void shuffle() = 0;
    List<std::string> getOptions() const;
    std::string getOption(unsigned int index);
    int getOptionCount() const;
    void addOption(const std::string& option, int index = -1);
    void removeOption(unsigned int index);
protected:
    List<std::string> options = {};
};