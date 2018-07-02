// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wvla-extension"
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvla"
#else
#define VLA_NOT_SUPPORTED
#endif

#ifndef VLA_NOT_SUPPORTED

TEST_CASE("Variable Length Array") {
  SECTION("deserializeJson()") {
    int i = 9;
    char vla[i];
    strcpy(vla, "{\"a\":42}");

    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    DeserializationError err = deserializeJson(doc, vla);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("deserializeMsgPack()") {
    int i = 16;
    char vla[i];
    memcpy(vla, "\xDE\x00\x01\xA5Hello\xA5world", 15);

    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    DeserializationError err = deserializeMsgPack(doc, vla);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("JsonVariant") {
    SECTION("constructor") {
      int i = 16;
      char vla[i];
      strcpy(vla, "42");

      JsonVariant variant(vla);

      REQUIRE(42 == variant.as<int>());
    }

    SECTION("operator=") {
      int i = 16;
      char vla[i];
      strcpy(vla, "42");

      JsonVariant variant(666);
      variant = vla;

      REQUIRE(42 == variant.as<int>());
    }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[]") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");
      JsonVariant variant = doc.as<JsonVariant>();

      REQUIRE(std::string("world") == variant[vla]);
    }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[] const") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");
      const JsonVariant variant = doc.as<JsonVariant>();

      REQUIRE(std::string("world") == variant[vla]);
    }
#endif

    SECTION("operator==") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      JsonVariant variant;
      variant = "hello";

      REQUIRE((vla == variant));
      REQUIRE((variant == vla));
      REQUIRE_FALSE((vla != variant));
      REQUIRE_FALSE((variant != vla));
    }

    SECTION("operator!=") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      JsonVariant variant;
      variant = "world";

      REQUIRE((vla != variant));
      REQUIRE((variant != vla));
      REQUIRE_FALSE((vla == variant));
      REQUIRE_FALSE((variant == vla));
    }
  }

  SECTION("JsonObject") {
#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[]") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj[vla] = "world";

      REQUIRE(std::string("world") == obj["hello"]);
    }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[] const") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");

      JsonObject obj = doc.as<JsonObject>();
      REQUIRE(std::string("world") == obj[vla]);
    }
#endif

    SECTION("get()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");

      JsonObject obj = doc.as<JsonObject>();
      REQUIRE(std::string("world") == obj.get<char*>(vla));
    }

    SECTION("set() key") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj.set(vla, "world");

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set() value") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj.set("hello", vla);

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set() key&value") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj.set(vla, vla);

      REQUIRE(std::string("world") == obj["world"]);
    }

    SECTION("containsKey()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");

      JsonObject obj = doc.as<JsonObject>();
      REQUIRE(true == obj.containsKey(vla));
    }

    SECTION("remove()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();
      obj.remove(vla);

      REQUIRE(0 == obj.size());
    }

    SECTION("is<T>()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      deserializeJson(doc, "{\"hello\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(true == obj.is<int>(vla));
    }

    SECTION("createNestedArray()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj.createNestedArray(vla);
    }

    SECTION("createNestedObject()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj.createNestedObject(vla);
    }
  }

  SECTION("JsonObjectSubscript") {
    SECTION("operator=") {  // issue #416
      int i = 32;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj["hello"] = vla;

      REQUIRE(std::string("world") == obj["hello"].as<char*>());
    }

    SECTION("set()") {
      int i = 32;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj["hello"].set(vla);

      REQUIRE(std::string("world") == obj["hello"].as<char*>());
    }
  }

  SECTION("JsonArray") {
    SECTION("add()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add(vla);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("set()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr.set(0, vla);

      REQUIRE(std::string("world") == arr[0]);
    }
  }

  SECTION("JsonArraySubscript") {
    SECTION("set()") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr[0].set(vla);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("operator=") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      DynamicJsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr[0] = vla;

      REQUIRE(std::string("world") == arr[0]);
    }
  }
}
#endif
