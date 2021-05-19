// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack() returns EmptyInput") {
  StaticJsonDocument<100> doc;

  SECTION("from sized buffer") {
    DeserializationError err = deserializeMsgPack(doc, "", 0);

    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SECTION("from stream") {
    std::istringstream input("");

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::EmptyInput);
  }
}
