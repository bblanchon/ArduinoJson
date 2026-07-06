// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <doctest.h>
#include <ArduinoJson/Json/Utf16.hpp>

using namespace ArduinoJson::detail;

static void testUtf16Codepoint(uint16_t codeunit, uint32_t expectedCodepoint) {
  Utf16::Codepoint cp;
  REQUIRE(cp.append(codeunit) == true);
  REQUIRE(cp.value() == expectedCodepoint);
}

static void testUtf16Codepoint(uint16_t codeunit1, uint16_t codeunit2,
                               uint32_t expectedCodepoint) {
  Utf16::Codepoint cp;
  REQUIRE(cp.append(codeunit1) == false);
  REQUIRE(cp.append(codeunit2) == true);
  REQUIRE(cp.value() == expectedCodepoint);
}

TEST_CASE("Utf16::Codepoint()") {
  SUBCASE("U+0000") {
    testUtf16Codepoint(0x0000, 0x000000);
  }

  SUBCASE("U+0001") {
    testUtf16Codepoint(0x0001, 0x000001);
  }

  SUBCASE("U+D7FF") {
    testUtf16Codepoint(0xD7FF, 0x00D7FF);
  }

  SUBCASE("U+E000") {
    testUtf16Codepoint(0xE000, 0x00E000);
  }

  SUBCASE("U+FFFF") {
    testUtf16Codepoint(0xFFFF, 0x00FFFF);
  }

  SUBCASE("U+010000") {
    testUtf16Codepoint(0xD800, 0xDC00, 0x010000);
  }

  SUBCASE("U+010001") {
    testUtf16Codepoint(0xD800, 0xDC01, 0x010001);
  }

  SUBCASE("U+0103FF") {
    testUtf16Codepoint(0xD800, 0xDFFF, 0x0103FF);
  }

  SUBCASE("U+010400") {
    testUtf16Codepoint(0xD801, 0xDC00, 0x010400);
  }

  SUBCASE("U+010400") {
    testUtf16Codepoint(0xDBFF, 0xDC00, 0x10FC00);
  }

  SUBCASE("U+10FFFF") {
    testUtf16Codepoint(0xDBFF, 0xDFFF, 0x10FFFF);
  }
}
