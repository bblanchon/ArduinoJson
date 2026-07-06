// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <doctest.h>

#include <string>

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/Writers/StaticStringWriter.hpp>

using namespace ArduinoJson::detail;

void check(const char* input, std::string expected) {
  char output[64] = {0};
  StaticStringWriter sb(output, sizeof(output));
  TextFormatter<StaticStringWriter> writer(sb);
  writer.writeString(input);
  REQUIRE(expected == output);
  REQUIRE(writer.bytesWritten() == expected.size());
}

TEST_CASE("TextFormatter::writeString()") {
  SUBCASE("EmptyString") {
    check("", "\"\"");
  }

  SUBCASE("QuotationMark") {
    check("\"", "\"\\\"\"");
  }

  SUBCASE("ReverseSolidus") {
    check("\\", "\"\\\\\"");
  }

  SUBCASE("Solidus") {
    check("/", "\"/\"");  // but the JSON format allows \/
  }

  SUBCASE("Backspace") {
    check("\b", "\"\\b\"");
  }

  SUBCASE("Formfeed") {
    check("\f", "\"\\f\"");
  }

  SUBCASE("Newline") {
    check("\n", "\"\\n\"");
  }

  SUBCASE("CarriageReturn") {
    check("\r", "\"\\r\"");
  }

  SUBCASE("HorizontalTab") {
    check("\t", "\"\\t\"");
  }
}
