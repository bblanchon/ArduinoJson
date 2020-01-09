// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::operator==()") {
  DynamicJsonDocument doc1(4096);
  JsonObject obj1 = doc1.to<JsonObject>();
  JsonObjectConst obj1c = obj1;

  DynamicJsonDocument doc2(4096);
  JsonObject obj2 = doc2.to<JsonObject>();
  JsonObjectConst obj2c = obj2;

  SECTION("should return false when objs differ") {
    obj1["hello"] = "coucou";
    obj2["world"] = 1;

    REQUIRE_FALSE(obj1 == obj2);
    REQUIRE_FALSE(obj1c == obj2c);
  }

  SECTION("should return false when LHS has more elements") {
    obj1["hello"] = "coucou";
    obj1["world"] = 666;
    obj2["hello"] = "coucou";

    REQUIRE_FALSE(obj1 == obj2);
    REQUIRE_FALSE(obj1c == obj2c);
  }

  SECTION("should return false when RKS has more elements") {
    obj1["hello"] = "coucou";
    obj2["hello"] = "coucou";
    obj2["world"] = 666;

    REQUIRE_FALSE(obj1 == obj2);
    REQUIRE_FALSE(obj1c == obj2c);
  }

  SECTION("should return true when objs equal") {
    obj1["hello"] = "world";
    obj1["anwser"] = 42;
    // insert in different order
    obj2["anwser"] = 42;
    obj2["hello"] = "world";

    REQUIRE(obj1 == obj2);
    REQUIRE(obj1c == obj2c);
  }
}
