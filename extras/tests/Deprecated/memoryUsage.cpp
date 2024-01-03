// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::memoryUsage()") {
  JsonArray array;
  REQUIRE(array.memoryUsage() == 0);
}

TEST_CASE("JsonArrayConst::memoryUsage()") {
  JsonArrayConst array;
  REQUIRE(array.memoryUsage() == 0);
}

TEST_CASE("JsonDocument::memoryUsage()") {
  JsonDocument doc;
  REQUIRE(doc.memoryUsage() == 0);
}

TEST_CASE("JsonObject::memoryUsage()") {
  JsonObject array;
  REQUIRE(array.memoryUsage() == 0);
}

TEST_CASE("JsonObjectConst::memoryUsage()") {
  JsonObjectConst array;
  REQUIRE(array.memoryUsage() == 0);
}

TEST_CASE("JsonVariant::memoryUsage()") {
  JsonVariant doc;
  REQUIRE(doc.memoryUsage() == 0);
}

TEST_CASE("JsonVariantConst::memoryUsage()") {
  JsonVariantConst doc;
  REQUIRE(doc.memoryUsage() == 0);
}

TEST_CASE("ElementProxy::memoryUsage()") {
  JsonDocument doc;
  REQUIRE(doc[0].memoryUsage() == 0);
}

TEST_CASE("MemberProxy::memoryUsage()") {
  JsonDocument doc;
  REQUIRE(doc["hello"].memoryUsage() == 0);
}
