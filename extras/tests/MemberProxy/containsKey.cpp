// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::containsKey()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument&, const char*> mp = doc["hello"];

  SECTION("containsKey(const char*)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey("key") == true);
    REQUIRE(mp.containsKey("key") == true);
  }

  SECTION("containsKey(std::string)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey(std::string("key")) == true);
    REQUIRE(mp.containsKey(std::string("key")) == true);
  }
}
