#ifndef HAMMING_H
#define HAMMING_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// data -> 1 byte in, checkbytes -> 2 bytes out ([0] = upper nibble, [1] = lower nibble)
void hamming_encode(const uint8_t *data, uint8_t *checkbytes);

// checkbytes -> 2 bytes in, data -> 1 byte out (single-bit errors corrected)
uint8_t hamming_decode(const uint8_t *checkbytes, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif
