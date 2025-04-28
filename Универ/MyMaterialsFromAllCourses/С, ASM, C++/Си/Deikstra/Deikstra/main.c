#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int minDistance(int m, int dist[], bool sptSet[]) //функция нахождения минимального расстояния
{
    // Инициализациия минимального значения
    int min = INT_MAX, min_index = 0;
  
    for (int v = 0; v < m; v++)
        if (sptSet[v] == false && dist[v] <= min)
            (min = dist[v]), min_index = v;
  
    return min_index;
}
  
// Вспомогательная функция для печати построенного массива расстояний
void printSolution(int m, int dist[])
{
    printf("Vertex \t\t Distance from Source\n");

    char *x = "ABCDE";
    for (int i = 0; i < m; i++)
        printf("A-%c \t\t %d\n", x[i], dist[i]);
}
  
// Функция, реализующая алгоритм кратчайшего пути из одной вершины.
// для графа, представленного матрицей
void dijkstra(int n, int m, int **graph, int src)
{
    int dist[n]; // выходной массив.  dist[i] будет содержать кратчайший путь
    // расстояние от источника до i
  
    bool sptSet[n]; /*sptSet[i] будет истинным, если вершина i включена в дерево
    кратчайших путей или кратчайшее расстояние от src до i завершено*/
  
    // Инициализация все расстояния как бесконечность и stpSet[] как false
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
  
    // Расстояние исходной вершины от самой себя всегда равно 0
    dist[src] = 0;
  
    // Поиск кратчайшего пути для всех вершин
    for (int count = 0; count < n - 1; count++) {
       // Выбираем вершину минимального расстояния из набора еще не обработанных вершин. u всегда равно src в первой итерации.
        int u = minDistance(m, dist, sptSet);
  
        // Помечается выбранную вершину как обработанную
        sptSet[u] = true;
        // Обновление значения расстояния смежных вершин выбранной вершины.
        for (int v = 0; v < m; v++)
  
            /* Обновляется dist[v] только в том случае, если его нет в sptSet, есть ребро от u до v и общий вес пути от src до v через u меньше текущего значения dist[v]*/
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
  
    // вывод построенного массива расстояний
    printSolution(m, dist);
   /*{0,2,1000000,2,1000000},
    {2,0,4,1000000,1000000},                //данные значения вводились в консоль
    {1000000,4,0,2,1},
    {2,1000000,2,0,7},
    {1000000,1000000,1,7,0}*/
}
 
// A-0,B-1,C-2,D-3,E-4
int main() {
    int n, m;
    int **matrix; // указатель на указатель на строку элементов
    printf("Введите количество строк: ");
      scanf("%d", &n);
      printf("Введите количество столбцов: ");
      scanf("%d", &m);
      // Выделение памяти под указатели на строки
      matrix = (int**)malloc(n * sizeof(int*));
      // Ввод элементов массива
      for (int i = 0; i<n; i++)  // цикл по строкам
      {
        // Выделение памяти под хранение строк
        matrix[i] = (int*)malloc(m * sizeof(int));
        for (int j = 0; j<m; j++)  // цикл по столбцам
        {
          printf("matrix[%d][%d] = ", i, j);
          scanf("%d", &matrix[i][j]);
        }
      }
    // Вывод элементов массива
      for (int i = 0; i < n; i++)  // цикл по строкам
      {
        for (int j = 0; j < m; j++)  // цикл по столбцам
        {
          printf("%5d ", matrix[i][j]); // 5 знакомест под элемент массива
        }
        printf("\n");
      }
    dijkstra(n, m, matrix, 0);//0 - вершина которую надо найти A-0,B-1,C-2,D-3,E-4
    for (int i = 0; i < n; i++)  // цикл по строкам
        free(matrix[i]);   // освобождение памяти под строку
      free(matrix);
    return 0;
}















/*#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
 
#define V 5
  
int minDistance(int m, int dist[], bool sptSet[]) //функция нахождения минимального расстояния
{
    // Инициализациия минимального значения
    int min = INT_MAX, min_index;
  
    for (int v = 0; v < m; v++)
        if (sptSet[m] == false && dist[m] <= min)
            min = dist[m], min_index = m;
  
    return min_index;
}
  
// Вспомогательная функция для печати построенного массива расстояний
void printSolution(int m, int dist[])
{
    printf("Vertex \t\t Distance from Source\n");

    char *x = "ABCDE";
    for (int i = 0; i < m; i++)
        printf("A-%c \t\t %d\n", x[i], dist[i]);
}
  
// Функция, реализующая алгоритм кратчайшего пути из одного источника Дейкстры.
// для графа, представленного с использованием представления матрицы смежности
void dijkstra(int n, int m, int graph[n][m], int src)
{
    int dist[n]; // выходной массив.  dist[i] будет содержать кратчайший путь
    // расстояние от источника до i
  
    bool sptSet[n]; sptSet[i] будет истинным, если вершина i включена в дерево
    кратчайших путей или кратчайшее расстояние от src до i завершено
  
    // Инициализация все расстояния как бесконечность и stpSet[] как false
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
  
    // Расстояние исходной вершины от самой себя всегда равно 0
    dist[src] = 0;
  
    // Поиск кратчайшего пути для всех вершин
    for (int count = 0; count < n - 1; count++) {
       // Выбираем вершину минимального расстояния из набора еще не обработанных вершин. u всегда равно src в первой итерации.
        int u = minDistance(m, dist, sptSet);
  
        // Пометить выбранную вершину как обработанную
        sptSet[u] = true;
  
        // Обновления значения расстояния смежных вершин выбранной вершины.
        for (int v = 0; v < m; v++)
  
            Обновлять dist[v] только в том случае, если его нет в sptSet, есть ребро от u до v и общий вес пути от src до v через u меньше текущего значения dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
  
    // вывод построенного массива расстояний
    printSolution(m, dist);
}
 
// A-0,B-1,C-2,D-3,E-4
int main() {
    int n, m;
    int **matrix; // указатель на указатель на строку элементов
    printf("Введите количество строк: ");
      scanf("%d", &n);
      printf("Введите количество столбцов: ");
      scanf("%d", &m);
      // Выделение памяти под указатели на строки
      matrix = (int**)malloc(n * sizeof(int*));
      // Ввод элементов массива
      for (int i = 0; i<n; i++)  // цикл по строкам
      {
        // Выделение памяти под хранение строк
        matrix[i] = (int*)malloc(m * sizeof(int));
        for (int j = 0; j<m; j++)  // цикл по столбцам
        {
          printf("matrix[%d][%d] = ", i, j);
          scanf("%d", &matrix[i][j]);
        }
      }
    // Вывод элементов массива
      for (int i = 0; i < n; i++)  // цикл по строкам
      {
        for (int j = 0; j < m; j++)  // цикл по столбцам
        {
          printf("%5d ", matrix[i][j]); // 5 знакомест под элемент массива
        }
        printf("\n");
      }
    
    int matrix[V][V] = {
        {0,2,1000000,2,1000000},
        {2,0,4,1000000,1000000},
        {1000000,4,0,2,1},
        {2,1000000,2,0,7},
        {1000000,1000000,1,7,0},
    };
    dijkstra(n, m, matrix, 0);//0 - вершина которую надо найти A-0,B-1,C-2,D-3,E-4
    for (int i = 0; i < n; i++)  // цикл по строкам
        free(matrix[i]);   // освобождение памяти под строку
      free(matrix);
    return 0;
}
*/











/*#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
 
#define V 5
  
int minDistance(int dist[], bool sptSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;
  
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
  
    return min_index;
}
  
// A utility function to print the constructed distance array
void printSolution(int dist[])
{
    printf("Vertex \t\t Distance from Source\n");

    char *x = "ABCDE";
    for (int i = 0; i < V; i++)
        printf("A-%c \t\t %d\n", x[i], dist[i]);
}
  
// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int graph[V][V], int src)
{
    int dist[V]; // The output array.  dist[i] will hold the shortest
    // distance from src to i
  
    bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized
  
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
  
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
  
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet);
  
        // Mark the picked vertex as processed
        sptSet[u] = true;
  
        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++)
  
            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
  
    // print the constructed distance array
    printSolution(dist);
}
 
// A-0,B-1,C-2,D-3,E-4
int main() {
    int matrix[V][V] = {
        {0,2,1000000,2,1000000},
        {2,0,4,1000000,1000000},
        {1000000,4,0,2,1},
        {2,1000000,2,0,7},
        {1000000,1000000,1,7,0},
    };
    dijkstra(matrix, 0);
}
*/
