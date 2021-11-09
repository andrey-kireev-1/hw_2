#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

#define TEN_TO_EIGHT_POW 100000000
#define ERROR_OPEN_FILE -3
#define GRADATION_NUMBER 100
#define RANGE 200000

void sequential();
void parallel();

void make_gradation(int *grad_arr, int step, int* p){
    int i = 0;
    int index = -1;
    while (i < TEN_TO_EIGHT_POW){
        if (p[i] < 0){ 
            index = (p[i] / step) + (GRADATION_NUMBER / 2) - 1;
        } else {
            index = p[i] / step + (GRADATION_NUMBER / 2);
        }
        grad_arr[index] += 1;
        i++;
        index = -1;
    }
}

void make_gradation_parall(int *grad_arr, int step, int* p, int process_count){
    int i = 0;
    int index = -1;
    int j;
    int status;
    int grad_arr_copy[GRADATION_NUMBER];
    while (i < GRADATION_NUMBER){
        grad_arr_copy[i] = 0;
        i++;
    }
    int step_read = TEN_TO_EIGHT_POW / process_count;
    int fd[2];
    if (pipe(fd) != 0) {
        printf("Opening pipe failed\n");
        exit(EXIT_FAILURE);
    }

    i = 0;
    pid_t pid;
    int arr_pid[process_count];
    int prindex = 1; //PARENT + "0", CHILD + "1";
    for(int n = 0; n < (process_count-1); n++){
        pid = fork();
        arr_pid[prindex] = pid;
        if (pid == 0){
            break;
        } else {
            prindex += 1;
        }
    }
    switch (pid)
    {
    case -1:
        perror("fork failed");
		exit(EXIT_FAILURE);
        break;
    case 0:
        i = prindex - 1;
        j = i * step_read;
        
        while (j < prindex * step_read){
            if (p[j] < 0){ 
                index = (p[j] / step) + (GRADATION_NUMBER / 2) - 1;
            } else {
                index = p[j] / step + (GRADATION_NUMBER / 2);
            }
            grad_arr_copy[index] += 1;
            index = -1;
            j++;
        }
        
        
        printf("Это дочерний Номер процесса = %d\n", prindex);
        if (close(fd[0])) {
            printf("CHILD: Failed to close read pipe\n");
        }
        if (write(fd[1], grad_arr_copy, GRADATION_NUMBER*sizeof(int)) < 0) {
            printf("CHILD: Failed to write into pipe\n");
        }
        if (close(fd[1])) {
            printf("CHILD: Failed to close write pipe\n");
        }

        
        exit(prindex);
        break;
    default:
        i = prindex - 1;
        j = i * step_read;
        while (j < prindex * step_read){
            if (p[j] < 0){ 
                index = (p[j] / step) + (GRADATION_NUMBER / 2) - 1;
            } else {
                index = p[j] / step + (GRADATION_NUMBER / 2);
            }
            grad_arr[index] += 1;
            index = -1;
            j++;
        }
        i = 0;
        
        j = 1;
        while (j < prindex){
            printf("%d\n", arr_pid[j]);
            waitpid(arr_pid[j], &status, 0);
            printf("Ждем пока завершится процесс = %d, его pid = %d,сверяем его номер процесса = %i\n", j, arr_pid[j], WEXITSTATUS(status));
            
            
            if (read(fd[0], grad_arr_copy, GRADATION_NUMBER*sizeof(int)) < 0) {
            printf("PARENT: Failed to read from pipe\n");
            }
            
            i = 0;
            
            while (i < GRADATION_NUMBER){
                grad_arr[i] += grad_arr_copy[i];
                i++;
            }
           
            
            j++;
        }
        printf("parent prindex = %d, child prindex = %i\n", prindex, WEXITSTATUS(status));
        break;
    }
}

int maximum_grad_arr(int *grad_arr){
    int max = -1;
    int i = 0;
    while (i < GRADATION_NUMBER){
        if (grad_arr[i] > max){
            max = grad_arr[i];
        }
        i++;
    }
    return max;
}

int make_border(int max_count, int *t_p){
    int count = 0;
    int i = 0;
    int tmp = max_count;
    while (tmp > 0){
        tmp = tmp / 10;
        count++;
    }
    int ten_pow = 1;
    while (i != (count-2)){
        ten_pow *= 10;
        i++;
    }
    tmp = ((max_count/ten_pow) + 1) * ten_pow;
    *t_p = ten_pow;
    return tmp;
}

void print_graph(int* grad_arr, int max_border, int ten_pow, int step){
    int y_step = ten_pow;
    int y_line = max_border;
    int x_border = RANGE / 2;
    printf("Данные гистограммы:\n");
    printf("На оси X расположен интервал (-%d ; %d) возможных значений входных данных,\nкоторый поделен на дискретные интервалы с шагом %d,\nкаждому дискретному интервалу соответствует свой столбец.\n", x_border, x_border, step);
    printf("На оси Y располагается количество чисел, чье значение входит в указанный по X интервал,\nверхняя граница оси Y: %d, шаг интервала по Y: %d,\nотметка на гистограмме '0' ставится в том случае, если количество входных данных превышает указанное по Y значение\n", max_border, y_step);
    printf("Количество чисел: \n");
    while (y_line > 0){
        printf("%8d|", y_line);
        for (int i = 0; i < GRADATION_NUMBER; i++){
            if ((grad_arr[i] < y_line )){
                printf("*");
            } else {
                printf ("0");
            }

        } 
        y_line -= y_step;
        printf("\n");
    }
}

int find_median(int* grad_arr, int max_border, int step){
    int i = 0;
    int left = 0;
    int freq_sum = TEN_TO_EIGHT_POW / 2;
    int x_start = -RANGE / 2;
    int x_start_check;
    double median = 0;
    double tmp = 0;
    while (left < freq_sum){
        left += grad_arr[i];
        i++;
    }
    i--;
    left -= grad_arr[i];
    printf("Медиана находится в столбце №%d, кол-во элементов в этом столбце: %d\n", i, grad_arr[i]);
    int j = 0;
    while (j < GRADATION_NUMBER){
        if (i == j){
            printf("В столбце №%d, интервал ( %d — %d )\n", j, x_start, x_start + step);
            x_start_check = x_start;
        }
        j++;
        x_start += step;
    }
    tmp = freq_sum - left;
    tmp = tmp / grad_arr[i];
    median = x_start_check + (step * tmp);
    printf("Медиана равна: %.2f\n", median);
    return median;
}

void get_vector(int* p, FILE* outfile){
    int i = 0;
    while (i < TEN_TO_EIGHT_POW){
        fread(&p[i], sizeof(int), 1, outfile);
        i++;

    }
}