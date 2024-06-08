#define ARDUINOJSON_STRING_LENGTH_SIZE 2
#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

#include "Literals.hpp"

TEST_CASE("ARDUINOJSON_STRING_LENGTH_SIZE == 2") {
  JsonDocument doc;

  SECTION("set(std::string)") {
    SECTION("returns true if len <= 65535") {
      auto result = doc.set(std::string(65535, '?'));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }

    SECTION("returns false if len >= 65536") {
      auto result = doc.set(std::string(65536, '?'));

      REQUIRE(result == false);
      REQUIRE(doc.overflowed() == true);
    }
  }

  SECTION("set(MsgPackBinary)") {
    SECTION("returns true if size <= 65532") {
      auto str = std::string(65532, '?');
      auto result = doc.set(MsgPackBinary(str.data(), str.size()));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }

    SECTION("returns false if size >= 65533") {
      auto str = std::string(65533, '?');
      auto result = doc.set(MsgPackBinary(str.data(), str.size()));

      REQUIRE(result == false);
      REQUIRE(doc.overflowed() == true);
    }
  }

  SECTION("set(MsgPackExtension)") {
    SECTION("returns true if size <= 65531") {
      auto str = std::string(65531, '?');
      auto result = doc.set(MsgPackExtension(1, str.data(), str.size()));

      REQUIRE(result == true);
      REQUIRE(doc.overflowed() == false);
    }

    SECTION("returns false if size >= 65532") {
      auto str = std::string(65532, '?');
      auto result = doc.set(MsgPackExtension(1, str.data(), str.size()));

      REQUIRE(result == false);
      REQUIRE(doc.overflowed() == true);
    }
  }

  SECTION("deserializeJson()") {
    SECTION("returns Ok if string length <= 65535") {
      auto input = "\"" + std::string(65535, '?') + "\"";

      auto err = deserializeJson(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if string length >= 65536") {
      auto input = "\"" + std::string(65536, '?') + "\"";

      auto err = deserializeJson(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }
  }

  SECTION("deserializeMsgPack()") {
    SECTION("returns Ok if string length <= 65535") {
      auto input = "\xda\xff\xff" + std::string(65535, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if string length >= 65536") {
      auto input = "\xdb\x00\x01\x00\x00"_s + std::string(65536, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }

    SECTION("returns Ok if binary size <= 65532") {
      auto input = "\xc5\xff\xfc" + std::string(65532, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if binary size >= 65534") {
      auto input = "\xc5\xff\xfd" + std::string(65534, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }

    // https://oss-fuzz.com/testcase?key=5354792971993088
    SECTION("doesn't overflow if binary size == 0xFFFF") {
      auto input = "\xc5\xff\xff"_s;

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }

    SECTION("returns Ok if extension size <= 65531") {
      auto input = "\xc8\xff\xfb\x01" + std::string(65531, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::Ok);
    }

    SECTION("returns NoMemory if extension size >= 65532") {
      auto input = "\xc8\xff\xfc\x01" + std::string(65532, '?');

      auto err = deserializeMsgPack(doc, input);

      REQUIRE(err == DeserializationError::NoMemory);
    }
  }
}
