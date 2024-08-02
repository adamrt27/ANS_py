#include "encoder.h"
#include "decoder.h"
#include "coder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

int compare_arrays(uint8_t *arr1, uint8_t *arr2, int l){
    for(int i = 0; i < l; i ++){
        if(arr1[i] != arr2[i]){
            return 0;
        }
    }
    return 1;
}

char *int_to_bool(int n){
    if(n == 0){
        return "False";
    }
    return "True";
}

#include <time.h>

int main(){
    // testing the coder
    uint8_t L_mul[3] = {4, 2, 1};

    int len_L_mul = 3;

    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 16);   

    for(int j= 0; j < len_L_mul; j ++) {

        int L = (int)(64 * L_mul[j]);

        printf("L: %d\n", L);

        for(int i = 0; i < 16; i ++){
            s_list[i] = i;
        }

        // define L_s
        uint8_t L_s[16] = {6, 1, 4, 4, 6, 1, 4, 6, 4, 2, 6, 6, 4, 6, 2, 2};
    
        // rescale L_s
        for (int i = 0; i < 16; i++) {
            L_s[i] = (uint8_t)(L_s[i] * L_mul[j]);
        }
        printf("Sum of L_s: %d\n", sum_arr_to_ind(L_s, 16, 16));

        uint8_t msg[34] = {0,3,2,3,2,1,2,3,4,7,3,1,2,3,4,5,6,7, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

        uint8_t l_msg = 34;

        // // track time
        // int n_iter = 10000;

        // struct timespec start_time, end_time;
        // clock_gettime(CLOCK_MONOTONIC, &start_time);

        // // start timer 
        // clock_gettime(CLOCK_MONOTONIC, &start_time);

        // for (int i = 0; i < n_iter; i++) {
        //     encodeDecode(c, msg, l_msg);
        // }

        // // end timer
        // clock_gettime(CLOCK_MONOTONIC, &end_time);

        // double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

        // printf("Time taken normal (microseconds): %f\n", elapsed_time * 1e6 / n_iter);

        // // use the python function

        // // time it
        // clock_gettime(CLOCK_MONOTONIC, &start_time);

        // // start timer
        // clock_gettime(CLOCK_MONOTONIC, &start_time);

        // for(int i = 0; i < n_iter; i ++){
        //     encodeDecodeWithInit(L, s_list, L_s, 16, msg, l_msg);
        // }

        // // end timer
        // clock_gettime(CLOCK_MONOTONIC, &end_time);

        // double elapsed_time_py = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

        // printf("Time taken with Python (microseconds): %f\n", elapsed_time_py * 1e6 / n_iter);

        printf("Compression ratio: %f\n\n", (float)l_msg * 4 / (float)encodeDecodeWithInit(L, s_list, L_s, 16, msg, l_msg));

    }

    free(s_list);

    return 0;
}