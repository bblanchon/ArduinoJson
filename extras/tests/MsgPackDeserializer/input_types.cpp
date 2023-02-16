// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "CustomReader.hpp"

TEST_CASE("deserializeMsgPack(const std::string&)") {
  DynamicJsonDocument doc(4096);

  SECTION("should accept const string") {
    const std::string input("\x92\x01\x02");

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("should accept temporary string") {
    DeserializationError err =
        deserializeMsgPack(doc, std::string("\x92\x01\x02"));

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("should duplicate content") {
    std::string input("\x91\xA5hello");

    DeserializationError err = deserializeMsgPack(doc, input);
    input[2] = 'X';  // alter the string tomake sure we made a copy

    JsonArray array = doc.as<JsonArray>();
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(std::string("hello") == array[0]);
  }

  SECTION("should accept a zero in input") {
    DeserializationError err =
        deserializeMsgPack(doc, std::string("\x92\x00\x02", 3));

    REQUIRE(err == DeserializationError::Ok);
    JsonArray arr = doc.as<JsonArray>();
    REQUIRE(arr[0] == 0);
    REQUIRE(arr[1] == 2);
  }
}

TEST_CASE("deserializeMsgPack(std::istream&)") {
  DynamicJsonDocument doc(4096);

  SECTION("should accept a zero in input") {
    std::istringstream input(std::string("\x92\x00\x02", 3));

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
    JsonArray arr = doc.as<JsonArray>();
    REQUIRE(arr[0] == 0);
    REQUIRE(arr[1] == 2);
  }

  SECTION("should detect incomplete input") {
    std::istringstream input("\x92\x00\x02");

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::IncompleteInput);
  }
}

#ifdef HAS_VARIABLE_LENGTH_ARRAY
TEST_CASE("deserializeMsgPack(VLA)") {
  size_t i = 16;
  char vla[i];
  memcpy(vla, "\xDE\x00\x01\xA5Hello\xA5world", 15);

  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  DeserializationError err = deserializeMsgPack(doc, vla);

  REQUIRE(err == DeserializationError::Ok);
}
#endif

TEST_CASE("deserializeMsgPack(CustomReader)") {
  DynamicJsonDocument doc(4096);
  CustomReader reader("\x92\xA5Hello\xA5world");
  DeserializationError err = deserializeMsgPack(doc, reader);

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc.size() == 2);
  REQUIRE(doc[0] == "Hello");
  REQUIRE(doc[1] == "world");
}
