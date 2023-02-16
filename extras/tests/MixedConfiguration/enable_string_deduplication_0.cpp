// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include "progmem_emulation.hpp"

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#define ARDUINOJSON_ENABLE_STRING_DEDUPLICATION 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_ENABLE_STRING_DEDUPLICATION = 0") {
  StaticJsonDocument<1024> doc;

  SECTION("deserializeJson()") {
    SECTION("Deduplicate values") {
      deserializeJson(doc, "[\"example\",\"example\"]");

      CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 16);
      CHECK(doc[0].as<const char*>() != doc[1].as<const char*>());
    }

    SECTION("Deduplicate keys") {
      deserializeJson(doc, "[{\"example\":1},{\"example\":2}]");

      CHECK(doc.memoryUsage() ==
            2 * JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(2) + 16);

      const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
      const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();

      CHECK(key1 != key2);
    }
  }

  SECTION("JsonDocument") {
    SECTION("values") {
      SECTION("std::string") {
        doc.add(std::string("example"));
        doc.add(std::string("example"));

        CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 16);
        CHECK(doc[0].as<const char*>() != doc[1].as<const char*>());
      }

      SECTION("char*") {
        char value[] = "example";
        doc.add(value);
        doc.add(value);

        CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 16);
        CHECK(doc[0].as<const char*>() != doc[1].as<const char*>());
      }

      SECTION("Arduino String") {
        doc.add(String("example"));
        doc.add(String("example"));

        CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 16);
        CHECK(doc[0].as<const char*>() != doc[1].as<const char*>());
      }

      SECTION("Flash string") {
        doc.add(F("example"));
        doc.add(F("example"));

        CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 16);
        CHECK(doc[0].as<const char*>() != doc[1].as<const char*>());
      }
    }

    SECTION("keys") {
      SECTION("std::string") {
        doc[0][std::string("example")] = 1;
        doc[1][std::string("example")] = 2;

        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 16);

        const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
        const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
        CHECK(key1 != key2);
      }

      SECTION("char*") {
        char key[] = "example";
        doc[0][key] = 1;
        doc[1][key] = 2;

        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 16);

        const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
        const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
        CHECK(key1 != key2);
      }

      SECTION("Arduino String") {
        doc[0][String("example")] = 1;
        doc[1][String("example")] = 2;

        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 16);

        const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
        const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
        CHECK(key1 != key2);
      }

      SECTION("Flash string") {
        doc[0][F("example")] = 1;
        doc[1][F("example")] = 2;

        CHECK(doc.memoryUsage() ==
              JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 16);

        const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
        const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
        CHECK(key1 != key2);
      }
    }
  }
}
