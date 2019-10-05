#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h>

#include <catch.hpp>

template <size_t size_of_long>
std::string get_expected_json();

template <>
std::string get_expected_json<4>() {
  return "{\"A\":2899336981,\"B\":2129924785}";
}

template <>
std::string get_expected_json<8>() {
  return "{\"A\":123456789123456789,\"B\":987654321987654321}";
}

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 0") {
  DynamicJsonDocument doc(4096);
  JsonObject root = doc.to<JsonObject>();

  root["A"] = 123456789123456789;
  root["B"] = 987654321987654321;

  std::string json;
  serializeJson(doc, json);

  REQUIRE(json == get_expected_json<sizeof(long)>());
}
