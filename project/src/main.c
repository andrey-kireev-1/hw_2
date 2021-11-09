#include "header.h"
#include "sequential.c"
#include "parallel.c"


int main(){
    printf("Последовательный алгоритм: ");
    sequential();
    sleep(1);
    printf("Параллельный алгоритм");
    parallel();
}