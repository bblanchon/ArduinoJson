// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(ArduinoStringTests, JsonBuffer_ParseArray) {
  DynamicJsonBuffer jsonBuffer;
  String json("[1,2]");
  JsonArray &array = jsonBuffer.parseArray(json);
  ASSERT_TRUE(array.success());
}

TEST(ArduinoStringTests, JsonBuffer_ParseObject) {
  DynamicJsonBuffer jsonBuffer;
  String json("{\"a\":1,\"b\":2}");
  JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.success());
}

TEST(ArduinoStringTests, JsonVariant) {
  String input = "Hello world!";
  JsonVariant variant(input);
  ASSERT_TRUE(variant.is<String>());
  String output = variant.as<String>();
  ASSERT_EQ(input, output);
}

TEST(ArduinoStringTests, JsonObject_Subscript) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object[String("key")]);
}

TEST(ArduinoStringTests, JsonObject_ConstSubscript) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object[String("key")]);
}

TEST(ArduinoStringTests, JsonObject_Set) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &object = jsonBuffer.createObject();
  String key = "key";
  object.set(key, "value");
  ASSERT_STREQ("value", object["key"]);
}

TEST(ArduinoStringTests, JsonObject_Get) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object.get(String("key")));
}

TEST(ArduinoStringTests, JsonObject_GetT) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object.get<const char *>(String("key")));
}

TEST(ArduinoStringTests, JsonObject_IsT) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.is<const char *>(String("key")));
}

TEST(ArduinoStringTests, JsonObject_CreateNestedObject) {
  DynamicJsonBuffer jsonBuffer;
  String key = "key";
  char json[64];
  JsonObject &object = jsonBuffer.createObject();
  object.createNestedObject(key);
  object.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"key\":{}}", json);
}

TEST(ArduinoStringTests, JsonObject_CreateNestedArray) {
  DynamicJsonBuffer jsonBuffer;
  String key = "key";
  char json[64];
  JsonObject &object = jsonBuffer.createObject();
  object.createNestedArray(key);
  object.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"key\":[]}", json);
}

TEST(ArduinoStringTests, JsonObject_ContainsKey) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.containsKey(String("key")));
}

TEST(ArduinoStringTests, JsonObject_Remove) {
  DynamicJsonBuffer jsonBuffer;
  char json[] = "{\"key\":\"value\"}";
  JsonObject &object = jsonBuffer.parseObject(json);
  ASSERT_EQ(1, object.size());
  object.remove(String("key"));
  ASSERT_EQ(0, object.size());
}