// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class StaticJsonBuffer_ParseArray_Tests : public testing::Test {
 protected:
  void with(JsonBuffer& jsonBuffer) { _jsonBuffer = &jsonBuffer; }

  void whenInputIs(const char* json) { strcpy(_jsonString, json); }

  void parseMustSucceed() {
    EXPECT_TRUE(_jsonBuffer->parseArray(_jsonString).success());
  }

  void parseMustFail() {
    EXPECT_FALSE(_jsonBuffer->parseArray(_jsonString).success());
  }

 private:
  JsonBuffer* _jsonBuffer;
  char _jsonString[256];
};

TEST_F(StaticJsonBuffer_ParseArray_Tests, TooSmallBufferForEmptyArray) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(0) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("[]");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, BufferOfTheRightSizeForEmptyArray) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(0)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("[]");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, TooSmallBufferForArrayWithOneValue) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("[1]");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests,
       BufferOfTheRightSizeForArrayWithOneValue) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("[1]");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests,
       TooSmallBufferForArrayWithNestedObject) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("[{}]");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests,
       BufferOfTheRightSizeForArrayWithNestedObject) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("[{}]");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, CharPtrNull) {
  ASSERT_FALSE(StaticJsonBuffer<100>().parseArray((char*)0).success());
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, ConstCharPtrNull) {
  ASSERT_FALSE(StaticJsonBuffer<100>().parseArray((const char*)0).success());
}
