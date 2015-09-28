// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class ArduinoStringTests : public ::testing::Test {
 protected:
  static void eraseString(String &str) {
    char *p = const_cast<char *>(str.c_str());
    while (*p) *p++ = '*';
  }

  DynamicJsonBuffer _jsonBuffer;
};

TEST_F(ArduinoStringTests, JsonBuffer_ParseArray) {
  String json("[\"hello\"]");
  JsonArray &array = _jsonBuffer.parseArray(json);
  eraseString(json);
  ASSERT_TRUE(array.success());
  ASSERT_STREQ("hello", array[0]);
}

TEST_F(ArduinoStringTests, JsonBuffer_ParseObject) {
  String json("{\"hello\":\"world\"}");
  JsonObject &object = _jsonBuffer.parseObject(json);
  eraseString(json);
  ASSERT_TRUE(object.success());
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObject_Subscript) {
  char json[] = "{\"key\":\"value\"}";
  JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object[String("key")]);
}

TEST_F(ArduinoStringTests, JsonObject_ConstSubscript) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object[String("key")]);
}

TEST_F(ArduinoStringTests, JsonObject_SetKey) {
  JsonObject &object = _jsonBuffer.createObject();
  String key("hello");
  object.set(key, "world");
  eraseString(key);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObject_SetValue) {
  JsonObject &object = _jsonBuffer.createObject();
  String value("world");
  object.set("hello", value);
  eraseString(value);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObject_SetKeyValue) {
  JsonObject &object = _jsonBuffer.createObject();
  String key("hello");
  String value("world");
  object.set(key, value);
  eraseString(key);
  eraseString(value);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObject_SetToArraySubscript) {
  JsonArray &arr = _jsonBuffer.createArray();
  arr.add("world");

  JsonObject &object = _jsonBuffer.createObject();
  object.set(String("hello"), arr[0]);

  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObject_SetToObjectSubscript) {
  JsonObject &arr = _jsonBuffer.createObject();
  arr.set("x", "world");

  JsonObject &object = _jsonBuffer.createObject();
  object.set(String("hello"), arr["x"]);

  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObject_Get) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object.get(String("key")));
}

TEST_F(ArduinoStringTests, JsonObject_GetT) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object.get<const char *>(String("key")));
}

TEST_F(ArduinoStringTests, JsonObject_IsT) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.is<const char *>(String("key")));
}

TEST_F(ArduinoStringTests, JsonObject_CreateNestedObject) {
  String key = "key";
  char json[64];
  JsonObject &object = _jsonBuffer.createObject();
  object.createNestedObject(key);
  eraseString(key);
  object.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"key\":{}}", json);
}

TEST_F(ArduinoStringTests, JsonObject_CreateNestedArray) {
  String key = "key";
  char json[64];
  JsonObject &object = _jsonBuffer.createObject();
  object.createNestedArray(key);
  eraseString(key);
  object.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"key\":[]}", json);
}

TEST_F(ArduinoStringTests, JsonObject_ContainsKey) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.containsKey(String("key")));
}

TEST_F(ArduinoStringTests, JsonObject_Remove) {
  char json[] = "{\"key\":\"value\"}";
  JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_EQ(1, object.size());
  object.remove(String("key"));
  ASSERT_EQ(0, object.size());
}

TEST_F(ArduinoStringTests, JsonObjectSubscript_SetKey) {
  JsonObject &object = _jsonBuffer.createObject();
  String key("hello");
  object[key] = "world";
  eraseString(key);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonObjectSubscript_SetValue) {
  JsonObject &object = _jsonBuffer.createObject();
  String value("world");
  object["hello"] = value;
  eraseString(value);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(ArduinoStringTests, JsonArray_Add) {
  JsonArray &array = _jsonBuffer.createArray();
  String value("hello");
  array.add(value);
  eraseString(value);
  ASSERT_STREQ("hello", array[0]);
}

TEST_F(ArduinoStringTests, JsonArray_Set) {
  JsonArray &array = _jsonBuffer.createArray();
  String value("world");
  array.add("hello");
  array.set(0, value);
  eraseString(value);
  ASSERT_STREQ("world", array[0]);
}

TEST_F(ArduinoStringTests, JsonArraySubscript) {
  JsonArray &array = _jsonBuffer.createArray();
  String value("world");
  array.add("hello");
  array[0] = value;
  eraseString(value);
  ASSERT_STREQ("world", array[0]);
}

TEST_F(ArduinoStringTests, JsonArray_PrintTo) {
  JsonArray &array = _jsonBuffer.createArray();
  array.add(4);
  array.add(2);
  String json;
  array.printTo(json);
  ASSERT_EQ(String("[4,2]"), json);
}

TEST_F(ArduinoStringTests, JsonArray_PrettyPrintTo) {
  JsonArray &array = _jsonBuffer.createArray();
  array.add(4);
  array.add(2);
  String json;
  array.prettyPrintTo(json);
  ASSERT_EQ(String("[\r\n  4,\r\n  2\r\n]"), json);
}

TEST_F(ArduinoStringTests, JsonObject_PrintTo) {
  JsonObject &object = _jsonBuffer.createObject();
  object["key"] = "value";
  String json;
  object.printTo(json);
  ASSERT_EQ(String("{\"key\":\"value\"}"), json);
}

TEST_F(ArduinoStringTests, JsonObject_PrettyPrintTo) {
  JsonObject &object = _jsonBuffer.createObject();
  object["key"] = "value";
  String json;
  object.prettyPrintTo(json);
  ASSERT_EQ(String("{\r\n  \"key\": \"value\"\r\n}"), json);
}
