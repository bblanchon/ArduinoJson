// Copyright Benoit Blanchon 2014-2015
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

TEST_F(DynamicJsonBuffer_Basic_Tests, InitialBlockCountIsOne) {
  ASSERT_EQ(1, buffer.blockCount());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, SizeIncreasesAfterAlloc) {
  buffer.alloc(1);
  ASSERT_EQ(1, buffer.size());
  buffer.alloc(1);
  ASSERT_EQ(2, buffer.size());
  buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY);
  ASSERT_EQ(2 + DynamicJsonBuffer::BLOCK_CAPACITY, buffer.size());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, BlockCountDoesntChangeWhenNotFull) {
  buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY);
  ASSERT_EQ(1, buffer.blockCount());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, BlockCountChangesWhenFull) {
  buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY);
  buffer.alloc(1);
  ASSERT_EQ(2, buffer.blockCount());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, CanAllocLessThanBlockCapacity) {
  void* p1 = buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY);
  ASSERT_FALSE(p1 == NULL);
  void* p2 = buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY);
  ASSERT_FALSE(p2 == NULL);
}

TEST_F(DynamicJsonBuffer_Basic_Tests, CantAllocMoreThanBlockCapacity) {
  void* p = buffer.alloc(DynamicJsonBuffer::BLOCK_CAPACITY + 1);
  ASSERT_TRUE(p == NULL);
}

TEST_F(DynamicJsonBuffer_Basic_Tests, ReturnDifferentPointer) {
  void* p1 = buffer.alloc(1);
  void* p2 = buffer.alloc(2);
  ASSERT_NE(p1, p2);
}
