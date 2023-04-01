// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

TEST_CASE("JsonDocument::memoryUsage()") {
  JsonDocument doc(4096);

  SECTION("starts at zero") {
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("sizeofArray(0)") {
    doc.to<JsonArray>();
    REQUIRE(doc.memoryUsage() == sizeofArray(0));
  }

  SECTION("sizeofArray(1)") {
    doc.to<JsonArray>().add(42);
    REQUIRE(doc.memoryUsage() == sizeofArray(1));
  }

  SECTION("sizeofArray(1) + sizeofArray(0)") {
    doc.to<JsonArray>().createNestedArray();
    REQUIRE(doc.memoryUsage() == sizeofArray(1) + sizeofArray(0));
  }

  SECTION("Increases after adding value to array") {
    JsonArray arr = doc.to<JsonArray>();

    REQUIRE(doc.memoryUsage() == sizeofArray(0));
    arr.add(42);
    REQUIRE(doc.memoryUsage() == sizeofArray(1));
    arr.add(43);
    REQUIRE(doc.memoryUsage() == sizeofArray(2));
  }

  SECTION("Increases after adding value to object") {
    JsonObject obj = doc.to<JsonObject>();

    REQUIRE(doc.memoryUsage() == sizeofObject(0));
    obj["a"] = 1;
    REQUIRE(doc.memoryUsage() == sizeofObject(1));
    obj["b"] = 2;
    REQUIRE(doc.memoryUsage() == sizeofObject(2));
  }
}
