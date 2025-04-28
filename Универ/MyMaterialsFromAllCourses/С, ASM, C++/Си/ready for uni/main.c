/*Создать массив структур содержащих информацию о студентах. Поля : ФИО, номер курса, номер группы, целочисленный массив из 5 оценок.
Записать в файл информацию о студентах.
Написать функцию которая выберет из файла студентов со средним балом не менее 7. Вывести на экран.
*/
/*
    int get_integer();
        void put_numbers1(FILE*);
        void put_numbers2(FILE*);
        void common_numbers(FILE*, FILE*, int[]);
        void del(FILE*, int[], int[]);
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
*/
/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct zap
{
char inf[50]; // информационное поле
struct zap *l; // адресное поле
};
void see(struct zap *); // просмотреть стек
struct zap* butld(struct zap *); // создать вершину стека
struct zap * add(struct zap *); // добавить элемент в стек
struct zap * del(struct zap *); // удалить элемент стека
void main(void)
{
    struct zap *s;
s=NULL;
//clrscr();
while(1)
{
puts("вид операции:А – создать/добавить");
puts(" D – удалить");
puts(" S – просмотреть");
puts(" E – закончить");
fflush(stdin);
switch(getchar())
{
case 'a': case 'A': s=add(s); break;
case 'd': case 'D': s=del(s); break;
case 's': case 'S': see(s); break;
case 'e': case 'E': return;

default: printf("Ошибка, повторите \n");
}
}
}
// функция создания и добавления элемента стека
struct zap * add(struct zap *s)
{
struct zap *s1;
s1=s; // текущая вершина стека
if((s=(struct zap *)malloc(sizeof(struct zap)))==NULL)
{ // размещаем новый элемент на вершину стека
puts("Нет свободной памяти");
return 0;
}
puts("Введите информацию в inf");
scanf("%s",s->inf);
s->l=s1; // указатель на предыдущий элемент стека
return s;
}
//функция просмотра элементов стека
void see(struct zap *s)
{
struct zap *s1;
s1=s; // текущая вершина стека
if(s==NULL)
{
puts("Стек не создан");
return;
}
do
{
printf("%s\n",s1->inf);
s1=s1->l; // переход к предыдущему элементу стека
}while(s1!=NULL);
puts("вывод стека закончен");
}
// функция удаления последнего элемента стека
struct zap * del(struct zap *s)
{
struct zap *s1;
if(s==NULL)
{
    puts("Стек пуст");
    return 0;
    }
    s1=s; // запоминаем старый указатель на вершину стека
    s=s->l; // передвигаем указатель на следующий элемент стека
    free(s1); // удаляем элемент с вершины стека
    puts("последний элемент стека удален");
    return (s); // возвращаем указатель на новую вершину стека
}
*/
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
struct zap
{ char inf[50]; // информационное поле
    struct zap *nx; // адресное поле
};
void add(struct zap **,struct zap **);
void del(struct zap **,struct zap **);
void del_any(struct zap **,struct zap **,char *);
void see(struct zap *);
void sort(struct zap **);
void main(void)
{ struct zap *h,*t; // указатели на голову и хвост очереди
char l,*st;
st=(char *)malloc(10);
h=t=NULL;
while(1)
{ puts("вид операции: 1– создать очередь");
puts(" 2 – вывод содержимого очереди");
puts(" 3 – удаление элемента из очереди");
puts(" 0 – окончить");
fflush(stdin);
switch(getchar())
{ case '1': add(&h,&t); break; // добавление в хвост очереди
case '2': see(h); break; // просмотр с головы очереди
case '3': if(h) del(&h,&t); break; // удаление с головы очереди
case '0': return;
default: printf("Выбран ошибочный режим, повторите \n");
}
}
}
// функция создания очереди
void add(struct zap **h,struct zap **t)
{ struct zap *n;
puts("Создание очереди \n");
do
{ if(!(n = (zap *)calloc(1,sizeof(zap))))
{ puts("Нет свободной памяти");
return;
}
puts("Введите информацию в inf");
scanf("%s",n->inf);
if (!*h) // очередь еще не создана
*h=*t=n; // устанавливаем оба указателя (голова и хвост)
// на единственный элемент очереди
else // очередь уже создана
{ (*t)->nx=n; // добавляем очередной элемент в очередь
*t=n; // передвигаем указатель на хвост
}
puts("Продолжить (y/n): ");
fflush(stdin);
} while(getch()=='y');
}
// функция вывода содержимого очереди
void see(struct zap *h)
{ puts("Вывод содержимого очереди \n");
if (!h) // указатель NULL
{ puts("Очередь пуста");
return;
}
do
{ printf("%s\n",h->inf); // вывод текущего элемента
h=h->nx; // переход к следующему
} while(h);
return;
}
// функция удаления первого элемента очереди
void del(struct zap **h,struct zap **t)
{
    struct zap *p;
if(*t==*h) // в очереди только один элемент
{ free(*h); // удаляем единственный элемент очереди
*t=*h=NULL; // очередь пуста
return;
}
p=(*h)->nx; // р указывает на элемент следующий за первым
free(*h); // удаление первого элемента из очереди
*h=p; // перемещение указателя h на голову очереди
}*/
/*#include <stdio.h>
 #include <alloc.h>
 #include <string.h>
 #include <conio.h>
 #include <dos.h>
 #define N 20
 #define M 20
 struct der {char *inf; // информационное поле
 int n; // число встреч информационного поля в бинарном дереве
 struct der *l,*r;}; // указатель на левое и правое поддерево
 void see_1(der *);
 void see_2(der *);
 der *sozd(der *);
 void add(der *);
 der *del(der *);
 void main(void)
 { der *dr;
 dr=NULL; // адрес корня бинарного дерева
 clrscr();
 while(1)

 PDF created with pdfFactory Pro trial version www.pdffactory.com
 Библиотека БГУИР

 22
 { puts("вид операции: 1– создать дерево");
 puts(" 2 – рекурсивный вывод содержимого дерева");
 puts(" 3 – нерекурсивный вывод содержимого дерева");
 puts(" 4 – добавление элементов в дерево");
 puts(" 5 – удаление любого элемента из дерева");
 puts(" 6 – выход");
 fflush(stdin);
 switch(getch())
 { case '1': dr=sozd(dr); break;
 case '2': see_1(dr); getch(); break;
 case '3': see_2(dr); getch(); break;
 case '4': add(dr); break;
 case '5': del(dr); break;
 case '6': return;
 }
 clrscr();
 }
 }
 // создание бинарного дерева
 der *sozd(der *dr)
 { if (dr)
 { puts("Бинарное дерево уже создано");
 return (dr);
 }
 if (!(dr=(der *) calloc(1,sizeof(der))))
 { puts("Нет свободной памяти");
 getch();
 return NULL;
 }
 puts("Введите информацию в корень дерева");
 dr->inf=(char *) calloc(1,sizeof(char)*N);
 gets(dr->inf);
 dr->n=1; // число повторов информации в дереве
 return dr;
 }
 // функция добавления узлов в бинарное дерево
 void add(der *dr)
 { struct der *dr1,*dr2;
 char *st; // строка для анализа информации
 int k; // результат сравнения двух строк
 int ind;
 if (!dr)
 { puts("Нет корня дерева \n");

 PDF created with pdfFactory Pro trial version www.pdffactory.com
 Библиотека БГУИР

 23

 getch();
 return;
 }
 do
 { puts("Введите информацию в очередной узел дерева (0 – выходl)");
 st=(char *) calloc(1,sizeof(char)*N); //память под символьную информацию
 gets(st);
 if(!*st) return; // выход в функцию main
 dr1=dr;
 ind=0; // 1 – признак выхода из цикла поиска
 do
 { if(!(k=strcmp(st,dr1->inf)))
 { dr1->n++; // увеличение числа встреч информации узла
 ind=1; // для выхода из цикла do ... while
 }
 else
 {if (k<0) // введ. строка < строки в анализируемом узле
 { if (dr1->l) dr1=dr1->l; // считываем новый узел дерева
 else ind=1; // выход из цикла do ... while
 }
 else // введ. строка > строки в анализируемом узле
 { if (dr1->r) dr1=dr1->r; // считываем новый узел дерева
 else ind=1; // выход из цикла do ... while
 }
 }
 } while(ind==0);
 if (k) // не найден узел с аналогичной информацией
 { if (!(dr2=(struct der *) calloc(1,sizeof(struct der))))
 { puts("Нет свободной памяти");
 return;
 }
 if (k<0) dr1->l=dr2; // ссылка в dr1 налево
 else dr1->r=dr2; // ............ направо
 dr2->inf=(char *) calloc(1,sizeof(char)*N);
 strcpy(dr2->inf,st); // заполнение нового узла dr2
 dr2->n=1;
 }
 free(st);
 } while(1); // любое условие, так как выход из цикло по return
 }
 // рекурсивный вывод содержимого бинарного дерева
 void see_1(der *dr1)
 { if(dr1)

 PDF created with pdfFactory Pro trial version www.pdffactory.com
 Библиотека БГУИР

 24
 // для вывода информации удалите комментарий с одной
 // из инструкций printf, содержащихся ниже
 { //printf("узел содержит : %s , число встреч %d\n",dr1->inf,dr1->n);
 if (dr1->l) see_1(dr1->l); // вывод левой ветви дерева
 //printf("узел содержит : %s , число встреч %d\n",dr1->inf,dr1->n);
 if (dr1->r) see_1(dr1->r); // вывод правой ветви дерева
 //printf("узел содержит : %s , число встреч %d\n",dr1->inf,dr1->n);
 }
 }
 // нерекурсивный вывод содержимого бинарного дерева,
 // используя стек для занесения адресов узлов дерева
 void see_2(der *dr1)
 { struct stek{ der *d;
 stek *s;} *st,*st1=NULL;
 int pr=1;
 for(int i=0;i<2;i++) // в стек заносятся два элемента, содержащие указатель
 { st=(stek *)calloc(1,sizeof(stek)); // на корень дерева для прохода
 st->d=dr1; // по левому и правому поддеревьям
 st->s=st1; // указатель на стек вниз
 st1=st;
 }
 printf("узел содержит : %s , число встреч %d\n",dr1->inf,dr1->n);
 while(st)
 { do
 { if(pr && dr1->l) dr1=dr1->l; // переход на узел слева
 else if (dr1->r) dr1=dr1->r; // переход на узел cправа
 pr=1; // сброс принудительного движения вправо
 if(dr1->l && dr1->r) // узел с двумя связями вниз
 { st1=st;
 st=(stek *)calloc(1,sizeof(stek));
 st->d=dr1; // указатель на найденый узел
 st->s=st1; // указатель на стек вниз
 }
 printf("узел содержит : %s , число встреч %d\n",dr1->inf,dr1->n);
 } while(dr1->l || dr1->r);
 dr1=st->d; // возврат на узел ветвления
 st1=st->s; // в стеке адрес узла выше удаляемого
 free(st); // удаление из стека указателя на узел
 // после прохода через него налево
 st=st1;
 if(dr1->r) pr=0; // признак принудительного перехода
 // на узел, расположенный справа от dr1, так как
 // dr1->inf уже выведен при проходе слева

 PDF created with pdfFactory Pro trial version www.pdffactory.com
 Библиотека БГУИР

 25

 }
 }
 // функция удаления узла дерева
 der *del(der *dr)
 { struct der *dr1,*dr2,*dr3;
 char *st; // строка для анализа информации
 int k; // результат сравнения двух строк
 int ind;
 if(!dr)
 { puts("Дерево не создано \n");
 return NULL;
 }
 puts("Введите информацию для поиска удаляемого узла");
 st=(char *) malloc(sizeof(char)*N);
 fflush(stdin);
 gets(st); //строка для поиска узла в дереве
 if(!*st) return NULL; // выход в функцию main
 dr2=dr1=dr;
 ind=0; // 1 – признак выхода из цикла поиска
 do // блок поиска удаляемого из дерева узла
 { if (!(k=strcmp(st,dr1->inf)))
 ind=1; // узел со строкой st найден
 if (k<0) // введ. строка < строки в анализируемом узле
 { if (dr1->l)
 { dr2=dr1; // запоминаем текущий узел
 dr1=dr1->l; // считываем новый левый узел дерева
 }
 else ind=1; // выход из цикла do ... while
 }
 if (k>0) // введ. строка > строки в анализируемом узле
 { if (dr1->r)
 { dr2=dr1; // запоминаем текущий узел
 dr1=dr1->r; // считываем новый правый узел дерева
 }
 else ind=1; // выход из цикла do ... while
 }
 } while(!ind);
 free(st);
 if (k)
 { puts("Требуемый узел не найден \n");
 getch();
 return dr;
 }

 PDF created with pdfFactory Pro trial version www.pdffactory.com
 Библиотека БГУИР

 26
 else
 { k=strcmp(dr1->inf,dr2->inf);
 dr3=dr1;
 if (k<0) // удаляемая вершина < предыдущей
 { dr3=dr1->r; // поиск ссылки NULL влево
 while(dr3->l) dr3=dr3->l;
 dr2->l=dr1->r; // сдвиг ветви, начинающейся с адреса dr1->r, влево
 dr3->l=dr1->l;
 }
 else // удаляемая вершина > предыдущей
 { dr3=dr1->l; // поиск ссылки NULL вправо
 while(dr3->r) dr3=dr3->r;
 dr2->r=dr1->l; // сдвиг ветви, начинающейся с адреса dr1->l, вправо }
 dr3->r=dr1->r;
 }
 }
 }
*/
/*
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#define N 5

static const int sizeOfFIO = 20;// статическая переменная, общая для всех объектов этой структуры хранит макс. размер массива с фио студента
    static const int maxNumberOfCourse = 2;
    static const int maxNumberOfGroup = 7;
    static const int maxNumberOfMarks = 5;
struct student
{

    char name;
    int numberOfCourse[maxNumberOfCourse];
    int numberOfGroup[maxNumberOfGroup];
    int marks[maxNumberOfMarks];
};

void fill_student(struct student*, int, FILE *);        // прототипы функций , необходимых для лабораторной
void show_student(struct student*, int);
void remove_student(struct student**, int, int);      // функция принимает указатель на указатель, т.к. при удалении элемента из массива будет
// изменен указатель из функции main(), т.к. он будет ссылаться на новую область памяти без фильмов большей длины чем указанная

int main(int argc, const char * argv[])
{
    setlocale(LC_ALL, "Russian");
    struct student* student_ptr;           // указатель, ссылается на массив фильмов

    printf("Кол-во студентов: ");
    //movie* = (int*)calloc(N, sizeof(int));
    student_ptr = (struct student*)calloc(N, sizeof(struct student*));
    //movie_ptr = new movie[N];   // создаем массив фильмов размером N
    FILE* textfile;
    if (!(textfile = fopen("test.txt", "w+t")))
    {
      printf("Файл не удалось создать\n"); return 1;
    }
    fclose(textfile);
    fill_student(student_ptr, N, textfile);  // функция заполнения массива фильмов

    show_student(student_ptr, N);  // функция вывода массива фильмов на экран
    //fflush(stdin);
    getchar();//cin.get();

    int len;
    show_student(student_ptr, N);
    remove_student(&student_ptr, N, len);          // функция "удаления" элементов больше заданной длины из массива фильмов
    printf("\n\n\n");
    show_student(student_ptr, N);

    return 0;
}

void fill_movies(struct student* student_ptr, int N, FILE* textfile)
{
    char buff[30];
    fopen("textfile.txt", "w+t");
    for (int i=0; i < N; ++i)
    {
        int value;
        
        //fflush(stdin);
        getchar();//cin.get();
        printf("Имя студента: \n", i + 1);
        fgets(buff, 30, stdin);
        student_ptr[i].name = (char*)calloc(strlen(buff), sizeof(char));
        //movie_ptr[i].name = new char[strlen(buff)];
        strcpy(student_ptr[i].name, buff);
        fprintf(textfile,"&s", buff);
        printf("номер группы: \n", i);
        scanf("%d", &student_ptr[i].numberOfGroup);
        // fflush(stdin);
        value = student_ptr[i].numberOfGroup;
        fprintf(textfile, "%d ", value);
        printf("номер курса: \n", i);
        scanf("%d", &student_ptr[i].numberOfCourse);
        value = student_ptr[i].marks;
        fprintf(textfile, "%d ", value);
        printf("5 оценок: \n", i);
        scanf("%d",  &student_ptr[i].marks);
        value = student_ptr[i].marks;
        fprintf(textfile, "%d ", value);
        printf("\n\n\n");
    }
}

void show_movies(struct student* student_ptr, int N)
{
    for (int i=0; i < N; ++i)
    {
        printf("Название %d фильма: %s", i, student_ptr[i].name);
        printf("Длительность %d фильма: %d\n", i , student_ptr[i].numberOfGroup);
        printf("Жанр %d фильма: %s",i , student_ptr[i].numberOfCourse);
        printf("Год выпуска %d фильма: %d\n", i , student_ptr[i].marks);
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


void send_into_file(ofstream&, student&);   // функция записи данных о студенте в файл

int main()
{
    setlocale(LC_ALL, "Russian");

    ofstream of("students.txt");    // создаём файл of с именем students.txt в папке с нашим проектом
    int size;

    cout << "Создание:\nВведите кол-во студентов: ";
    cin >> size;

    vector<student> studs_vec(size);        // вектор (массив) студентов указанного размера N

    for (int i(0); i < size; ++i)      // заполняем массив из студентов
    {
        fill_stud(studs_vec[i]);            // заполняем данные студента i
        show_stud(studs_vec[i]);            // выводим данные студента i
        send_into_file(of, studs_vec[i]);   // записываем в файл данные студента i
    }

    int choice = 0;     // переменная выбора - в зависимости от её будет выполняться то либо иное услове в операторе switch
    system("cls");      // очищаем экран для красоты

    student Stud;       // создаём буферного студента для ввода данных
    while (1)
    {
        cout << "\n\nВыберите ваше дальнейшее действие:\n"
            << "1.Добавить студента;\n"
            << "2.Просмотр списка студентов;\n"
            << "3.Индивидуальное задание;\n"
            << "\nВаш выбор (1-3, Q - выйти):";
        
        if (!(cin >> choice))       // если ввод выбора некорректен (либо равен Q - сигнал окончания программы), то выходим из цикла
            break;

        switch (choice)
        {
        case 1:                     // cоздаём нового студента и добавляем в наш вектор, записываем в файл
            system("cls");
            cout << "ДОБАВЛЕНИЕ СТУДЕНТА\n\n";

            fill_stud(Stud);
            send_into_file(of, Stud);
            studs_vec.push_back(move(Stud));

            break;
        
        case 2:                     // выводим список студентов
            system("cls");
            cout << "СПИСОК СТУДЕНТОВ:\n\n";

            for (int i(0); i < studs_vec.size(); ++i)
                show_stud(studs_vec[i]);
            break;
        
        case 3:                 // по условию задачи выводим тех студентов, которые нам необходимы
            system("cls");
            cout << "СТУДЕНТЫ С 4 ЛИБО 5 ПО ФИЗИКЕ И БОЛЬШЕ 8 ПО ОСТАЛЬНЫМ ПРЕДМЕТАМ:\n\n";

            for (int i(0); i < studs_vec.size(); ++i)
            {
                if (((studs_vec[i].sub[0].mark == 4) || (studs_vec[i].sub[0].mark == 5)) && (studs_vec[i].sub[1].mark > 8) && (studs_vec[i].sub[2].mark > 8))
                {
                    show_stud(studs_vec[i]);
                }
            }

            break;
        }
        system("pause");
        system("cls");
    }

    system("pause");
    return 0;
}

void fill_stud(student &stud) // заполняем все
{
    cin.get();
    cout << "Фамилию: ";
    getline(cin, stud.surname);

    cout << "Номер группы: ";
    cin >> stud.group;

    stud.sub[0].Type = subject::PHISIC;         // указываем тип каждого элемента предмета
    stud.sub[1].Type = subject::MATH;
    stud.sub[2].Type = subject::INFORMATIC;

    cout << "Оценка по физике: ";
    cin >> stud.sub[0].mark;
    cout << "Оценка по математике: ";
    cin >> stud.sub[1].mark;
    cout << "Оценка по информатике: ";
    cin >> stud.sub[2].mark;

    stud.average_mark = (stud.sub[0].mark + stud.sub[1].mark + stud.sub[2].mark) / 3;   // вычисляем средний балл
}

void send_into_file(ofstream& of, student& stud) // of - файл, в который мы записываем все данные переданного в функцию студента
{
    of << "Фамилия: " << stud.surname
        << "\nГруппа: " << stud.group
        << "\nОценка по физике: " << stud.sub[0].mark
        << "\nОценка по математике: " << stud.sub[1].mark
        << "\nОценка по информатике: " << stud.sub[2].mark
        << "\nСредний бал: " << stud.average_mark << endl << endl;
}

void show_stud(student &stud)       // выводит все поля (данные) указанного студента в консоль
{
    cout << "\nФамилия: " << stud.surname
        << "\nГруппа: " << stud.group
        << "\nОценка по физике: " << stud.sub[0].mark
        << "\nОценка по математике: " << stud.sub[1].mark
        << "\nОценка по информатике: " << stud.sub[2].mark
        << "\nСредний бал: " << stud.average_mark << endl << endl;
}
*/











