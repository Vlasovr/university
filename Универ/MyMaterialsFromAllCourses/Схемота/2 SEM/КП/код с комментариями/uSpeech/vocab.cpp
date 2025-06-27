// Определение макроса uspeech_h
#define uspeech_h

// Подключение заголовочного файла Arduino.h для доступа к функциям платформы Arduino
#include "Arduino.h"
// Подключение библиотеки для работы со строками
#include <string.h>
// Подключение математической библиотеки для использования математических функций
#include <math.h>
// Подключение стандартной библиотеки для общего функционала
#include <stdlib.h>

// Определение константы SILENCE (порог тишины)
#define SILENCE 2000
// Определение константы F_DETECTION для обнаружения фонемы /f/
#define F_DETECTION 3
// Определение константы F_CONSTANT (порог для /f/)
#define F_CONSTANT 350
// Определение константы MAX_PLOSIVETIME (максимальное время для определения взрывного звука)
#define MAX_PLOSIVETIME 1000
// Определение константы PROCESS_SKEWNESS_TIME (период анализа данных)
#define PROCESS_SKEWNESS_TIME 15

/**
 * Класс для распознавания звука
 */
class signal {
public:
    // Объявление массива для хранения аудиоданных (буфер из 32 выборок)
    int arr[32];
    // Объявление переменной для средней мощности сигнала
    int avgPower;
    // Объявление тестового коэффициента для отладки
    int testCoeff;
    // Объявление минимального значения громкости, после которого сигнал считается готовым к распознанию
    int minVolume;
    // Объявление порога для распознавания фонемы /f/ (настраиваемый)
    int fconstant;
    // Объявление порога для распознавания фонем, таких как /ee/ или /i/
    int econstant;
    // Объявление порога для распознавания фонем, таких как /a/, /o/, /r/ или /l/
    int aconstant;
    // Объявление порога для распознавания фонем, таких как /z/, /v/ или /w/
    int vconstant;
    // Объявление порога для распознавания фонем, таких как /sh/ или /ch/ (значения выше – фонема /s/)
    int shconstant;
    // Объявление флага, разрешающего распознавание фонемы /f/
    bool f_enabled;
    // Объявление коэффициента усиления для регулировки чувствительности
    int amplificationFactor;
    // Объявление порога мощности микрофона (значения ниже игнорируются)
    int micPowerThreshold;
    // Объявление масштабного коэффициента для входного сигнала
    int scale;
    // Объявление переменной для хранения распознанной фонемы (символ)
    char phoneme;
    // Объявление конструктора класса, принимающего номер порта
    signal(int port);
    // Объявление переменной для хранения мощности микрофона
    int micPower;
    // Объявление метода для выборки звука
    void sample();
    // Объявление метода для определения максимальной мощности сигнала
    unsigned int maxPower();
    // Объявление метода для вычисления общей мощности сигнала
    unsigned int power();
    // Объявление метода для вычисления отношения сигнал/шум (SNR)
    int snr(int power);
    // Объявление метода для калибровки микрофона (определение фонового уровня)
    void calibrate();
    // Объявление метода для распознавания фонемы (возврат символа)
    char getPhoneme();
    // Объявление переменной для хранения значения калибровки (средний уровень фонового шума)
    int calib;
private:
    // Объявление переменной для хранения номера порта, к которому подключён микрофон
    int pin;
    // Объявление переменной для хранения времени (миллисекунды) – может использоваться для отладки или тайминга
    int mil;
    // Объявление переменной для хранения позиции в массиве с максимальной амплитудой
    int maxPos;
    // Объявление флага, указывающего на наличие тишины
    bool silence;
    // Объявление массива для хранения истории коэффициентов (используемого для фильтрации шума)
    unsigned int overview[7];
    // Объявление приватного метода для вычисления «сложности» сигнала (отношение суммы модулей производных к мощности)
    unsigned int complexity(int power);
};

/**
 * Класс для накопления статистических показателей (возможно удаление, если асимметрия будет работать корректно)
 */
class statCollector {
public:
    // Объявление переменных для количества выборок, среднего значения и моментов 2-го, 3-го и 4-го порядка
    int n, mean, M2, M3, M4;
    // Объявление конструктора класса
    statCollector();
    // Объявление метода для вычисления куртоза (остроты распределения)
    int kurtosis();
    // Объявление метода для вычисления асимметрии распределения
    int skew();
    // Объявление метода для получения среднего значения
    int _mean();
    // Объявление метода для получения стандартного отклонения (возвращается M2)
    int stdev();
    // Объявление метода для накопления статистических данных (обновление моментов) с новым значением x
    void collect(int x);
};

