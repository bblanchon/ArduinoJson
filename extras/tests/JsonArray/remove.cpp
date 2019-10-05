// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::remove()") {
  DynamicJsonDocument doc(4096);
  JsonArray _array = doc.to<JsonArray>();
  _array.add(1);
  _array.add(2);
  _array.add(3);

  SECTION("RemoveFirstByIndex") {
    _array.remove(0);

    REQUIRE(2 == _array.size());
    REQUIRE(_array[0] == 2);
    REQUIRE(_array[1] == 3);
  }

  SECTION("RemoveMiddleByIndex") {
    _array.remove(1);

    REQUIRE(2 == _array.size());
    REQUIRE(_array[0] == 1);
    REQUIRE(_array[1] == 3);
  }

  SECTION("RemoveLastByIndex") {
    _array.remove(2);

    REQUIRE(2 == _array.size());
    REQUIRE(_array[0] == 1);
    REQUIRE(_array[1] == 2);
  }

  SECTION("RemoveFirstByIterator") {
    JsonArray::iterator it = _array.begin();
    _array.remove(it);

    REQUIRE(2 == _array.size());
    REQUIRE(_array[0] == 2);
    REQUIRE(_array[1] == 3);
  }

  SECTION("RemoveMiddleByIterator") {
    JsonArray::iterator it = _array.begin();
    ++it;
    _array.remove(it);

    REQUIRE(2 == _array.size());
    REQUIRE(_array[0] == 1);
    REQUIRE(_array[1] == 3);
  }

  SECTION("RemoveLastByIterator") {
    JsonArray::iterator it = _array.begin();
    ++it;
    ++it;
    _array.remove(it);

    REQUIRE(2 == _array.size());
    REQUIRE(_array[0] == 1);
    REQUIRE(_array[1] == 2);
  }
}
