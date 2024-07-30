// Implementation of ANS decoding functions, where 
// L = 256, and symbols are 8-bit unsigned integers.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct encodeTable{
    int L;
    uint8_t *s_list;
    uint8_t *L_s;
    uint8_t *table;
    uint8_t n_sym;              // number of symbols

    uint8_t *k;
    uint8_t *nb;
    int *start;
} encodeTable;

typedef struct encoder{
    uint8_t state;
    uint8_t *bitstream;
    uint8_t n_bits_used;        // number of bits used in current block
    long l_bitstream;
    uint8_t *msg;
    int l_msg;
    int ind_msg;                // current index in message you are encoding
} encoder;

uint8_t *fast_spread(encodeTable *table, int X, float step) {
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

int sum_arr_to_ind(uint8_t *arr, uint8_t n_sym, uint8_t ind) {
    if (ind >= n_sym) {
        ind = n_sym - 1;
    }

    int sum = 0;
    for(uint8_t i = 0; i < ind; i ++) {
        sum += (int)arr[i];
    }
    return sum;
}

void createEncodeTable(encodeTable *table, const uint8_t *sym_spread) {
    // Calculate R as log2(L)
    uint8_t R = 0;
    int L_temp = table->L;
    while (L_temp > 1) {
        L_temp >>= 1;
        R++;
    }

    int r = R + 1;

    // Calculate k, nb, start and next arrays
    uint8_t *next = (uint8_t *)malloc(table->n_sym * sizeof(uint8_t));

    for (int i = 0; i < table->n_sym; i++) {
        table->k[i] = R - (uint8_t)floor(log2(table->L_s[i]));
        table->nb[i] = (table->k[i] << r) - (table->L_s[i] << table->k[i]);
        table->start[i] = (-table->L_s[i] + sum_arr_to_ind(table->L_s, table->n_sym, i));
        next[i] = table->L_s[i];
    }

    // Generate the encoding table
    for (int i = 0; i < table->L; i++) {
        uint8_t s = sym_spread[i];
        table->table[(int)table->start[s] + (int)next[s]] = i + table->L;
        next[s]++;
    }

    free(next);
}

encodeTable *initEncodeTable(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym) {
    encodeTable *table = (encodeTable *)malloc(sizeof(encodeTable));
    table->L = L;
    table->s_list = s_list;
    table->L_s = L_s;
    table->n_sym = n_sym;

    table->k = (uint8_t *)malloc(sizeof(uint8_t) * table->n_sym);
    table->nb = (uint8_t *)malloc(sizeof(uint8_t) * table->n_sym);
    table->start = (int *)malloc(sizeof(int) * table->n_sym);

    table->table = (uint8_t *)malloc(L * sizeof(uint8_t)); 
    uint8_t *sym_spread = fast_spread(table, 0, (int)((5.0/8.0) * table->L + 3));
    createEncodeTable(table, sym_spread);
    return table;
}

void displayEncodeTable(encodeTable *table){
    printf("Encoding Table:\n[");
    for (int i = 0; i < table->L; i ++){
        printf("%d, ", table->table[i]);
    }
    printf("\b\b]\n");
}

int useBits(encoder *e, int nb) {
    // get the first nb LSB from e->state
    if (nb == 0){
        return 0;
    }

    // get the nb LSB
    int bits = e->state & ((1<< nb) - 1);

    // update the state by getting rid of the nb LSB
    e->state = e->state >> nb;

    return bits;
}

void encode_step(encoder *e, encodeTable *table) {
    int r = log2(table->L * 2);

    // get current symbol to encode
    uint8_t s = e->msg[e->ind_msg];
    e->ind_msg ++;

    // get number of bits to append to bitstream
    int nb = (e->state + (table->nb[s])) >> r;

    // get bits to append to bitstream
    uint8_t temp = useBits(e, nb);

    // TODO: append to bitstream
    // check if current index in bitstream is going to overflow with new bits added
    if ((8 - e->n_bits_used) < nb) { // if bits are going to overflow
        // append as many as I can to current block
        e->bitstream[e->l_bitstream] += (temp && (1 << 8 - e->n_bits_used)) << e->n_bits_used;
        
        // increment block
        e->l_bitstream ++;
        // append remaining to new block 
        e->bitstream[e->l_bitstream] += temp >> (nb - (8 - e->n_bits_used));

        // update n_bits_used
        e->n_bits_used = nb - (8 - e->n_bits_used);
    } else {
        // append the bits to the current block
        e->bitstream[e->l_bitstream] += temp << e->n_bits_used;
        // update the number of bits used
        e->n_bits_used += nb;
        // check if block is full
        if (e->n_bits_used == 8) {
            // if so, increment block and reset n_bits_used
            e->l_bitstream ++;
            e->n_bits_used = 0;
        }
    }

    // update state
    e->state = table->table[table->start[s] + e->state];
}

encoder *encode(uint8_t *msg, int l_msg, encodeTable *table){
    // initialize an encoder 
    encoder *e = (encoder *)malloc(sizeof(encoder));
    e->bitstream = (uint8_t *)malloc(sizeof(uint8_t));
    e->l_bitstream = 0;
    e->ind_msg = 0;
    e->l_msg = l_msg;
    e->msg = msg;
    e->state = 0;
    e->n_bits_used = 0;

    // initialize the state
    encode_step(e, table);

    // save the original state
    int state_orig = e->state;

    // reset all values modified by encode_step
    e->bitstream = (uint8_t *)malloc(sizeof(uint8_t));
    e->l_bitstream = 0;
    e->ind_msg = 0;
    e->n_bits_used = 0;

    int s;

    // go through msg and encode
    for(int i = 0; i < e->l_msg; i ++){
        s = table->s_list[e->msg[i]];
        encode_step(e, table);
    }

    // encode final state
    

}

int main(){

    uint8_t *s_list = (uint8_t *)malloc(sizeof(uint8_t) * 256);
    for(int i = 0; i < 8; i ++){
        s_list[i] = i;
    }
    uint8_t L_s[8] = {50,40,60,50,30,24,1,1};

    int length = sizeof(L_s) / sizeof(L_s[0]); // Calculate the number of elements in the array
    int sum = 0;

    // Loop through the array and calculate the sum
    for (int i = 0; i < length; i++) {
        sum += L_s[i];
    }

    printf("The sum of L_s is %d\n", sum);

    encodeTable *res = initEncodeTable(256, s_list, L_s, 8);
}