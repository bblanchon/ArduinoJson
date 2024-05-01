#define ARDUINOJSON_STRING_LENGTH_SIZE 2
#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

TEST_CASE("ARDUINOJSON_STRING_LENGTH_SIZE == 2") {
  JsonDocument doc;

  SECTION("set() returns true if string has 65535 characters") {
    auto result = doc.set(std::string(65535, '?'));

    REQUIRE(result == true);
    REQUIRE(doc.overflowed() == false);
  }

  SECTION("set() returns false if string has 65536 characters") {
    auto result = doc.set(std::string(65536, '?'));

    REQUIRE(result == false);
    REQUIRE(doc.overflowed() == true);
  }

  SECTION("set() returns true if string has 65532 characters") {
    auto str = std::string(65532, '?');
    auto result = doc.set(MsgPackBinary(str.data(), str.size()));

    REQUIRE(result == true);
    REQUIRE(doc.overflowed() == false);
  }

  SECTION("set() returns false if string has 65533 characters") {
    auto str = std::string(65533, '?');
    auto result = doc.set(MsgPackBinary(str.data(), str.size()));

    REQUIRE(result == false);
    REQUIRE(doc.overflowed() == true);
  }

  SECTION("deserializeJson() returns Ok if string has 65535 characters") {
    auto input = "\"" + std::string(65535, '?') + "\"";

    auto err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("deserializeJson() returns NoMemory if string has 65536 characters") {
    auto input = "\"" + std::string(65536, '?') + "\"";

    auto err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SECTION("deserializeMsgPack() returns Ok if string has 65535 characters") {
    auto input = "\xda\xff\xff" + std::string(65535, '?');

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION(
      "deserializeMsgPack() returns NoMemory if string has 65536 characters") {
    auto input =
        std::string("\xdb\x00\x01\x00\x00", 5) + std::string(65536, '?');

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SECTION("deserializeMsgPack() returns Ok if binary has 65532 characters") {
    auto input = "\xc5\xff\xfc" + std::string(65532, '?');

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION(
      "deserializeMsgPack() returns NoMemory of binary has 65534 characters") {
    auto input = "\xc5\xff\xfd" + std::string(65534, '?');

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
  }
}
