// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

#if defined(__clang__)
#  define CONFLICTS_WITH_BUILTIN_OPERATOR
#endif

TEST_CASE("unsigned char[]") {
  SECTION("deserializeJson()") {
    unsigned char input[] = "{\"a\":42}";

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("deserializeMsgPack()") {
    unsigned char input[] = "\xDE\x00\x01\xA5Hello\xA5world";

    JsonDocument doc;
    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("serializeMsgPack(unsigned char[])") {
    unsigned char buffer[32];
    JsonDocument doc;
    doc["hello"] = "world";

    size_t n = serializeMsgPack(doc, buffer);

    REQUIRE(n == 13);
    REQUIRE(memcmp(buffer, "\x81\xA5hello\xA5world", 13) == 0);
  }

  SECTION("serializeMsgPack(unsigned char*)") {
    unsigned char buffer[32];
    JsonDocument doc;
    doc["hello"] = "world";

    size_t n = serializeMsgPack(doc, buffer, sizeof(buffer));

    REQUIRE(n == 13);
    REQUIRE(memcmp(buffer, "\x81\xA5hello\xA5world", 13) == 0);
  }

  SECTION("serializeJson(unsigned char[])") {
    unsigned char buffer[32];
    JsonDocument doc;
    doc["hello"] = "world";

    size_t n = serializeJson(doc, buffer);

    REQUIRE(n == 17);
    REQUIRE(memcmp(buffer, "{\"hello\":\"world\"}", n) == 0);
  }

  SECTION("serializeJson(unsigned char*)") {
    unsigned char buffer[32];
    JsonDocument doc;
    doc["hello"] = "world";

    size_t n = serializeJson(doc, buffer, sizeof(buffer));

    REQUIRE(n == 17);
    REQUIRE(memcmp(buffer, "{\"hello\":\"world\"}", n) == 0);
  }

  SECTION("serializeJsonPretty(unsigned char[])") {
    unsigned char buffer[32];
    JsonDocument doc;
    doc["hello"] = "world";

    size_t n = serializeJsonPretty(doc, buffer);

    REQUIRE(n == 24);
  }

  SECTION("serializeJsonPretty(unsigned char*)") {
    unsigned char buffer[32];
    JsonDocument doc;
    doc["hello"] = "world";

    size_t n = serializeJsonPretty(doc, buffer, sizeof(buffer));

    REQUIRE(n == 24);
  }

  SECTION("JsonVariant") {
    JsonDocument doc;

    SECTION("set") {
      unsigned char value[] = "42";

      JsonVariant variant = doc.to<JsonVariant>();
      variant.set(value);

      REQUIRE(42 == variant.as<int>());
    }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[]") {
      unsigned char key[] = "hello";

      deserializeJson(doc, "{\"hello\":\"world\"}");
      JsonVariant variant = doc.as<JsonVariant>();

      REQUIRE("world"_s == variant[key]);
    }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[] const") {
      unsigned char key[] = "hello";

      deserializeJson(doc, "{\"hello\":\"world\"}");
      const JsonVariant variant = doc.as<JsonVariant>();

      REQUIRE("world"_s == variant[key]);
    }
#endif

    SECTION("operator==") {
      unsigned char comparand[] = "hello";

      JsonVariant variant = doc.to<JsonVariant>();
      variant.set("hello");

      REQUIRE(comparand == variant);
      REQUIRE(variant == comparand);
      REQUIRE_FALSE(comparand != variant);
      REQUIRE_FALSE(variant != comparand);
    }

    SECTION("operator!=") {
      unsigned char comparand[] = "hello";

      JsonVariant variant = doc.to<JsonVariant>();
      variant.set("world");

      REQUIRE(comparand != variant);
      REQUIRE(variant != comparand);
      REQUIRE_FALSE(comparand == variant);
      REQUIRE_FALSE(variant == comparand);
    }
  }

  SECTION("JsonObject") {
#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[]") {
      unsigned char key[] = "hello";

      JsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj[key] = "world";

      REQUIRE("world"_s == obj["hello"]);
    }

    SECTION("JsonObject::operator[] const") {
      unsigned char key[] = "hello";

      JsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");

      JsonObject obj = doc.as<JsonObject>();
      REQUIRE("world"_s == obj[key]);
    }
#endif

    SECTION("remove()") {
      unsigned char key[] = "hello";

      JsonDocument doc;
      deserializeJson(doc, "{\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();
      obj.remove(key);

      REQUIRE(0 == obj.size());
    }
  }

  SECTION("MemberProxy") {
    SECTION("operator=") {  // issue #416
      unsigned char value[] = "world";

      JsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj["hello"] = value;

      REQUIRE("world"_s == obj["hello"]);
    }

    SECTION("set()") {
      unsigned char value[] = "world";

      JsonDocument doc;
      JsonObject obj = doc.to<JsonObject>();
      obj["hello"].set(value);

      REQUIRE("world"_s == obj["hello"]);
    }
  }

  SECTION("JsonArray") {
    SECTION("add()") {
      unsigned char value[] = "world";

      JsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add(value);

      REQUIRE("world"_s == arr[0]);
    }
  }

  SECTION("ElementProxy") {
    SECTION("set()") {
      unsigned char value[] = "world";

      JsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr[0].set(value);

      REQUIRE("world"_s == arr[0]);
    }

    SECTION("operator=") {
      unsigned char value[] = "world";

      JsonDocument doc;
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr[0] = value;

      REQUIRE("world"_s == arr[0]);
    }
  }
}
