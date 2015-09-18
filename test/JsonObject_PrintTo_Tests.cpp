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
  JsonObject_PrintTo_Tests() : _object(_jsonBuffer.createObject()) {}

 protected:
  void outputMustBe(const char *expected) {
    char actual[256];
    size_t actualLen = _object.printTo(actual, sizeof(actual));
    size_t measuredLen = _object.measureLength();

    EXPECT_STREQ(expected, actual);
    EXPECT_EQ(strlen(expected), actualLen);
    EXPECT_EQ(strlen(expected), measuredLen);
  }

  DynamicJsonBuffer _jsonBuffer;
  JsonObject &_object;
};

TEST_F(JsonObject_PrintTo_Tests, EmptyObject) { outputMustBe("{}"); }

TEST_F(JsonObject_PrintTo_Tests, TwoStrings) {
  _object["key1"] = "value1";
  _object.set("key2", "value2");

  outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, RemoveFirst) {
  _object["key1"] = "value1";
  _object["key2"] = "value2";
  _object.remove("key1");

  outputMustBe("{\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, RemoveLast) {
  _object["key1"] = "value1";
  _object["key2"] = "value2";
  _object.remove("key2");

  outputMustBe("{\"key1\":\"value1\"}");
}

TEST_F(JsonObject_PrintTo_Tests, RemoveUnexistingKey) {
  _object["key1"] = "value1";
  _object["key2"] = "value2";
  _object.remove("key3");

  outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, ReplaceExistingKey) {
  _object["key"] = "value1";
  _object["key"] = "value2";

  outputMustBe("{\"key\":\"value2\"}");
}

TEST_F(JsonObject_PrintTo_Tests, TwoIntegers) {
  _object["a"] = 1;
  _object.set("b", 2);
  outputMustBe("{\"a\":1,\"b\":2}");
}

TEST_F(JsonObject_PrintTo_Tests, TwoDoublesFourDigits) {
  _object["a"] = double_with_n_digits(3.14159265358979323846, 4);
  _object.set("b", 2.71828182845904523536, 4);
  outputMustBe("{\"a\":3.1416,\"b\":2.7183}");
}

TEST_F(JsonObject_PrintTo_Tests, TwoDoubleDefaultDigits) {
  _object["a"] = 3.14159265358979323846;
  _object.set("b", 2.71828182845904523536);
  outputMustBe("{\"a\":3.14,\"b\":2.72}");
}

TEST_F(JsonObject_PrintTo_Tests, TwoNull) {
  _object["a"] = static_cast<char *>(0);
  _object.set("b", static_cast<char *>(0));
  outputMustBe("{\"a\":null,\"b\":null}");
}

TEST_F(JsonObject_PrintTo_Tests, TwoBooleans) {
  _object["a"] = true;
  _object.set("b", false);
  outputMustBe("{\"a\":true,\"b\":false}");
}

TEST_F(JsonObject_PrintTo_Tests, ThreeNestedArrays) {
  _object.createNestedArray("a");
  _object["b"] = _jsonBuffer.createArray();
  _object.set("c", _jsonBuffer.createArray());

  outputMustBe("{\"a\":[],\"b\":[],\"c\":[]}");
}

TEST_F(JsonObject_PrintTo_Tests, ThreeNestedObjects) {
  _object.createNestedObject("a");
  _object["b"] = _jsonBuffer.createObject();
  _object.set("c", _jsonBuffer.createObject());

  outputMustBe("{\"a\":{},\"b\":{},\"c\":{}}");
}
