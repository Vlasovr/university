#include "Test.h"
#include "InputError.h"
#include "FileError.h"
#include "ChoiceQuestion.h"
#include <algorithm>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "dirent.h"
#include <direct.h>
#include <random>

Test::Test(const Test& other)
{
    title = other.title;
    questions = { };
    for (auto it = other.questions.begin(); it != other.questions.end(); it++) {
        questions.pushBack((*it)->clone());
    }
    fileName = other.fileName;
}

Test::Test(const std::string& title)
{
    setTitle(title);
}

Test::~Test()
{
	for (Question* question : questions) {
		delete question;
	}
    questions.clear();
}

Test& Test::operator=(const Test& other) {
    if (this == &other) {
        return *this;
    }

    title = other.title;
    questions.clear();
    for (auto it = other.questions.begin(); it != other.questions.end(); it++) {
        questions.pushBack((*it)->clone());
    }
    fileName = other.fileName;

    return *this;
}

const std::string Test::getTitle() const 
{
    return title;
}

int Test::getQuestionCount() const
{
	return questions.getSize();
}

void Test::setTitle(const std::string& newTitle)
{
    if (newTitle.size() > MAX_TEST_TITLE_LENGTH) {
        throw InputError("Название превышает максимально допустимую длину (" +
            std::to_string(MIN_TITLE_LENGTH) + " символов).");
    }

    if (newTitle.size() < MIN_TITLE_LENGTH) {
        throw InputError("Название должно содерать минимум " +
            std::to_string(MIN_TITLE_LENGTH) + " символов.");
    }

    if (newTitle.empty()) {
        throw InputError("Название не может быть пустым.");
    }

    if (newTitle.front() == ' ' || newTitle.back() == ' ') {
        throw InputError("Название не может начинаться или заканчиваться пробелом.");
    }

    if (!Question::isAlpha(newTitle.front())) {
        throw InputError("Название должно начинаться с буквы");
    }

    for (size_t i = 0; i < newTitle.size(); ++i) {
        unsigned char ch = newTitle[i];
        if (!Question::isAlpha(ch) && !std::isdigit(ch) && !(ch == ' ' || ch == '+' || ch == '-' || ch == '_')) {
            throw InputError("Название может содержать только русские/английские буквы, пробелы и символы \"+-_\"");
        }

        if (i > 0 && newTitle[i - 1] == ' ' && ch == ' ') {
            throw InputError("Название не может содержать два и более пробела подряд.");
        }

    }

    this->title = newTitle;
}


void Test::addQuestion(Question* question)
{
    questions.pushBack(question);
}

void Test::removeQuestion(int index) 
{
    auto question = getQuestion(index);
    questions.removeAt(index);
    delete question;
}

void Test::print() {
    std::cout << "Название теста: " << title << "\n";
    std::cout << "===============================\n\n";
    for (int i = 0; i < questions.getSize(); ++i) {
        std::cout << "Вопрос " << (i + 1) << ":\n";
        questions[i]->printQuestion();
        questions[i]->printAnswer();
        std::cout << "\n";
    }
    std::cout << "===============================\n";
    std::cout << "Конец.\n";
}

void Test::shuffleQuestions()
{
    int n = questions.getSize();
    std::srand(static_cast<unsigned>(std::time(0)));

    for (int i = 0; i < n; ++i) {
        int j = std::rand() % n;
        std::swap(questions[i], questions[j]);
    }

    for (auto question : questions) {
        if (auto choiceQuestion = dynamic_cast<ChoiceQuestion*>(question)) {
            choiceQuestion->shuffle();
        }
    }
}

Question* Test::getQuestion(int index) 
{
	return questions[(unsigned int)index];
}

std::string Test::generateFileName() const 
{
    std::string baseName = sanitizeFileName(title);
    std::string extension = ".ktst";
    std::string fileName = baseName + extension;

    int counter = 1;

    while (true) {
        bool fileExists = false;
        DIR* dir = opendir(PATH);
        if (!dir) {
            throw FileError("Не удалось открыть текущий каталог для проверки.");
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (fileName == entry->d_name) {
                fileExists = true;
                break;
            }
        }
        closedir(dir);

        if (!fileExists) {
            break;
        }

        std::ostringstream newFileName;
        newFileName << baseName << counter << extension;
        fileName = newFileName.str();
        ++counter;
    }

    return fileName;
}

