// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::operator==()") {
  DynamicJsonDocument doc(4096);

  SECTION("same values") {
    doc["key1"] = "value";
    doc["key2"] = "value";
    REQUIRE(doc["key1"] == doc["key2"]);
    REQUIRE_FALSE(doc["key1"] != doc["key2"]);
  }

  SECTION("different values") {
    doc["key1"] = "value1";
    doc["key2"] = "value2";
    REQUIRE_FALSE(doc["key1"] == doc["key2"]);
    REQUIRE(doc["key1"] != doc["key2"]);
  }
}
