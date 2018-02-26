// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::containsKey()") {
  DynamicJsonObject _object;

  SECTION("ContainsKeyReturnsFalseForNonExistingKey") {
    _object.set("hello", 42);

    REQUIRE(false == _object.containsKey("world"));
  }

  SECTION("ContainsKeyReturnsTrueForDefinedValue") {
    _object.set("hello", 42);

    REQUIRE(true == _object.containsKey("hello"));
  }

  SECTION("ContainsKeyReturnsFalseAfterRemove") {
    _object.set("hello", 42);
    _object.remove("hello");

    REQUIRE(false == _object.containsKey("hello"));
  }
}
