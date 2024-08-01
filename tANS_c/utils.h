#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>

// Bit manipulation functions

// Define a macro for setting and getting bit positions
#define SET_BIT(byte, bit) ((byte) |= (1 << (bit)))
#define CLEAR_BIT(byte, bit) ((byte) &= ~(1 << (bit)))
#define GET_BIT(byte, bit) (((byte) >> (bit)) & 1)

// Function to set a bit in the bitstream
void set_bit(uint8_t *bitstream, size_t bit_pos);

// Function to get a bit from the bitstream
int get_bit(uint8_t *bitstream, size_t bit_pos);

// function to get n bits from the bitstream, starting at bit_pos, going in direction dir
int get_n_bits(uint8_t *bitstream, size_t bit_pos, size_t n, int dir);

// reverse the bits of a number, i.e. 0b1010 -> 0b0101
unsigned int reverse_bits(unsigned int num, unsigned int bit_size);

// Function to print the bitstream
void print_bitstream(uint8_t *bitstream, size_t num_bits);

uint8_t *fast_spread(int L, uint8_t *L_s, uint8_t n_sym, int X, float step);

// reverse array
void reverse_array(uint8_t *array, size_t size);

#endif // UTILS_H