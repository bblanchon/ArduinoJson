// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

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

TEST_F(DynamicJsonBuffer_Basic_Tests, SizeIncreasesAfterAlloc) {
  buffer.alloc(1);
  ASSERT_LE(1U, buffer.size());
  buffer.alloc(1);
  ASSERT_LE(2U, buffer.size());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, ReturnDifferentPointer) {
  void* p1 = buffer.alloc(1);
  void* p2 = buffer.alloc(2);
  ASSERT_NE(p1, p2);
}

TEST_F(DynamicJsonBuffer_Basic_Tests, Alignment) {
  size_t mask = sizeof(void*) - 1;

  for (size_t size = 1; size <= sizeof(void*); size++) {
    size_t addr = reinterpret_cast<size_t>(buffer.alloc(1));
    ASSERT_EQ(0, addr & mask);
  }
}