/*Cоздать массив структур содержащих информацию о студентах. Поля : ФИО, номер курса, номер группы, целочисленный массив из 5 оценок.
   Записать в файл информацию о студентах.
   Написать функцию которая выберет из файла студентов со средним балом не менее 7. Вывести на экран.
*/
/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

struct person
{
    char name[20];
    int age;
};
 
int save(char * filename, struct person *st, int n);
int load(char * filename);
 
int main(void)
{
    char * filename = "people.dat";
    struct person people[] = { "Tom", 23, "Alice", 27, "Bob", 31, "Kate", 29 };
    int n = sizeof(people) / sizeof(people[0]);
 
    save(filename, people, n);
    load(filename);
    return 0;
}
 
// запись в файл массива структур
int save(char * filename, struct person * st, int n)
{
    FILE * fp;
    char *c;
 
    // число записываемых байтов
    int size = n * sizeof(struct person);
     
    if ((fp = fopen(filename, "wb")) == NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }
    // записываем количество структур
    c = (char *)&n;
    for (int i = 0; i<sizeof(int); i++)
    {
        putc(*c++, fp);
    }
 
    // посимвольно записываем в файл все структуры
    c = (char *)st;
    for (int i = 0; i < size; i++)
    {
        putc(*c, fp);
        c++;
    }
    fclose(fp);
    return 0;
}
 
