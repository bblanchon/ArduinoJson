// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonVariant::operator[]") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("The JsonVariant is null") {
    REQUIRE(0 == var.size());
    REQUIRE(var["0"].isNull());
    REQUIRE(var[0].isNull());
  }

  SECTION("The JsonVariant is a string") {
    var.set("hello world");
    REQUIRE(0 == var.size());
    REQUIRE(var["0"].isNull());
    REQUIRE(var[0].isNull());
  }

  SECTION("The JsonVariant is a JsonArray") {
    JsonArray array = var.to<JsonArray>();

    SECTION("get value") {
      array.add("element at index 0");
      array.add("element at index 1");

      REQUIRE(2 == var.size());
      var[0].as<std::string>();
      // REQUIRE("element at index 0"_s == );
      REQUIRE("element at index 1"_s == var[1]);
      REQUIRE("element at index 0"_s ==
              var[static_cast<unsigned char>(0)]);  // issue #381
      REQUIRE(var[666].isNull());
      REQUIRE(var[3].isNull());
      REQUIRE(var["0"].isNull());
    }

    SECTION("set value") {
      array.add("hello");

      var[1] = "world";

      REQUIRE(var.size() == 2);
      REQUIRE("world"_s == var[1]);
    }

    SECTION("set value in a nested object") {
      array.add<JsonObject>();

      var[0]["hello"] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(1 == var[0].size());
      REQUIRE("world"_s == var[0]["hello"]);
    }

    SECTION("variant[0] when variant contains an integer") {
      var.set(123);

      var[0] = 345;  // no-op

      REQUIRE(var.is<int>());
      REQUIRE(var.as<int>() == 123);
    }

    SECTION("use JsonVariant as index") {
      array.add("A");
      array.add("B");
      array.add(1);

      REQUIRE(var[var[2]] == "B");
      REQUIRE(var[var[3]].isNull());
    }
  }

  SECTION("The JsonVariant is a JsonObject") {
    JsonObject object = var.to<JsonObject>();

    SECTION("get value") {
      object["a"] = "element at key \"a\"";
      object["b"] = "element at key \"b\"";

      REQUIRE(2 == var.size());
      REQUIRE("element at key \"a\""_s == var["a"]);
      REQUIRE("element at key \"b\""_s == var["b"]);
      REQUIRE(var["c"].isNull());
      REQUIRE(var[0].isNull());
    }

    SECTION("set value, key is a const char*") {
      var["hello"] = "world";

      REQUIRE(1 == var.size());
      REQUIRE("world"_s == var["hello"]);
    }

    SECTION("set value, key is a char[]") {
      char key[] = "hello";
      var[key] = "world";
      key[0] = '!';  // make sure the key is duplicated

      REQUIRE(1 == var.size());
      REQUIRE("world"_s == var["hello"]);
    }

    SECTION("var[key].to<JsonArray>()") {
      JsonArray arr = var["hello"].to<JsonArray>();
      REQUIRE(arr.isNull() == false);
    }

    SECTION("use JsonVariant as key") {
      object["a"] = "a";
      object["b"] = "b";
      object["c"] = "b";

      REQUIRE(var[var["c"]] == "b");
      REQUIRE(var[var["d"]].isNull());
    }
  }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("key is a VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    deserializeJson(doc, "{\"hello\":\"world\"}");
    JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE("world"_s == variant[vla]);
  }

  SECTION("key is a VLA, const JsonVariant") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    deserializeJson(doc, "{\"hello\":\"world\"}");
    const JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE("world"_s == variant[vla]);
  }
#endif
}
