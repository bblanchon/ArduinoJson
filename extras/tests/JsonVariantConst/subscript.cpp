// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariantConst::operator[]") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  SECTION("null") {
    REQUIRE(0 == var.size());
    REQUIRE(var["0"].isNull());
    REQUIRE(var[0].isNull());
  }

  SECTION("string") {
    doc.set("hello world");
    REQUIRE(0 == var.size());
    REQUIRE(var["0"].isNull());
    REQUIRE(var[0].isNull());
  }

  SECTION("array") {
    JsonArray array = doc.to<JsonArray>();
    array.add("A");
    array.add("B");

    REQUIRE(std::string("A") == var[0]);
    REQUIRE(std::string("B") == var[1]);
    REQUIRE(std::string("A") ==
            var[static_cast<unsigned char>(0)]);  // issue #381
    REQUIRE(var[666].isNull());
    REQUIRE(var[3].isNull());
    REQUIRE(var["0"].isNull());
  }

  SECTION("object") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = "A";
    object["b"] = "B";

    SECTION("supports const char*") {
      REQUIRE(std::string("A") == var["a"]);
      REQUIRE(std::string("B") == var["b"]);
      REQUIRE(var["c"].isNull());
      REQUIRE(var[0].isNull());
    }

    SECTION("supports std::string") {
      REQUIRE(std::string("A") == var[std::string("a")]);
      REQUIRE(std::string("B") == var[std::string("b")]);
      REQUIRE(var[std::string("c")].isNull());
    }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
    SECTION("supports VLA") {
      size_t i = 16;
      char vla[i];
      strcpy(vla, "a");

      REQUIRE(std::string("A") == var[vla]);
    }
#endif
  }
}
