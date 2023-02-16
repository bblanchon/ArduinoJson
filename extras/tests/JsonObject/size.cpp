// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::size()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();

  SECTION("initial size is zero") {
    REQUIRE(0 == obj.size());
  }

  SECTION("increases when values are added") {
    obj["hello"] = 42;
    REQUIRE(1 == obj.size());
  }

  SECTION("decreases when values are removed") {
    obj["hello"] = 42;
    obj.remove("hello");
    REQUIRE(0 == obj.size());
  }

  SECTION("doesn't increase when the same key is added twice") {
    obj["hello"] = 1;
    obj["hello"] = 2;
    REQUIRE(1 == obj.size());
  }

  SECTION("doesn't decrease when another key is removed") {
    obj["hello"] = 1;
    obj.remove("world");
    REQUIRE(1 == obj.size());
  }
}
