// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

struct CCLASS {
  static const char mszKey[];
};

TEST_CASE("Issue #2166") {
  JsonDocument doc;
  doc[CCLASS::mszKey] = 12;
  REQUIRE(doc.as<std::string>() == "{\"test3\":12}");

  JsonObject obj = doc.to<JsonObject>();
  obj[CCLASS::mszKey] = 12;
  REQUIRE(doc.as<std::string>() == "{\"test3\":12}");
}

const char CCLASS::mszKey[] = "test3";
