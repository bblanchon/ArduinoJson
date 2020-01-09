// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ElementProxy::clear()") {
  DynamicJsonDocument doc(4096);
  doc.addElement();
  ElementProxy<JsonDocument&> ep = doc[0];

  SECTION("size goes back to zero") {
    ep.add(42);
    ep.clear();

    REQUIRE(ep.size() == 0);
  }

  SECTION("isNull() return true") {
    ep.add("hello");
    ep.clear();

    REQUIRE(ep.isNull() == true);
  }
}
