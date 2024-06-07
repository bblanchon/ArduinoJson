// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonDocument::containsKey()") {
  JsonDocument doc;

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

    REQUIRE(doc.containsKey("hello"_s) == true);
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

  SECTION("support JsonVariant") {
    doc["hello"] = "world";
    doc["key"] = "hello";

    REQUIRE(doc.containsKey(doc["key"]) == true);
    REQUIRE(doc.containsKey(doc["foo"]) == false);
  }
}
