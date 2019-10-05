// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::remove()") {
  DynamicJsonDocument doc(4096);

  SECTION("remove(int)") {
    doc.add(1);
    doc.add(2);
    doc.add(3);

    doc.remove(1);

    REQUIRE(doc.as<std::string>() == "[1,3]");
  }

  SECTION("remove(const char *)") {
    doc["a"] = 1;
    doc["b"] = 2;

    doc.remove("a");

    REQUIRE(doc.as<std::string>() == "{\"b\":2}");
  }

  SECTION("remove(std::string)") {
    doc["a"] = 1;
    doc["b"] = 2;

    doc.remove(std::string("b"));

    REQUIRE(doc.as<std::string>() == "{\"a\":1}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("remove(vla)") {
    doc["a"] = 1;
    doc["b"] = 2;

    int i = 4;
    char vla[i];
    strcpy(vla, "b");
    doc.remove(vla);

    REQUIRE(doc.as<std::string>() == "{\"a\":1}");
  }
#endif
}