/**
 * Класс для аккумуляции фонем (слогов), полезен для базового распознавания слов
 */
class syllable {
public:
    // Объявление аккумуляторов для счёта появлений фонем: f, e, o, s, h, v
    int f, e, o, s, h, v;
    // Объявление переменных для хранения максимальных позиций (или показателей) для каждой фонемы
    int maxf, maxe, maxo, maxs, maxh, maxv;
    // Объявление переменных для модальности каждой фонемы (индикатор наличия двух пиков в распределении)
    int modalityf, modalitye, modalityo, modalitys, modalityh, modalityv;
    // Объявление переменной для хранения длины произнесённого слога (количество фонем)
    int length;
    // Объявление счётчика взрывных звуков (плозивов)
    int plosiveCount;
    // Объявление конструктора класса syllable
    syllable();
    // Объявление метода для сброса аккумуляторов (вызывается при обнаружении тишины)
    void reset();
    // Объявление метода для классификации символа (фонемы) с обновлением аккумуляторов в зависимости от входного символа
    void classify(char c);
    // Объявление метода для возврата аккумуляторов в виде указателя на массив int
    int* tointptr();
    // Объявление метода для отладочного вывода накопленных данных на Arduino
    void debugPrint();
    // Объявление метода для расчёта «расстояния» (схожести) между двумя слогами
    void distance(syllable s);
private:
    // Объявление временных аккумуляторов для промежуточного подсчёта фонем
    char cf, ce, co, cs, ch, cv;
    // Объявление переменных для хранения предыдущих значений временных аккумуляторов (для вычисления пиков и модальностей)
    char prevf, preve, prevo, prevs, prevh, prevv;
    // Объявление переменной для хранения текущего пика (фонемы) и флага ожидания пробела (для разделения слогов)
    char currPeak, expectSp;
    // Объявление переменной для хранения времени последнего обновления (определение интервала между слогами)
    unsigned long lastTime;
};

#include "uspeech.h"

// Реализация метода распознавания фонемы getPhoneme класса signal (занимает 1-4 мс)
char signal::getPhoneme() {
    // Выполнение выборки звуковых данных
    sample();
    // Вычисление суммарной мощности (энергии) сигнала
    unsigned int pp = power();

    // Сравнение вычисленной мощности с пороговым значением тишины
    if (pp > SILENCE) {
        // Вычисление "сложности" сигнала
        int k = complexity(pp);
        // Сдвиг значений в истории коэффициентов (низкочастотная фильтрация)
        overview[6] = overview[5];
        overview[5] = overview[4];
        overview[4] = overview[3];
        overview[3] = overview[2];
        overview[2] = overview[1];
        overview[1] = overview[0];
        // Запись нового значения сложности в начало массива истории
        overview[0] = k;
        // Инициализация переменной для расчёта среднего коэффициента
        int coeff = 0;
        // Итерация по элементам массива для суммирования значений
        for (uint8_t f = 0; f < 6; f++) {
            coeff += overview[f];
        }
        // Вычисление среднего коэффициента фильтрации
        coeff /= 7;
        // Расчёт мощности микрофона с экспоненциальным сглаживанием
        micPower = 0.05 * maxPower() + (1 - 0.05) * micPower;
        // Сохранение вычисленного коэффициента для отладки
        testCoeff = coeff;
        // Классификация фонемы на основе среднего коэффициента
        if (coeff < econstant) {
            // Выбор фонемы 'e'
            phoneme = 'e';
        } else if (coeff < aconstant) {
            // Выбор фонемы 'o'
            phoneme = 'o';
        } else if (coeff < vconstant) {
            // Выбор фонемы 'v'
            phoneme = 'v';
        } else if (coeff < shconstant) {
            // Выбор фонемы 'h'
            phoneme = 'h';
        } else {
            // Выбор фонемы 's'
            phoneme = 's';
        }
        // Проверка разрешения распознавания фонемы /f/
        if (f_enabled) {
            // Сравнение мощности микрофона с порогом для фонемы /f/
            if (micPower > fconstant) {
                // Возврат фонемы 'f'
                return 'f';
            }
        }
        // Возврат распознанной фонемы
        return phoneme;
    }
    else {
        // Обнуление мощности микрофона при отсутствии звука
        micPower = 0;
        // Обнуление тестового коэффициента
        testCoeff = 0;
        // Возврат пробела (отсутствие звука)
        return ' ';
    }
}

