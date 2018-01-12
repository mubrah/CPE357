#include "part3.h"

int sum(int arr[], int len) {
    int result = 0;
    for (int i; i < len; i++) {
        result += arr[i];
    }
    return result;
}
