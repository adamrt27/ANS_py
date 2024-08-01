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
    uint8_t *next = (uint8_t *)malloc(256 * sizeof(uint8_t));
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

size_t num_bytes_uint16(uint16_t val){
    if (val == 0) return 0;
    if (val <= 0xFF) return 1;
    return 2;
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

    printf("Log2(L): %d\n", (int)log2(table->L));

    printf("Original state: %d\n", state_orig);

    print_bitstream(d->bitstream, d->l_bitstream);

    // get the initial state, by reading log2(L) bits from the bitstream
    d->state = readBits(d, (int)log2(table->L));

    printf("Initial state: %d\n", d->state);

    print_bitstream(d->bitstream, d->l_bitstream);

    // iterate over bitstream decoding each symbol
    while((d->l_bitstream != 0) || d->state != state_orig){
        if (d->l_bitstream >= 0) {
            printf("l_bitstream: %ld, bitstream: ", d->l_bitstream);
            print_bitstream(d->bitstream, d->l_bitstream);
            printf(" Message: ");
            for (int i = 0; i < d->l_msg; i++) {
                printf("%d ", d->msg[i]);
            }
            printf("\n");
            printf("State: %d\n", d->state);

        }

        // reallocate d->msg and incrememnt l_msg
        d->l_msg ++;
        uint8_t *temp = (uint8_t *)realloc(d->msg, sizeof(uint8_t) * d->l_msg);
        if (temp == NULL) {
            printf("Memory reallocation failed\n");
            free(d->bitstream);
            free(d->msg);
            free(d);
            return NULL;
        }
        d->msg = temp;


        // decode a single symbol
        decodeStep(d, table);
    }

    return d;
}



#include <time.h>

int main() {

    // TODO: BITSTREAM READING IS WRONG
    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 256);
    for (int i = 0; i < 8; i++) {
        s_list[i] = i;
    }
    uint8_t L_s[8] = {50, 40, 60, 50, 30, 24, 1, 1};

    decodeTable *res = initDecodeTable(256, s_list, L_s, 8);

    uint8_t bs[4] = {0b11011101, 0b01110101,0b00100010, 0b0100};

    print_bitstream(bs, 3*8 + 4);

    printf("Testing get_n_bits function\n");
    printf("get_n_bits: %d\n", get_n_bits(bs, 3*8 + 4, 3, -1));
    printf("get_n_bits: %d\n", get_n_bits(bs, 3*8 + 4 - 8, 6, 1));

    print_bitstream(bs, 3*8 + 4);


    // long num_iter = 1000000;

    // // Warm-up iterations
    // for (int i = 0; i < 1000; i++) {
    //     decoder *d = decode(bs, 3*8 + 4, res);
    //     free(d->msg); // Free allocated memory to avoid memory leaks
    //     free(d);
    // }

    // struct timespec start_time, end_time;
    // clock_gettime(CLOCK_MONOTONIC, &start_time);

    // decoder *d = NULL;
    // for (int i = 0; i < num_iter; i++) {
    //     d = decode(bs, 3, res);
    //     //free(d->msg); // Free allocated memory to avoid memory leaks
    //     //free(d);
    // }

    // clock_gettime(CLOCK_MONOTONIC, &end_time);

    // double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    // printf("Elapsed time per iteration: %.6f microseconds\n", (elapsed_time * 1e6) / num_iter);

    decoder *d = decode(bs, 3*8 + 4, res);

    printf("Len of message: %d\n", d->l_bitstream);
    printf("Decoded message: ");
    for (int i = 0; i < d->l_msg; i++) {
        printf("%d ", d->msg[i]);
    }

    // Free the allocated memory
    free(s_list);
    free(res->table);
    free(res);

    return 0;
} 