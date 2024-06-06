#define ARDUINOJSON_STRING_LENGTH_SIZE 4
#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

TEST_CASE("ARDUINOJSON_STRING_LENGTH_SIZE == 4") {
  JsonDocument doc;

  SECTION("set(std::string)") {
    SECTION("returns true if string length >= 65536") {
      auto result = doc.set(std::string(65536, '?'));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }
  }

  SECTION("set(MsgPackBinary)") {
    SECTION("returns true if size >= 65536") {
      auto str = std::string(65536, '?');
      auto result = doc.set(MsgPackBinary(str.data(), str.size()));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }
  }

  SECTION("deserializeJson()") {
    SECTION("returns Ok if string length >= 65536") {
      auto input = "\"" + std::string(65536, '?') + "\"";

      auto err = deserializeJson(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }
  }

  SECTION("deserializeMsgPack()") {
    SECTION("returns Ok if string size >= 65536") {
      auto input = "\xda\xff\xff" + std::string(65536, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns Ok if binary size >= 65536") {
      auto input = "\xc5\xff\xff" + std::string(65536, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }
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
