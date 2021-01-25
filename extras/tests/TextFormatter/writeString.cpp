// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <catch.hpp>

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/Writers/StaticStringWriter.hpp>

using namespace ARDUINOJSON_NAMESPACE;

void check(const char* input, std::string expected) {
  char output[1024];
  StaticStringWriter sb(output, sizeof(output));
  TextFormatter<StaticStringWriter> writer(sb);
  writer.writeString(input);
  REQUIRE(expected == output);
  REQUIRE(writer.bytesWritten() == expected.size());
}

TEST_CASE("TextFormatter::writeString()") {
  SECTION("EmptyString") {
    check("", "\"\"");
  }

  SECTION("QuotationMark") {
    check("\"", "\"\\\"\"");
  }

  SECTION("ReverseSolidus") {
    check("\\", "\"\\\\\"");
  }

  SECTION("Solidus") {
    check("/", "\"/\"");  // but the JSON format allows \/
  }

  SECTION("Backspace") {
    check("\b", "\"\\b\"");
  }

  SECTION("Formfeed") {
    check("\f", "\"\\f\"");
  }

  SECTION("Newline") {
    check("\n", "\"\\n\"");
  }

  SECTION("CarriageReturn") {
    check("\r", "\"\\r\"");
  }

  SECTION("HorizontalTab") {
    check("\t", "\"\\t\"");
  }
}
