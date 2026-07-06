// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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
    object["ab"_s] = "AB";
    object["abc"_s] = "ABC";
    object["abcd"_s] = "ABCD";

    SECTION("string literal") {
      REQUIRE(var["ab"] == "AB"_s);
      REQUIRE(var["abc"] == "ABC"_s);
      REQUIRE(var["abcd"] == "ABCD"_s);
      REQUIRE(var["def"].isNull());
      REQUIRE(var[0].isNull());
    }

    SECTION("const char*") {
      REQUIRE(var[static_cast<const char*>("ab")] == "AB"_s);
      REQUIRE(var[static_cast<const char*>("abc")] == "ABC"_s);
      REQUIRE(var[static_cast<const char*>("abc\0d")] == "ABC"_s);
      REQUIRE(var[static_cast<const char*>("def")].isNull());
      REQUIRE(var[static_cast<const char*>(0)].isNull());
    }

    SECTION("supports std::string") {
      REQUIRE(var["ab"_s] == "AB"_s);
      REQUIRE(var["abc"_s] == "ABC"_s);
      REQUIRE(var["abcd"_s] == "ABCD"_s);
      REQUIRE(var["def"_s].isNull());
    }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
    SECTION("supports VLA") {
      size_t i = 16;
      char vla[i];
      strcpy(vla, "abc");

      REQUIRE(var[vla] == "ABC"_s);
    }
#endif

    SECTION("supports JsonVariant") {
      object["key1"] = "ab";
      object["key2"] = "abc";
      object["key3"] = "abcd"_s;
      object["key4"] = "foo";

      REQUIRE(var[var["key1"]] == "AB"_s);
      REQUIRE(var[var["key2"]] == "ABC"_s);
      REQUIRE(var[var["key3"]] == "ABCD"_s);
      REQUIRE(var[var["key4"]].isNull());
      REQUIRE(var[var["key5"]].isNull());
    }
  }
}
