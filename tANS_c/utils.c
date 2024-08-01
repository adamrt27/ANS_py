#include "utils.h"
#include "encoder.h"
#include "decoder.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//* Bitstream functions

// Function to set a bit in the bitstream
void set_bit(uint8_t *bitstream, size_t bit_pos) {
    size_t byte_pos = bit_pos / 8;
    size_t bit_offset = bit_pos % 8;
    SET_BIT(bitstream[byte_pos], bit_offset);
}

// Function to get a bit from the bitstream
int get_bit(uint8_t *bitstream, size_t bit_pos) {
    size_t byte_pos = bit_pos / 8;
    size_t bit_offset = bit_pos % 8;
    return GET_BIT(bitstream[byte_pos], bit_offset);
}

// function to get n bits from the bitstream, starting at bit_pos, going in direction dir
int get_n_bits(uint8_t *bitstream, size_t bit_pos, size_t n, int dir) {
    int result = 0;
    for (size_t i = 0; i < n; ++i) {
        int bit = get_bit(bitstream, bit_pos + i * dir);
        //printf("%d ", bit);
        if (dir == 1) {
            result |= bit << i;
        } else {
            result |= bit << (n - 1 - i);
        }
    }
    return result;
}

unsigned int reverse_bits(unsigned int num, unsigned int bit_size) {
    unsigned int reversed_num = 0;
    for (unsigned int i = 0; i < bit_size; i++) {
        if (num & (1 << i)) {
            reversed_num |= (1 << (bit_size - 1 - i));
        }
    }
    return reversed_num;
}

// Function to print the bitstream
void print_bitstream(uint8_t *bitstream, size_t num_bits) {
    for (size_t i = 0; i < num_bits; ++i) {
        printf("%d", get_bit(bitstream, i));
        if ((i + 1) % 8 == 0) {
            printf(" "); // Print a space every byte for readability
        }
    }
    printf("\n");
}

//* Spread Functions

uint8_t *fast_spread(int L, uint8_t *L_s, uint8_t n_sym, int X, float step) {

    // Initialize the symbol spread array
    uint8_t *sym_spread = (uint8_t *)malloc(sizeof(uint8_t) * L);
    if (!sym_spread) {
        perror("Failed to allocate memory for sym_spread");
        exit(EXIT_FAILURE);
    }

    // Create the spread
    for (int s = 0; s < n_sym; s++) {
        for (int _ = 0; _ < L_s[s]; _++) {
            sym_spread[X] = s;
            X = (uint8_t)(X + step) % L;
        }
    }

    return sym_spread;
}

//* Other Functions

// function to reverse an array
void reverse_array(uint8_t *array, size_t size) {
    for (size_t i = 0; i < size / 2; ++i) {
        uint8_t temp = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = temp;
    }
}