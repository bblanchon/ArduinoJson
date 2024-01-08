// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArrayConst::operator==()") {
  JsonDocument doc1;
  JsonArrayConst array1 = doc1.to<JsonArray>();

  JsonDocument doc2;
  JsonArrayConst array2 = doc2.to<JsonArray>();

  SECTION("should return false when arrays differ") {
    doc1.add("coucou");
    doc2.add(1);

    REQUIRE_FALSE(array1 == array2);
  }

  SECTION("should return false when LHS has more elements") {
    doc1.add(1);
    doc1.add(2);
    doc2.add(1);

    REQUIRE_FALSE(array1 == array2);
  }

  SECTION("should return false when RHS has more elements") {
    doc1.add(1);
    doc2.add(1);
    doc2.add(2);

    REQUIRE_FALSE(array1 == array2);
  }

  SECTION("should return true when arrays equal") {
    doc1.add("coucou");
    doc2.add("coucou");

    REQUIRE(array1 == array2);
  }

  SECTION("should return false when RHS is null") {
    JsonArrayConst null;

    REQUIRE_FALSE(array1 == null);
  }

  SECTION("should return false when LHS is null") {
    JsonArrayConst null;

    REQUIRE_FALSE(null == array1);
  }

  SECTION("should return true when both are null") {
    JsonArrayConst null1;
    JsonArrayConst null2;

    REQUIRE(null1 == null2);
  }
}
