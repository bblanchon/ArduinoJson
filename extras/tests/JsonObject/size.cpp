// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <string>

TEST_CASE("JsonObject::size()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SUBCASE("initial size is zero") {
    REQUIRE(0 == obj.size());
  }

  SUBCASE("increases when values are added") {
    obj["hello"] = 42;
    REQUIRE(1 == obj.size());
  }

  SUBCASE("decreases when values are removed") {
    obj["hello"] = 42;
    obj.remove("hello");
    REQUIRE(0 == obj.size());
  }

  SUBCASE("doesn't increase when the same key is added twice") {
    obj["hello"] = 1;
    obj["hello"] = 2;
    REQUIRE(1 == obj.size());
  }

  SUBCASE("doesn't decrease when another key is removed") {
    obj["hello"] = 1;
    obj.remove("world");
    REQUIRE(1 == obj.size());
  }
}
