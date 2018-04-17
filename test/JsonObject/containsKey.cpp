// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::containsKey()") {
  DynamicJsonDocument doc;
  JsonObject& obj = doc.to<JsonObject>();

  SECTION("ContainsKeyReturnsFalseForNonExistingKey") {
    obj.set("hello", 42);

    REQUIRE(false == obj.containsKey("world"));
  }

  SECTION("ContainsKeyReturnsTrueForDefinedValue") {
    obj.set("hello", 42);

    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("ContainsKeyReturnsFalseAfterRemove") {
    obj.set("hello", 42);
    obj.remove("hello");

    REQUIRE(false == obj.containsKey("hello"));
  }
}