#include "uspeech.h"

// Реализация конструктора класса signal
signal::signal(int port) {
    // Присвоение номера порта для микрофона
    pin = port;
    // Инициализация порога для фонемы /f/ значением константы F_CONSTANT
    fconstant = F_CONSTANT;
    // Установка порога для фонем 'e'
    econstant = 2;
    // Установка порога для фонем 'o'
    aconstant = 4;
    // Установка порога для фонем 'v'
    vconstant = 6;
    // Установка порога для фонем 'h'
    shconstant = 10;
    // Установка коэффициента усиления для вычисления сложности сигнала
    amplificationFactor = 10;
    // Задание порога, ниже которого мощность микрофона считается слишком низкой
    micPowerThreshold = 50;
    // Установка масштаба для входного сигнала
    scale = 1;
}

// Реализация метода калибровки микрофона на основе усреднения фонового уровня шума
void signal::calibrate(){
    // Инициализация переменной калибровки нулем
    calib = 0;
    // Инициализация переменной для суммирования измерений
    uint32_t samp = 0;
    // Цикл для сбора 10 000 выборок фонового шума
    for (uint16_t ind = 0; ind < 10000; ind++){
        // Считывание значения с аналового входа, масштабирование и накопление
        samp += analogRead(pin) * scale;
    }
    // Вычисление среднего значения фонового шума
    calib = samp / 10000;
}

// Реализация метода выборки звукового сигнала с вычитанием калибровочного значения
void signal::sample(){
    // Инициализация счётчика выборки
    int i = 0;
    // Цикл для сбора 32 значений с микрофона
    while (i < 32){
        // Считывание значения с аналога, масштабирование, вычитание калибровки и запись в массив
        arr[i] = (analogRead(pin) * scale - calib);
        // Инкремент счётчика
        i++;
    }
}

// Реализация метода для вычисления общей мощности сигнала
unsigned int signal::power(){
    // Инициализация переменной для накопления суммы модулей значений сигнала
    unsigned int j = 0;
    // Инициализация счётчика
    uint8_t i = 0;
    // Цикл по всем элементам массива с выборками
    while (i < 32){
        // Прибавление абсолютного значения текущей выборки к сумме
        j += abs(arr[i]);
        // Инкремент счётчика
        i++;
    }
    // Возврат суммарной мощности сигнала
    return j;
}

// Реализация метода для вычисления «сложности» сигнала (отношение суммы модулей разностей соседних значений к мощности)
unsigned int signal::complexity(int power){
    // Инициализация переменной для накопления суммы модулей разностей соседних выборок
    unsigned int j = 0;
    // Установка счётчика с началом со второго элемента массива
    uint8_t i = 1;
    // Цикл по элементам массива, начиная со второго значения
    while (i < 32){
        // Прибавление модуля разности текущей и предыдущей выборки
        j += abs(arr[i] - arr[i - 1]);
        // Инкремент счётчика
        i++;
    }
    // Вычисление и возврат значения «сложности» сигнала с учётом коэффициента усиления
    return (j * amplificationFactor) / power;
}

// Реализация метода для определения максимальной амплитуды сигнала
unsigned int signal::maxPower() {
    // Инициализация счётчика для обхода массива
    int i = 0;
    // Инициализация переменной для хранения максимальной амплитуды
    unsigned int max = 0;
    // Цикл по всем 32 выборкам сигнала
    while (i < 32){
        // Сравнение текущего максимального значения с модулем текущей выборки
        if (max < abs(arr[i])){
            // Обновление максимального значения амплитуды
            max = abs(arr[i]);
            // Запись позиции, на которой зафиксирован максимум
            maxPos = i;
        }
        // Инкремент счётчика
        i++;
        // Накопление значения для расчёта средней мощности
        avgPower += arr[i];
    }
    // Вычисление средней мощности сигнала
    avgPower /= 32;
    // Возврат максимальной амплитуды
    return max;
}

// Реализация метода для вычисления отношения сигнал/шум (SNR)
int signal::snr(int power){
    // Инициализация счётчиков для обхода массива
    uint8_t i = 0, j = 0;
    // Вычисление среднего значения сигнала
    int mean = power / 32;
    // Цикл по всем выборкам сигнала
    while (i < 32){
        // Накопление квадрата разности между выборкой и средним значением
        j += sq(arr[i] - mean);
        // Инкремент счётчика
        i++;
    }
    // Вычисление и возврат соотношения (корень из среднего квадрата ошибки к мощности)
    return sqrt(j / mean) / power;
}

