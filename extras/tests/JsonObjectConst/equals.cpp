// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObjectConst::operator==()") {
  JsonDocument doc1;
  JsonObjectConst obj1 = doc1.to<JsonObject>();

  JsonDocument doc2;
  JsonObjectConst obj2 = doc2.to<JsonObject>();

  SUBCASE("should return false when objs differ") {
    doc1["hello"] = "coucou";
    doc2["world"] = 1;

    REQUIRE_FALSE(obj1 == obj2);
  }

  SUBCASE("should return false when LHS has more elements") {
    doc1["hello"] = "coucou";
    doc1["world"] = 666;
    doc2["hello"] = "coucou";

    REQUIRE_FALSE(obj1 == obj2);
  }

  SUBCASE("should return false when RKS has more elements") {
    doc1["hello"] = "coucou";
    doc2["hello"] = "coucou";
    doc2["world"] = 666;

    REQUIRE_FALSE(obj1 == obj2);
  }

  SUBCASE("should return true when objs equal") {
    doc1["hello"] = "world";
    doc1["anwser"] = 42;
    // insert in different order
    doc2["anwser"] = 42;
    doc2["hello"] = "world";

    REQUIRE(obj1 == obj2);
  }

  SUBCASE("should return false when RHS is null") {
    JsonObjectConst null;

    REQUIRE_FALSE(obj1 == null);
  }

  SUBCASE("should return false when LHS is null") {
    JsonObjectConst null;

    REQUIRE_FALSE(null == obj2);
  }

  SUBCASE("should return true when both are null") {
    JsonObjectConst null1, null2;

    REQUIRE(null1 == null2);
  }
}
