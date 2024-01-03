// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <stdint.h>
#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("canConvertNumber<TOut, TIn>()") {
  SECTION("int8_t -> int8_t") {
    CHECK(canConvertNumber<int8_t, int8_t>(0) == true);
    CHECK(canConvertNumber<int8_t, int8_t>(127) == true);
    CHECK(canConvertNumber<int8_t, int8_t>(-128) == true);
  }

  SECTION("int8_t -> int16_t") {
    CHECK(canConvertNumber<int16_t, int8_t>(0) == true);
    CHECK(canConvertNumber<int16_t, int8_t>(127) == true);
    CHECK(canConvertNumber<int16_t, int8_t>(-128) == true);
  }

  SECTION("int8_t -> uint8_t") {
    CHECK(canConvertNumber<uint8_t, int8_t>(0) == true);
    CHECK(canConvertNumber<uint8_t, int8_t>(127) == true);
    CHECK(canConvertNumber<uint8_t, int8_t>(-128) == false);
  }

  SECTION("int8_t -> uint16_t") {
    CHECK(canConvertNumber<uint16_t, int8_t>(0) == true);
    CHECK(canConvertNumber<uint16_t, int8_t>(127) == true);
    CHECK(canConvertNumber<uint16_t, int8_t>(-128) == false);
  }

  SECTION("int16_t -> int8_t") {
    CHECK(canConvertNumber<int8_t, int16_t>(0) == true);
    CHECK(canConvertNumber<int8_t, int16_t>(127) == true);
    CHECK(canConvertNumber<int8_t, int16_t>(128) == false);
    CHECK(canConvertNumber<int8_t, int16_t>(-128) == true);
    CHECK(canConvertNumber<int8_t, int16_t>(-129) == false);
  }

  SECTION("int16_t -> uint8_t") {
    CHECK(canConvertNumber<uint8_t, int16_t>(0) == true);
    CHECK(canConvertNumber<uint8_t, int16_t>(255) == true);
    CHECK(canConvertNumber<uint8_t, int16_t>(256) == false);
    CHECK(canConvertNumber<uint8_t, int16_t>(-1) == false);
  }

  SECTION("uint8_t -> int8_t") {
    CHECK(canConvertNumber<int8_t, uint8_t>(0) == true);
    CHECK(canConvertNumber<int8_t, uint8_t>(127) == true);
    CHECK(canConvertNumber<int8_t, uint8_t>(128) == false);
    CHECK(canConvertNumber<int8_t, uint8_t>(255) == false);
  }

  SECTION("uint8_t -> int16_t") {
    CHECK(canConvertNumber<int16_t, uint8_t>(0) == true);
    CHECK(canConvertNumber<int16_t, uint8_t>(127) == true);
    CHECK(canConvertNumber<int16_t, uint8_t>(128) == true);
    CHECK(canConvertNumber<int16_t, uint8_t>(255) == true);
  }

  SECTION("uint8_t -> uint8_t") {
    CHECK(canConvertNumber<uint8_t, uint8_t>(0) == true);
    CHECK(canConvertNumber<uint8_t, uint8_t>(127) == true);
    CHECK(canConvertNumber<uint8_t, uint8_t>(128) == true);
    CHECK(canConvertNumber<uint8_t, uint8_t>(255) == true);
  }

  SECTION("uint8_t -> uint16_t") {
    CHECK(canConvertNumber<uint16_t, uint8_t>(0) == true);
    CHECK(canConvertNumber<uint16_t, uint8_t>(127) == true);
    CHECK(canConvertNumber<uint16_t, uint8_t>(128) == true);
    CHECK(canConvertNumber<uint16_t, uint8_t>(255) == true);
  }

  SECTION("float -> int32_t") {
    CHECK(canConvertNumber<int32_t, float>(0) == true);
    CHECK(canConvertNumber<int32_t, float>(-2.147483904e9f) == false);
    CHECK(canConvertNumber<int32_t, float>(-2.147483648e+9f) == true);
    CHECK(canConvertNumber<int32_t, float>(2.14748352e+9f) == true);
    CHECK(canConvertNumber<int32_t, float>(2.14748365e+9f) == false);
  }

  SECTION("double -> int32_t") {
    CHECK(canConvertNumber<int32_t, double>(0) == true);
    CHECK(canConvertNumber<int32_t, double>(-2.147483649e+9) == false);
    CHECK(canConvertNumber<int32_t, double>(-2.147483648e+9) == true);
    CHECK(canConvertNumber<int32_t, double>(2.147483647e+9) == true);
    CHECK(canConvertNumber<int32_t, double>(2.147483648e+9) == false);
  }

  SECTION("float -> uint32_t") {
    CHECK(canConvertNumber<uint32_t, float>(0) == true);
    CHECK(canConvertNumber<uint32_t, float>(-1.401298e-45f) == false);
    CHECK(canConvertNumber<uint32_t, float>(4.29496704e+9f) == true);
    CHECK(canConvertNumber<uint32_t, float>(4.294967296e+9f) == false);
  }

  SECTION("float -> int64_t") {
    CHECK(canConvertNumber<int64_t, float>(0) == true);
    CHECK(canConvertNumber<int64_t, float>(-9.22337204e+18f) == true);
    CHECK(canConvertNumber<int64_t, float>(9.22337149e+18f) == true);
    CHECK(canConvertNumber<int64_t, float>(9.22337204e+18f) == false);
  }

  SECTION("double -> int64_t") {
    CHECK(canConvertNumber<int64_t, double>(0) == true);
    CHECK(canConvertNumber<int64_t, double>(-9.2233720368547758e+18) == true);
    CHECK(canConvertNumber<int64_t, double>(9.2233720368547748e+18) == true);
    CHECK(canConvertNumber<int64_t, double>(9.2233720368547758e+18) == false);
  }

  SECTION("float -> uint64_t") {
    CHECK(canConvertNumber<uint64_t, float>(0) == true);
    CHECK(canConvertNumber<uint64_t, float>(-1.401298e-45f) == false);
    CHECK(canConvertNumber<uint64_t, float>(1.84467429741979238e+19f) == true);
    CHECK(canConvertNumber<uint64_t, float>(1.844674407370955161e+19f) ==
          false);
  }

  SECTION("double -> uint64_t") {
    CHECK(canConvertNumber<uint64_t, double>(0) == true);
    CHECK(canConvertNumber<uint64_t, double>(-4.9406564584124e-324) == false);
    CHECK(canConvertNumber<uint64_t, double>(1.844674407370954958e+19) == true);
    CHECK(canConvertNumber<uint64_t, double>(1.844674407370955166e+19) ==
          false);
  }
}
