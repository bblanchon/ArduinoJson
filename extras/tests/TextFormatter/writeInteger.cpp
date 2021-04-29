// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <catch.hpp>
#include <limits>
#include <string>

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/Writer.hpp>

using namespace ARDUINOJSON_NAMESPACE;

template <typename T>
void checkWriteInteger(T value, std::string expected) {
  char output[64] = {0};
  StaticStringWriter sb(output, sizeof(output));
  TextFormatter<StaticStringWriter> writer(sb);
  writer.writeInteger<T>(value);
  REQUIRE(expected == output);
  REQUIRE(writer.bytesWritten() == expected.size());
}

TEST_CASE("int8_t") {
  checkWriteInteger<int8_t>(0, "0");
  checkWriteInteger<int8_t>(-128, "-128");
  checkWriteInteger<int8_t>(127, "127");
}

TEST_CASE("uint8_t") {
  checkWriteInteger<uint8_t>(0, "0");
  checkWriteInteger<uint8_t>(255, "255");
}

TEST_CASE("int16_t") {
  checkWriteInteger<int16_t>(0, "0");
  checkWriteInteger<int16_t>(-32768, "-32768");
  checkWriteInteger<int16_t>(32767, "32767");
}

TEST_CASE("uint16_t") {
  checkWriteInteger<uint16_t>(0, "0");
  checkWriteInteger<uint16_t>(65535, "65535");
}

TEST_CASE("int32_t") {
  checkWriteInteger<int32_t>(0, "0");
  checkWriteInteger<int32_t>(-2147483647 - 1, "-2147483648");
  checkWriteInteger<int32_t>(2147483647, "2147483647");
}

TEST_CASE("uint32_t") {
  checkWriteInteger<uint32_t>(0, "0");
  checkWriteInteger<uint32_t>(4294967295U, "4294967295");
}
