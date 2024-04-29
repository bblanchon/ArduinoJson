#define ARDUINOJSON_STRING_LENGTH_SIZE 4
#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

TEST_CASE("ARDUINOJSON_STRING_LENGTH_SIZE == 4") {
  JsonDocument doc;

  SECTION("set() returns true if string has 65536 characters") {
    auto result = doc.set(std::string(65536, '?'));

    REQUIRE(result == true);
    REQUIRE(doc.overflowed() == false);
  }

  SECTION("set() returns true if binary has 65536 characters") {
    auto str = std::string(65536, '?');
    auto result = doc.set(MsgPackBinary(str.data(), str.size()));

    REQUIRE(result == true);
    REQUIRE(doc.overflowed() == false);
  }

  SECTION("deserializeJson() returns Ok if string has 65536 characters") {
    auto input = "\"" + std::string(65536, '?') + "\"";

    auto err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("deserializeMsgPack() returns Ok if string has 65536 characters") {
    auto input = "\xda\xff\xff" + std::string(65536, '?');

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("deserializeMsgPack() returns Ok if binary has 65536 characters") {
    auto input = "\xc5\xff\xff" + std::string(65536, '?');

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("bin 32 deserialization") {
    auto str = std::string(65536, '?');
    auto input = std::string("\xc6\x00\x01\x00\x00", 5) + str;

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<MsgPackBinary>());
    auto binary = doc.as<MsgPackBinary>();
    REQUIRE(binary.size() == 65536);
    REQUIRE(binary.data() != nullptr);
    REQUIRE(std::string(reinterpret_cast<const char*>(binary.data()),
                        binary.size()) == str);
  }

  SECTION("bin 32 serialization") {
    auto str = std::string(65536, '?');
    doc.set(MsgPackBinary(str.data(), str.size()));

    std::string output;
    auto result = serializeMsgPack(doc, output);

    REQUIRE(result == 5 + str.size());
    REQUIRE(output == std::string("\xc6\x00\x01\x00\x00", 5) + str);
  }
}