#include "uspeech.h"

// Реализация конструктора класса syllable
syllable::syllable(){
    // Инициализация аккумуляторов для каждой фонемы
    f = 0;
    e = 0;
    o = 0;
    s = 0;
    h = 0;
    v = 0;
    // Инициализация длины слога и временных аккумуляторов фонем
    length = 0;
    cf = 0;
    ce = 0;
    co = 0;
    cs = 0;
    ch = 0;
    cv = 0;
    // Инициализация модальности каждой фонемы
    modalityf = 0;
    modalitye = 0;
    modalityo = 0;
    modalitys = 0;
    modalityh = 0;
    modalityv = 0;
    // Установка флага ожидания пробела в исходное состояние
    expectSp = 1;
    // Инициализация счётчика взрывных звуков
    plosiveCount = 0;
}

// Реализация метода для сброса накопленных значений слога
void syllable::reset(){
    // Сброс аккумуляторов для фонем
    f = 0;
    e = 0;
    o = 0;
    s = 0;
    h = 0;
    v = 0;
    // Сброс длины слога и временных аккумуляторов
    length = 0;
    cf = 0;
    ce = 0;
    co = 0;
    cs = 0;
    ch = 0;
    cv = 0;
    // Сброс модальности для каждой фонемы
    modalityf = 0;
    modalitye = 0;
    modalityo = 0;
    modalitys = 0;
    modalityh = 0;
    modalityv = 0;
    // Сброс флага ожидания пробела в исходное состояние
    expectSp = 1;
    // Сброс счётчика взрывных звуков
    plosiveCount = 0;
}

/***
 * Метод для классификации входного символа (фонемы) с обновлением соответствующих счетчиков
 */
void syllable::classify(char c){
    // Увеличение длины слога (количество обработанных символов)
    length++;
    // Проверка флага ожидания пробела
    if (expectSp == 0){
        // Проверка, является ли текущий символ не пробелом
        if (c != ' '){
            // Сброс флага ожидания пробела (объединение символов в один слог)
            expectSp = 1;
            // Проверка, попадает ли интервал между символами в допустимый порог для взрывных звуков
            if ((millis() - lastTime) < MAX_PLOSIVETIME){
                // Увеличение счётчика взрывных звуков
                plosiveCount++;
            }
        }
    }
    // Обработка символа в конструкции switch
    switch (c) {
        // Обработка символа 'f'
        case 'f':
            // Увеличение основного счётчика для 'f'
            f++;
            // Увеличение временного счётчика для 'f'
            cf++;
            // Выход из оператора switch для 'f'
            break;
        // Обработка символа 'e'
        case 'e':
            // Увеличение счётчика для 'e'
            e++;
            // Увеличение временного счётчика для 'e'
            ce++;
            break;
        // Обработка символа 'o'
        case 'o':
            // Увеличение счётчика для 'o'
            o++;
            // Увеличение временного счётчика для 'o'
            co++;
            break;
        // Обработка символа 'v'
        case 'v':
            // Увеличение счётчика для 'v'
            v++;
            // Увеличение временного счётчика для 'v'
            cv++;
            break;
        // Обработка символа 'h'
        case 'h':
            // Увеличение счётчика для 'h'
            h++;
            // Увеличение временного счётчика для 'h'
            ch++;
            break;
        // Обработка символа 's'
        case 's':
            // Увеличение счётчика для 's'
            s++;
            // Увеличение временного счётчика для 's'
            cs++;
            break;
        // Обработка символа пробела
        case ' ':
            // Проверка активности флага ожидания пробела
            if (expectSp != 0){
                // Сброс флага ожидания пробела для разделения слогов
                expectSp = 0;
                // Фиксация времени, используемого для расчёта интервала между слогами
                lastTime = millis();
            }
            break;
        // Обработка любых остальных символов
        default:
            break;
    }
    // Периодический анализ для определения пиков и модальности
    if ((length & PROCESS_SKEWNESS_TIME) == 0){
        // Анализ временного счётчика для 'f'
        if ((cf > prevf) & (prevf < PROCESS_SKEWNESS_TIME)){
            // Обновление предыдущего значения для 'f'
            prevf = cf;
            // Фиксация длины слога при достижении пика 'f'
            maxf = length;
            // Установка текущего пика и увеличение модальности для 'f'
            if (currPeak != 'f'){
                currPeak = 'f';
                modalityf++;
            }
        }
        // Анализ временного счётчика для 'e'
        if ((ce > preve) & (preve < PROCESS_SKEWNESS_TIME)){
            preve = ce;
            maxe = length;
            if (currPeak != 'e'){
                currPeak = 'e';
                modalitye++;
            }
        }
        // Анализ временного счётчика для 'o'
        if ((co > prevo) & (prevo < PROCESS_SKEWNESS_TIME)){
            prevo = co;
            maxo = length;
            if (currPeak != 'o'){
                currPeak = 'o';
                modalityo++;
            }
        }
        // Анализ временного счётчика для 's'
        if ((cs > prevs) & (prevs < PROCESS_SKEWNESS_TIME)){
            prevs = cs;
            maxs = length;
            if (currPeak != 's'){
                currPeak = 's';
                modalitys++;
            }
        }
        // Анализ временного счётчика для 'h'
        if ((ch > prevh) & (prevh < PROCESS_SKEWNESS_TIME)){
            prevh = ch;
            maxh = length;
            if (currPeak != 'h'){
                currPeak = 'h';
                modalityh++;
            }
        }
        // Анализ временного счётчика для 'v' с порогом 15
        if ((cv > prevv) & (prevv < 15)){
            prevv = cv;
            maxv = length;
            if (currPeak != 'v'){
                currPeak = 'v';
                modalityv++;
            }
        }
        // Сброс временных счётчиков для нового интервала анализа
        cf = 0;
        ce = 0;
        co = 0;
        cs = 0;
        ch = 0;
        cv = 0;
    }
}