// загрузка из файла массива структур
int load(char * filename)
{
    FILE * fp;
    char *c;
    int m = sizeof(int);
    int n, i;
 
    // выделяем память для количества данных
    int *pti = (int *)malloc(m);
 
    if ((fp = fopen(filename, "r")) == NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }
    // считываем количество структур
    c = (char *)pti;
    while (m>0)
    {
        i = getc(fp);
        if (i == EOF) break;
        *c = i;
        c++;
        m--;
    }
    //получаем число элементов
    n = *pti;
 
    // выделяем память для считанного массива структур
    struct person * ptr = (struct person *) malloc(n * sizeof(struct person));
    c = (char *)ptr;
    // после записи считываем посимвольно из файла
    while ((i= getc(fp))!=EOF)
    {
        *c = i;
        c++;
    }
    // перебор загруженных элементов и вывод на консоль
    printf("\n%d people in the file stored\n\n", n);
 
    for (int k = 0; k<n; k++)
    {
        printf("%-5d %-20s %5d \n", k + 1, (ptr + k)->name, (ptr + k)->age);
    }
 
    free(pti);
    free(ptr);
    fclose(fp);
    return 0;
}
/*struct person {
   char name;
   int age;
    int marks[4];
};
int save(char * filename, struct person *p);
int load(char * filename);
void fill(struct person* person_ptr, int);

    int main(void) {
    
    setlocale(LC_ALL, "Russian");
    int n;
    scanf("%d", &n);
    struct person * person_ptr = NULL;
    person_ptr = (struct person*)calloc(n, sizeof(struct person*));
        fill(person_ptr, n);
        char * filename = "person.dat";
        save(filename, person_ptr);
        load(filename);
         
        return 0;
    }
void fill_movies(struct person* person_ptr, int N)
    {
    char buff[30];
        for (int i=0; i < N; ++i)
              {
                  getchar();
                //  printf("Имя человека: \n", i + 1);
                  fgets(buff, 30, stdin);
                  person_ptr[i].name = *(char*)calloc(strlen(buff), sizeof(char));
                  strcpy(&person_ptr[i].name, buff);
                  //printf("номер группы: \n", i);
                  scanf("%d", &person_ptr[i].age);
                  //printf("5 оценок: \n", i);
                  for(int k = 0; k<4; k++){
                  scanf("%d",  &person_ptr[i].marks[k]);
                  }
                  printf("\n\n\n");
              }
          }
    // запись структуры в файл
    int save(char * filename, struct person *p)
    {
        FILE * fp;
        char *c;
        int size = sizeof(struct person); // количество записываемых байтов
     
        if ((fp = fopen(filename, "wb")) == NULL)
        {
            perror("Error occured while opening file");
            return 1;
        }
        // устанавливаем указатель на начало структуры
        c = (char *)p;
        // посимвольно записываем в файл структуру
        for (int i = 0; i < size; i++)
        {
            putc(*c++, fp);
        }
        fclose(fp);
        return 0;
    }
     
    // загрузка из файла структуры
    int load(char * filename)
    {
        FILE * fp;
        char *c;
        int i; // для считывания одного символа
        // количество считываемых байтов
        int size = sizeof(struct person);
        // выделяем память для считываемой структуры
        struct person * ptr = (struct person *) malloc(size);
     
        if ((fp = fopen(filename, "rb")) == NULL)
        {
            perror("Error occured while opening file");
            return 1;
        }
     
        // устанавливаем указатель на начало блока выделенной памяти
        c = (char *)ptr;
        // считываем посимвольно из файла
        while ((i = getc(fp))!=EOF)
        {
            *c = i;
            c++;
        }
        fclose(fp);
        // вывод на консоль загруженной структуры
        printf("%-20c %5d \n", ptr->name, ptr->age, ptr->marks);
        free(ptr);
   return 0;
}
*/
/*Создать массив структур содержащих информацию о студентах. Поля : ФИО, номер курса, номер группы, целочисленный массив из 5 оценок.
Записать в файл информацию о студентах.
Написать функцию которая выберет из файла студентов со средним балом не менее 7. Вывести на экран.
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

struct student {
    char name[15];
    int course;
    int group;
    int marks[5];
};

int main() {
    int n;
    printf("Enter amount of persons: ");
    scanf("%d", &n);

    struct student* student_ptr = (struct student*)malloc(n* sizeof(struct student));
    for (int i = 0; i < n; i++) {
        printf("Enter student name: ");
        fgets(student_ptr[i].name, 15, stdin);
        fgets(student_ptr[i].name, 15, stdin);
        printf("Enter student course: ");
        scanf("%d", &student_ptr[i].course);
        printf("Enter student course: ");
        scanf("%d", &student_ptr[i].group);
        printf("Enter student marks: \n");
        for (int l = 0; l < 5; l++) {
            scanf("%d ", &student_ptr[i].marks[l]);
        }
    }

    // Запись в файл
    const char* filename = "file.dat";
    FILE* fp;
    char* c;
    int size = n * sizeof(struct student);

    fp = fopen(filename, "wb");

    c = (char*)student_ptr;
    for (int i = 0; i < size; i++) {
        putc(*c, fp);
        c++;
    }
    if (fp != NULL) fclose(fp);

    // Чтение из файла
    int j;

    fp = fopen(filename, "rb");
    struct student* ptr = (struct student*)malloc(n *sizeof(struct student));
    c = (char*)ptr;
    while ((j = getc(fp)) != EOF) {
        *c = j;
        c++;
    }

    int counter = 0;
    float mid = 0;
    for (int k = 0; k < n; k++)
    {
        for (int x = 0; x < 5; x++) {
            mid += ptr[k].marks[x];
        }

        if ((mid / 5) > 7) {
            printf("%d - %s", k + 1, ptr[k].name);
            counter++;
        }

        mid = 0;
    }

    printf("\n%d people in the file stored, have mid mark greater than 7\n\n", counter);

    if (fp != NULL) fclose(fp);
    free(ptr);
    free(student_ptr);
    return 0;
}*/





