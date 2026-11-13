#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Literals.hpp"

using ArduinoJson::detail::NumberType;
using ArduinoJson::detail::parseNumber;

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 0") {
  JsonDocument doc;

  SECTION("smoke test") {
    doc["A"] = 42;
    doc["B"] = 84;

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"A\":42,\"B\":84}");
  }

  SECTION("deserializeMsgPack()") {
    SECTION("cf 00 00 00 00 ff ff ff ff") {
      auto err =
          deserializeMsgPack(doc, "\xcf\x00\x00\x00\x00\xff\xff\xff\xff"_s);

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.as<uint32_t>() == 0xFFFFFFFF);
    }

    SECTION("cf 00 00 00 01 00 00 00 00") {
      auto err =
          deserializeMsgPack(doc, "\xcf\x00\x00\x00\x01\x00\x00\x00\x00"_s);

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.isNull());
    }
  }

  SECTION("parseNumber()") {
    SECTION("uint32_t overflow") {
      SECTION("ULONG_MAX") {
        auto num1 = parseNumber("4294967295");
        auto num2 = parseNumber("4294967296");

        REQUIRE(num1.type() == NumberType::UnsignedInteger);
        CHECK(num1.asUnsignedInteger() == 4294967295); 

        REQUIRE(num2.type() == NumberType::Float);
        CHECK(num2.asFloat() == Approx(4294967296.0));
      }
    }
  }
}