// Реализация метода для отладочного вывода накопленных данных слога на Arduino
void syllable::debugPrint(){
    // Вывод начала структуры данных аккумулятора
    Serial.print("{Accum: [ ");
    // Вывод значения аккумулятора для 'f'
    Serial.print(f);
    Serial.print(", ");
    // Вывод значения аккумулятора для 'e'
    Serial.print(e);
    Serial.print(", ");
    // Вывод значения аккумулятора для 'o'
    Serial.print(o);
    Serial.print(", ");
    // Вывод значения аккумулятора для 'v'
    Serial.print(v);
    Serial.print(", ");
    // Вывод значения аккумулятора для 'h'
    Serial.print(h);
    Serial.print(", ");
    // Вывод значения аккумулятора для 's'
    Serial.print(s);
    Serial.print("], \n");
    // Вывод секции модальности фонем
    Serial.print("Modality: [");
    Serial.print(modalityf);
    Serial.print(",");
    Serial.print(modalitye);
    Serial.print(",");
    Serial.print(modalityo);
    Serial.print(",");
    Serial.print(modalityv);
    Serial.print(",");
    Serial.print(modalitys);
    Serial.print(",");
    Serial.print(modalityh);
    Serial.print("],");
    // Вывод информации о пиках для каждой фонемы
    Serial.print("Peak:[");
    Serial.print(maxf);
    Serial.print(",\n");
    Serial.print(maxe);
    Serial.print(",");
    Serial.print(maxo);
    Serial.print(",");
    Serial.print(maxv);
    Serial.print(",");
    Serial.print(maxs);
    Serial.print(",");
    Serial.print(maxh);
    Serial.print("],\n");
    // Вывод длины слога
    Serial.print("length:");
    Serial.print(length);
    Serial.print(",\nplosives:");
    // Вывод значения счётчика взрывных звуков
    Serial.print(plosiveCount);
    Serial.print(" }");
}

// Реализация метода для возврата указателя на массив с накопленными данными
int* syllable::tointptr(){
    // Объявление статического массива для хранения накопленных значений
    static int matrix[20];
    // Запись значения аккумулятора 'f' в массив
    matrix[0] = f;
    // Запись значения аккумулятора 'e' в массив
    matrix[1] = e;
    // Запись значения аккумулятора 'o' в массив
    matrix[2] = o;
    // Запись значения аккумулятора 'v' в массив
    matrix[3] = v;
    // Запись значения аккумулятора 's' в массив
    matrix[4] = s;
    // Запись значения аккумулятора 'h' в массив
    matrix[5] = h;
    // Запись значения модальности для 'f' в массив
    matrix[6] = modalityf;
    // Запись значения модальности для 'e' в массив
    matrix[7] = modalitye;
    // Запись значения модальности для 'o' в массив
    matrix[8] = modalityo;
    // Запись значения модальности для 'v' в массив
    matrix[9] = modalityv;
    // Запись значения модальности для 's' в массив
    matrix[10] = modalitys;
    // Запись значения модальности для 'h' в массив
    matrix[11] = modalityh;
    // Запись значения пика для 'f' в массив
    matrix[12] = maxf;
    // Запись значения пика для 'e' в массив
    matrix[13] = maxe;
    // Запись значения пика для 'o' в массив
    matrix[14] = maxo;
    // Запись значения пика для 'v' в массив
    matrix[15] = maxv;
    // Запись значения пика для 's' в массив
    matrix[16] = maxs;
    // Запись значения пика для 'h' в массив
    matrix[17] = maxh;
    // Запись значения длины слога в массив
    matrix[18] = length;
    // Запись значения счётчика взрывных звуков в массив
    matrix[19] = plosiveCount;
    // Возврат указателя на заполненный массив
    return matrix;
}

