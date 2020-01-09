// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::clear()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument&, const char*> mp = doc["hello"];

  SECTION("size goes back to zero") {
    mp.add(42);
    mp.clear();

    REQUIRE(mp.size() == 0);
  }

  SECTION("isNull() return true") {
    mp.add("hello");
    mp.clear();

    REQUIRE(mp.isNull() == true);
  }
}
