// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObjectConst::begin()/end()") {
  JsonDocument doc;
  JsonObjectConst obj = doc.to<JsonObject>();
  doc["ab"] = 12;
  doc["cd"] = 34;

  SECTION("Iteration") {
    JsonObjectConst::iterator it = obj.begin();
    REQUIRE(obj.end() != it);
    REQUIRE(it->key() == "ab");
    REQUIRE(12 == it->value());

    ++it;
    REQUIRE(obj.end() != it);
    JsonPairConst pair = *it;
    REQUIRE(pair.key() == "cd");
    REQUIRE(34 == pair.value());

    ++it;
    REQUIRE(obj.end() == it);
  }

  SECTION("Dereferencing end() is safe") {
    REQUIRE(obj.end()->key().isNull());
    REQUIRE(obj.end()->value().isNull());
  }

  SECTION("null JsonObjectConst") {
    JsonObjectConst null;
    REQUIRE(null.begin() == null.end());
  }
}
