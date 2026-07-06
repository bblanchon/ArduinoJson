// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("measureMsgPack()") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object["hello"] = "world";

  REQUIRE(measureMsgPack(doc) == 13);
}
