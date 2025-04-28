

/*Структура содержит информацию о фильмах : длительность в минутах
(число), название(указатель), вложенную структуру – жанр(строка
    фиксированной длины) и год выпуска.Найти фильмы с заданным названием.
    Удалить фильмы с длительностью больше заданной.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>
#include <locale.h>
int N;
static const int sizeOfJanr = 20;     // статическая переменная, общая для всех объектов этой структуры хранит макс. размер массива с названием жанра
struct janr             // описывает жанр
{

    char j[sizeOfJanr];
    int year_of_film;
};

struct movie        // структура, описывает фильм
{
    char* name;
    int length;
    struct janr Janr;
};

void fill_movies(struct movie*, int);        // прототипы функций , необходимых для лабораторной
void show_movies(struct movie*, int);
int  find_movie(const struct movie*, const char*, int);
void remove_movies(struct movie**, int, int);      // функция принимает указатель на указатель, т.к. при удалении элемента из массива будет
// изменен указатель из функции main(), т.к. он будет ссылаться на новую область памяти без фильмов большей длины чем указанная

int main()
{
    setlocale(LC_ALL, "Russian");
    struct movie* movie_ptr;           // указатель, ссылается на массив фильмов

    printf("Кол-во фильмов: ");
    scanf("%d", &N);
    //movie* = (int*)calloc(N, sizeof(int));
    movie_ptr = (struct movie*)calloc(N, sizeof(struct movie*));
    //movie_ptr = new movie[N];   // создаем массив фильмов размером N
    fill_movies(movie_ptr, N);  // функция заполнения массива фильмов


    system("cls");

    show_movies(movie_ptr, N);  // функция вывода массива фильмов на экран
    //fflush(stdin);
    getchar();//cin.get();
    char name[20];
    printf("Название фильма для поиска: \n");

    fgets(name, 20, stdin);

    int index = find_movie(movie_ptr, name, N);         // функция поиска фильма по названию. возвращает индекс фильма, если он там есть
    if (index == -1) printf("Данный фильм в массиве не указан!\n\n");   // если вернула -1, то фильма нет
    else printf("Фильм с названием %s в массиве под индексом %d\n\n\n", name , index);




    int len;
    show_movies(movie_ptr, N);
    printf("Введите пороговую длительность фильма: ");
    scanf("%d", &len);

    remove_movies(&movie_ptr, N, len);          // функция "удаления" элементов больше заданной длины из массива фильмов
    printf("\n\n\n");
    show_movies(movie_ptr, N);

    return 0;
}

void fill_movies( struct movie* movie_ptr, int N)
{
    char buff[30];

    for (int i=0; i < N; ++i)
    {
        //fflush(stdin);
        getchar();//cin.get();
        printf("Название %d фильма: \n", i + 1);
        fgets(buff, 30, stdin);
        movie_ptr[i].name = (char*)calloc(strlen(buff), sizeof(char));
        //movie_ptr[i].name = new char[strlen(buff)];
        strcpy(movie_ptr[i].name, buff);

        printf("Длительность %d фильма: \n", i + 1);
        scanf("%d", &movie_ptr[i].length);
        getchar();//cin.get();         // мб нужно переписать на С
        // fflush(stdin);
        printf("Жанр %d фильма: \n", i + 1 );
        fgets(movie_ptr[i].Janr.j, sizeOfJanr, stdin);

        printf("Год выпуска %d фильма: \n", i + 1);
        scanf("%d",  &movie_ptr[i].Janr.year_of_film);

        printf("\n\n\n");
    }
}

void show_movies(struct movie* movie_ptr, int N)
{
    for (int i=0; i < N; ++i)
    {
        printf("Название %d фильма: %s", i + 1, movie_ptr[i].name);
        printf("Длительность %d фильма: %d\n", i + 1, movie_ptr[i].length);
        printf("Жанр %d фильма: %s",i + 1, movie_ptr[i].Janr.j);
        printf("Год выпуска %d фильма: %d\n", i + 1, movie_ptr[i].Janr.year_of_film);
    }
}

int find_movie(const struct movie* movie_ptr, const char* name, int N)
{
    for (int i=0; i < N; ++i)
    {
        if (!strcmp(name, movie_ptr[i].name))
            return i;
    }
    return -1;
}
void remove_movies(struct movie** movie_ptr, int N, int len)
{
    struct movie* buff_ptr;        // буферный указатель для нового массива с фильмами, без учета тех, что нужно будет удалить
    int count = 0;

    for (int i=0; i < N; ++i)
    {
        if ((*movie_ptr)[i].length > len)   // делаем доп. разыменовывание , т.к. у нас передан адрес указателя из main() (указатель на указатель)
            ++count;            // увеличиваем счетчик кол-ва фильмов с длительностью выше заданной
    }
    if (count == 0) return;     // если таких фильмов нет - просто выходим из функции

    N -= count;// уменьшаем глобальную переменную размера массива на кол-во фильмов с длиной выше заданной
    buff_ptr = (struct movie*)calloc(N, sizeof(struct movie*));
    // buff_ptr = new movie[::N];
    int j = 0;

    for (int i=0; i < N; ++i)
    {
        if ((*movie_ptr)[i].length > len)       // если длина рассматриваемого фильма в исходном массиве больше, чем указанная, то этот элемент не записываем и переходим к след. итерации цикла
        {
            continue;
        }

        buff_ptr[j].name = (char*)calloc(strlen((*movie_ptr)[i].name) + 1, sizeof(char));
        //buff_ptr[j].name = new char[strlen((*movie_ptr)[i].name) + 1];  // иначе копируем фильм в буферную область
        strcpy(buff_ptr[j].name, (*movie_ptr)[i].name);

        buff_ptr[j].length = (*movie_ptr)[i].length;
        buff_ptr[j].Janr.year_of_film = (*movie_ptr)[i].Janr.year_of_film;

        strcpy(buff_ptr[j].Janr.j, (*movie_ptr)[i].Janr.j);
        ++j;            // увеличиваем счетчик j (счетчик буферного массива) на 1
    }

    free(*movie_ptr);      // удаляем старый исходный массив
    *movie_ptr = buff_ptr;      // присваиваем исходному указателю из функции main() адрес новой область с фильмами, длительность которых меньше указанной
}
