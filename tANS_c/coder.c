// This file defines the coder which connects the
// encoder.c and decoder.c

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "encoder.h"
#include "decoder.h"
#include "coder.h"

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

void encodeCoder(coder *c, uint8_t *msg, int l_msg) {
    // encode the message
    c->e = encode(msg, l_msg, c->e_table);

    // save all relevant values for decoding
}

void decodeCoder(coder *c) {
    // decode the bitstream
    c->d = decode(c->e->bitstream, c->e->l_bitstream, c->d_table);
}