// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#define true 0x1
#define false 0x0

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Issue #2181") {
  JsonDocument doc;
  doc["hello"] = "world";
  REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
}
