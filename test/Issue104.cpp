// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#define ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson.h>

#define SOURCE "{\"mqtt.host\":\"mqtt.test.com\",\"mqtt.port\":1883}"

#define SIZE_WITH_COPY (sizeof(SOURCE) + JSON_OBJECT_SIZE(2))
#define SIZE_WITHOUT_COPY (JSON_OBJECT_SIZE(2))

template <int N, typename S>
void mustSucceedWith(S source) {
  StaticJsonBuffer<N> jsonBuffer;
  ASSERT_TRUE(jsonBuffer.parseObject(source).success());
}

template <int N, typename S>
void mustFailWith(S source) {
  StaticJsonBuffer<N> jsonBuffer;
  ASSERT_FALSE(jsonBuffer.parseObject(source).success());
}

TEST(Issue104, CharPtrSucceeds) {
  char source[] = SOURCE;
  mustSucceedWith<SIZE_WITHOUT_COPY, char *>(source);
}

TEST(Issue104, CharPtrFails) {
  char source[] = SOURCE;
  mustFailWith<SIZE_WITHOUT_COPY - 1, char *>(source);
}

TEST(Issue104, ConstCharPtrSucceeds) {
  mustSucceedWith<SIZE_WITH_COPY, const char *>(SOURCE);
}

TEST(Issue104, ConstCharPtrFails) {
  mustFailWith<SIZE_WITH_COPY - 1, const char *>(SOURCE);
}

TEST(Issue104, StringSucceeds) {
  mustSucceedWith<SIZE_WITH_COPY, const String &>(SOURCE);
}

TEST(Issue104, StringFails) {
  mustFailWith<SIZE_WITH_COPY - 1, const String &>(SOURCE);
}

TEST(Issue104, TooSmallForStrDup) {
  mustFailWith<sizeof(SOURCE) - 1, const char *>(SOURCE);
}
