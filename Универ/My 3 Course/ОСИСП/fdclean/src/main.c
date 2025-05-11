#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>

#define CREATE_ERROR -3      // Ошибка создания lock файла
#define FILE_IS_CLOSED -1    // Флаг закрытия файла
#define HASH_SIZE 33         // Длина строки для MD5 хеша
#define MESSAGE_BUF_SIZE 700 // Размер буфера для сообщений

#ifndef LOG_FILE
#define LOG_FILE "/home/artem/.cache/fdclean/fdclean.log" // Путь к лог-файлу
#endif

#ifndef LOCK_FILE
#define LOCK_FILE "/home/artem/.cache/fdclean/fdclean.lock" // Путь к файлу блокировки
#endif

#ifndef BIN_FILE
#define BIN_FILE "/home/artem/.cache/fdclean/fdclear.bin" // Путь к бинарному файлу
#endif

#ifndef DEFAULT_DIR
#define DEFAULT_DIR "/home/artem/" // Директория по умолчанию
#endif

#ifndef SCAN_DELAY_SEC
#define SCAN_DELAY_SEC 30 // Задержка до следующего сканирования
#endif

int lock_fd = FILE_IS_CLOSED; // Дескриптор файла блокировки

typedef struct FileInfo // Структура для хранения информации о файле
{
    off_t pathLength;     // Длинна пути к файлу
    char *path;           // Путь к файлу
    off_t size;           // Размер файла
    char hash[HASH_SIZE]; // MD5 хеш
} FileInfo;

void log_message(const char *, const char *);
int check_same_inode(const char *path1, const char *path2);
void write_file_info(const FileInfo *);
char *find_duplicate(const FileInfo *);
void compute_md5(const char *, char *);
void scan_directory(const char *);
void remove_files();
void signal_handler(int);
void daemonize();
int can_create_lock_file();
int create_lock_file();
void handle_kill();
void clean_bin_file();

// Основная функция
int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "-kill") == 0)
        {
            handle_kill();
        }
    }

    if (can_create_lock_file() == CREATE_ERROR)
    {
        return EXIT_FAILURE;
    }

    const char *directory = (argc > 1) ? argv[1] : DEFAULT_DIR;

    // Устанавливаем обработчик сигнала
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGABRT, signal_handler);

    daemonize(); // Переводим программу в фоновый режим

    lock_fd = create_lock_file();
    if (lock_fd < 0)
    {
        remove_files();
        return EXIT_FAILURE;
    }

    char message_start[MESSAGE_BUF_SIZE];
    char message_end[MESSAGE_BUF_SIZE];
    snprintf(message_start, MESSAGE_BUF_SIZE, "Начало сканирования директории %s", directory);
    snprintf(message_end, MESSAGE_BUF_SIZE, "Директория %s была успешно отсканирована, следующее сканинование через %d сек", directory, SCAN_DELAY_SEC);
    while (1)
    {

        log_message("INFO", message_start);
        clean_bin_file();
        scan_directory(directory);
        log_message("INFO", message_end);
        sleep(SCAN_DELAY_SEC);
    }

    remove_files();
    return 0;
}

// Функция для логирования
void log_message(const char *type, const char *message)
{
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file)
    {
        syslog(LOG_ERR, "Ошибка открытия лог-файла");
        remove_files();
        exit(EXIT_FAILURE);
    }

    // Получаем текущее время для записи в лог
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Удаляем символ новой строки

    // Записываем сообщение в лог с указанием типа (INFO или ERROR)
    fprintf(log_file, "[%s] [%s] %s\n", time_str, type, message);
    fclose(log_file);
}

int check_same_inode(const char *path1, const char *path2)
{
    struct stat stat1, stat2;

    // Получаем информацию о первом файле
    if (stat(path1, &stat1) == -1)
    {
        char error_message[MESSAGE_BUF_SIZE];
        snprintf(error_message, MESSAGE_BUF_SIZE, "Ошибка получения информации о файле %s: %s", path1, strerror(errno));
        log_message("ERROR", error_message);
        remove_files();
        exit(EXIT_FAILURE);
    }

    // Получаем информацию о втором файле
    if (stat(path2, &stat2) == -1)
    {
        char error_message[MESSAGE_BUF_SIZE];
        snprintf(error_message, MESSAGE_BUF_SIZE, "Ошибка получения информации о файле %s: %s", path2, strerror(errno));
        log_message("ERROR", error_message);
        remove_files();
        exit(EXIT_FAILURE);
    }
    // Сравниваем номера inode
    return (stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev);
}

