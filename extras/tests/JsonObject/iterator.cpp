// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::begin()/end()") {
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["ab"] = 12;
  obj["cd"] = 34;

  SECTION("NonConstIterator") {
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

  SECTION("Dereferencing end() is safe") {
    REQUIRE(obj.end()->key().isNull());
    REQUIRE(obj.end()->value().isNull());
  }
}

TEST_CASE("JsonObjectConst::begin()/end()") {
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["ab"] = 12;
  obj["cd"] = 34;

  JsonObjectConst cobj = obj;

  SECTION("NonConstIterator") {
    JsonObjectConst::iterator it = cobj.begin();
    REQUIRE(cobj.end() != it);
    REQUIRE(it->key() == "ab");
    REQUIRE(12 == it->value());
    ++it;
    REQUIRE(cobj.end() != it);
    REQUIRE(it->key() == "cd");
    REQUIRE(34 == it->value());
    ++it;
    REQUIRE(cobj.end() == it);
  }

  SECTION("Dereferencing end() is safe") {
    REQUIRE(cobj.end()->key().isNull());
    REQUIRE(cobj.end()->value().isNull());
  }
}
