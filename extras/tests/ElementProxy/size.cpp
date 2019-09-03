// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ElementProxy::size()") {
  DynamicJsonDocument doc(4096);
  doc.addElement();
  ElementProxy<JsonDocument&> ep = doc[0];

  SECTION("returns 0") {
    REQUIRE(ep.size() == 0);
  }

  SECTION("as an array, returns 2") {
    ep.add(1);
    ep.add(2);
    REQUIRE(ep.size() == 2);
  }

  SECTION("as an object, returns 2") {
    ep["a"] = 1;
    ep["b"] = 2;
    REQUIRE(ep.size() == 2);
  }
}
