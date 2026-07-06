#define ARDUINOJSON_DECODE_UNICODE 0
#include <ArduinoJson.h>

#include <doctest.h>

TEST_CASE("ARDUINOJSON_DECODE_UNICODE == 0") {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, "\"\\uD834\\uDD1E\"");

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc.as<std::string>() == "\\uD834\\uDD1E");
}
