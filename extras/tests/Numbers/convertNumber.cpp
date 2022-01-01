// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <stdint.h>
#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("canConvertNumber<TOut, TIn>()") {
  SECTION("int8_t -> int8_t") {
    CHECK((canConvertNumber<int8_t, int8_t>(0)) == true);
    CHECK((canConvertNumber<int8_t, int8_t>(127)) == true);
    CHECK((canConvertNumber<int8_t, int8_t>(-128)) == true);
  }

  SECTION("int8_t -> int16_t") {
    CHECK((canConvertNumber<int16_t, int8_t>(0)) == true);
    CHECK((canConvertNumber<int16_t, int8_t>(127)) == true);
    CHECK((canConvertNumber<int16_t, int8_t>(-128)) == true);
  }

  SECTION("int8_t -> uint8_t") {
    CHECK((canConvertNumber<uint8_t, int8_t>(0)) == true);
    CHECK((canConvertNumber<uint8_t, int8_t>(127)) == true);
    CHECK((canConvertNumber<uint8_t, int8_t>(-128)) == false);
  }

  SECTION("int8_t -> uint16_t") {
    CHECK((canConvertNumber<uint16_t, int8_t>(0)) == true);
    CHECK((canConvertNumber<uint16_t, int8_t>(127)) == true);
    CHECK((canConvertNumber<uint16_t, int8_t>(-128)) == false);
  }

  SECTION("int16_t -> int8_t") {
    CHECK((canConvertNumber<int8_t, int16_t>(0)) == true);
    CHECK((canConvertNumber<int8_t, int16_t>(127)) == true);
    CHECK((canConvertNumber<int8_t, int16_t>(128)) == false);
    CHECK((canConvertNumber<int8_t, int16_t>(-128)) == true);
    CHECK((canConvertNumber<int8_t, int16_t>(-129)) == false);
  }

  SECTION("int16_t -> uint8_t") {
    CHECK((canConvertNumber<uint8_t, int16_t>(0)) == true);
    CHECK((canConvertNumber<uint8_t, int16_t>(255)) == true);
    CHECK((canConvertNumber<uint8_t, int16_t>(256)) == false);
    CHECK((canConvertNumber<uint8_t, int16_t>(-1)) == false);
  }

  SECTION("uint8_t -> int8_t") {
    CHECK((canConvertNumber<int8_t, uint8_t>(0)) == true);
    CHECK((canConvertNumber<int8_t, uint8_t>(127)) == true);
    CHECK((canConvertNumber<int8_t, uint8_t>(128)) == false);
    CHECK((canConvertNumber<int8_t, uint8_t>(255)) == false);
  }

  SECTION("uint8_t -> int16_t") {
    CHECK((canConvertNumber<int16_t, uint8_t>(0)) == true);
    CHECK((canConvertNumber<int16_t, uint8_t>(127)) == true);
    CHECK((canConvertNumber<int16_t, uint8_t>(128)) == true);
    CHECK((canConvertNumber<int16_t, uint8_t>(255)) == true);
  }

  SECTION("uint8_t -> uint8_t") {
    CHECK((canConvertNumber<uint8_t, uint8_t>(0)) == true);
    CHECK((canConvertNumber<uint8_t, uint8_t>(127)) == true);
    CHECK((canConvertNumber<uint8_t, uint8_t>(128)) == true);
    CHECK((canConvertNumber<uint8_t, uint8_t>(255)) == true);
  }

  SECTION("uint8_t -> uint16_t") {
    CHECK((canConvertNumber<uint16_t, uint8_t>(0)) == true);
    CHECK((canConvertNumber<uint16_t, uint8_t>(127)) == true);
    CHECK((canConvertNumber<uint16_t, uint8_t>(128)) == true);
    CHECK((canConvertNumber<uint16_t, uint8_t>(255)) == true);
  }
}
