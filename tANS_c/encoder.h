#ifndef ENCODE_H
#define ENCODE_H

#include <stdlib.h>
#include <stdint.h>

// Encoding

// encode table struct
typedef struct encodeTable{
    int L;
    uint8_t *s_list;
    uint8_t *L_s;
    int *table;
    uint8_t n_sym;              // number of symbols

    uint8_t *k;
    int *nb;
    int *start;
} encodeTable;

// struct to handle data while encoding
typedef struct encoder{
    int state;
    uint8_t *bitstream;
    long bitstream_capacity;
    long l_bitstream;
    uint8_t *msg;
    int l_msg;
    int ind_msg;                // current index in message you are encoding
} encoder;

// Function declarations

// function to sum the array up to an index
int sum_arr_to_ind(uint8_t *arr, uint8_t n_sym, uint8_t ind);
// Function to initialize the encode table
encodeTable *initEncodeTable(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym);
// helper function to create the encode table
void createEncodeTable(encodeTable *table, const uint8_t *sym_spread);
// displays the encode table
void displayEncodeTable(encodeTable *table);
// helper function to append bits to the bitstream
int useBits(encoder *e, int nb);
// helper function to append bits to the bitstream
void append_to_bitstream(encoder *e, uint8_t bits, int nb);
// helper function to encode a step
void encode_step(encoder *e, encodeTable *table);
// function to encode the message
encoder *encode(uint8_t *msg, int l_msg, encodeTable *table);

#endif // ENCODE_H