#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include <stdlib.h>

// defines on column of the decodeTable
typedef struct decodeTableColumn {
    uint8_t x;
    uint8_t sym;
    uint8_t nb;
    uint8_t newX;
} decodeTableColumn;

// defines the entire decodeTable
typedef struct decodeTable {
    decodeTableColumn *table;
    int L;
    uint8_t *s_list;
    uint8_t *L_s;
    uint8_t n_sym;              // number of symbols
} decodeTable;

// used to hold values for decoding process
typedef struct decoder {
    uint8_t state;
    uint8_t *bitstream;
    long l_bitstream;           // the length of the bitstream
    uint8_t *msg;
    int l_msg;                  // length of message
} decoder;

// Function declarations

// initialize the decode table
decodeTable *initDecodeTable(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym);
// helper function to create the decode table
void createDecodeTable(decodeTable *table, uint8_t* symbol_spread);
// displays the decode table
void displayDecodeTable(decodeTable *table);
// helper function to read bits from the bitstream
int readBits(decoder *d, int nb);
// helper function to decode a step
void decodeStep(decoder *d, decodeTable *table);
// function to decode the bitstream
decoder *decode(uint8_t *bitstream, long l_bitstream, decodeTable *table);

#endif // DECODE_H