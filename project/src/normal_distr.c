//////////////////////////////////////////////////////////////
//  НЕ ИСПОЛЬЗУЕТСЯ В ПРОЕКТЕ Т.К. НЕ ВХОДИТ В УСЛОВИЕ      //
//  ЗАПОЛНЯЕТ ИСПОЛЬЗУЕМЫЕ ФАЙЛЫ НА 10^8 ЧИСЕЛ              //
//  ПО НОРМАЛЬНОМУ РАСПРЕДЕЛЕНИЮ ПО АЛГОРИТМУ БОКСА-МЮЛЛЕРА //
//////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define ERROR_OPEN_FILE -3
#define RANGE_MAX 1
#define RANGE_MIN -1
#define TEN_TO_EIGHT_POW 100000000
#define RANGE 200000


int main(){
    FILE *iofile1 = NULL;
    iofile1 = fopen("vector_1.bin", "w+b");
    srand(101); 
    int i = 0;
    double z1 = 0, z2 = 0;
    int tmp1 = 0, tmp2 = 0;
    double x = 0, y = 0;
    double s = 0;
    int x_border = RANGE / 2;
    while (i < TEN_TO_EIGHT_POW){
        x = ((double)rand() / RAND_MAX) * (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
        y = ((double)rand() / RAND_MAX) * (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
        s = x*x + y*y;
        if ((s <= 1) && (s > 0)){
            z1 = (x - 0.05) * sqrt(((-2) * log(s)) / s+0.01);
            z2 = (y - 0.05) * sqrt(((-2) * log(s)) / s+0.01);
            tmp1 = (int)(z1 * (x_border / 10));
            tmp2 = (int)(z2 * (x_border / 10));
            if ( ((tmp1 > - x_border) && (tmp1 < x_border)) && ((tmp2 > - x_border) && (tmp2 < x_border)) ){
                fwrite(&tmp1, sizeof(int), 1, iofile1);
                i++;
                fwrite(&tmp2, sizeof(int), 1, iofile1);
                i++;
            }
        } 
    } 
    fclose(iofile1);
    FILE *iofile2 = NULL;
    iofile2 = fopen("vector_2.bin", "w+b");
    srand(102);
    i = 0;
    z1 = 0;
    z2 = 0;
    tmp1 = 0;
    tmp2 = 0;
    x = 0;
    y = 0;
    s = 0;
    while (i < TEN_TO_EIGHT_POW){
        x = ((double)rand() / RAND_MAX) * (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
        y = ((double)rand() / RAND_MAX) * (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
        s = x*x + y*y;
        if ((s <= 1) && (s > 0)){
            z1 = x * sqrt(((-2) * log(s)) / s);
            z2 = y * sqrt(((-2) * log(s)) / s);
            tmp1 = (int)(z1 * (x_border / 10));
            tmp2 = (int)(z2 * (x_border / 10));
            if ( ((tmp1 > - x_border) && (tmp1 < x_border)) && ((tmp2 > - x_border) && (tmp2 < x_border)) ){
                fwrite(&tmp1, sizeof(int), 1, iofile2);
                i++;
                fwrite(&tmp2, sizeof(int), 1, iofile2);
                i++;
            }
        } 
    } 
    fclose(iofile2);
    return 0;
}

