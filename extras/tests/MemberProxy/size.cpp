// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::size()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument&, const char*> mp = doc["hello"];

  SECTION("returns 0") {
    REQUIRE(mp.size() == 0);
  }

  SECTION("as an array, return 2") {
    mp.add(1);
    mp.add(2);

    REQUIRE(mp.size() == 2);
  }

  SECTION("as an object, return 2") {
    mp["a"] = 1;
    mp["b"] = 2;

    REQUIRE(mp.size() == 2);
  }
}
