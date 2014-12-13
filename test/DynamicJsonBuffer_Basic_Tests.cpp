// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>

#define protected public
#include <ArduinoJson/DynamicJsonBuffer.hpp>

using namespace ArduinoJson;

class DynamicJsonBuffer_Basic_Tests : public testing::Test {
 protected:
  DynamicJsonBuffer buffer;
};

TEST_F(DynamicJsonBuffer_Basic_Tests, InitialSizeIsZero) {
  ASSERT_EQ(0, buffer.size());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, GrowsAfterAlloc) {
  buffer.alloc(1);
  ASSERT_EQ(1, buffer.size());
  buffer.alloc(1);
  ASSERT_EQ(2, buffer.size());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, CanAllocLessThanBlockCapacity) {
  void* p = buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY);
  ASSERT_TRUE(p);
}

TEST_F(DynamicJsonBuffer_Basic_Tests, CantAllocMoreThanBlockCapacity) {
  void* p = buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY + 1);
  ASSERT_FALSE(p);
}

TEST_F(DynamicJsonBuffer_Basic_Tests, ReturnDifferentPointer) {
  void* p1 = buffer.alloc(1);
  void* p2 = buffer.alloc(2);
  ASSERT_NE(p1, p2);
}
