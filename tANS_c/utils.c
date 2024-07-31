#include "utils.h"
#include "encoder.h"
#include "decoder.h"
#include <stdint.h>
#include <stdio.h>

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