#include "uspeech.h"

// Реализация конструктора класса statCollector для накопления статистических данных
statCollector::statCollector(){
    // Инициализация переменных для количества выборок, среднего значения и моментов нулевыми значениями
    n = 0;
    mean = 0;
    M2 = 0;
    M3 = 0;
    M4 = 0;
}

// Реализация метода для возврата среднего значения
int statCollector::_mean(){
    // Возврат рассчитанного среднего значения
    return mean;
}

// Реализация метода для получения стандартного отклонения (в данном случае дисперсии – M2)
int statCollector::stdev(){
    // Возврат значения второго центрального момента
    return M2;
}

// Реализация метода для вычисления куртоза (измерение «остроты» распределения)
int statCollector::kurtosis(){
    // Вычисление куртоза по формуле (нормированный 4-й момент минус 3)
    int kurtosis = (n * M4) / (M2 * M2) - 3;
    // Возврат вычисленного значения куртоза
    return kurtosis;
}

// Реализация метода для вычисления асимметрии распределения
int statCollector::skew(){
    // Вычисление асимметрии по формуле (нормированный 3-й момент минус 3)
    int kurtosis = (n * M3) / (M2 * M2 * M2) - 3;
    // Возврат полученного значения асимметрии
    return kurtosis;
}

// Реализация метода для накопления статистических данных с новым значением x
void statCollector::collect(int x) {
    // Сохранение предыдущего количества выборок
    int n1 = n;
    // Увеличение счётчика выборок
    n = n + 1;
    // Вычисление разницы между новым значением и текущим средним
    int delta = x - mean;
    // Вычисление поправочного коэффициента для среднего
    int delta_n = delta / n;
    // Вычисление квадрата поправочного коэффициента
    int delta_n2 = delta_n * delta_n;
    // Вычисление вспомогательного терма для обновления статистических моментов
    int term1 = delta * delta_n * n1;
    // Обновление среднего значения
    mean = mean + delta_n;
    // Обновление четвёртого момента распределения
    M4 = M4 + term1 * delta_n2 * (n * n - 3 * n + 3) + 6 * delta_n2 * M2 - 4 * delta_n * M3;
    // Обновление третьего момента (асимметрия)
    M3 = M3 + term1 * delta_n * (n - 2) - 3 * delta_n * M2;
    // Обновление второго момента (сумма квадратов отклонений)
    M2 = M2 + term1;
}




#include <Wire.h>
// Подключение библиотеки для работы с интерфейсом I²C

#include <LiquidCrystal_I2C.h>
// Подключение библиотеки для управления LCD-дисплеем по шине I²C

#include <uspeech.h>
// Подключение библиотеки uspeech для распознавания речи

#include <string.h>
// Подключение стандартной библиотеки C для работы со строками (strcmp, strlen)

#define ledGreen   7
// Определение номера пина 7 для зелёного светодиода

#define ledOrange  6
// Определение номера пина 6 для оранжевого светодиода

#define ledWhite   5
// Определение номера пина 5 для белого светодиода

#define buzzerPin  2
// Определение номера пина 2 для подключения пьезодинамика (буззера)

#define enablePin  A1
// Определение номера аналогового пина A1 для переключателя (замыкание на GND = включено)

#define MIN3(a,b,c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))
// Макрос для вычисления минимального значения из трёх аргументов

signal voice(A0);
// Создание объекта voice класса signal, подключённого к аналоговому пину A0 для сбора аудиосэмплов

LiquidCrystal_I2C lcd(0x27, 16, 2);
// Создание объекта lcd для работы с I²C LCD-дисплеем по адресу 0x27 (16×2 символа)

const int  BUFFER_MAX_PHONEMES = 32;
// Константа: максимальный размер буфера для накопления фонем

