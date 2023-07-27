// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("deserializeJson() misc cases") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("null") {
    DeserializationError err = deserializeJson(doc, "null");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == false);
  }

  SECTION("true") {
    DeserializationError err = deserializeJson(doc, "true");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == true);
  }

  SECTION("false") {
    DeserializationError err = deserializeJson(doc, "false");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == false);
  }

  SECTION("Should clear the JsonVariant") {
    deserializeJson(doc, "[1,2,3]");
    deserializeJson(doc, "{}");

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Deallocate(sizeofPool()),
                         });
  }
}
