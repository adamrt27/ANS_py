#include "encoder.h"
#include "decoder.h"
#include "coder.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>


int main(){
    // testing the coder
    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 256);
    for(int i = 0; i < 8; i ++){
        s_list[i] = i;
    }

    uint8_t L_s[8] = {50,40,60,50,30,24,1,1};

    coder *c = initCoder(256, s_list, L_s, 8);

    uint8_t msg[5] = {0, 1, 2, 3, 4};

    encodeCoder(c, msg, 5);

    printf("Encoded message: ");
    print_bitstream(c->e->bitstream, c->e->l_bitstream);

    decodeCoder(c);

    printf("Original message: ");
    for(int i = 0; i < 5; i ++){
        printf("%d ", msg[i]);
    }

    printf("\nDecoded message: ");
    for(int i = 0; i < 5; i ++){
        printf("%d ", c->d->msg[i]);
    }

    printf("\n");

    return 0;
}