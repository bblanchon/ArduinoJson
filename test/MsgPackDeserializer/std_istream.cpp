// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack(std::istream&)") {
  DynamicJsonDocument doc;

  SECTION("should accept a zero in input") {
    std::istringstream input(std::string("\x92\x00\x02", 3));

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
    JsonArray arr = doc.as<JsonArray>();
    REQUIRE(arr[0] == 0);
    REQUIRE(arr[1] == 2);
  }

  SECTION("should detect incomplete input") {
    std::istringstream input("\x92\x00\x02");

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::IncompleteInput);
  }
}