char       inputString[BUFFER_MAX_PHONEMES];
// Массив для накопления входных фонем в виде строки

byte       index = 0;
// Текущий индекс в буфере inputString

const int  DICT_MAX_ELEMNTS = 3;
// Константа: количество элементов (паттернов) в словаре

char dict[DICT_MAX_ELEMNTS][BUFFER_MAX_PHONEMES] = {
  "vvvoeeeeeeeofff",   // Паттерн для команды "green"
  "hhhhhvoovvvvf",     // Паттерн для команды "orange"
  "booooooffffffff"    // Паттерн для команды "white"
};
// Двумерный массив строк с паттернами фонем для распознавания трёх команд

int    LOWEST_COST_MAX_THREASHHOLD = 20;
// Пороговое значение для допустимой стоимости Левенштейна

unsigned long lastCommandTime      = 0;
// Время (в миллисекундах) последнего распознанного и обработанного слова

bool         idleDisplayed        = false;
// Флаг, указывающий, выводилось ли сообщение "ожидание" на дисплей

bool         voiceEnabled         = false;
// Флаг, указывающий, включено ли распознавание речи (состояние переключателя)

/**
 * Функция beep
 * Простая «пищалка» на buzzerPin: замыкание на GND включает звук.
 * Параметр ms задаёт длительность звукового сигнала в миллисекундах (по умолчанию 100 мс).
 */
void beep(int ms = 100) {
  digitalWrite(buzzerPin, LOW);
  delay(ms);
  digitalWrite(buzzerPin, HIGH);
}

/**
 * Функция strLength
 * Вычисляет длину строки до первого пробела или до символа конца строки '\0'.
 */
int strLength(const char *s) {
  int i = 0;
  while (s[i] && s[i] != ' ')
    ++i;
  return i;
}

/**
 * Функция levenshtein
 * Вычисляет расстояние Левенштейна между строками s1 и s2.
 */
unsigned int levenshtein(char *s1, char *s2) {
  unsigned int s1len = strlen(s1), s2len = strlen(s2);
  unsigned int column[s1len + 1];
  // Инициализация первого столбца матрицы расстояний
  for (unsigned int y = 1; y <= s1len; y++)
    column[y] = y;
  // Основной цикл по символам второй строки
  for (unsigned int x = 1; x <= s2len; x++) {
    column[0] = x;
    unsigned int lastdiag = x - 1;
    for (unsigned int y = 1; y <= s1len; y++) {
      unsigned int olddiag = column[y];
      // Вычисляем стоимость удаления, вставки и замены
      column[y] = MIN3(
        column[y] + 1,
        column[y - 1] + 1,
        lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1)
      );
      lastdiag = olddiag;
    }
  }
  return column[s1len];
}

/**
 * Функция guessWord
 * По входной строке target вычисляет стоимость Левенштейна до каждого паттерна
 * и возвращает указатель на наиболее близкий или пустую строку, если ни один
 * паттерн не уложился в порог.
 */
char* guessWord(char* target) {
  static unsigned int cost[DICT_MAX_ELEMNTS];
  // Считаем стоимости для каждого паттерна
  for (int j = 0; j < DICT_MAX_ELEMNTS; j++)
    cost[j] = levenshtein(dict[j], target);
  // Ищем лучший (минимальный) cost
  int best = -1, bestCost = LOWEST_COST_MAX_THREASHHOLD;
  for (int j = 0; j < DICT_MAX_ELEMNTS; j++) {
    if (cost[j] < bestCost) {
      bestCost = cost[j];
      best = j;
    }
  }
  // Возвращаем либо указатель на строку словаря, либо пустую строку
  return (best >= 0 ? dict[best] : (char*)"");
}

/**
 * Функция parseCommand
 * Обрабатывает распознанное слово: включает нужный светодиод,
 * выводит текст на дисплей и сбрасывает флаг ожидания.
 */
void parseCommand(char* str) {
  char *g = guessWord(str);
  if (!g[0]) return;  // Если слово не распознано — выходим

  // Вывод в сериал для отладки
  Serial.print("guessed: "); Serial.println(g);

  // Очистка дисплея и вывод метки
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Word:");
  lcd.setCursor(0,1);

  // Сначала выключаем все светодиоды
  digitalWrite(ledGreen,  LOW);
  digitalWrite(ledOrange, LOW);
  digitalWrite(ledWhite,  LOW);

  // Сравнение распознанного слова с паттернами через strcmp
  if      (strcmp(g, dict[0]) == 0) {
    lcd.print("green");
    digitalWrite(ledGreen,  HIGH);
  }
  else if (strcmp(g, dict[1]) == 0) {
    lcd.print("orange");
    digitalWrite(ledOrange, HIGH);
  }
  else if (strcmp(g, dict[2]) == 0) {
    lcd.print("white");
    digitalWrite(ledWhite,  HIGH);
  }

  // Обновляем время последней команды и сбрасываем флаг idle
  lastCommandTime = millis();
  idleDisplayed   = false;
}

