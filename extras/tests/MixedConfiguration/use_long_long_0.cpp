#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h>

#include <doctest.h>

#include "Literals.hpp"

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 0") {
  JsonDocument doc;

  SUBCASE("smoke test") {
    doc["A"] = 42;
    doc["B"] = 84;

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"A\":42,\"B\":84}");
  }

  SUBCASE("deserializeMsgPack()") {
    SUBCASE("cf 00 00 00 00 ff ff ff ff") {
      auto err =
          deserializeMsgPack(doc, "\xcf\x00\x00\x00\x00\xff\xff\xff\xff"_s);

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.as<uint32_t>() == 0xFFFFFFFF);
    }

    SUBCASE("cf 00 00 00 01 00 00 00 00") {
      auto err =
          deserializeMsgPack(doc, "\xcf\x00\x00\x00\x01\x00\x00\x00\x00"_s);

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.isNull());
    }
  }
}
