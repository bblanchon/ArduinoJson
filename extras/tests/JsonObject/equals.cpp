// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::operator==()") {
  JsonDocument doc1;
  JsonObject obj1 = doc1.to<JsonObject>();

  JsonDocument doc2;
  JsonObject obj2 = doc2.to<JsonObject>();

  SECTION("should return false when objs differ") {
    obj1["hello"] = "coucou";
    obj2["world"] = 1;

    REQUIRE_FALSE(obj1 == obj2);
  }

  SECTION("should return false when LHS has more elements") {
    obj1["hello"] = "coucou";
    obj1["world"] = 666;
    obj2["hello"] = "coucou";

    REQUIRE_FALSE(obj1 == obj2);
  }

  SECTION("should return false when RKS has more elements") {
    obj1["hello"] = "coucou";
    obj2["hello"] = "coucou";
    obj2["world"] = 666;

    REQUIRE_FALSE(obj1 == obj2);
  }

  SECTION("should return true when objs equal") {
    obj1["hello"] = "world";
    obj1["anwser"] = 42;
    // insert in different order
    obj2["anwser"] = 42;
    obj2["hello"] = "world";

    REQUIRE(obj1 == obj2);
  }

  SECTION("should return false when RHS is null") {
    JsonObject null;

    REQUIRE_FALSE(obj1 == null);
  }

  SECTION("should return false when LHS is null") {
    JsonObject null;

    REQUIRE_FALSE(null == obj2);
  }
}
