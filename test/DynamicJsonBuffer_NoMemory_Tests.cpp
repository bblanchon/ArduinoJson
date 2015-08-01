// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class DynamicJsonBuffer_NoMemory_Tests : public ::testing::Test {
  class NoMemoryAllocator {
   public:
    void* allocate(size_t) { return NULL; }
    void deallocate(void*) {}
  };

 protected:
  Internals::BlockJsonBuffer<NoMemoryAllocator> _jsonBuffer;
};

TEST_F(DynamicJsonBuffer_NoMemory_Tests, CreateArray) {
  ASSERT_FALSE(_jsonBuffer.createArray().success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, CreateObject) {
  ASSERT_FALSE(_jsonBuffer.createObject().success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, ParseArray) {
  char json[] = "[]";
  ASSERT_FALSE(_jsonBuffer.parseArray(json).success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, ParseObject) {
  char json[] = "{}";
  ASSERT_FALSE(_jsonBuffer.parseObject(json).success());
}
