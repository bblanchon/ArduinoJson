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
  DynamicJsonBuffer jb;

  SECTION("JsonArray") {
    DynamicJsonArray array;

    SECTION("deserializeJson") {
      std::string json("[\"hello\"]");

      bool success = deserializeJson(array, json);
      eraseString(json);

      REQUIRE(true == success);
      REQUIRE(std::string("hello") == array[0]);
    }

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
    DynamicJsonObject object;

    SECTION("deserializeJson()") {
      std::string json("{\"hello\":\"world\"}");

      bool success = deserializeJson(object, json);
      eraseString(json);

      REQUIRE(true == success);
      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("operator[]") {
      char json[] = "{\"key\":\"value\"}";

      deserializeJson(object, json);

      REQUIRE(std::string("value") == object[std::string("key")]);
    }

    SECTION("operator[] const") {
      char json[] = "{\"key\":\"value\"}";

      deserializeJson(object, json);
      const JsonObject &obj = object;

      REQUIRE(std::string("value") == obj[std::string("key")]);
    }

    SECTION("set(key)") {
      std::string key("hello");
      object.set(key, "world");
      eraseString(key);
      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("set(value)") {
      std::string value("world");
      object.set("hello", value);
      eraseString(value);
      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("set(key,value)") {
      std::string key("hello");
      std::string value("world");
      object.set(key, value);
      eraseString(key);
      eraseString(value);
      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("set(JsonArraySubscript)") {
      DynamicJsonArray arr;
      arr.add("world");

      object.set(std::string("hello"), arr[0]);

      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("set(JsonObjectSubscript)") {
      DynamicJsonObject obj;
      obj.set("x", "world");

      object.set(std::string("hello"), obj["x"]);

      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("get<T>()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(object, json);

      REQUIRE(std::string("value") ==
              object.get<const char *>(std::string("key")));
    }

    SECTION("is<T>()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(object, json);
      REQUIRE(true == object.is<const char *>(std::string("key")));
    }

    SECTION("createNestedObject()") {
      std::string key = "key";
      char json[64];
      object.createNestedObject(key);
      eraseString(key);
      serializeJson(object, json, sizeof(json));
      REQUIRE(std::string("{\"key\":{}}") == json);
    }

    SECTION("createNestedArray()") {
      std::string key = "key";
      char json[64];
      object.createNestedArray(key);
      eraseString(key);
      serializeJson(object, json, sizeof(json));
      REQUIRE(std::string("{\"key\":[]}") == json);
    }

    SECTION("containsKey()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(object, json);
      REQUIRE(true == object.containsKey(std::string("key")));
    }

    SECTION("remove()") {
      char json[] = "{\"key\":\"value\"}";
      deserializeJson(object, json);
      REQUIRE(1 == object.size());
      object.remove(std::string("key"));
      REQUIRE(0 == object.size());
    }

    SECTION("operator[], set key") {
      std::string key("hello");
      object[key] = "world";
      eraseString(key);
      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("operator[], set value") {
      std::string value("world");
      object["hello"] = value;
      eraseString(value);
      REQUIRE(std::string("world") == object["hello"]);
    }

    SECTION("serializeJson()") {
      object["key"] = "value";
      std::string json;
      serializeJson(object, json);
      REQUIRE(std::string("{\"key\":\"value\"}") == json);
    }

    SECTION("serializeJsonPretty()") {
      object["key"] = "value";
      std::string json;
      serializeJsonPretty(object, json);
      REQUIRE(std::string("{\r\n  \"key\": \"value\"\r\n}") == json);
    }

    SECTION("memoryUsage() increases when adding a new key") {
      std::string key1("hello"), key2("world");

      object[key1] = 1;
      size_t sizeBefore = object.memoryUsage();
      object[key2] = 2;
      size_t sizeAfter = object.memoryUsage();

      REQUIRE(sizeAfter - sizeBefore >= key2.size());
    }

    SECTION("memoryUsage() remains when adding the same key") {
      std::string key("hello");

      object[key] = 1;
      size_t sizeBefore = object.memoryUsage();
      object[key] = 2;
      size_t sizeAfter = object.memoryUsage();

      REQUIRE(sizeBefore == sizeAfter);
    }
  }
}