std::string Test::sanitizeFileName(const std::string& input) const
{
    static const std::unordered_map<unsigned char, std::string> transliteration = {
        {'А', "a"}, {'а', "a"}, {'Б', "b"}, {'б', "b"}, {'В', "v"}, {'в', "v"},
        {'Г', "g"}, {'г', "g"}, {'Д', "d"}, {'д', "d"}, {'Е', "e"}, {'е', "e"},
        {'Ё', "e"}, {'ё', "e"}, {'Ж', "zh"}, {'ж', "zh"}, {'З', "z"}, {'з', "z"},
        {'И', "i"}, {'и', "i"}, {'Й', "y"}, {'й', "y"}, {'К', "k"}, {'к', "k"},
        {'Л', "l"}, {'л', "l"}, {'М', "m"}, {'м', "m"}, {'Н', "n"}, {'н', "n"},
        {'О', "o"}, {'о', "o"}, {'П', "p"}, {'п', "p"}, {'Р', "r"}, {'р', "r"},
        {'С', "s"}, {'с', "s"}, {'Т', "t"}, {'т', "t"}, {'У', "u"}, {'у', "u"},
        {'Ф', "f"}, {'ф', "f"}, {'Х', "kh"}, {'х', "kh"}, {'Ц', "ts"}, {'ц', "ts"},
        {'Ч', "ch"}, {'ч', "ch"}, {'Ш', "sh"}, {'ш', "sh"}, {'Щ', "shch"}, {'щ', "shch"},
        {'Ъ', ""},  {'ъ', ""},  {'Ы', "y"}, {'ы', "y"}, {'Ь', ""},  {'ь', ""},
        {'Э', "e"}, {'э', "e"}, {'Ю', "yu"}, {'ю', "yu"}, {'Я', "ya"}, {'я', "ya"}
    };

    std::string fileName;
    for (unsigned char ch : input)
    {
        auto it = transliteration.find(ch);
        if (it != transliteration.end() ||  std::isalnum(ch) || ch == '-' || ch == '_')
        {
            if (std::isalpha(ch))
            {
                fileName += std::tolower(ch);
            }
            else if (it != transliteration.end())
            {
                fileName += it->second;
            }
            else
            {
                fileName += ch;
            }
        }
    }
    return fileName;
}

void Test::saveToFile()
{
    std::string finalFileName = fileName.empty() ? generateFileName() : fileName;

    std::ofstream outFile(PATH + finalFileName, std::ios::binary);
    if (!outFile.is_open())
    {
        throw FileError("Невозможно открыть файл для записи: " + finalFileName);
    }

    saveToStream(outFile);
    outFile.close();
    fileName = finalFileName;
}

void Test::saveToStream(std::ofstream& out) {
    size_t titleSize = title.size();
    out.write(reinterpret_cast<const char*>(&titleSize), sizeof(titleSize));
    out.write(title.data(), titleSize);

    int questionCount = questions.getSize();
    out.write(reinterpret_cast<const char*>(&questionCount), sizeof(questionCount));

    for (int i = 0; i < questionCount; ++i) {
        questions[i]->saveToStream(out);
    }
}

Test* Test::loadFromStream(std::ifstream& in) {
    size_t titleSize;
    in.read(reinterpret_cast<char*>(&titleSize), sizeof(titleSize));
    std::string title(titleSize, '\0');
    in.read(&title[0], titleSize);
    Test* test = new Test(title);
    int questionCount;
    in.read(reinterpret_cast<char*>(&questionCount), sizeof(questionCount));
    for (int i = 0; i < questionCount; ++i)
    {
        Question* question = Question::loadFromStream(in);
        test->addQuestion(question);
    }
    return test;
}


Test* Test::loadFromFile(const std::string& fileName)
{
    std::ifstream inFile(PATH + fileName, std::ios::binary);
    if (!inFile.is_open())
    {
        throw FileError("Невозможно открыть файл для чтения: " + fileName);
    }
    Test* test = loadFromStream(inFile);
    inFile.close();
    test->setFileName(fileName);
    return test;
}

List<Test*> Test::loadAllTests() 
{
    List<Test*>* tests =  new List<Test*>();
    const std::string extension = ".ktst";
    DIR* dir;
    struct dirent* entry;

    dir = opendir(PATH);
    if (!dir) {
        if (_mkdir(PATH) != 0) { 
            throw FileError("Ошибка: Не удалось создать каталог для тестов");
        }
        return *tests;
    }

    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        if (fileName.size() > extension.size() &&
            fileName.substr(fileName.size() - extension.size()) == extension) {
            try {
                Test* test = Test::loadFromFile(fileName);
                tests->pushBack(test);
            } catch (std::exception&) {}
        }
    }

    closedir(dir);
    return *tests;
}

void Test::deleteTestFile()
{
    if (fileName.empty())
    {
        throw FileError("Невозможно удалить файл, так как он ещё не создан");
    }
    if (std::remove((PATH + fileName).c_str()) != 0) {
        throw FileError("Не удалось удалить файл теста \"" + fileName + "\".");
    }
    fileName.clear();
}

void Test::setFileName(const std::string& fileName) 
{
    this->fileName = fileName;
}
