// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <string>

static void checkObject(const JsonObject obj, const std::string& expected) {
  char actual[256];
  memset(actual, '!', sizeof(actual));

  size_t actualLen = serializeJson(obj, actual);
  size_t measuredLen = measureJson(obj);

  REQUIRE(expected.size() == measuredLen);
  REQUIRE(expected.size() == actualLen);
  REQUIRE(actual[actualLen] == 0);  // serializeJson() adds a null terminator
  REQUIRE(expected == actual);
}

TEST_CASE("serializeJson(JsonObject)") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SUBCASE("EmptyObject") {
    checkObject(obj, "{}");
  }

  SUBCASE("TwoStrings") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";

    checkObject(obj, "{\"key1\":\"value1\",\"key2\":\"value2\"}");
  }

  SUBCASE("RemoveFirst") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    obj.remove("key1");

    checkObject(obj, "{\"key2\":\"value2\"}");
  }

  SUBCASE("RemoveLast") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    obj.remove("key2");

    checkObject(obj, "{\"key1\":\"value1\"}");
  }

  SUBCASE("RemoveUnexistingKey") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    obj.remove("key3");

    checkObject(obj, "{\"key1\":\"value1\",\"key2\":\"value2\"}");
  }

  SUBCASE("ReplaceExistingKey") {
    obj["key"] = "value1";
    obj["key"] = "value2";

    checkObject(obj, "{\"key\":\"value2\"}");
  }

  SUBCASE("TwoIntegers") {
    obj["a"] = 1;
    obj["b"] = 2;
    checkObject(obj, "{\"a\":1,\"b\":2}");
  }

  SUBCASE("serialized(const char*)") {
    obj["a"] = serialized("[1,2]");
    obj["b"] = serialized("[4,5]");
    checkObject(obj, "{\"a\":[1,2],\"b\":[4,5]}");
  }

  SUBCASE("Two doubles") {
    obj["a"] = 12.34;
    obj["b"] = 56.78;
    checkObject(obj, "{\"a\":12.34,\"b\":56.78}");
  }

  SUBCASE("TwoNull") {
    obj["a"] = static_cast<char*>(0);
    obj["b"] = static_cast<char*>(0);
    checkObject(obj, "{\"a\":null,\"b\":null}");
  }

  SUBCASE("TwoBooleans") {
    obj["a"] = true;
    obj["b"] = false;
    checkObject(obj, "{\"a\":true,\"b\":false}");
  }

  SUBCASE("ThreeNestedArrays") {
    JsonDocument b;
    JsonDocument c;

    obj["a"].to<JsonArray>();
    obj["b"] = b.to<JsonArray>();
    obj["c"] = c.to<JsonArray>();

    checkObject(obj, "{\"a\":[],\"b\":[],\"c\":[]}");
  }

  SUBCASE("ThreeNestedObjects") {
    JsonDocument b;
    JsonDocument c;

    obj["a"].to<JsonObject>();
    obj["b"] = b.to<JsonObject>();
    obj["c"] = c.to<JsonObject>();

    checkObject(obj, "{\"a\":{},\"b\":{},\"c\":{}}");
  }
}
