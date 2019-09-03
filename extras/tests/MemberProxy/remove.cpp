// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::remove()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument&, const char*> mp = doc["hello"];

  SECTION("remove(int)") {
    mp.add(1);
    mp.add(2);
    mp.add(3);

    mp.remove(1);

    REQUIRE(mp.as<std::string>() == "[1,3]");
  }

  SECTION("remove(const char *)") {
    mp["a"] = 1;
    mp["b"] = 2;

    mp.remove("a");

    REQUIRE(mp.as<std::string>() == "{\"b\":2}");
  }

  SECTION("remove(std::string)") {
    mp["a"] = 1;
    mp["b"] = 2;

    mp.remove(std::string("b"));

    REQUIRE(mp.as<std::string>() == "{\"a\":1}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("remove(vla)") {
    mp["a"] = 1;
    mp["b"] = 2;

    int i = 4;
    char vla[i];
    strcpy(vla, "b");
    mp.remove(vla);

    REQUIRE(mp.as<std::string>() == "{\"a\":1}");
  }
#endif
}
