#include <stdio.h>

#include "part3.h"

void test_calc_0() {
    int arr[1]= {0};
    printf(sum(arr, 0));
}

void test_calc_1() {
    int arr[6] = {0, 1, 2, 3, 4, 5};
    printf(sum(arr, 6));
}

void test_calc_2() {
    int arr[5] = {-2, -1, 0, 1, 2};
    printf(sum(arr, 5));
}

void test_calc_3() {
    int arr[6] = {0, -1, -2, -3, -4, -5}; 
    printf(sum(arr, 6));
}

void test_calc() {
    test_calc_0();
    test_calc_1();
    test_calc_2();
    test_calc_3();
}

int main(void){
   test_calc();

   return 0;
}
