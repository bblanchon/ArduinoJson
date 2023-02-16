// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#  define CONFLICTS_WITH_BUILTIN_OPERATOR
#endif

TEST_CASE("unsigned char[]") {
  SECTION("deserializeJson()") {
    unsigned char input[] = "{\"a\":42}";

    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("deserializeMsgPack()") {
    unsigned char input[] = "\xDE\x00\x01\xA5Hello\xA5world";

    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("serializeMsgPack(unsigned char[])") {
    unsigned char buffer[32];
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["hello"] = "world";

    size_t n = serializeMsgPack(doc, buffer);

    REQUIRE(n == 13);
    REQUIRE(memcmp(buffer, "\x81\xA5hello\xA5world", 13) == 0);
  }

  SECTION("serializeMsgPack(unsigned char*)") {
    unsigned char buffer[32];
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["hello"] = "world";

    size_t n = serializeMsgPack(doc, buffer, sizeof(buffer));

    REQUIRE(n == 13);
    REQUIRE(memcmp(buffer, "\x81\xA5hello\xA5world", 13) == 0);
  }

  SECTION("serializeJson(unsigned char[])") {
    unsigned char buffer[32];
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["hello"] = "world";

    size_t n = serializeJson(doc, buffer);

    REQUIRE(n == 17);
    REQUIRE(memcmp(buffer, "{\"hello\":\"world\"}", n) == 0);
  }

  SECTION("serializeJson(unsigned char*)") {
    unsigned char buffer[32];
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["hello"] = "world";

    size_t n = serializeJson(doc, buffer, sizeof(buffer));

    REQUIRE(n == 17);
    REQUIRE(memcmp(buffer, "{\"hello\":\"world\"}", n) == 0);
  }

  SECTION("serializeJsonPretty(unsigned char[])") {
    unsigned char buffer[32];
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["hello"] = "world";

    size_t n = serializeJsonPretty(doc, buffer);

    REQUIRE(n == 24);
  }

  SECTION("serializeJsonPretty(unsigned char*)") {
    unsigned char buffer[32];
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["hello"] = "world";

    size_t n = serializeJsonPretty(doc, buffer, sizeof(buffer));

    REQUIRE(n == 24);
  }

  SECTION("JsonVariant") {
    DynamicJsonDocument doc(4096);

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

      REQUIRE(std::string("world") == variant[key]);
    }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[] const") {
      unsigned char key[] = "hello";

      deserializeJson(doc, "{\"hello\":\"world\"}");
      const JsonVariant variant = doc.as<JsonVariant>();

      REQUIRE(std::string("world") == variant[key]);
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

      DynamicJsonDocument doc(4096);
      JsonObject obj = doc.to<JsonObject>();
      obj[key] = "world";

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("JsonObject::operator[] const") {
      unsigned char key[] = "hello";

      DynamicJsonDocument doc(4096);
      deserializeJson(doc, "{\"hello\":\"world\"}");

      JsonObject obj = doc.as<JsonObject>();
      REQUIRE(std::string("world") == obj[key]);
    }
#endif

    SECTION("containsKey()") {
      unsigned char key[] = "hello";

      DynamicJsonDocument doc(4096);
      deserializeJson(doc, "{\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();
      REQUIRE(true == obj.containsKey(key));
    }

    SECTION("remove()") {
      unsigned char key[] = "hello";

      DynamicJsonDocument doc(4096);
      deserializeJson(doc, "{\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();
      obj.remove(key);

      REQUIRE(0 == obj.size());
    }

    SECTION("createNestedArray()") {
      unsigned char key[] = "hello";

      DynamicJsonDocument doc(4096);
      JsonObject obj = doc.to<JsonObject>();
      obj.createNestedArray(key);
    }

    SECTION("createNestedObject()") {
      unsigned char key[] = "hello";

      DynamicJsonDocument doc(4096);
      JsonObject obj = doc.to<JsonObject>();
      obj.createNestedObject(key);
    }
  }

  SECTION("MemberProxy") {
    SECTION("operator=") {  // issue #416
      unsigned char value[] = "world";

      DynamicJsonDocument doc(4096);
      JsonObject obj = doc.to<JsonObject>();
      obj["hello"] = value;

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set()") {
      unsigned char value[] = "world";

      DynamicJsonDocument doc(4096);
      JsonObject obj = doc.to<JsonObject>();
      obj["hello"].set(value);

      REQUIRE(std::string("world") == obj["hello"]);
    }
  }

  SECTION("JsonArray") {
    SECTION("add()") {
      unsigned char value[] = "world";

      DynamicJsonDocument doc(4096);
      JsonArray arr = doc.to<JsonArray>();
      arr.add(value);

      REQUIRE(std::string("world") == arr[0]);
    }
  }

  SECTION("ElementProxy") {
    SECTION("set()") {
      unsigned char value[] = "world";

      DynamicJsonDocument doc(4096);
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr[0].set(value);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("operator=") {
      unsigned char value[] = "world";

      DynamicJsonDocument doc(4096);
      JsonArray arr = doc.to<JsonArray>();
      arr.add("hello");
      arr[0] = value;

      REQUIRE(std::string("world") == arr[0]);
    }
  }
}
