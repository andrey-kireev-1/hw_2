#include "header.h"

void sequential(){
    int i = 0;
    int *p = NULL;
    p = (int*) malloc(TEN_TO_EIGHT_POW * sizeof(int));
    int *p2 = NULL;
    p2 = (int*) malloc(TEN_TO_EIGHT_POW * sizeof(int));
    FILE *outfile1 = NULL;
    outfile1 = fopen("../project/src/vector_1.bin", "rb");
    if (outfile1 == NULL) {
        printf("Error opening file");
        exit(ERROR_OPEN_FILE);
    }
    FILE *outfile2 = NULL;
    outfile2 = fopen("../project/src/vector_2.bin", "rb");
    if (outfile2 == NULL) {
        printf("Error opening file");
        exit(ERROR_OPEN_FILE);
    }
    int grad_arr[GRADATION_NUMBER];
    while (i < GRADATION_NUMBER){
        grad_arr[i] = 0;
        i++;
    }
    int step = RANGE / GRADATION_NUMBER;

    printf("Начало построение гистограммы №1 = %ld с\n", clock() / CLOCKS_PER_SEC);
    get_vector(p, outfile1);
    make_gradation(grad_arr, step, p);
    free(p);
    int sum = 0;
    i = 0;
    while (i < GRADATION_NUMBER){
        printf("%d\n", grad_arr[i]);
        sum += grad_arr[i];
        i++;
    }
    printf("summa = %d\n", sum);
    
    int max_count = maximum_grad_arr(grad_arr);
    int ten_pow = -999;
    int max_border = make_border(max_count, &ten_pow);
    print_graph(grad_arr, max_border, ten_pow, step);
    i = find_median(grad_arr, max_border, step);
    fclose(outfile1);
    printf("Конец построения гистограммы №1 = %ld с\n", clock() / CLOCKS_PER_SEC);
    i = 0;
    while (i < GRADATION_NUMBER){
        grad_arr[i] = 0;
        i++;
    }

    printf("Начало построение гистограммы №2 = %ld с\n", clock() / CLOCKS_PER_SEC);
    get_vector(p2, outfile2);
    make_gradation(grad_arr, step, p2);
    free(p2);
    i = 0;
    while (i < GRADATION_NUMBER){
        printf("%d\n", grad_arr[i]);
        i++;
    }
    max_count = maximum_grad_arr(grad_arr);
    ten_pow = -999;
    max_border = make_border(max_count, &ten_pow);
    print_graph(grad_arr, max_border, ten_pow, step);
    i = find_median(grad_arr, max_border, step);
    fclose(outfile2);
    printf("Конец построения гистограммы №2 = %ld с\n", clock() / CLOCKS_PER_SEC);
}