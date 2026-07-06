// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <doctest.h>
#include <limits>
#include <string>

#define ARDUINOJSON_ENABLE_NAN 1
#define ARDUINOJSON_ENABLE_INFINITY 1
#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/Writer.hpp>

using namespace ArduinoJson::detail;

template <typename TFloat>
void check(TFloat input, const std::string& expected) {
  std::string output;
  Writer<std::string> sb(output);
  TextFormatter<Writer<std::string>> writer(sb);
  writer.writeFloat(input);
  REQUIRE(writer.bytesWritten() == output.size());
  CHECK(expected == output);
}

TEST_CASE("TextFormatter::writeFloat(double)") {
  SUBCASE("Pi") {
    check<double>(3.14159265359, "3.141592654");
  }

  SUBCASE("Signaling NaN") {
    double nan = std::numeric_limits<double>::signaling_NaN();
    check<double>(nan, "NaN");
  }

  SUBCASE("Quiet NaN") {
    double nan = std::numeric_limits<double>::quiet_NaN();
    check<double>(nan, "NaN");
  }

  SUBCASE("Infinity") {
    double inf = std::numeric_limits<double>::infinity();
    check<double>(inf, "Infinity");
    check<double>(-inf, "-Infinity");
  }

  SUBCASE("Zero") {
    check<double>(0.0, "0");
    check<double>(-0.0, "0");
  }

  SUBCASE("Espilon") {
    check<double>(2.2250738585072014E-308, "2.225073859e-308");
    check<double>(-2.2250738585072014E-308, "-2.225073859e-308");
  }

  SUBCASE("Max double") {
    check<double>(1.7976931348623157E+308, "1.797693135e308");
    check<double>(-1.7976931348623157E+308, "-1.797693135e308");
  }

  SUBCASE("Big exponent") {
    // this test increases coverage of normalize()
    check<double>(1e255, "1e255");
    check<double>(1e-255, "1e-255");
  }

  SUBCASE("Exponentation when <= 1e-5") {
    check<double>(1e-4, "0.0001");
    check<double>(1e-5, "1e-5");

    check<double>(-1e-4, "-0.0001");
    check<double>(-1e-5, "-1e-5");
  }

  SUBCASE("Exponentation when >= 1e7") {
    check<double>(9999999.999, "9999999.999");
    check<double>(10000000.0, "1e7");

    check<double>(-9999999.999, "-9999999.999");
    check<double>(-10000000.0, "-1e7");
  }

  SUBCASE("Rounding when too many decimals") {
    check<double>(0.000099999999999, "0.0001");
    check<double>(0.0000099999999999, "1e-5");
    check<double>(0.9999999996, "1");
  }

  SUBCASE("9 decimal places") {
    check<double>(0.100000001, "0.100000001");
    check<double>(0.999999999, "0.999999999");

    check<double>(9.000000001, "9.000000001");
    check<double>(9.999999999, "9.999999999");
  }

  SUBCASE("10 decimal places") {
    check<double>(0.1000000001, "0.1");
    check<double>(0.9999999999, "1");

    check<double>(9.0000000001, "9");
    check<double>(9.9999999999, "10");
  }
}

TEST_CASE("TextFormatter::writeFloat(float)") {
  SUBCASE("Pi") {
    check<float>(3.14159265359f, "3.141593");
  }

  SUBCASE("999.9") {  // issue #543
    check<float>(999.9f, "999.9");
  }

  SUBCASE("24.3") {  // # issue #588
    check<float>(24.3f, "24.3");
  }
}
