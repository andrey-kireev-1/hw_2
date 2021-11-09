#include "gtest/gtest.h"

extern "C" {
    #include "../src/header.h"
    #include "../src/parallel.c"
    #include "../src/sequential.c"
}

TEST(MaximumGradArr, test1){
    srand(1);
    int i = 0;
    int grad_arr[GRADATION_NUMBER];
    while (i < GRADATION_NUMBER){
        grad_arr[i] = ((double)rand() / RAND_MAX) * (1000 - 0) + 0;
        i++;
    }
    grad_arr[GRADATION_NUMBER/2] = 1001;
    ASSERT_EQ(maximum_grad_arr(grad_arr), 1001);
}

TEST(ParallelNSequentialMakeGradation, test2){
    srand(2);
    int* p = NULL;
    p = (int*) malloc(TEN_TO_EIGHT_POW * sizeof(int));
    int i = 0;
    int grad_arr_1[GRADATION_NUMBER];
    while (i < GRADATION_NUMBER)
    {
        grad_arr_1[i] = 0;
        i++;
    }
    int grad_arr_2[GRADATION_NUMBER];
    i = 0;
    while (i < GRADATION_NUMBER)
    {
        grad_arr_2[i] = 0;
        i++;
    }
    i = 0;
    while (i < TEN_TO_EIGHT_POW){
        p[i] = ((double)rand() / RAND_MAX) * ((RANGE/20) - (-RANGE/20)) + (-RANGE/20);
        i++;
    }
    int step = RANGE / GRADATION_NUMBER;
    int process_count = sysconf(_SC_NPROCESSORS_ONLN);
    make_gradation(grad_arr_1, step, p);
    make_gradation_parall(grad_arr_2, step, p, process_count);
    free(p);
    srand(2);
    i = ((double)rand() / RAND_MAX) * (GRADATION_NUMBER - 0) + 0;
    ASSERT_EQ(grad_arr_1[i], grad_arr_2[i]);
    i = ((double)rand() / RAND_MAX) * (GRADATION_NUMBER - 0) + 0;
    ASSERT_EQ(grad_arr_1[i], grad_arr_2[i]);
    i = ((double)rand() / RAND_MAX) * (GRADATION_NUMBER - 0) + 0;
    ASSERT_EQ(grad_arr_1[i], grad_arr_2[i]);
}

TEST(MakeBorder, test3){
    int border = 0;
    int max_count = 1337;
    int ten_pow;
    border = make_border(max_count, &ten_pow);
    ASSERT_EQ(border, 1400);
    ASSERT_EQ(ten_pow, 100);
    max_count = 65999;
    border = make_border(max_count, &ten_pow);
    ASSERT_EQ(border, 66000);
    ASSERT_EQ(ten_pow, 1000);
    max_count = 1000001;
    border = make_border(max_count, &ten_pow);
    ASSERT_EQ(border, 1100000);
    ASSERT_EQ(ten_pow, 100000);
}

int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
