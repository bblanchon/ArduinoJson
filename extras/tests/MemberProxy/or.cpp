// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::operator|()") {
  DynamicJsonDocument doc(4096);

  SECTION("const char*") {
    doc["a"] = "hello";

    REQUIRE((doc["a"] | "world") == std::string("hello"));
    REQUIRE((doc["b"] | "world") == std::string("world"));
  }

  SECTION("Issue #1411") {
    doc["sensor"] = "gps";

    const char *test = "test";  // <- the literal must be captured in a variable
                                // to trigger the bug
    const char *sensor = doc["sensor"] | test;  // "gps"

    REQUIRE(sensor == std::string("gps"));
  }
}
