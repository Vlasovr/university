// 1. Создать класс для работы с файлами.В классе реализовать функцию замены местами в бинарном файле,
// содержащем структуры (несколько полей) предпоследнего и последнего элементов (структур).
// Для работы с файлами использовать классы ifstream, ofstream, fstream.
//В каждой задаче производить обработку исключений.
#include "InterfaceWithTemplateList.cpp"
#include "InterfaceWithList.cpp"

int main() {
    InterfaceWithList<int> list;
    list.menuForUser();
//    InterfaceWithTemplateList<int> list2;
//    list2.menuForUser();
//    InterfaceWithFile<Student> file;
//    file.menuForUser();


    return 0;
}
