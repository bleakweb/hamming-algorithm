# Hamming(7,4) in C

Encodes a byte into two 7-bit Hamming checkbytes and decodes them back,
correcting single-bit errors in each checkbyte.

## API
- `void hamming_encode(const uint8_t *data, uint8_t *checkbytes);`
- `uint8_t hamming_decode(const uint8_t *checkbytes, uint8_t *data);`

## Build and test
Requires gcc, g++ and GoogleTest (`sudo apt install libgtest-dev`).

    make test