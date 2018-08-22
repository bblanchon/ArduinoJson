// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::get()") {
  DynamicJsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("get<const char*>(const char*)") {
    obj.set("hello", "world");
    const char* value = obj.get<const char*>("hello");
    REQUIRE_THAT(value, Equals("world"));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("get<const char*>(VLA)") {
    obj.set("hello", "world");
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(std::string("world") == obj.get<char*>(vla));
  }
#endif
}
