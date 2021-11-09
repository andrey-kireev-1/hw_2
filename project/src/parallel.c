#include "header.h"

void parallel(){
    int i = 0;
    int step = RANGE / GRADATION_NUMBER;
    int grad_arr[GRADATION_NUMBER];
    while (i < GRADATION_NUMBER){
        grad_arr[i] = 0;
        i++;
    }
    i = 0;
    int max_count;
    int ten_pow;
    int max_border;
    int status;
    int process_count = sysconf(_SC_NPROCESSORS_ONLN);
    FILE *outfile1 = NULL;
    FILE *outfile2 = NULL;
    int *p = NULL;
    int *p2 = NULL;
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
		exit(EXIT_FAILURE);
        break;
    case 0:
        printf("Начало построение гистограммы №1 = %ld с\n", clock() / CLOCKS_PER_SEC);
        outfile1 = fopen("../project/src/vector_1.bin", "rb");
        if (outfile1 == NULL) {
            printf("Error opening file");
            exit(ERROR_OPEN_FILE);
        }
        p = (int*) malloc(TEN_TO_EIGHT_POW * sizeof(int));
        get_vector(p, outfile1);
        make_gradation_parall(grad_arr, step, p, process_count);
        free(p);
        max_count = maximum_grad_arr(grad_arr);
        ten_pow = -999;
        max_border = make_border(max_count, &ten_pow);
        print_graph(grad_arr, max_border, ten_pow, step);
        i = find_median(grad_arr, max_border, step);
        fclose(outfile1);
        printf("Конец построения гистограммы №1 = %ld с\n", clock() / CLOCKS_PER_SEC);
        exit(EXIT_SUCCESS);
        break;
    
    default:
        printf("Начало построение гистограммы №2 = %ld с\n", clock() / CLOCKS_PER_SEC);
        outfile2 = fopen("../project/src/vector_2.bin", "rb");
        if (outfile2 == NULL) {
            printf("Error opening file");
            exit(ERROR_OPEN_FILE);
        }
        p2 = (int*) malloc(TEN_TO_EIGHT_POW * sizeof(int));
        get_vector(p2, outfile2);
        make_gradation_parall(grad_arr, step, p2, process_count);
        free(p2);
        i = 0;
        max_count = maximum_grad_arr(grad_arr);
        ten_pow = -999;
        max_border = make_border(max_count, &ten_pow);
        waitpid(pid, &status, 0);
        print_graph(grad_arr, max_border, ten_pow, step);
        i = find_median(grad_arr, max_border, step);
        fclose(outfile2);
        printf("Конец построения гистограммы №2 = %ld с\n", clock() / CLOCKS_PER_SEC);
        break;
    }
}