// Функция для записи информации о файле в бинарный файл
void write_file_info(const FileInfo *file_info)
{
    FILE *file = fopen(BIN_FILE, "ab"); // Открываем файл в режиме добавления (бинарный)
    if (!file)
    {
        log_message("ERROR", "Ошибка чтения бинарного файла");
        remove_files();
        exit(EXIT_FAILURE);
    }

    // Записываем данные
    fwrite(&file_info->pathLength, sizeof(file_info->pathLength), 1, file);
    fwrite(file_info->path, 1, file_info->pathLength, file);
    fwrite(&file_info->size, sizeof(file_info->size), 1, file);
    fwrite(file_info->hash, 1, HASH_SIZE, file);

    fclose(file);
}

// Функция для удаления информации о файле из бинарного файла по пути
void delete_info_by_path(const char *search_path)
{
    FILE *file = fopen(BIN_FILE, "rb");
    if (!file)
    {
        log_message("ERROR", "Ошибка открытия бинарного файла для чтения");
        remove_files();
        exit(EXIT_FAILURE);
    }

    // Создаем временный файл для записи обновленного содержимого
    FILE *temp_file = fopen(BIN_FILE ".tmp", "wb");
    if (!temp_file)
    {
        log_message("ERROR", "Ошибка открытия временного файла для записи");
        fclose(file);
        remove_files();
        exit(EXIT_FAILURE);
    }

    size_t path_length;
    char *path;
    int found = 0;

    while (fread(&path_length, sizeof(path_length), 1, file))
    {
        path = malloc(path_length);
        if (!path)
        {
            log_message("ERROR", "Ошибка выделения памяти");
            fclose(file);
            fclose(temp_file);
            remove_files();
            exit(EXIT_FAILURE);
        }

        fread(path, 1, path_length, file);
        off_t file_size;
        fread(&file_size, sizeof(file_size), 1, file);
        char file_hash[HASH_SIZE];
        fread(file_hash, 1, HASH_SIZE, file);

        // Записываем в временный файл, если путь не соответствует искомому
        if (strcmp(path, search_path) != 0)
        {
            fwrite(&path_length, sizeof(path_length), 1, temp_file);
            fwrite(path, 1, path_length, temp_file);
            fwrite(&file_size, sizeof(file_size), 1, temp_file);
            fwrite(file_hash, 1, HASH_SIZE, temp_file);
        }
        else
        {
            found = 1; // Помечаем, что удаление произошло
        }

        free(path); // Освобождаем выделенную память для пути
    }

    fclose(file);
    fclose(temp_file);

    if (remove(BIN_FILE) != 0)
    {
        log_message("ERROR", "Ошибка удаления оригинального бинарного файла");
        return;
    }

    if (rename(BIN_FILE ".tmp", BIN_FILE) != 0)
    {
        log_message("ERROR", "Ошибка переименования временного файла");
        return;
    }

    if (found)
    {
        // Логируем успешное удаление данных о файле
        char message[MESSAGE_BUF_SIZE];
        snprintf(message, MESSAGE_BUF_SIZE, "Успешное удаление данных о файле с путем %s", search_path);
        log_message("INFO", message);
    }
}

// Функция для проверки на дубликаты перед записью
char *find_duplicate(const FileInfo *new_file_info)
{
    FILE *file = fopen(BIN_FILE, "rb");
    if (!file)
    {
        log_message("ERROR", "Ошибка открытия файла для чтения");
        remove_files();
        exit(EXIT_FAILURE);
    }

    FileInfo file_info;
    while (fread(&file_info.pathLength, sizeof(file_info.pathLength), 1, file))
    {
        file_info.path = malloc(file_info.pathLength);
        if (!file_info.path)
        {
            fclose(file);
            log_message("ERROR", "Ошибка выделения памяти");
            remove_files();
            exit(EXIT_FAILURE);
        }

        fread(file_info.path, 1, file_info.pathLength, file);
        fread(&file_info.size, sizeof(file_info.size), 1, file);
        fread(file_info.hash, 1, HASH_SIZE, file);

        if (file_info.size == new_file_info->size &&
            memcmp(file_info.hash, new_file_info->hash, HASH_SIZE) == 0)
        {
            fclose(file);
            return file_info.path; // Возвращаем путь к дубликату
        }

        free(file_info.path);
    }
    fclose(file);
    return NULL; // Нет дубликата
}

