#include <stdio.h>
#include <string.h>

void swap_first_last(char *string) {
    char *start = string;
    char *end = string + strlen(string) - 1;
    char temp;
    while (start <= end) {
        if (*start == ' ') {
            start++;
            continue;
        }
        if (*end == ' ') {
            end--;
            continue;
        }
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int main() {
    char string[] = "Hello World";
    swap_first_last(string);
    printf("%s\n", string); // will print "oellH dlorW"
    return 0;
}

