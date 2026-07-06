// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObject::begin()/end()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["ab"] = 12;
  obj["cd"] = 34;

  SUBCASE("NonConstIterator") {
    JsonObject::iterator it = obj.begin();
    REQUIRE(obj.end() != it);
    REQUIRE(it->key() == "ab");
    REQUIRE(12 == it->value());
    ++it;
    REQUIRE(obj.end() != it);
    REQUIRE(it->key() == "cd");
    REQUIRE(34 == it->value());
    ++it;
    REQUIRE(obj.end() == it);
  }

  SUBCASE("Dereferencing end() is safe") {
    REQUIRE(obj.end()->key().isNull());
    REQUIRE(obj.end()->value().isNull());
  }

  SUBCASE("null JsonObject") {
    JsonObject null;
    REQUIRE(null.begin() == null.end());
  }
}
