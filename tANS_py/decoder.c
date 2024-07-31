// Implementation of ANS decoding functions, where 
// L = 256, and symbols are 8-bit unsigned integers.

#include "decoder.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


uint8_t *fast_spread(decodeTable *table, int X, float step) {
    // initialize the symbol spread array
    uint8_t *sym_spread = (uint8_t *)malloc(sizeof(uint8_t) * table->L);

    // create the spread
    for(int s = 0; s < table->n_sym; s ++) {
        for(int _ = 0; _ < table->L_s[s]; _ ++){
            sym_spread[X] = s;
            X = (uint8_t)(X + step) % table->L;
        }
    }

    return sym_spread;
}

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
    uint8_t *sym_spread = fast_spread(table, 0, (int)((5.0/8.0) * table->L + 3));
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

    // get the nb LSB
    int bits = d->cur_bitstream & ((1 << nb) - 1);

    // update the bitstream by getting rid of the nb LSB
    d->cur_bitstream = d->cur_bitstream >> nb;

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

    // load the first two bytes from bitstream to cur_bitstream
    if (l_bitstream >= 2) {
        d->cur_bitstream = d->bitstream[0] + (d->bitstream[1] << 8);
        d->l_bitstream -= 2;
        d->bitstream += 2;
    } else {
        if (l_bitstream == 1) {
            d->cur_bitstream = d->bitstream[0];
            d->l_bitstream --;
            d->bitstream += 1;
        } else {
            printf("Error: bitstream is empty\n");
            return NULL;
        }
    }

    // get original state, by reading log2(L) bits from the bitstream
    uint8_t state_orig;
    state_orig = readBits(d, (int)log2(table->L));

    // get the initial state, by reading log2(L) bits from the bitstream
    d->state = readBits(d, (int)log2(table->L));

    // iterate over bitstream decoding each symbol
    while((d->l_bitstream != 0 && d->cur_bitstream != 0) || d->state != state_orig){
        // check if cur_bitstream can handle another byte
        if (num_bytes_uint16(d->cur_bitstream) == 1 && d->l_bitstream > 0) {
            d->cur_bitstream += (d->bitstream[0] << 8);
            d->l_bitstream --;
            d->bitstream += 1;
        } else if (num_bytes_uint16(d->cur_bitstream) == 0 && d->l_bitstream > 0) {
            d->cur_bitstream += (d->bitstream[0]);
            d->l_bitstream --;
            d->bitstream += 1;
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
    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 256);
    for (int i = 0; i < 8; i++) {
        s_list[i] = i;
    }
    uint8_t L_s[8] = {50, 40, 60, 50, 30, 24, 1, 1};

    decodeTable *res = initDecodeTable(256, s_list, L_s, 8);

    uint8_t bs[3] = {48, 245, 1};

    long num_iter = 1000000;

    // Warm-up iterations
    for (int i = 0; i < 1000; i++) {
        decoder *d = decode(bs, 3, res);
        free(d->msg); // Free allocated memory to avoid memory leaks
        free(d);
    }

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    decoder *d = NULL;
    for (int i = 0; i < num_iter; i++) {
        d = decode(bs, 3, res);
        free(d->msg); // Free allocated memory to avoid memory leaks
        free(d);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Elapsed time per iteration: %.6f microseconds\n", (elapsed_time * 1e6) / num_iter);

    // Free the allocated memory
    free(s_list);
    free(res->table);
    free(res);

    return 0;
}