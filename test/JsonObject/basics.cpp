// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject basics") {
  DynamicJsonObject _object;

  SECTION("InitialSizeIsZero") {
    REQUIRE(0 == _object.size());
  }

  SECTION("SuccessIsTrue") {
    REQUIRE(_object.success());
  }
}
