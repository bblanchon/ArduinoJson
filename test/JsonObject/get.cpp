// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::get()") {
  DynamicJsonBuffer jb;
  JsonObject& obj = jb.createObject();

  SECTION("GetConstCharPointer_GivenStringLiteral") {
    obj.set("hello", "world");
    const char* value = obj.get<const char*>("hello");
    REQUIRE_THAT(value, Equals("world"));
  }
}
