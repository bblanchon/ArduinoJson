// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void eraseString(std::string &str) {
  char *p = const_cast<char *>(str.c_str());
  while (*p) *p++ = '*';
}

TEST_CASE("std::string") {
  DynamicJsonDocument doc(4096);
  JsonArray array = doc.to<JsonArray>();

  SECTION("add()") {
    std::string value("hello");
    array.add(value);
    eraseString(value);
    REQUIRE(std::string("hello") == array[0]);
  }

  SECTION("operator[]") {
    std::string value("world");
    array.add("hello");
    array[0] = value;
    eraseString(value);
    REQUIRE(std::string("world") == array[0]);
  }
}
