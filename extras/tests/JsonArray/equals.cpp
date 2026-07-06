// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::operator==()") {
  JsonDocument doc1;
  JsonArray array1 = doc1.to<JsonArray>();

  JsonDocument doc2;
  JsonArray array2 = doc2.to<JsonArray>();

  SECTION("should return false when arrays differ") {
    array1.add("coucou");
    array2.add(1);

    REQUIRE_FALSE(array1 == array2);
  }

  SECTION("should return false when LHS has more elements") {
    array1.add(1);
    array1.add(2);
    array2.add(1);

    REQUIRE_FALSE(array1 == array2);
  }

  SECTION("should return false when RHS has more elements") {
    array1.add(1);
    array2.add(1);
    array2.add(2);

    REQUIRE_FALSE(array1 == array2);
  }

  SECTION("should return true when arrays equal") {
    array1.add("coucou");
    array2.add("coucou");

    REQUIRE(array1 == array2);
  }

  SECTION("should return false when RHS is null") {
    JsonArray null;

    REQUIRE_FALSE(array1 == null);
  }

  SECTION("should return false when LHS is null") {
    JsonArray null;

    REQUIRE_FALSE(null == array1);
  }

  SECTION("should return true when both are null") {
    JsonArray null1;
    JsonArray null2;

    REQUIRE(null1 == null2);
  }
}