//билет 10 Отсортировать бинарный файл целых длинных чисел методом вставок
/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>  
int  main()
{
FILE *f;
 int i,j;
 long i1,i2, kol;
 f=fopen ("ccc.bin","r+b");
 rewind(f);
 kol=filelength(fileno(f))/sizeof(long int);
 for (i=1;i<kol;i++)
 {fseek (f,i*sizeof(long int),0);
  fread (&i1,sizeof(long int),1,f);
  j=i-1;
  fseek (f,j*sizeof(long int),0);
  fread (&i2,sizeof(long int),1,f);
  while (j>=0 && i1<i2)
  {fseek (f,(j+1)*sizeof(long int),0);
   fwrite (&i2,sizeof(long int),1,f);
   j--;
   fseek (f,j*sizeof(long int),0);
   fread (&i2,sizeof(long int),1,f);
  }
  fseek (f,(j+1)*sizeof(long int),0);
  fwrite(&i1,sizeof(long int),1,f);
 }
 fclose(f);
}*/
/*#include <stdio.h>
 
 
int main(void)
{
    FILE *ptr_file;
    char ch;        // читаемый символ из файла
    int length_word;    // длина слова (набра ascii символов состоящих только из букв без пробелов и тире)
    int max_length;     // максимальная длина слова
    int begin_word; // флаг начала слова
    int seek_max_word;  // смещение от начала файла, максимально длинного слова
    int i;
 
    ptr_file=fopen("data.txt","r");
    if(ptr_file == NULL)
       {
          perror("Error in opening file");
          return(-1);
       }
 
    length_word = seek_max_word = max_length = 0;
 
    while (!(feof(ptr_file)) )
       {
 
        ch=fgetc(ptr_file);
 
            if( ((ch >= 'A')&&(ch <= 'Z')) || ((ch >= 'a')&&(ch <= 'z')) )  // проверка начала слова и установка или сброс флага
            begin_word = 1;
            else
            begin_word = 0;
 
            if(begin_word)      // если слово началось, то считаем сивволы
            length_word+=1;
            else if((begin_word == 0)&&(length_word != 0)&&(length_word >= max_length))   // будет найдено посленее слово максимальной длины
            {           // если слово закончилось, то сохраняем его длину и смекщение в файле
              max_length = length_word;
              seek_max_word = ftell(ptr_file) - max_length - 1;
              length_word = 0;
            }
           
            else
            length_word = 0;
 
       }
 
    fseek(ptr_file, seek_max_word, SEEK_SET);   // устанавливаем нужное смещение
 
    for(i = 0; i < max_length; i++)     // читаем нужное слово по символам
    printf("%c", fgetc(ptr_file));
    
    if(begin_word)      // если слово началось, то считаем сивволы
    length_word+=1;
    else if((begin_word == 0)&&(length_word != 0)&&(length_word >= max_length))   // будет найдено посленее слово максимальной длины
    {           // если слово закончилось, то сохраняем его длину и смекщение в файле
      max_length = length_word;
      seek_max_word = ftell(ptr_file) - max_length - 1;
      length_word = 0;
    }
   
    else
    length_word = 0;

}

fseek(ptr_file, seek_max_word, SEEK_SET);   // устанавливаем нужное смещение

for(i = 0; i < max_length; i++)     // читаем нужное слово по символам
printf("%c", fgetc(ptr_file));
 
 
    fclose(ptr_file);
 
    printf(" <--- max length of word is %d", max_length);  // выводим длинну максимального слова
 
    return (0);
}*/
/*

#include <stdlib.h>
#include <stdio.h>

int main() {
    char ch;
    char buff[30];
    int number = 0;
    FILE* f = fopen("Test.txt", "w+");
    if (f == NULL){
        printf("ERROR");
        return -1;
    }
    fclose(f);
    f = fopen("Text.txt", "r");
    fgets(buff, 30, f);
    fgets(buff, 30, f);
    fseek(f, i,SEEK_SET);
    while (!(feof(f)) )
       {
        ch=fgetc(f);
           if ((ch >= '0')&&(ch <= '9')){
               number = ch;
               printf("число %d есть в файле", number);
           }
       }
           fclose(f);
    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
 
int main(int argc, char* argv[])
{
 FILE* f = fopen(argv[1], "r");
 char num = 0, prev = 0;
 int counter = 0;
 while (!(feof(f)))
 {
  int ch = fscanf(f, "%c", &num);
  counter++;
  if (ch <= 0)
   break;
  if (num == '\n')
  {
   if (counter > 80)
   {
    printf("length of one of strings is more than 80 characters");
    return -1;
   }
   counter = 0;
  }
 }
 rewind(f);
 num = 0;
  while (!feof(f))
  {
   prev = num;
   int ch = fscanf(f, "%c", &num);
   if (ch <= 0)
    return 0;
   if (num >= '0' && num <= '9')
   {
    printf("%c", num);
    continue;
   }
   if (prev >= '0' && prev <= '9')
    printf(" ");
   if (num == '\n')
    puts("");
  }
}










/*int firstWord(FILE*);
void longWord(FILE*, int, int);
void shortWord(FILE*, int, int);


int main() {
  FILE* file;
  file = fopen("file.txt", "r");
  longWord(file, firstWord(file), 0);
  shortWord(file, firstWord(file), 0);
  return 0;
}

int firstWord(FILE* file) {
  char ch;
  int worldLen = 0;
  while ((ch = getc(file)) != ' ') {
    worldLen++;
  }

  return worldLen;
}

void longWord(FILE* file, int wordLen, int k) {
  char ch;
  int wordLenRes = 0;
  int startIndex = 0;
  fseek(file, k, SEEK_SET);
  while ((ch = getc(file)) != EOF) {
    if (ch != ' ') {
      wordLenRes++;
      k++;
    }
    if (ch == ' ') {
      if (wordLen < wordLenRes) {
        wordLen = wordLenRes;
        wordLenRes = 0;
        startIndex = k + 1;
        longWord(file, wordLen, k + 1);
      }
      else {
        wordLenRes = 0;
      }
    }
  }

  char* result = (char*)malloc(wordLen * sizeof(char));
  for (int i = startIndex; i < wordLen; i++) {
    fseek(file, i, SEEK_SET);
    result[i] = getc(file);
  }

  printf("The most long - %s", result);
}

void shortWord(FILE* file, int wordLen, int k) {
  char ch;
  int wordLenRes = 0;
  int startIndex = 0;
  fseek(file, k, SEEK_SET);
  while ((ch = getc(file)) != EOF) {
    if (ch != ' ') {
      wordLenRes++;
      k++;
    }
    if (ch == ' ') {
      if (wordLen > wordLenRes) {
        wordLen = wordLenRes;
        wordLenRes = 0;
        startIndex = k + 1;
        shortWord(file, wordLen, k + 1);
      }
      else {
        wordLenRes = 0;
      }
    }
  }

  char* result = (char*)malloc(wordLen * sizeof(char));
  for (int i = startIndex; i < wordLen; i++) {
    fseek(file, i, SEEK_SET);
    result[i] = getc(file);
  }

  printf("The most short - %s", result);
}
*/
/*#include <stdio.h>

int create_binary();
int read_binary();

int main(){
    create_binary();
    FILE *f;
    if(!(f = fopen("data.bin", "r+b"))) //Открываем бинарный файл
    {
        printf("\nНе удалось открытьь файл\n");
        return 0;
    }
    float s_a_ch = 0, s_a_ts= 0;
    int n, counter = 0;
    while(!feof(f)){
        fread(&n, sizeof(int), 1, f);
        s_a_ch +=n;
        counter++;
    }
    s_a_ch /= counter;
    counter = 0;
    fseek(f, 0, SEEK_SET);
    while(!feof(f)){
        fread(&n, 1, 1, f);
        s_a_ts +=n;
        counter++;
    }
    s_a_ts /= counter;
    printf("\nСреднее арифметическое чисел: %f; Среднее арифметическое цифр: %f\n", s_a_ch, s_a_ts);
    fseek(f, 0, SEEK_SET);
    fread(&n, sizeof(int), 1, f);
    int temp = n;
    while(!feof(f)){
        fread(&n, sizeof(int), 1, f);
        if(n < temp){
            fseek(f, (-2)*sizeof(int), SEEK_CUR);
            fwrite(&n, sizeof(int), 1, f);
            fwrite(&temp, sizeof(int), 1, f);
            fseek(f, (-3)*sizeof(int), SEEK_CUR);
            fread(&temp, sizeof(int), 1, f);
            continue;
        }
        temp = n;
    }
    fclose(f);
    read_binary();
    return 0;
}

int create_binary(){ //Для случая, если мы не успели создать бинарный файл
    FILE *f;
    if(!(f = fopen("data.bin", "wb"))) //Создаем бинарный файл
    {
        printf("\nНе удалось создать файл\n");
        return 0;
    }
    int n;
    while(scanf("%d", &n) && n != 990099){ //Записываем числа
        fwrite(&n, sizeof(int), 1, f);
    }
    fclose(f);
    printf("\nФайл был создан.\n");
    return 1;
}

int read_binary(){ //Читам файл
    FILE *f;
    if(!(f = fopen("data.bin", "rb")))
    {
        printf("\nНе удалось открыть файл\n");
        return 0;
    }
    fseek(f,0,SEEK_END);
    long END = ftell(f); //Узнаем кол-во битов
    int num;
    fseek(f,0,SEEK_SET);
    printf("\nРезультат чтения файла:\n");
    for(long i = 0; i < END/sizeof(int); i++){  //Кол-во битов делим на размер целочисленного числа и узнаем кол-во чисел
        fread(&num, sizeof(int), 1, f);
        printf("%d ", num);
    }
    printf("\n");
    return 1;
}
*/




