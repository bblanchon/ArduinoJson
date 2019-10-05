// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StdStreamReader") {
  std::istringstream src("\x01\xFF");
  StdStreamReader reader(src);

  REQUIRE(reader.read() == 0x01);
  REQUIRE(reader.read() == 0xFF);
  REQUIRE(reader.read() == -1);
}

TEST_CASE("SafeCharPointerReader") {
  SafeCharPointerReader reader("\x01\xFF", 2);

  REQUIRE(reader.read() == 0x01);
  REQUIRE(reader.read() == 0xFF);
  REQUIRE(reader.read() == -1);
}

TEST_CASE("UnsafeCharPointerReader") {
  UnsafeCharPointerReader reader("\x01\xFF");

  REQUIRE(reader.read() == 0x01);
  REQUIRE(reader.read() == 0xFF);
  REQUIRE(reader.read() == 0);
}
