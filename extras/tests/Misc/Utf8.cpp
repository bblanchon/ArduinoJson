// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <string>

using namespace ARDUINOJSON_NAMESPACE;

static void testCodepoint(uint32_t codepoint, std::string expected) {
  char buffer[4096];
  MemoryPool pool(buffer, 4096);
  StringCopier str(pool);
  str.startString();

  CAPTURE(codepoint);
  Utf8::encodeCodepoint(codepoint, str);

  str.append('\0');
  REQUIRE(str.c_str() == expected);
}

TEST_CASE("Utf8::encodeCodepoint()") {
  SECTION("U+0000") {
    testCodepoint(0x0000, "");
  }

  SECTION("U+0001") {
    testCodepoint(0x0001, "\x01");
  }

  SECTION("U+007F") {
    testCodepoint(0x007F, "\x7f");
  }

  SECTION("U+0080") {
    testCodepoint(0x0080, "\xc2\x80");
  }

  SECTION("U+07FF") {
    testCodepoint(0x07FF, "\xdf\xbf");
  }

  SECTION("U+0800") {
    testCodepoint(0x0800, "\xe0\xa0\x80");
  }

  SECTION("U+FFFF") {
    testCodepoint(0xFFFF, "\xef\xbf\xbf");
  }

  SECTION("U+10000") {
    testCodepoint(0x10000, "\xf0\x90\x80\x80");
  }

  SECTION("U+10FFFF") {
    testCodepoint(0x10FFFF, "\xf4\x8f\xbf\xbf");
  }
}
