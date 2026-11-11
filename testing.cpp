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

TEST(Encode, OnlyLowestBitSet) {
    uint8_t data = 0x01;
    uint8_t cb[2];

    hamming_encode(&data, cb);

    EXPECT_EQ(cb[0], 0x00);   // upper nibble 0x0
    EXPECT_EQ(cb[1], 0x07);   // lower nibble 0x1
}

TEST(Encode, OnlyUpperNibbleBitSet) {
    uint8_t data = 0x10;
    uint8_t cb[2];

    hamming_encode(&data, cb);

    EXPECT_EQ(cb[0], 0x07);   // upper nibble 0x1
    EXPECT_EQ(cb[1], 0x00);   // lower nibble 0x0
}

TEST(Encode, AnotherKnownValue) {
    uint8_t data = 0x3C;
    uint8_t cb[2];

    hamming_encode(&data, cb);

    EXPECT_EQ(cb[0], 0x1E);   // upper nibble 0x3
    EXPECT_EQ(cb[1], 0x61);   // lower nibble 0xC
}

TEST(Decode, AllOnesNoError) {
    uint8_t cb[2] = {0x7F, 0x7F};
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xFF);
    EXPECT_EQ(errs, 0);
}

TEST(Decode, DataBitErrorInFirstCheckbyte) {
    uint8_t cb[2] = {0x12, 0x2D};   // 0x52 with bit 6 (d4) flipped
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xA5);
    EXPECT_EQ(errs, 1);
}

TEST(Decode, ParityBitErrorInSecondCheckbyte) {
    uint8_t cb[2] = {0x52, 0x25};   // 0x2D with bit 3 (p3) flipped
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xA5);
    EXPECT_EQ(errs, 1);
}

TEST(Decode, ZeroDataWithErrorInFirstCheckbyte) {
    uint8_t cb[2] = {0x04, 0x00};   // bit 2 (d1) flipped
    uint8_t data = 0xFF;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0x00);
    EXPECT_EQ(errs, 1);
}

TEST(Decode, ZeroDataWithErrorInSecondCheckbyte) {
    uint8_t cb[2] = {0x00, 0x40};   // bit 6 (d4) flipped
    uint8_t data = 0xFF;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0x00);
    EXPECT_EQ(errs, 1);
}

TEST(Decode, ZeroDataWithErrorInBothCheckbytes) {
    uint8_t cb[2] = {0x01, 0x02};   // p1 flipped in first, p2 flipped in second
    uint8_t data = 0xFF;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0x00);
    EXPECT_EQ(errs, 2);
}

TEST(Decode, AllOnesWithOneBitError) {
    uint8_t cb[2] = {0x6F, 0x7F};   // 0x7F with bit 4 (d2) flipped
    uint8_t data = 0;

    uint8_t errs = hamming_decode(cb, &data);

    EXPECT_EQ(data, 0xFF);
    EXPECT_EQ(errs, 1);
}

TEST(RoundTrip, Zero) {
    uint8_t data = 0x00;
    uint8_t cb[2];
    uint8_t out = 0xFF;

    hamming_encode(&data, cb);
    hamming_decode(cb, &out);

    EXPECT_EQ(out, 0x00);
}

TEST(RoundTrip, AllOnes) {
    uint8_t data = 0xFF;
    uint8_t cb[2];
    uint8_t out = 0;

    hamming_encode(&data, cb);
    hamming_decode(cb, &out);

    EXPECT_EQ(out, 0xFF);
}

TEST(RoundTrip, AlternatingBits) {
    uint8_t data = 0x5A;
    uint8_t cb[2];
    uint8_t out = 0;

    hamming_encode(&data, cb);
    hamming_decode(cb, &out);

    EXPECT_EQ(out, 0x5A);
}

TEST(RoundTrip, WithErrorInjected) {
    uint8_t data = 0x7E;
    uint8_t cb[2];
    uint8_t out = 0;

    hamming_encode(&data, cb);
    cb[0] ^= 0x20;   // flip bit 5 in first checkbyte
    cb[1] ^= 0x02;   // flip bit 1 in second checkbyte
    uint8_t errs = hamming_decode(cb, &out);

    EXPECT_EQ(out, 0x7E);
    EXPECT_EQ(errs, 2);
}