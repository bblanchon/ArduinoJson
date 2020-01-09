// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::set()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument&, const char*> mp = doc["hello"];

  SECTION("set(int)") {
    mp.set(42);

    REQUIRE(doc.as<std::string>() == "{\"hello\":42}");
  }

  SECTION("set(const char*)") {
    mp.set("world");

    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }
}
