#pragma once
#include "Question.h"

#define PATH "tests/"
#define MAX_TEST_TITLE_LENGTH 25

class Test 
{
public:
	Test(const Test& other);
	Test(const std::string& title);
	~Test();
	Test& operator=(const Test& other);
	const std::string getTitle() const;
	int getQuestionCount() const;
	void setFileName(const std::string& fileName);
	void setTitle(const std::string& newTitle);
	void addQuestion(Question* question);
	void removeQuestion(int index);
	void print();
	void shuffleQuestions();
	Question* getQuestion(int index);
	void saveToFile();
	void deleteTestFile();
	void saveToStream(std::ofstream& out);
	static Test* loadFromStream(std::ifstream& in);
	static Test* loadFromFile(const std::string& fileName);
	static List<Test*> loadAllTests();
private:
	std::string sanitizeFileName(const std::string& input) const;
	std::string generateFileName() const;
	std::string title;
	List<Question*> questions;
	std::string fileName = "";
};