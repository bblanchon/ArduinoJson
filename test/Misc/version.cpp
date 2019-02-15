// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/version.hpp>
#include <catch.hpp>
#include <sstream>

TEST_CASE("ARDUINOJSON_VERSION") {
  std::stringstream version;

  version << ARDUINOJSON_VERSION_MAJOR << "." << ARDUINOJSON_VERSION_MINOR
          << "." << ARDUINOJSON_VERSION_REVISION;

  REQUIRE(version.str() == ARDUINOJSON_VERSION);
}
