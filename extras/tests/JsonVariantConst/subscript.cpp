// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

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

    SECTION("int") {
      REQUIRE("A"_s == var[0]);
      REQUIRE("B"_s == var[1]);
      REQUIRE("A"_s == var[static_cast<unsigned char>(0)]);  // issue #381
      REQUIRE(var[666].isNull());
      REQUIRE(var[3].isNull());
    }

    SECTION("const char*") {
      REQUIRE(var["0"].isNull());
    }

    SECTION("JsonVariant") {
      array.add(1);
      REQUIRE(var[var[2]] == "B"_s);
      REQUIRE(var[var[3]].isNull());
    }
  }

  SECTION("object") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = "A";
    object["b"] = "B";

    SECTION("supports const char*") {
      REQUIRE("A"_s == var["a"]);
      REQUIRE("B"_s == var["b"]);
      REQUIRE(var["c"].isNull());
      REQUIRE(var[0].isNull());
    }

    SECTION("supports std::string") {
      REQUIRE("A"_s == var["a"_s]);
      REQUIRE("B"_s == var["b"_s]);
      REQUIRE(var["c"_s].isNull());
    }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
    SECTION("supports VLA") {
      size_t i = 16;
      char vla[i];
      strcpy(vla, "a");

      REQUIRE("A"_s == var[vla]);
    }
#endif

    SECTION("supports JsonVariant") {
      object["c"] = "b";
      REQUIRE(var[var["c"]] == "B");
      REQUIRE(var[var["d"]].isNull());
    }
  }
}