//сортировка вставкой
/*#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE * f = fopen("bin.bin", "w+b");
    if (f == NULL){
        printf("EERROOROROROROROROR");
        return -1;
    }
    int k = 0;
    int arr[] = {1, 2, 4, 7667, 0, 245, 9};
    for(int i = 0; i<sizeof(arr)/sizeof(int); i++){
        fwrite(&arr[i], sizeof(int), 1, f);
        k++;
    }
    fseek(f, 0, SEEK_SET);
    int tmp = 0,artem =0;
    fread(&tmp, sizeof(int), 1, f);
    while(!feof(f)){
        fread(&artem, sizeof(int), 1, f);
        if(artem<tmp){
            fseek(f, -2*sizeof(int), SEEK_CUR);
            fwrite(&artem, sizeof(int), 1, f);
            fwrite(&tmp, sizeof(int), 1, f);
            fseek(f, -3*sizeof(int), SEEK_CUR);
            fread(&tmp, sizeof(int), 1, f);
        }
        else tmp = artem;
        
    }
    
    fseek(f, 0, SEEK_SET);
    for(int i = 0; i<k; i++){
        fread(&artem,sizeof(int), 1, f);
        printf("%d ", artem);
    }
    return 0;
    
}
*/


/*
//сортировка пузырьком
#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE * f = fopen("bin.bin", "w+b");
    if (f == NULL){
        printf("EERROOROROROROROROR");
        return -1;
    }
    int k = 0;
    int arr[] = {5, 6, 1, 77, 0, 25, 10, 32, 11, 255677, 43, 100, 69, 63, 25};
    for(int i = 0; i<sizeof(arr)/sizeof(int); i++){
        fwrite(&arr[i], sizeof(int), 1, f);
        k++;
    }
    int tmp = 0, artem =0;
    int vadimCar = 1;
    while(vadimCar){
        vadimCar = 0;
        fseek(f, 0, SEEK_SET);
        fread(&tmp, sizeof(int), 1, f);
          while(!feof(f)){
              fread(&artem, sizeof(int), 1, f);
              if(artem<tmp){
                  vadimCar = 1;
                  fseek(f, -2*sizeof(int), SEEK_CUR);
                  fwrite(&artem, sizeof(int), 1, f); // artem made the greatest algorithm of bubble sort ever!
                  fwrite(&tmp, sizeof(int), 1, f);
                  fseek(f, -1*sizeof(int), SEEK_CUR);
              }
              tmp = artem;
          }
          }
    fseek(f, 0, SEEK_SET);
    for(int i = 0; i<k; i++){
        fread(&artem,sizeof(int), 1, f);
        printf("%d ", artem);
    }
    printf("\n\n");
    return 0;
}*/
// есть структура пахари, внести данные пахарей(какой ранг у пахаря, был ли пахарь на сессии, оценки пахаря, если оценка пахаря меньше заданной, то удалить его из //пахарей
#include <stdlib.h>
#include <stdio.h>

