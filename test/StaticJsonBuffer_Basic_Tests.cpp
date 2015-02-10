// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>

#define protected public
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

class StaticJsonBuffer_Basic_Tests : public testing::Test {
 protected:
  StaticJsonBuffer<42> buffer;
};

TEST_F(StaticJsonBuffer_Basic_Tests, CapacityMatchTemplateParameter) {
  ASSERT_EQ(42, buffer.capacity());
}

TEST_F(StaticJsonBuffer_Basic_Tests, InitialSizeIsZero) {
  ASSERT_EQ(0, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, GrowsAfterAlloc) {
  buffer.alloc(1);
  ASSERT_EQ(1, buffer.size());
  buffer.alloc(1);
  ASSERT_EQ(2, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, DoesntGrowWhenFull) {
  buffer.alloc(42);
  buffer.alloc(1);
  ASSERT_EQ(42, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, DoesntGrowWhenTooSmall) {
  buffer.alloc(43);
  ASSERT_EQ(0, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, ReturnsNonNull) {
  void *p = buffer.alloc(42);
  ASSERT_NE(static_cast<void *>(0), p);
}

TEST_F(StaticJsonBuffer_Basic_Tests, ReturnsNullWhenFull) {
  buffer.alloc(42);
  void *p = buffer.alloc(1);
  ASSERT_EQ(NULL, p);
}

TEST_F(StaticJsonBuffer_Basic_Tests, ReturnsNullWhenTooSmall) {
  void *p = buffer.alloc(43);
  ASSERT_EQ(NULL, p);
}
