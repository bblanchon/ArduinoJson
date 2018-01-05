// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::begin()/end()") {
  StaticJsonBuffer<JSON_OBJECT_SIZE(2)> jb;
  JsonObject& obj = jb.createObject();
  obj["ab"] = 12;
  obj["cd"] = 34;

  SECTION("NonConstIterator") {
    JsonObject::iterator it = obj.begin();
    REQUIRE(obj.end() != it);
    REQUIRE_THAT(it->key, Equals("ab"));
    REQUIRE(12 == it->value);
    it->key = "a.b";
    it->value = 1.2;
    ++it;
    REQUIRE(obj.end() != it);
    REQUIRE_THAT(it->key, Equals("cd"));
    REQUIRE(34 == it->value);
    it->key = "c.d";
    it->value = 3.4;
    ++it;
    REQUIRE(obj.end() == it);

    REQUIRE(2 == obj.size());
    REQUIRE(1.2 == obj["a.b"]);
    REQUIRE(3.4 == obj["c.d"]);
  }

  SECTION("ConstIterator") {
    const JsonObject& const_object = obj;
    JsonObject::const_iterator it = const_object.begin();

    REQUIRE(const_object.end() != it);
    REQUIRE_THAT(it->key, Equals("ab"));
    REQUIRE(12 == it->value);
    ++it;
    REQUIRE(const_object.end() != it);
    REQUIRE_THAT(it->key, Equals("cd"));
    REQUIRE(34 == it->value);
    ++it;
    REQUIRE(const_object.end() == it);
  }
}
