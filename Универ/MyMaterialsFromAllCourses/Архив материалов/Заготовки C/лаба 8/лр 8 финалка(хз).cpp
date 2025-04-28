/* Даны натуральное число n и символы s1..s10. Среди символов этой строки особую роль играет знак #,
например, строка vr##y#helo#lo должна быть hello.
резудьтируюшую строку вывести на экран преобразовать строку с учетом роли знака # .
появление которого означает отмену предыдущего символа*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
 
#define N 50

int main() {

    int count = 0;
    char* str = (char*)calloc(N, sizeof(char));
    fgets(str, N, stdin);
    printf("\n");

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '#') {
            count++; 
        }
    }

    for (int k = 0; k < count; k++) {                   
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] == '#') {
                for (int j = i; str[j] != '\0'; j++) {
                    str[j] = str[j + 1];
                }
                for (int j = i; str[j] != '\0'; j++) {
                    str[j - 1] = str[j];                       
                }                                                                    
            }
        }
    }

    puts(str);
    return 0;

}