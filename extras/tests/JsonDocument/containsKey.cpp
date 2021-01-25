// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::containsKey()") {
  DynamicJsonDocument doc(4096);

  SECTION("returns true on object") {
    doc["hello"] = "world";

    REQUIRE(doc.containsKey("hello") == true);
  }

  SECTION("returns true when value is null") {
    doc["hello"] = static_cast<const char*>(0);

    REQUIRE(doc.containsKey("hello") == true);
  }

  SECTION("returns true when key is a std::string") {
    doc["hello"] = "world";

    REQUIRE(doc.containsKey(std::string("hello")) == true);
  }

  SECTION("returns false  on object") {
    doc["world"] = "hello";

    REQUIRE(doc.containsKey("hello") == false);
  }

  SECTION("returns false on array") {
    doc.add("hello");

    REQUIRE(doc.containsKey("hello") == false);
  }

  SECTION("returns false on null") {
    REQUIRE(doc.containsKey("hello") == false);
  }
}
