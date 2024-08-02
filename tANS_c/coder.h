#ifndef CODER_H
#define CODER_H

typedef struct coder{
    int L;
    uint8_t *s_list;
    uint8_t *L_s;
    uint8_t n_sym;
    encoder *e;
    encodeTable *e_table;
    decoder *d;
    decodeTable *d_table;
} coder;

coder *initCoder(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym); // initialize the coder
void encodeCoder(coder *c, uint8_t *msg, int l_msg); // encode the message
void decodeCoder(coder *c); // decode the bitstream
int encodeDecode(coder *c, uint8_t *msg, int l_msg); // encodes and decodes the message, returns the number of bits in the bitstream
int encodeDecodeWithInit(int L, uint8_t *s_list, uint8_t *L_s, uint8_t n_sym, uint8_t *msg, int l_msg); // encodes and decodes the message using the python function
void freeCoder(coder *c); // free the coder

#endif // CODER_H