int sort(FILE);

static const int maxRang = 4;
static const int maxNumberofMarks = 4;
struct paxar{
    char name[15];
    int rang;
};


int main(){
    int n;
    printf("Enter amount of persons: ");
    scanf("%d", &n);
    FILE *f = fopen("paxar.bin", "wb+");
    struct paxar *paxarPtr = (struct paxar*)calloc(n, sizeof(struct paxar));
    for(int i = 0; i< n; i++){
        fgets(paxarPtr[i].name, 15, f);
        scanf("%d", &paxarPtr[i].rang);
    }
    
    char* c;
    int size = n * sizeof(struct paxar);

    f = fopen("paxar.bin", "wb");

    c = (char*)paxarPtr;
    for (int i = 0; i < size; i++) {
        putc(*c, f);
        c++;
    }
    if (f != NULL) fclose(f);

    // Чтение из файла
    int j;

    f = fopen("paxar.bin", "rb");
    struct paxar* ptr = (struct paxar*)malloc(n *sizeof(struct paxar));
    c = (char*)ptr;
    while ((j = getc(f)) != EOF) {
        *c = j;
        c++;
    }
   
    if (f != NULL) fclose(f);
    free(ptr);
    free(paxarPtr);
    return 0;
}


int sort(FILE *f){
    for (int i = 0; i<15; i++){
        
    while(!feof(f)){
        fread(&artem, sizeof(int), 1, f);
        if(artem<tmp){
            fseek(f, -2*sizeof(int), SEEK_CUR);
            fwrite(&artem, sizeof(int), 1, f);
            fwrite(&tmp, sizeof(int), 1, f);
            fseek(f, -3*sizeof(int), SEEK_CUR);
            fread(&tmp, sizeof(int), 1, f);
        }
        else tmp = artem;
    }
    }
}
