// Implementation of ANS decoding functions, where 
// L = 256, and symbols are 8-bit unsigned integers.

#include "decoder.h"
#include "utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

void createDecodeTable(decodeTable *table, uint8_t* symbol_spread) {
    // define next, which is a deep copy of L_s
    uint8_t *next = (uint8_t *)malloc(table->L * sizeof(uint8_t));
    for(int i = 0; i < table->n_sym; i++) {
        next[i] = table->L_s[i];
    }

    // Calculate R, which is log_2(L)
    uint8_t R = 0;
    int L_temp = table->L;
    while(L_temp > 1) {
        L_temp >>= 1;
        R++;
    }

    uint8_t x_tmp;

    // Generate the decoding table
    for(int X = 0; X < table->L; X ++){
        // set the x value
        table->table[X].x = X;

        // set the symbol value 
        table->table[X].sym = symbol_spread[X];

        // calculate x_tmp
        x_tmp = next[symbol_spread[X]];

        // increment next
        next[symbol_spread[X]] += 1;

        // calculate number of bits needed to represent x_tmp
        table->table[X].nb = (uint8_t)(R - floor(log2(x_tmp)));

        // calculate new X value
        table->table[X].newX = (x_tmp << table->table[X].nb) - table->L;
    }

    free(next);
}

decodeTable *initDecodeTable(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym) {
    decodeTable *table = (decodeTable *)malloc(sizeof(decodeTable));
    table->L = L;
    table->s_list = s_list;
    table->L_s = L_s;
    table->n_sym = n_sym;

    table->table = (decodeTableColumn *)malloc(L * sizeof(decodeTableColumn));
    uint8_t *sym_spread = fast_spread(table->L, table->L_s, table->n_sym, 0, (int)((5.0/8.0) * table->L + 3));
    createDecodeTable(table, sym_spread);

    free(sym_spread);

    return table;
}

void displayDecodeTable(decodeTable *table){
    printf("Decode Table:\n");
    printf("Index\tx\tsym\tnb\tnewX\n");
    for (int i = 0; i < table->L; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", i, table->table[i].x, table->table[i].sym, table->table[i].nb, table->table[i].newX);
    }
}

int readBits(decoder *d, int nb){
    // reads nb bits from bitstream in d
    if(nb == 0) {
        return 0;
    }

    // get the nb MSB of the bitstream
    int bits = get_n_bits(d->bitstream, d->l_bitstream - nb, nb, 1);

    bits = reverse_bits(bits, nb);

    // update the bitstream to get rid of the nb MSB, by decrementing l_bitstream
    d->l_bitstream -= nb;

    return bits;
}

void decodeStep(decoder *d, decodeTable *table){
    // get the symbol
    uint8_t s_decode = table->table[d->state].sym;

    // read bits from bitstream
    uint8_t bits = readBits(d, table->table[d->state].nb);
    
    // calculate next state
    d->state = table->table[d->state].newX + bits;

    // store the symbol in the message
    d->msg[d->l_msg - 1] = s_decode;
}

decoder *decode(uint8_t *bitstream, long l_bitstream, decodeTable *table){
    // initialize decoder d
    decoder *d = (decoder *)malloc(sizeof(decoder));
    d->bitstream = bitstream;
    d->l_bitstream = l_bitstream;
    d->msg = (uint8_t *)malloc(sizeof(uint8_t));
    d->l_msg = 0;

    // get original state, by reading log2(L) bits from the bitstream
    uint8_t state_orig;
    state_orig = readBits(d, (int)log2(table->L));

    // get the initial state, by reading log2(L) bits from the bitstream
    d->state = readBits(d, (int)log2(table->L));

    // iterate over bitstream decoding each symbol
    while((d->l_bitstream != 0) || d->state != state_orig){

        // reallocate d->msg and incrememnt l_msg
        d->l_msg ++;
        d->msg = (uint8_t *)realloc(d->msg, sizeof(uint8_t) * (d->l_msg + 1));

        // decode a single symbol
        decodeStep(d, table);
    }

    // reverse the message
    reverse_array(d->msg, d->l_msg);

    return d;
}

#include <time.h>

/*
int main() {

    // uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 256);
    // for (int i = 0; i < 8; i++) {
    //     s_list[i] = i;
    // }
    // uint8_t L_s[8] = {49,39,59,49,29,23,4,4};

    // uint8_t bs[6] = {0b10110100, 0b11011111, 0b10010001, 0b11011000, 0b11100101, 0b00000000};

    // uint8_t l_bs = 5*8 + 4;

    // read the info from file encoded_message.bin
    FILE *f = fopen("encoded_message.bin", "rb");
    if (f == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    // get the length of the bitstream
    long l_bs;
    fread(&l_bs, sizeof(long), 1, f);

    printf("Length of bitstream: %d\n", l_bs);

    // get the number of symbols
    uint8_t n_sym;
    fread(&n_sym, sizeof(uint8_t), 1, f);

    printf("Number of symbols: %d\n", n_sym);

    // get the symbols
    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * n_sym);
    fread(s_list, sizeof(uint8_t), n_sym, f);

    printf("Symbols: ");
    for (int i = 0; i < n_sym; i++) {
        printf("%d ", s_list[i]);
    }

    // get the L_s
    uint8_t L_s[n_sym];
    fread(L_s, sizeof(uint8_t), n_sym, f);

    printf("\nL_s: ");
    for (int i = 0; i < n_sym; i++) {
        printf("%d ", L_s[i]);
    }

    // read the bitstream
    uint8_t *bs = (uint8_t *)malloc(sizeof(uint8_t) * l_bs);
    fread(bs, sizeof(uint8_t), l_bs, f);
    fclose(f);


    // make the decode table
    decodeTable *res = initDecodeTable(256, s_list, L_s, 8);

    print_bitstream(bs, l_bs);

    long num_iter = 1000000;

    // Warm-up iterations
    for (int i = 0; i < 1000; i++) {
        decoder *d = decode(bs, l_bs, res);
    }

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    decoder *d = NULL;
    for (int i = 0; i < num_iter; i++) {
        d = decode(bs, l_bs, res);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time per iteration: %.6f mu s\n", (elapsed_time * 1e6) / num_iter);

    d = decode(bs, l_bs, res);

    printf("Len of message: %ld\n", d->l_bitstream);
    printf("Decoded message: ");
    for (int i = 0; i < d->l_msg; i++) {
        printf("%d ", d->msg[i]);
    }

    // Free the allocated memory
    free(s_list);
    free(res->table);
    free(res);

    return 0;
} */