void setup() {
  Serial.begin(9600);
  // Инициализация дисплея
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Настройка пинов на выход
  pinMode(ledGreen,  OUTPUT);
  pinMode(ledOrange, OUTPUT);
  pinMode(ledWhite,  OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(enablePin, INPUT_PULLUP);
  digitalWrite(buzzerPin, HIGH); // Выключаем буззер по умолчанию

  // Стартовая фаза с индикатором загрузки и калибровкой
  lcd.print("Loading...");
  unsigned long t0 = millis();
  while (millis() - t0 < 5000) {
    unsigned long dt = millis() - t0;
    // Пищим первые 1 секунду
    if (dt < 1000) digitalWrite(buzzerPin, LOW);
    else           digitalWrite(buzzerPin, HIGH);

    // После первой секунды выводим сообщение о калибровке
    if (dt >= 1000 && dt < 1250) {
      lcd.clear();
      lcd.print("Calibrating...");
      static bool calibStarted = false;
      if (!calibStarted) {
        calibStarted = true;
        // Настройка параметров распознавания перед калибровкой
        voice.f_enabled = true;
        voice.minVolume  = 1500;
        voice.fconstant  = 500;
        voice.econstant  = 2;
        voice.aconstant  = 4;
        voice.vconstant  = 6;
        voice.shconstant = 10;
        voice.calibrate();
      }
    }

    // Мигаем всеми светодиодами в такт
    bool st = ((millis() / 250) & 1);
    digitalWrite(ledGreen,  st);
    digitalWrite(ledOrange, st);
    digitalWrite(ledWhite,  st);
    delay(50);
  }

  // После загрузки — выключаем все индикаторы
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledGreen,  LOW);
  digitalWrite(ledOrange, LOW);
  digitalWrite(ledWhite,  LOW);

  // Читаем состояние переключателя и выводим финальное сообщение
  voiceEnabled = (digitalRead(enablePin) == LOW);
  lcd.clear();
  if (voiceEnabled) lcd.print("Say a color...");
  else              lcd.print("Voice OFF");

  lastCommandTime = millis();
  idleDisplayed   = false;
}

void loop() {
  // Отслеживаем смену положения переключателя
  bool en = (digitalRead(enablePin) == LOW);
  if (en != voiceEnabled) {
    voiceEnabled = en;
    // Сбрасываем буфер ввода
    index = 0;
    inputString[0] = '\0';
    // Обновляем экран и при необходимости калибруем микрофон
    lcd.clear();
    if (voiceEnabled) {
      lcd.print("Calibrating...");
      voice.calibrate();
      lcd.clear(); lcd.print("Say a color...");
    } else {
      lcd.print("Voice OFF");
      // Выключаем все светодиоды
      digitalWrite(ledGreen,  LOW);
      digitalWrite(ledOrange, LOW);
      digitalWrite(ledWhite,  LOW);
    }
    lastCommandTime = millis();
    idleDisplayed   = false;
  }

  // Если голосовое управление включено — собираем и обрабатываем фонемы
  if (voiceEnabled) {
    voice.sample();
    char p = voice.getPhoneme();
    // Если пришёл пробел или буфер полон — обрабатываем накопленную строку
    if (p == ' ' || index >= BUFFER_MAX_PHONEMES) {
      if (strLength(inputString) > 0) {
        parseCommand(inputString);
        index = 0;
        inputString[0] = '\0';
      }
    } else {
      // Добавляем фонему в буфер
      inputString[index++] = p;
      inputString[index]   = '\0';
    }

    // Если нет команд больше 5 секунд — показываем приглашение к вводу
    if (millis() - lastCommandTime > 5000 && !idleDisplayed) {
      digitalWrite(ledGreen,  LOW);
      digitalWrite(ledOrange, LOW);
      digitalWrite(ledWhite,  LOW);
      lcd.clear();
      lcd.print("Say a color...");
      idleDisplayed = true;
    }
  }
}
