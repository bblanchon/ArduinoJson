// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void eraseString(std::string &str) {
  char *p = const_cast<char *>(str.c_str());
  while (*p) *p++ = '*';
}

TEST_CASE("std::string") {
  SECTION("deserializeJson duplicates content") {
    std::string json("[\"hello\"]");

    DynamicJsonDocument doc;
    JsonError err = deserializeJson(doc, json);
    eraseString(json);

    JsonArray &array = doc.as<JsonArray>();
    REQUIRE(err == JsonError::Ok);
    REQUIRE(std::string("hello") == array[0]);
  }

  SECTION("JsonArray") {
    DynamicJsonDocument doc;
    JsonArray &array = doc.to<JsonArray>();

    SECTION("add()") {
      std::string value("hello");
      array.add(value);
      eraseString(value);
      REQUIRE(std::string("hello") == array[0]);
    }

    SECTION("set()") {
      std::string value("world");
      array.add("hello");
      array.set(0, value);
      eraseString(value);
      REQUIRE(std::string("world") == array[0]);
    }

    SECTION("operator[]") {
      std::string value("world");
      array.add("hello");
      array[0] = value;
      eraseString(value);
      REQUIRE(std::string("world") == array[0]);
    }

    SECTION("serializeJson()") {
      array.add(4);
      array.add(2);
      std::string json;
      serializeJson(array, json);
      REQUIRE(std::string("[4,2]") == json);
    }

    SECTION("serializeJsonPretty()") {
      array.add(4);
      array.add(2);
      std::string json;
      serializeJsonPretty(array, json);
      REQUIRE(std::string("[\r\n  4,\r\n  2\r\n]") == json);
    }
  }

  SECTION("JsonObject") {
    DynamicJsonDocument doc;

    SECTION("deserializeJson()") {
      std::string json("{\"hello\":\"world\"}");

      JsonError err = deserializeJson(doc, json);
      JsonObject &obj = doc.as<JsonObject>();
      eraseString(json);

      REQUIRE(err == JsonError::Ok);
      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("operator[]") {
      char json[] = "{\"key\":\"value\"}";

      deserializeJson(doc, json);
      JsonObject &obj = doc.as<JsonObject>();

      REQUIRE(std::string("value") == obj[std::string("key")]);
    }

    SECTION("operator[] const") {
      char json[] = "{\"key\":\"value\"}";

      deserializeJson(doc, json);
      JsonObject &obj = doc.as<JsonObject>();

      REQUIRE(std::string("value") == obj[std::string("key")]);
    }

    SECTION("set(key)") {
      JsonObject &obj = doc.to<JsonObject>();
      std::string key("hello");
      obj.set(key, "world");
      eraseString(key);
      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(value)") {
      JsonObject &obj = doc.to<JsonObject>();
      std::string value("world");
      obj.set("hello", value);
      eraseString(value);
      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(key,value)") {
      JsonObject &obj = doc.to<JsonObject>();
      std::string key("hello");
      std::string value("world");
      obj.set(key, value);
      eraseString(key);
      eraseString(value);
      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(JsonArraySubscript)") {
      JsonObject &obj = doc.to<JsonObject>();
      DynamicJsonDocument doc2;
      JsonArray &arr = doc2.to<JsonArray>();
      arr.add("world");

      obj.set(std::string("hello"), arr[0]);

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(JsonObjectSubscript)") {
      JsonObject &obj = doc.to<JsonObject>();
      DynamicJsonDocument doc2;
      JsonObject &obj2 = doc2.to<JsonObject>();
      obj2.set("x", "world");

      obj.set(std::string("hello"), obj2["x"]);

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("get<T>()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);
      JsonObject &obj = doc.as<JsonObject>();

      REQUIRE(std::string("value") ==
              obj.get<const char *>(std::string("key")));
    }

    SECTION("is<T>()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);
      JsonObject &obj = doc.as<JsonObject>();

      REQUIRE(true == obj.is<const char *>(std::string("key")));
    }

    SECTION("createNestedObject()") {
      JsonObject &obj = doc.to<JsonObject>();
      std::string key = "key";
      char json[64];
      obj.createNestedObject(key);
      eraseString(key);
      serializeJson(doc, json, sizeof(json));
      REQUIRE(std::string("{\"key\":{}}") == json);
    }

    SECTION("createNestedArray()") {
      JsonObject &obj = doc.to<JsonObject>();
      std::string key = "key";
      char json[64];
      obj.createNestedArray(key);
      eraseString(key);
      serializeJson(doc, json, sizeof(json));
      REQUIRE(std::string("{\"key\":[]}") == json);
    }

    SECTION("containsKey()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(doc, json);
      JsonObject &obj = doc.as<JsonObject>();
      REQUIRE(true == obj.containsKey(std::string("key")));
    }

    SECTION("remove()") {
      JsonObject &obj = doc.to<JsonObject>();
      obj["key"] = "value";

      obj.remove(std::string("key"));

      REQUIRE(0 == obj.size());
    }

    SECTION("operator[], set key") {
      std::string key("hello");
      JsonObject &obj = doc.to<JsonObject>();
      obj[key] = "world";
      eraseString(key);
      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("operator[], set value") {
      std::string value("world");
      JsonObject &obj = doc.to<JsonObject>();
      obj["hello"] = value;
      eraseString(value);
      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("serializeJson()") {
      JsonObject &obj = doc.to<JsonObject>();
      obj["key"] = "value";
      std::string json;
      serializeJson(doc, json);
      REQUIRE(std::string("{\"key\":\"value\"}") == json);
    }

    SECTION("serializeJsonPretty()") {
      JsonObject &obj = doc.to<JsonObject>();
      obj["key"] = "value";
      std::string json;
      serializeJsonPretty(doc, json);
      REQUIRE(std::string("{\r\n  \"key\": \"value\"\r\n}") == json);
    }

    SECTION("memoryUsage() increases when adding a new key") {
      std::string key1("hello"), key2("world");
      JsonObject &obj = doc.to<JsonObject>();

      obj[key1] = 1;
      size_t sizeBefore = doc.memoryUsage();
      obj[key2] = 2;
      size_t sizeAfter = doc.memoryUsage();

      REQUIRE(sizeAfter - sizeBefore >= key2.size());
    }

    SECTION("memoryUsage() remains when adding the same key") {
      std::string key("hello");
      JsonObject &obj = doc.to<JsonObject>();

      obj[key] = 1;
      size_t sizeBefore = doc.memoryUsage();
      obj[key] = 2;
      size_t sizeAfter = doc.memoryUsage();

      REQUIRE(sizeBefore == sizeAfter);
    }
  }
}
