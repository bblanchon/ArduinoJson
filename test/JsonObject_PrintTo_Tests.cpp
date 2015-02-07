// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

using namespace ArduinoJson::Internals;

class JsonObject_PrintTo_Tests : public testing::Test {
 public:
  JsonObject_PrintTo_Tests() : object(json.createObject()) {}

 protected:
  void outputMustBe(const char *expected) {
    char actual[256];
    int result = object.printTo(actual, sizeof(actual));

    EXPECT_STREQ(expected, actual);
    EXPECT_EQ(strlen(expected), result);
  }

  StaticJsonBuffer<JSON_OBJECT_SIZE(2)> json;
  JsonObject &object;
};

TEST_F(JsonObject_PrintTo_Tests, EmptyObject) { outputMustBe("{}"); }

TEST_F(JsonObject_PrintTo_Tests, OneString) {
  object["key"] = "value";

  outputMustBe("{\"key\":\"value\"}");
}

TEST_F(JsonObject_PrintTo_Tests, TwoStrings) {
  object["key1"] = "value1";
  object["key2"] = "value2";

  outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, RemoveFirst) {
  object["key1"] = "value1";
  object["key2"] = "value2";
  object.remove("key1");

  outputMustBe("{\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, RemoveLast) {
  object["key1"] = "value1";
  object["key2"] = "value2";
  object.remove("key2");

  outputMustBe("{\"key1\":\"value1\"}");
}

TEST_F(JsonObject_PrintTo_Tests, RemoveUnexistingKey) {
  object["key1"] = "value1";
  object["key2"] = "value2";
  object.remove("key3");

  outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, ReplaceExistingKey) {
  object["key"] = "value1";
  object["key"] = "value2";

  outputMustBe("{\"key\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, OneStringOverCapacity) {
  object["key1"] = "value1";
  object["key2"] = "value2";
  object["key3"] = "value3";

  outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, OneInteger) {
  object["key"] = 1;
  outputMustBe("{\"key\":1}");
}

TEST_F(JsonObject_PrintTo_Tests, OneDoubleFourDigits) {
  object["key"].set(3.14159265358979323846, 4);
  outputMustBe("{\"key\":3.1416}");
}

TEST_F(JsonObject_PrintTo_Tests, OneDoubleDefaultDigits) {
  object["key"] = 3.14159265358979323846;
  outputMustBe("{\"key\":3.14}");
}

TEST_F(JsonObject_PrintTo_Tests, OneNull) {
  object["key"] = static_cast<char *>(0);
  outputMustBe("{\"key\":null}");
}

TEST_F(JsonObject_PrintTo_Tests, OneTrue) {
  object["key"] = true;
  outputMustBe("{\"key\":true}");
}

TEST_F(JsonObject_PrintTo_Tests, OneFalse) {
  object["key"] = false;
  outputMustBe("{\"key\":false}");
}

TEST_F(JsonObject_PrintTo_Tests, OneEmptyNestedArrayViaProxy) {
  JsonArray &nestedArray = json.createArray();

  object["key"] = nestedArray;

  outputMustBe("{\"key\":[]}");
}

TEST_F(JsonObject_PrintTo_Tests, OneEmptyNestedObjectViaProxy) {
  JsonObject &nestedArray = json.createObject();

  object["key"] = nestedArray;

  outputMustBe("{\"key\":{}}");
}

TEST_F(JsonObject_PrintTo_Tests, OneEmptyNestedObject) {
  object.createNestedObject("key");

  outputMustBe("{\"key\":{}}");
}

TEST_F(JsonObject_PrintTo_Tests, OneEmptyNestedArray) {
  object.createNestedArray("key");

  outputMustBe("{\"key\":[]}");
}
