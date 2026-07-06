// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Literals.hpp"

TEST_CASE("JsonDocument::remove()") {
  JsonDocument doc;

  SUBCASE("remove(int)") {
    doc.add(1);
    doc.add(2);
    doc.add(3);

    doc.remove(1);

    REQUIRE(doc.as<std::string>() == "[1,3]");
  }

  SUBCASE("string literal") {
    doc["a"] = 1;
    doc["ab"_s] = 2;
    doc["b"] = 3;

    doc.remove("ab");

    REQUIRE(doc.as<std::string>() == "{\"a\":1,\"b\":3}");
  }

  SUBCASE("remove(const char *)") {
    doc["a"] = 1;
    doc["b"] = 2;

    doc.remove(static_cast<const char*>("a"));

    REQUIRE(doc.as<std::string>() == "{\"b\":2}");
  }

  SUBCASE("remove(std::string)") {
    doc["a"] = 1;
    doc["b"] = 2;

    doc.remove("b"_s);

    REQUIRE(doc.as<std::string>() == "{\"a\":1}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("remove(vla)") {
    doc["a"] = 1;
    doc["b"] = 2;

    size_t i = 4;
    char vla[i];
    strcpy(vla, "b");
    doc.remove(vla);

    REQUIRE(doc.as<std::string>() == "{\"a\":1}");
  }
#endif

  SUBCASE("remove(JsonVariant) from object") {
    doc["a"] = 1;
    doc["b"] = 2;
    doc["c"] = "b";

    doc.remove(doc["c"]);

    REQUIRE(doc.as<std::string>() == "{\"a\":1,\"c\":\"b\"}");
  }

  SUBCASE("remove(JsonVariant) from array") {
    doc[0] = 3;
    doc[1] = 2;
    doc[2] = 1;

    doc.remove(doc[2]);
    doc.remove(doc[3]);  // noop

    REQUIRE(doc.as<std::string>() == "[3,1]");
  }
}