// Функция для вычисления MD5 хеша файла
void compute_md5(const char *path, char *hash_str)
{
    unsigned char hash[EVP_MAX_MD_SIZE]; // Буфер для хеша
    unsigned int hash_len;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

    FILE *file = fopen(path, "rb");
    if (!file)
    {
        char message[MESSAGE_BUF_SIZE];
        snprintf(message, MESSAGE_BUF_SIZE, "Ошибка открытия файла для хеширования: %s", path);
        log_message("ERROR", message);
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        remove_files();
        exit(EXIT_FAILURE);
    }

    if (EVP_DigestInit_ex(mdctx, EVP_md5(), NULL) != 1)
    {
        log_message("ERROR", "Ошибка инициализации MD5");
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        remove_files();
        exit(EXIT_FAILURE);
        return;
    }

    char buf[MESSAGE_BUF_SIZE];
    int bytes;
    while ((bytes = fread(buf, 1, MESSAGE_BUF_SIZE, file)) > 0)
    {
        EVP_DigestUpdate(mdctx, buf, bytes);
    }
    fclose(file);

    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    for (unsigned int i = 0; i < hash_len; i++)
    {
        sprintf(&hash_str[i * 2], "%02x", hash[i]);
    }
    hash_str[HASH_SIZE - 1] = '\0'; // Завершающий ноль
}

// Функция для рекурсивного обхода директории и записи уникальных файлов
void scan_directory(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        fprintf(stderr, "fdclean: Ошибка открытия директории: %s", dir_path);
        remove_files();
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Динамическое выделение памяти для пути
        size_t path_length = strlen(dir_path) + strlen(entry->d_name) + 2; // +2 для '/' и '\0'
        char *path = malloc(path_length);
        if (!path)
        {
            fprintf(stderr, "fdclean: Ошибка выделения памяти");
            closedir(dir);
            remove_files();
            exit(EXIT_FAILURE);
        }

        snprintf(path, path_length, "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == 0)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                scan_directory(path); // Рекурсивно сканируем директорию
            }
            else if (S_ISREG(statbuf.st_mode))
            {
                char hash[HASH_SIZE];
                compute_md5(path, hash);

                FileInfo file_info;
                file_info.pathLength = strlen(path) + 1;
                file_info.path = malloc(file_info.pathLength);
                if (!file_info.path)
                {
                    fprintf(stderr, "fdclean: Ошибка выделения памяти");
                    free(path);
                    remove_files();
                    exit(EXIT_FAILURE);
                }
                strcpy(file_info.path, path);
                file_info.size = statbuf.st_size;
                memcpy(file_info.hash, hash, HASH_SIZE);
                char *file_duplicate_path = find_duplicate(&file_info);
                // Проверка на дубликаты
                if (file_duplicate_path == NULL)
                {
                    write_file_info(&file_info); // Записываем информацию о файле
                }
                else
                {
                    if (!check_same_inode(file_duplicate_path, file_info.path))
                    {
                        // Удаляем дубликат
                        if (unlink(file_info.path) == -1)
                        {
                            fprintf(stderr, "fdclean: Ошибка удаления дубликата %s: %s", file_info.path, strerror(errno));
                            remove_files();
                            exit(EXIT_FAILURE);
                        }
                        else
                        {
                            // Создаем жесткую ссылку на основной файл
                            if (link(file_duplicate_path, file_info.path) == -1)
                            {
                                fprintf(stderr, "fdclean: Ошибка создания жесткой ссылки на %s: %s", file_info.path, strerror(errno));
                                remove_files();
                                exit(EXIT_FAILURE);
                            }
                            else
                            {
                                char message[MESSAGE_BUF_SIZE * 2];
                                snprintf(message, MESSAGE_BUF_SIZE * 2, "Замена дубликата: удален %s и создана жесткая ссылка на %s", file_info.path, file_duplicate_path);
                                log_message("INFO", message);
                            }
                        }
                    }

                    free(file_duplicate_path); // Освобождаем выделенную память
                }

                free(file_info.path); // Освобождаем выделенную память
            }
        }

        free(path); // Освобождаем выделенную память
    }
    closedir(dir);
}

