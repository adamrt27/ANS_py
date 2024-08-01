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

int main(){
    // testing the coder
    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 256);
    for(int i = 0; i < 16; i ++){
        s_list[i] = i;
    }

    uint8_t L_s[16] = {19, 4, 8, 33, 8, 18, 4, 10, 19, 8, 30, 4, 8, 38, 26, 19};

    coder *c = initCoder(256, s_list, L_s, 16);

    uint8_t msg[34] = {0,3,2,3,2,1,2,3,4,7,3,1,2,3,4,5,6,7, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    uint8_t l_msg = 34;

    encodeCoder(c, msg, l_msg);

    printf("Encoded message: ");
    print_bitstream(c->e->bitstream, c->e->l_bitstream);

    decodeCoder(c);

    printf("Original message: ");
    for(int i = 0; i < l_msg; i ++){
        printf("%d ", msg[i]);
    }

    printf("\nDecoded message: ");
    for(int i = 0; i < l_msg; i ++){
        printf("%d ", c->d->msg[i]);
    }

    printf("\nCoding Worked: %s\n", int_to_bool(compare_arrays(msg, c->d->msg, l_msg)));

    printf("\nBitstream: ");
    print_bitstream(c->e->bitstream, c->e->l_bitstream);

    printf("\nCompression ratio: %f\n", (float)l_msg * 4 / (float)c->e->l_bitstream);

    return 0;
}