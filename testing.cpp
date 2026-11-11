#include <gtest/gtest.h>
#include <cstdint>
#include "Hamming.h"

TEST(Encode, AllZeros) {
    uint8_t data = 0x00;
    uint8_t cb[2];

    hamming_encode(&data, cb);

    EXPECT_EQ(cb[0], 0x00);
    EXPECT_EQ(cb[1], 0x00);
}

TEST(Encode, AllOnes) {
    uint8_t data = 0xFF;
    uint8_t cb[2];

    hamming_encode(&data, cb);

    EXPECT_EQ(cb[0], 0x7F);
    EXPECT_EQ(cb[1], 0x7F);
}

TEST(Encode, KnownValue) {
    uint8_t data = 0xA5;
    uint8_t cb[2];

    hamming_encode(&data, cb);

    EXPECT_EQ(cb[0], 0x52);   // upper nibble 0xA
    EXPECT_EQ(cb[1], 0x2D);   // lower nibble 0x5
}

TEST(Decode, NoError) {
    uint8_t cb[2] = {0x52, 0x2D};
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xA5);
    EXPECT_EQ(errs, 0);
}

TEST(Decode, OneBitErrorInFirstCheckbyte) {
    uint8_t cb[2] = {0x53, 0x2D};   // 0x52 with bit 0 flipped
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xA5);
    EXPECT_EQ(errs, 1);
}

TEST(Decode, OneBitErrorInSecondCheckbyte) {
    uint8_t cb[2] = {0x52, 0x3D};   // 0x2D with bit 4 flipped
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xA5);
    EXPECT_EQ(errs, 1);
}

TEST(Decode, OneBitErrorInBothCheckbytes) {
    uint8_t cb[2] = {0x53, 0x3D};
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xA5);
    EXPECT_EQ(errs, 2);
}

TEST(RoundTrip, EncodeThenDecode) {
    uint8_t data = 0x3C;
    uint8_t cb[2];
    uint8_t out = 0;

    hamming_encode(&data, cb);
    hamming_decode(cb, &out);

    EXPECT_EQ(out, 0x3C);
}