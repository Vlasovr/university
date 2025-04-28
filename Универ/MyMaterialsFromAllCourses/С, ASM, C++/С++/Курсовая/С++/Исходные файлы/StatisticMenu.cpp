#include "StatisticMenu.h"
#include "Menu.h"
#include "FileError.h"
#include <fstream>
#include <iomanip>
#include <functional>

StatisticMenu::StatisticMenu() {
    statistics = {};
    statistics = Statistic::loadAllFromFile();
    sortStatistics();
}

StatisticMenu::~StatisticMenu()
{
    for (auto statistic : statistics) {
        delete statistic;
    }
    statistics.clear();
}

void StatisticMenu::start() 
{
    char choice;
    do {
        std::cout << "Меню статистики:\n";
        std::cout << "1. Просмотреть статистику\n";
        std::cout << "2. Удалить статистику\n";
        std::cout << "0. Вернуться в режим администратора\n";
        try {
            choice = Menu::getInput("012");
        }
        catch (const InputError& e) {
            cout << e.what() << endl;
            continue;
        }
        switch (choice) {
        case '1':
            displayStatisticList();
            break;
        case '2':
            deleteStatistic();
            saveStatistics();
            break;
        }

    } while (choice != '0');
}

void StatisticMenu::displayStatisticList() 
{
    while (true) {
        int maxIndex = printStatisticList();
        if (maxIndex == 0) {
            std::cout << "Список статистики пуст.\n\nВведите любой символ для возврата...";
            std::string input;
            getline(std::cin, input);
            system("cls");
            return;
        }

        std::cout << "\nВведите номер статистики для просмотра, либо 0 для возврата: ";
        std::string input;
        getline(std::cin, input);
        system("cls");
        if (input == "0") {
            return;
        }
        try {
            int selectedIndex = Menu::getValidatedNumber(input, 1, maxIndex);
            statistics[selectedIndex - 1]->print();
        }
        catch (const InputError& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

int StatisticMenu::printStatisticList()
{
    if (statistics.getSize() == 0) {
        return 0;
    }

    const int colWidthIndex = 4;
    const int colWidthTestTitle = MAX_TEST_TITLE_LENGTH + 1;
    const int colWidthUserName = MAX_FULL_NAME_LENGTH + 1;
    const int colWidthTime = 15;

    std::cout << "Список статистики:\n";
    std::cout << std::left << std::setw(colWidthIndex) << "№"
        << std::left << std::setw(colWidthTestTitle) << "Название теста"
        << std::left << std::setw(colWidthUserName) << "Имя тестируемого"
        << std::left << std::setw(colWidthTime) << "Дата и время" << "\n";

    std::cout << std::string(colWidthIndex + colWidthTestTitle + colWidthUserName + colWidthTime, '-') << "\n";

    for (int i = 0; i < statistics.getSize(); ++i) {
        const auto& stat = statistics[i];
        std::cout << std::left << std::setw(colWidthIndex) << (i + 1)
            << std::left << std::setw(colWidthTestTitle) << stat->getTestTitle()
            << std::left << std::setw(colWidthUserName) << stat->getUserName()
            << std::left << std::setw(colWidthTime) << stat->getTime() << "\n";
    }

    return statistics.getSize();
}

void StatisticMenu::deleteStatistic()
{
    while (true) {
        int maxIndex = printStatisticList();
        if (maxIndex == 0) {
            std::cout << "Список статистики пуст.\n\nВведите любой символ для возврата...";
            std::string input;
            getline(std::cin, input);
            system("cls");
            return;
        }

        std::cout << "\nВведите номер статистики для удаления (или 0 для отмены): ";
        std::string input;
        getline(std::cin, input);

        if (input == "0") {
            system("cls");
            return;
        }

        try {
            int selectedIndex = Menu::getValidatedNumber(input, 1, maxIndex);
            auto stat = statistics[selectedIndex - 1];
            std::cout << "Вы действительно хотите удалить статистику для теста \""
                << stat->getTestTitle()
                << "\" от пользователя \"" 
                << stat->getUserName()
                << "\"?\n(Введите Y для подтверждения или любой другой символ для отмены): ";

            getline(std::cin, input);
            system("cls");

            if (input != "Y" && input != "y") {
                std::cout << "Удаление отменено.\n";
                continue;
            }

            delete statistics[selectedIndex - 1];
            statistics.removeAt(selectedIndex - 1);
            std::cout << "Статистика успешно удалена!\n";
        }
        catch (const InputError& e) {
            system("cls");
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

void StatisticMenu::saveStatistics()
{
    do {
        try {
            std::ofstream out(STAT_FILE, std::ios::binary);
            if (!out.is_open()) {
                std::string errorTest = "Не удалось открыть файл для записи: ";
                throw FileError(errorTest + STAT_FILE);
            }
            out.close();
            for (const auto& stat : statistics) {
                stat->saveToFile();
            }
            break; 
        }
        catch (const FileError& e) {
            cout << "Ошибка: " << e.what() << endl;
            cout << "Введите 0 для выхода без сохранения или любой другой символ для повторной попытки: ";
            string input;
            getline(cin, input);
            system("cls");
            if (input == "0") return;
        }
    } while (true);
}

void StatisticMenu::sortStatistics() {
    int n = statistics.getSize(); 

    std::function<void(int, int)> heapify = [&](int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && statistics[left]->getTestTitle() > statistics[largest]->getTestTitle()) {
            largest = left;
        }

        if (right < n && statistics[right]->getTestTitle() > statistics[largest]->getTestTitle()) {
            largest = right;
        }

        if (largest != i) {
            std::swap(statistics[i], statistics[largest]);
            heapify(n, largest);
        }
        };

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(n, i);
    }

    for (int i = n - 1; i > 0; --i) {
        std::swap(statistics[0], statistics[i]);
        heapify(i, 0);
    }
}
