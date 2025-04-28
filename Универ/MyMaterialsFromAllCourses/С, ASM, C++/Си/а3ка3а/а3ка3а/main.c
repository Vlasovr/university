#include <stdio.h>

#include <stdlib.h>
int main(){
    int n, m;
    int trigger, position, switcher, counter = 0;
    printf("\nВведите сумму кол-ва значений 'x' и '1' функции:\n");
    scanf("%d", &n);
    printf("\nВведите кол-во переменных:\n");
    scanf("%d", &m);
    char nums[n][m];
    int used[n];
    for(int i = 0; i < n; i++){
        used[i] = 0;
    }
    printf("\nВведите наборы, при которых функция имеет значения 'x' и '1':\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            scanf("%c", &nums[i][j]);
            while((nums[i][j] != 'x') && (nums[i][j] != '0') && (nums[i][j] != '1')){
                scanf("%c", &nums[i][j]);
            }
        }
    }
    printf("\nВыберете режим работы:\n 0 - Склеивание \n 1 - Разделить на группы \n 2 - Убрать копии\n");
    scanf("%d", &switcher);
    if(switcher == 1){
        printf("\nРезультат:\n\n");
        int one, x;
        int group[m][n];
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                group[i][j] = 0;
            }
        }
        for(int i = 0; i < n; i++){
            one = 0;
            x = 0;
            for(int j = 0; j < m; j++){
                if(nums[i][j] == '1'){
                    one++;
                }
                if(nums[i][j] == 'x'){
                    position = j;
                    x++;
                    break;
                }
            }
            if(x){
                group[position][i] = 1;
                continue;
            }
            if(one == 0){
                printf("000000\n\n");
                continue;
            }
            group[one - 1][i] = 1;
        }
        for(int i = 0; i < m; i++){
            trigger = 0;
            for(int j = 0; j < n; j++){
                if(group[i][j]){
                    for(int k = 0; k < m; k++){
                        printf("%c", nums[j][k]);
                    }
                    printf("\n");
                    trigger = 1;
                }
            }
            if(trigger)
                printf("\n");
        }
    }else if(!switcher){
        printf("Результат:\n");
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){
                trigger = 0;
                for(int k = 0; k < m; k++){
                    if(nums[i][k] != nums[j][k]){
                        if(nums[i][k] == 'x' || nums[j][k] == 'x'){
                            trigger = 2;
                        } else{
                            trigger++;
                            position = k;
                        }
                    }
                }
                if(trigger == 1){
                    counter++;
                    used[i] = 1;
                    used[j] = 1;
                    for(int k = 0; k < m; k++){
                        if(k != position){
                            printf("%c", nums[i][k]);
                        } else {
                            printf("x");
                        }
                    }
                    printf("\n");
                }
            }
        }
        printf("\nНе использованые значения:\n");
        for(int i = 0; i < n; i++){
            if(!used[i]){
                for(int j = 0; j < m; j++){
                    printf("%c", nums[i][j]);
                }
                printf("\n");
            }
        }
        printf("\nВсего значений: %d\n", counter);
    } else {
        printf("Результат:\n");
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){
                for(int k = 0; k < m; k++){
                    if(nums[i][k] == nums[j][k]){
                        trigger++;
                                            }
                                        }
                                        if(trigger == 6)
                                            break;
                                        trigger = 0;
                                    }
                                    if(trigger != 6){
                                        counter++;
                                        for(int k = 0; k < m; k++){
                                            printf("%c", nums[i][k]);
                                        }
                                        printf("\n");
                                    } else{
                                        trigger = 0;
                                    }
                                }
                                printf("\nВсего значений: %d\n", counter);
                            }
                            return 0;
                        }
