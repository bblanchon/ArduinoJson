// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ElementProxy::set()") {
  DynamicJsonDocument doc(4096);
  doc.addElement();
  ElementProxy<JsonDocument&> ep = doc[0];

  SECTION("set(int)") {
    ep.set(42);

    REQUIRE(doc.as<std::string>() == "[42]");
  }

  SECTION("set(const char*)") {
    ep.set("world");

    REQUIRE(doc.as<std::string>() == "[\"world\"]");
  }
}
