// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

class __FlashStringHelper;

TEST_CASE("JsonVariant::operator[]") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("The JsonVariant is undefined") {
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

  SECTION("The JsonVariant is a const __FlashStringHelper*") {
    const __FlashStringHelper *value = reinterpret_cast<const __FlashStringHelper *>("hello world");
    var.set(value);
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
      // REQUIRE(std::string("element at index 0") == );
      REQUIRE(std::string("element at index 1") == var[1]);
      REQUIRE(std::string("element at index 0") ==
              var[static_cast<unsigned char>(0)]);  // issue #381
      REQUIRE(var[666].isNull());
      REQUIRE(var[3].isNull());
      REQUIRE(var["0"].isNull());
    }

    SECTION("set value as a string") {
      array.add("hello");

      var[0] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var[0]);
    }

    SECTION("set value as a const __FlashStringHelper*") {
      const __FlashStringHelper *value = reinterpret_cast<const __FlashStringHelper *>("world");

      array.add("hello");

      var[0] = value;

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var[0]);
    }

    SECTION("set value in a nested object") {
      array.createNestedObject();

      var[0]["hello"] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(1 == var[0].size());
      REQUIRE(std::string("world") == var[0]["hello"]);
    }
  }

  SECTION("The JsonVariant is a JsonObject") {
    JsonObject object = var.to<JsonObject>();

    SECTION("get value") {
      object["a"] = "element at key \"a\"";
      object["b"] = "element at key \"b\"";

      REQUIRE(2 == var.size());
      REQUIRE(std::string("element at key \"a\"") == var["a"]);
      REQUIRE(std::string("element at key \"b\"") == var["b"]);
      REQUIRE(var["c"].isNull());
      REQUIRE(var[0].isNull());
    }

    SECTION("set value, key is a const char*") {
      var["hello"] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var["hello"]);
    }

    SECTION("set value, key is a char[]") {
      char key[] = "hello";
      var[key] = "world";
      key[0] = '!';  // make sure the key is duplicated

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var["hello"]);
    }

    SECTION("set value, key is a const __FlashStringHelper*") {
      const __FlashStringHelper *key = reinterpret_cast<const __FlashStringHelper *>("hello");
      var[key] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var[key]);
    }

    SECTION("var[key].to<JsonArray>()") {
      JsonArray arr = var["hello"].to<JsonArray>();
      REQUIRE(arr.isNull() == false);
    }
  }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("key is a VLA") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    deserializeJson(doc, "{\"hello\":\"world\"}");
    JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE(std::string("world") == variant[vla]);
  }

  SECTION("key is a VLA, const JsonVariant") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    deserializeJson(doc, "{\"hello\":\"world\"}");
    const JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE(std::string("world") == variant[vla]);
  }
#endif
}

TEST_CASE("JsonVariantConst::operator[]") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();
  JsonVariantConst cvar = var;

  SECTION("The JsonVariant is undefined") {
    REQUIRE(0 == cvar.size());
    REQUIRE(cvar["0"].isNull());
    REQUIRE(cvar[0].isNull());
  }

  SECTION("The JsonVariant is a string") {
    var.set("hello world");
    REQUIRE(0 == cvar.size());
    REQUIRE(cvar["0"].isNull());
    REQUIRE(cvar[0].isNull());
  }

  SECTION("The JsonVariant is a JsonArray") {
    JsonArray array = var.to<JsonArray>();

    SECTION("get value") {
      array.add("element at index 0");
      array.add("element at index 1");

      REQUIRE(2 == cvar.size());
      REQUIRE(std::string("element at index 0") == cvar[0]);
      REQUIRE(std::string("element at index 1") == cvar[1]);
      REQUIRE(std::string("element at index 0") ==
              var[static_cast<unsigned char>(0)]);  // issue #381
      REQUIRE(cvar[666].isNull());
      REQUIRE(cvar[3].isNull());
      REQUIRE(cvar["0"].isNull());
    }
  }

  SECTION("The JsonVariant is a JsonObject") {
    JsonObject object = var.to<JsonObject>();

    SECTION("get value") {
      object["a"] = "element at key \"a\"";
      object["b"] = "element at key \"b\"";

      REQUIRE(2 == cvar.size());
      REQUIRE(std::string("element at key \"a\"") == cvar["a"]);
      REQUIRE(std::string("element at key \"b\"") == cvar["b"]);
      REQUIRE(cvar["c"].isNull());
      REQUIRE(cvar[0].isNull());
    }
  }

  SECTION("Auto promote null JsonVariant to JsonObject") {
    var["hello"] = "world";

    REQUIRE(var.is<JsonObject>() == true);
  }

  SECTION("Don't auto promote non-null JsonVariant to JsonObject") {
    var.set(42);
    var["hello"] = "world";

    REQUIRE(var.is<JsonObject>() == false);
  }

  SECTION("Don't auto promote null JsonVariant to JsonObject when reading") {
    const char* value = var["hello"];

    REQUIRE(var.is<JsonObject>() == false);
    REQUIRE(value == 0);
  }
}
