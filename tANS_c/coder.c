// This file defines the coder which connects the
// encoder.c and decoder.c

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "encoder.h"
#include "decoder.h"
#include "coder.h"
#include "utils.h"

coder *initCoder(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym) {
    coder *c = (coder *)malloc(sizeof(coder));
    c->L = L;
    c->s_list = s_list;
    c->L_s = L_s;
    c->n_sym = n_sym;

    // initialize the encoder
    c->e_table = initEncodeTable(L, s_list, L_s, n_sym);

    // initialize the decoder
    c->d_table = initDecodeTable(L, s_list, L_s, n_sym);

    return c;
}

// Does Encoding with the Coder struct
void encodeCoder(coder *c, uint8_t *msg, int l_msg) {
    // encode the message
    c->e = encode(msg, l_msg, c->e_table);

    // save all relevant values for decoding
}

// Does Decoding with the Coder struct
void decodeCoder(coder *c) {
    // decode the bitstream
    c->d = decode(c->e->bitstream, c->e->l_bitstream, c->d_table);
}

// encodes and decodes the message, returns the number of bits in the bitstream
int encodeDecode(coder *c, uint8_t *msg, int l_msg) {
    // encode the message
    encodeCoder(c, msg, l_msg);

    // decode the message
    decodeCoder(c);

    // check if the message is the same
    for(int i = 0; i < c->e->l_msg; i ++){
        if(c->e->msg[i] != c->d->msg[i]){
            printf("Error: %d %d\n", c->e->msg[i], c->d->msg[i]);
            return -1;
        }
    }

    // return number of bits in the bitstream
    return c->e->l_bitstream;
}

int encodeDecodeWithInit(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym, uint8_t *msg, int l_msg) {
    coder *c = initCoder(L, s_list, L_s, n_sym);
    int res = encodeDecode(c, msg, l_msg);

    freeCoder(c);

    return res;
}

void freeCoder(coder *c) {
    free(c->e_table->table);
    free(c->e_table->k);
    free(c->e_table->nb);
    free(c->e_table->start);
    free(c->e_table);

    free(c->d_table->table);
    free(c->d_table);

    free(c->e->bitstream);
    free(c->e);

    free(c->d->msg);
    free(c->d);
    free(c);
}
