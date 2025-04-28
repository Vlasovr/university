//Из текстового файла удалить числа, содержащиеся в бинарном файле.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#define n 5
#define empty -858993460 //Если массив не до конца заполнился, то пустые элементы будут иметь это значение Просто чтоб исключить
int get_integer();
void put_numbers1(FILE*);
void put_numbers2(FILE*);
void common_numbers(FILE*, FILE*, int[]);
void del(FILE*, int[], int[]);

int main() {
  setlocale(LC_ALL, "Russian");
  int arr[n];
  int arr1[n];
  FILE* textfile;
  if (!(textfile = fopen("test.txt", "w+t")))
  {
    printf("Файл не удалось создать\n"); return 1;
  }
  fclose(textfile);
  put_numbers1(textfile);
  FILE* binfile;
  if (!(binfile = fopen("bin.txt", "w+b")))
  {
    printf("Файл не удалось создать\n"); return 1;
  }
  fclose(binfile);
  put_numbers2(binfile);
  common_numbers(textfile, binfile, arr);
  del(textfile, arr, arr1);
  return 0;
}
int get_integer()
{
    char answer[256]; // строка для чтения
    int z; // итоговое целое число
    fgets(answer, sizeof(answer), stdin); // считываем строку

    // пока не будет считано целое число
    while (sscanf(answer, "%d", &z) != 1) {
        printf("Incorrect input. Try again: "); // выводим сообщение об ошибке
        fgets(answer, sizeof(answer), stdin); // и заново считываем строку
    }

    return z; // возвращаем корректное целое число
}

void put_numbers1(FILE* textfile)
{
  textfile = fopen("test.txt", "w+t");
  printf("Введите числа, содержащиеся в текстовом файле\n");
  for (int i = 0; i < n; i++)
  {
    int value = get_integer();
    fprintf(textfile, "%d ", value);
  }
  printf("\nВведеный файл: ");
  fseek(textfile, 0, SEEK_SET);
  for (int i = 0; i < n; i++)
  {
    int value;
    fscanf(textfile, "%d", &value);
    printf("%d ", value);
  }
  fclose(textfile);
  printf("\n\n");
  return;
}
void put_numbers2(FILE* binfile)
{
  binfile = fopen("bin.txt", "w+b");
  printf("Введите числа, содержащиеся в бинарном файле\n");
  for (int i = 0; i < n; i++)
  {
    int value = get_integer();
    fwrite(&value, sizeof(int), 1, binfile);
  }

  fclose(binfile);
}
void common_numbers(FILE* textfile, FILE* binfile, int arr[])
{
  textfile = fopen("test.txt", "r");
  binfile = fopen("bin.txt", "rb");
  fseek(textfile, 0, SEEK_SET);
  fseek(binfile, 0, SEEK_SET);
  int value1, value2;
  for (int i = 0; i < n; i++)
  {
    fscanf(textfile, "%d", &value1);
    for (int j = 0; j < n * 4; j += 4)
    {
      fseek(binfile, j, SEEK_SET);
      fread(&value2, sizeof(int), 1, binfile);
      if (value1 == value2) { // находим общие числа
        arr[i] = value1; // запоминает в массив числа 1 файла которые есть во 2 файле
        break;

      }
    }
  }

}
void del(FILE* textfile, int arr[], int arr1[])
{
  textfile = fopen("test.txt", "r");
  int counter = 0;
  if (textfile != NULL) {
    for (int i = 0; i < n; i++)
    {
      int value;
      fscanf(textfile, "%d", &value);
      if (value != arr[i]) { // если число не раво массиву общих чисел
        arr1[i] = value; // записываем новый массив чисел текстового файла без общих чисел
        counter++;
      }
    }
  }
  fclose(textfile);

  textfile = fopen("test.txt", "w+");
  if (textfile != NULL) {
    for (int i = 0; i < n; i++)
    {
      if (arr1[i] != empty) { // проверка пустой ли индекс массива
        fprintf(textfile, "%d ", arr1[i]);
      }
    }
    printf("\nИзмененный файл ");
    fseek(textfile, 0, SEEK_SET);
    for (int i = 0; i < counter; i++)
    {
      int value;
      fscanf(textfile, "%d", &value);
      printf("%d ", value);
    }
  }
  printf("\n\n");
  fclose(textfile);
}
