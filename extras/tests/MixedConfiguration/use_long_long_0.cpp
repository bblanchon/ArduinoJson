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
      auto num1 = parseNumber("4294967295");
      auto num2 = parseNumber("4294967296");

      REQUIRE(num1.type() == NumberType::UnsignedInteger);
      CHECK(num1.asUnsignedInteger() == 4294967295);

      REQUIRE(num2.type() == NumberType::Double);
      CHECK(num2.asDouble() == Approx(4294967296.0));
    }

    SECTION("int32_t overflow") {
      auto num1 = parseNumber("-2147483648");
      auto num2 = parseNumber("-2147483649");

      REQUIRE(num1.type() == NumberType::SignedInteger);
      CHECK(num1.asSignedInteger() == -2147483648);

      REQUIRE(num2.type() == NumberType::Double);
      CHECK(num2.asDouble() == Approx(-2147483649.0));
    }
  }
}
