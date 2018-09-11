// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
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
    REQUIRE_THAT(it->key(), Equals("ab"));
    REQUIRE(12 == it->value());
    ++it;
    REQUIRE(obj.end() != it);
    REQUIRE_THAT(it->key(), Equals("cd"));
    REQUIRE(34 == it->value());
    ++it;
    REQUIRE(obj.end() == it);
  }

  // SECTION("ConstIterator") {
  //   const JsonObject const_object = obj;
  //   JsonObject::iterator it = const_object.begin();

  //   REQUIRE(const_object.end() != it);
  //   REQUIRE_THAT(it->key(), Equals("ab"));
  //   REQUIRE(12 == it->value());
  //   ++it;
  //   REQUIRE(const_object.end() != it);
  //   REQUIRE_THAT(it->key(), Equals("cd"));
  //   REQUIRE(34 == it->value());
  //   ++it;
  //   REQUIRE(const_object.end() == it);
  // }

  SECTION("Dereferencing end() is safe") {
    REQUIRE(obj.end()->key() == 0);
    REQUIRE(obj.end()->value().isNull());
  }
}
