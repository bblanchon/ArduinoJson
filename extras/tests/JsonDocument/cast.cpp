// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include <string>

TEST_CASE("Implicit cast to JsonVariant") {
  JsonDocument doc;

  doc["hello"] = "world";

  JsonVariant var = doc;

  CHECK(var.as<std::string>() == "{\"hello\":\"world\"}");
}
