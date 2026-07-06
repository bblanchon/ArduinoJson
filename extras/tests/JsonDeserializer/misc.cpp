// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("deserializeJson() misc cases") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SUBCASE("null") {
    DeserializationError err = deserializeJson(doc, "null");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == false);
  }

  SUBCASE("true") {
    DeserializationError err = deserializeJson(doc, "true");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == true);
  }

  SUBCASE("false") {
    DeserializationError err = deserializeJson(doc, "false");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == false);
  }

  SUBCASE("Should clear the JsonVariant") {
    deserializeJson(doc, "[1,2,3]");
    spy.clearLog();

    deserializeJson(doc, "{}");

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofArray(3)),
                         });
  }
}
