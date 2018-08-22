// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::is<T>()") {
  DynamicJsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["int"] = 42;
  obj["str"] = "hello";

  SECTION("is<int>(const char*)") {
    REQUIRE(true == obj.is<int>("int"));
    REQUIRE(false == obj.is<int>("str"));
  }

#if HAS_VARIABLE_LENGTH_ARRAY
  SECTION("is<T>(VLA)") {
    int i = 16;
    char vla[i];
    strcpy(vla, "int");

    REQUIRE(true == obj.is<int>(vla));
  }
#endif
}