// Функция для удаления файла блокировки
void remove_files()
{
    if (lock_fd != FILE_IS_CLOSED)
    {
        close(lock_fd);
    }
    unlink(LOCK_FILE);
    unlink(BIN_FILE);
}

// Обработчик сигнала завершения демона
void signal_handler(int signal)
{
    switch (signal)
    {
    case SIGINT:
        log_message("INFO", "Получен сигнал SIGINT (Ctrl+C), завершение работы");
        remove_files();
        exit(EXIT_SUCCESS);
        break;
    case SIGTERM:
        log_message("INFO", "Получен сигнал SIGTERM, завершение работы");
        remove_files();
        exit(EXIT_SUCCESS);
        break;
    case SIGSEGV:
        log_message("ERROR", "Ошибка сегментации (SIGSEGV). Завершение работы программы.");
        remove_files();
        exit(EXIT_FAILURE);
        break;
    case SIGABRT:
        log_message("ERROR", "Принудительный сброс (SIGABRT). Завершение работы программы.");
        remove_files();
        exit(EXIT_FAILURE);
        break;
    default:
        break;
    }
}

// Функция для создания демона
void daemonize()
{

    pid_t pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS); // Родительский процесс завершает выполнение
    }

    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Перекрываем сигналы SIGHUP и SIGCHLD
    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // fork для защиты от повторной инициализации терминала
    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS); // Родительский процесс завершает выполнение
    }

    // Закрываем все файловые дескрипторы
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Открываем стандартные файловые дескрипторы на /dev/null
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_RDWR);   // stdout
    open("/dev/null", O_RDWR);   // stderr
}

// Функция для проверки, можно ли создать файл блокировки
int can_create_lock_file()
{
    int fd = open(LOCK_FILE, O_CREAT | O_EXCL | O_WRONLY, 0600);
    if (fd < 0)
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "fdclean: Процесс уже запущен.\n");
        }
        else if (errno == EACCES)
        {
            fprintf(stderr, "fdclean: Недостаточно прав для создания .lock файла.\n");
        }
        else if (errno == EROFS)
        {
            fprintf(stderr, "fdclean: Файловая система доступна только для чтения.\n");
        }
        else
        {
            fprintf(stderr, "fdclean: Ошибка создания .lock файла: ");
            perror(NULL); // Выводит описание ошибки, основанное на значении errno
        }
        return CREATE_ERROR;
    }
    close(fd); // Закрываем файл, т.к. проверка пройдена
    return 0;  // Успешная проверка
}

// Функция для создания файла блокировки и записи PID
int create_lock_file()
{
    int fd = open(LOCK_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
    {
        perror("fdclean: Ошибка открытия .lock файла для записи.");
        return CREATE_ERROR;
    }

    // Записываем PID в файл блокировки
    char pid_str[20];

    snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    if (write(fd, pid_str, strlen(pid_str)) != strlen(pid_str))
    {
        perror("fdclean: Ошибка записи PID в .lock файл.");
        close(fd);
        return CREATE_ERROR;
    }

    return fd; // Возвращаем дескриптор файла блокировки
}

void handle_kill()
{
    FILE *lock_file = fopen(LOCK_FILE, "r");
    if (lock_file == NULL)
    {
        fprintf(stderr, "fdclean: Процесс не запущен.\n");
        exit(EXIT_FAILURE);
    }

    int pid;
    if (fscanf(lock_file, "%d", &pid) != 1)
    {
        fprintf(stderr, "fdclean: Ошибка чтения PID из файла блокировки.\n");
        fclose(lock_file);
        exit(EXIT_FAILURE);
    }
    fclose(lock_file);

    // Отправляем сигнал SIGTERM процессу
    if (kill(pid, SIGTERM) != 0)
    {
        if (errno == ESRCH)
        {
            fprintf(stderr, "fdclean: Процесс не найден.\n", pid);
        }
        else
        {
            perror("fdclean: Ошибка при попытке остановить процесс");
        }
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void clean_bin_file()
{
    FILE *file = fopen(BIN_FILE, "wb");
    if (!file)
    {
        log_message("ERROR", "fdclean: Ошибка открытия бинарного файла");
        remove_files();
        exit(EXIT_FAILURE);
    }
    fclose(file);
}
