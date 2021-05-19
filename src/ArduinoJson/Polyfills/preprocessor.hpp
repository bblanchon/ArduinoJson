// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#define ARDUINOJSON_EXPAND6(a, b, c, d, e, f) a, b, c, d, e, f
#define ARDUINOJSON_EXPAND9(a, b, c, d, e, f, g, h, i) a, b, c, d, e, f, g, h, i
#define ARDUINOJSON_EXPAND18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, \
                             q, r)                                           \
  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r

#define ARDUINOJSON_CONCAT_(A, B) A##B
#define ARDUINOJSON_CONCAT2(A, B) ARDUINOJSON_CONCAT_(A, B)
#define ARDUINOJSON_CONCAT4(A, B, C, D) \
  ARDUINOJSON_CONCAT2(ARDUINOJSON_CONCAT2(A, B), ARDUINOJSON_CONCAT2(C, D))

#define ARDUINOJSON_HEX_DIGIT_0000() 0
#define ARDUINOJSON_HEX_DIGIT_0001() 1
#define ARDUINOJSON_HEX_DIGIT_0010() 2
#define ARDUINOJSON_HEX_DIGIT_0011() 3
#define ARDUINOJSON_HEX_DIGIT_0100() 4
#define ARDUINOJSON_HEX_DIGIT_0101() 5
#define ARDUINOJSON_HEX_DIGIT_0110() 6
#define ARDUINOJSON_HEX_DIGIT_0111() 7
#define ARDUINOJSON_HEX_DIGIT_1000() 8
#define ARDUINOJSON_HEX_DIGIT_1001() 9
#define ARDUINOJSON_HEX_DIGIT_1010() A
#define ARDUINOJSON_HEX_DIGIT_1011() B
#define ARDUINOJSON_HEX_DIGIT_1100() C
#define ARDUINOJSON_HEX_DIGIT_1101() D
#define ARDUINOJSON_HEX_DIGIT_1110() E
#define ARDUINOJSON_HEX_DIGIT_1111() F
#define ARDUINOJSON_HEX_DIGIT_(A, B, C, D) ARDUINOJSON_HEX_DIGIT_##A##B##C##D()
#define ARDUINOJSON_HEX_DIGIT(A, B, C, D) ARDUINOJSON_HEX_DIGIT_(A, B, C, D)
