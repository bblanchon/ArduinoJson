// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::remove()") {
  DynamicJsonDocument doc(4096);
  JsonArray array = doc.to<JsonArray>();
  array.add(1);
  array.add(2);
  array.add(3);

  SECTION("remove first by index") {
    array.remove(0);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 2);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove middle by index") {
    array.remove(1);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove last by index") {
    array.remove(2);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
  }

  SECTION("remove first by iterator") {
    JsonArray::iterator it = array.begin();
    array.remove(it);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 2);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove middle by iterator") {
    JsonArray::iterator it = array.begin();
    ++it;
    array.remove(it);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove last bty iterator") {
    JsonArray::iterator it = array.begin();
    ++it;
    ++it;
    array.remove(it);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
  }

  SECTION("In a loop") {
    for (JsonArray::iterator it = array.begin(); it != array.end(); ++it) {
      if (*it == 2)
        array.remove(it);
    }

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove by index on unbound reference") {
    JsonArray unboundArray;
    unboundArray.remove(20);
  }

  SECTION("remove by iterator on unbound reference") {
    JsonArray unboundArray;
    unboundArray.remove(unboundArray.begin());
  }
}
