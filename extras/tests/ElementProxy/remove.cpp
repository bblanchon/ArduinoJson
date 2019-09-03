// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ElementProxy::remove()") {
  DynamicJsonDocument doc(4096);
  doc.addElement();
  ElementProxy<JsonDocument&> ep = doc[0];

  SECTION("remove(int)") {
    ep.add(1);
    ep.add(2);
    ep.add(3);

    ep.remove(1);

    REQUIRE(ep.as<std::string>() == "[1,3]");
  }

  SECTION("remove(const char *)") {
    ep["a"] = 1;
    ep["b"] = 2;

    ep.remove("a");

    REQUIRE(ep.as<std::string>() == "{\"b\":2}");
  }

  SECTION("remove(std::string)") {
    ep["a"] = 1;
    ep["b"] = 2;

    ep.remove(std::string("b"));

    REQUIRE(ep.as<std::string>() == "{\"a\":1}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("remove(vla)") {
    ep["a"] = 1;
    ep["b"] = 2;

    int i = 4;
    char vla[i];
    strcpy(vla, "b");
    ep.remove(vla);

    REQUIRE(ep.as<std::string>() == "{\"a\":1}");
  }
#endif
}
