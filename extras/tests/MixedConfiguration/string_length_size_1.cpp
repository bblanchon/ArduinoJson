#define ARDUINOJSON_STRING_LENGTH_SIZE 1
#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

#include "Literals.hpp"

TEST_CASE("ARDUINOJSON_STRING_LENGTH_SIZE == 1") {
  JsonDocument doc;

  SECTION("set(std::string)") {
    SECTION("returns true if len <= 255") {
      auto result = doc.set(std::string(255, '?'));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }

    SECTION("returns false if len >= 256") {
      auto result = doc.set(std::string(256, '?'));

      REQUIRE(result == false);
      REQUIRE(doc.overflowed() == true);
    }
  }

  SECTION("set(MsgPackBinary)") {
    SECTION("returns true if size <= 253") {
      auto str = std::string(253, '?');
      auto result = doc.set(MsgPackBinary(str.data(), str.size()));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }

    SECTION("returns false if size >= 254") {
      auto str = std::string(254, '?');
      auto result = doc.set(MsgPackBinary(str.data(), str.size()));

      REQUIRE(result == false);
      REQUIRE(doc.overflowed() == true);
    }
  }

  SECTION("set(MsgPackExtension)") {
    SECTION("returns true if size <= 252") {
      auto str = std::string(252, '?');
      auto result = doc.set(MsgPackExtension(1, str.data(), str.size()));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }

    SECTION("returns false if size >= 253") {
      auto str = std::string(253, '?');
      auto result = doc.set(MsgPackExtension(1, str.data(), str.size()));

      REQUIRE(result == false);
      REQUIRE(doc.overflowed() == true);
    }
  }

  SECTION("deserializeJson()") {
    SECTION("returns Ok if string length <= 255") {
      auto input = "\"" + std::string(255, '?') + "\"";

      auto err = deserializeJson(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if string length >= 256") {
      auto input = "\"" + std::string(256, '?') + "\"";

      auto err = deserializeJson(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }
  }

  SECTION("deserializeMsgPack()") {
    SECTION("returns Ok if string length <= 255") {
      auto input = "\xd9\xff" + std::string(255, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if string length >= 256") {
      auto input = "\xda\x01\x00"_s + std::string(256, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }

    SECTION("returns Ok if binary size <= 253") {
      auto input = "\xc4\xfd" + std::string(253, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if binary size >= 254") {
      auto input = "\xc4\xfe" + std::string(254, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }

    SECTION("returns Ok if extension size <= 252") {
      auto input = "\xc7\xfc\x01" + std::string(252, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if binary size >= 253") {
      auto input = "\xc7\xfd\x01" + std::string(253, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }
  }
}
