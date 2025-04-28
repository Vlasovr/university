
//int main(int argc, const char * argv[]) {
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


int get_longlong(const char* msg);

int main()
{
    const char msg[25] = "put number:\n";
    short size = sizeof(long long);
    long long n = get_longlong(msg);

    int val = (n >> ((size * 8) - 1));
    if (val == 0)
        printf("positive\n");
    else printf("negative\n");
    return 0;
}
int get_longlong(const char* msg) {
    char answer[256]; // строка для чтения
    long long n; // итоговое целое число

    printf("%s", msg); // выводим приглашение ко вводу
    fgets(answer, 256, stdin); // считываем строку

    // пока не будет считано целое число
    while (sscanf(answer, "%lld", &n) != 1) { //тение данных происходит не из стандартного потока ввода, а из переданной ей первым аргументом строки.
        printf("Incorrect input. Try again: "); // выводим сообщение об ошибке
        fgets(answer, sizeof(answer), stdin); // и заново считываем строку
    }
    return n; // возвращаем корректное целое число
}
