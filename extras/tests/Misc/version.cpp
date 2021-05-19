// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson/version.hpp>
#include <catch.hpp>
#include <sstream>

using Catch::Matchers::StartsWith;

TEST_CASE("ARDUINOJSON_VERSION") {
  std::stringstream version;

  version << ARDUINOJSON_VERSION_MAJOR << "." << ARDUINOJSON_VERSION_MINOR
          << "." << ARDUINOJSON_VERSION_REVISION;

  REQUIRE_THAT(ARDUINOJSON_VERSION, StartsWith(version.str()));
}
