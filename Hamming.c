#include "Hamming.h"

// Bit index:  6   5   4   3   2   1   0
// checkbyte:  d4  d3  d2  p3  d1  p2  p1

static uint8_t encode_nibble(uint8_t d)
{
    uint8_t d1 = (d >> 0) & 1;
    uint8_t d2 = (d >> 1) & 1;
    uint8_t d3 = (d >> 2) & 1;
    uint8_t d4 = (d >> 3) & 1;

    uint8_t p1 = d1 ^ d2 ^ d4;
    uint8_t p2 = d1 ^ d3 ^ d4;
    uint8_t p3 = d2 ^ d3 ^ d4;

    return p1 | (p2 << 1) | (d1 << 2) | (p3 << 3) | (d2 << 4) | (d3 << 5) | (d4 << 6);
}

// 0 = no error, else (index of flipped bit + 1)
static uint8_t syndrome(uint8_t cb)
{
    uint8_t s1 = ((cb >> 0) & 1) ^ ((cb >> 2) & 1) ^ ((cb >> 4) & 1) ^ ((cb >> 6) & 1);
    uint8_t s2 = ((cb >> 1) & 1) ^ ((cb >> 2) & 1) ^ ((cb >> 5) & 1) ^ ((cb >> 6) & 1);
    uint8_t s3 = ((cb >> 3) & 1) ^ ((cb >> 4) & 1) ^ ((cb >> 5) & 1) ^ ((cb >> 6) & 1);

    return s1 | (s2 << 1) | (s3 << 2);
}

static uint8_t correct(uint8_t cb)
{
    uint8_t syn = syndrome(cb);

    if (syn != 0) {
        cb ^= (1 << (syn - 1));
    }
    return cb;
}

static uint8_t extract_nibble(uint8_t cb)
{
    uint8_t n = 0;

    n |= ((cb >> 2) & 1) << 0;  // d1
    n |= ((cb >> 4) & 1) << 1;  // d2
    n |= ((cb >> 5) & 1) << 2;  // d3
    n |= ((cb >> 6) & 1) << 3;  // d4

    return n;
}

void hamming_encode(const uint8_t *data, uint8_t *checkbytes)
{
    checkbytes[0] = encode_nibble(*data >> 4);   // upper nibble
    checkbytes[1] = encode_nibble(*data & 0x0F); // lower nibble
}

uint8_t hamming_decode(const uint8_t *checkbytes, uint8_t *data)
{
    uint8_t e0 = (syndrome(checkbytes[0]) != 0);
    uint8_t e1 = (syndrome(checkbytes[1]) != 0);

    uint8_t c0 = correct(checkbytes[0]);
    uint8_t c1 = correct(checkbytes[1]);

    *data = (extract_nibble(c0) << 4) | extract_nibble(c1);

    return e0 + e1;   // number of checkbytes that had an error (0, 1 or 2)
}