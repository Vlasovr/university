/*Даны три файла целых чисел одинакового размера с именами
NameA, NameB и NameC.Создать новый файл с именем NameD, в котором
чередовались бы элементы исходных файлов с одним и тем же номером : A0, B0, C0,
A1, B1, C1, A2, B2, C2, ...*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
int get_integer();
void massiv4(FILE* nameA, FILE* nameB, FILE* nameC, int n);
int main()
{
    setlocale(LC_ALL, "Rus");
    const int n = 5;
    FILE *nameA, *nameB, *nameC;
    if (!(nameA = fopen("test4.txt", "w+t")))
    {
        printf("Невозможно создать файл\n"); return 1;
    }
    printf("Введите числа 1 файла + enter\n");
    for (int i = 0; i < n; i++)
    {
        int value = get_integer();
        fprintf(nameA, "%d ", value);
    }
    fclose(nameA);
    printf("Введите числа 2 файла + enter\n");
    if (!(nameB = fopen("test5.txt", "w+t")))
    {
        printf("Невозможно создать файл\n"); return 1;
    }
    for (int i = 0; i < n; i++)
    {
        int value = get_integer();
        fprintf(nameA, "%d ", value);
    }
    fclose(nameB);
    printf("Введите числа 3 файла + enter\n");
    if (!(nameC = fopen("test6.txt", "w+t")))
    {
        printf("Невозможно создать файл\n"); return 1;
    }
    for (int i = 0; i < n; i++)
    {
        int value = get_integer();
        fprintf(nameA, "%d ", value);;
    }
    fclose(nameC);
    massiv4(nameA, nameB, nameC, n);
    return 0;
}
int get_integer() {
    char answer[256]; // строка для чтения
    int n; // итоговое целое число

    fgets(answer, sizeof(answer), stdin); // считываем строку

    // пока не будет считано целое число
    while (sscanf(answer, "%d", &n) != 1) {
        printf("Incorrect input. Try again: "); // выводим сообщение об ошибке
        fgets(answer, sizeof(answer), stdin); // и заново считываем строку
    }

    return n; // возвращаем корректное целое число
}
void massiv4(FILE* nameA, FILE* nameB, FILE* nameC, int n) {
        //создаем и записываем в файл все элементы массива как написано в задании
    int value1, value2, value3;
    nameA = fopen("test4.txt", "r");
    fseek(nameA, 0, SEEK_SET);
    nameB = fopen("test5.txt", "r");
    fseek(nameB, 0, SEEK_SET);
    nameC = fopen("test6.txt", "r");
    fseek(nameC, 0, SEEK_SET);
    FILE* nameD;
    if (!(nameD = fopen("test7.txt", "w+t")))
        {
            printf("Невозможно создать файл\n");
        }
        for (int i = 0; i < n; i++)
        {
            fscanf(nameA, "%d", &value1);
            fscanf(nameB, "%d", &value2);
            fscanf(nameC, "%d", &value3);
            fprintf(nameD, "%d %d %d ", value1, value2, value3);
            printf("%d %d %d ", value1, value2, value3);
        }
        fclose(nameD);
        fclose(nameA);
        fclose(nameB);
        fclose(nameC);
    }
