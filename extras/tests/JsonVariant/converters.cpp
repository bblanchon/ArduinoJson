// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

namespace {
struct Date {
  int day;
  int month;
  int year;
};

void convertToJson(const Date& src, JsonVariant dst) {
  dst["day"] = src.day;
  dst["month"] = src.month;
  dst["year"] = src.year;
}

void convertFromJson(JsonVariantConst src, Date& dst) {
  dst.day = src["day"];
  dst.month = src["month"];
  dst.year = src["year"];
}

bool canConvertFromJson(JsonVariantConst src, const Date&) {
  return src["day"].is<int>() && src["month"].is<int>() &&
         src["year"].is<int>();
}
}  // namespace

TEST_CASE("Custom converter with overloading") {
  DynamicJsonDocument doc(4096);

  SECTION("convert JSON to Date") {
    doc["date"]["day"] = 2;
    doc["date"]["month"] = 3;
    doc["date"]["year"] = 2021;

    Date date = doc["date"];

    REQUIRE(date.day == 2);
    REQUIRE(date.month == 3);
    REQUIRE(date.year == 2021);
  }

  SECTION("is<Date>() returns true") {
    doc["date"]["day"] = 2;
    doc["date"]["month"] = 3;
    doc["date"]["year"] = 2021;

    REQUIRE(doc["date"].is<Date>());
  }

  SECTION("is<Date>() returns false") {
    doc["date"]["day"] = 2;
    doc["date"]["month"] = 3;
    doc["date"]["year"] = "2021";

    REQUIRE(doc["date"].is<Date>() == false);
  }

  SECTION("convert Date to JSON") {
    Date date = {19, 3, 2021};
    doc["date"] = date;

    REQUIRE(doc["date"]["day"] == 19);
    REQUIRE(doc["date"]["month"] == 3);
    REQUIRE(doc["date"]["year"] == 2021);
  }
}

class Complex {
 public:
  explicit Complex(double r, double i) : _real(r), _imag(i) {}

  double real() const {
    return _real;
  }

  double imag() const {
    return _imag;
  }

 private:
  double _real, _imag;
};

namespace ArduinoJson {
template <>
struct Converter<Complex> {
  static void toJson(const Complex& src, JsonVariant dst) {
    dst["real"] = src.real();
    dst["imag"] = src.imag();
  }

  static Complex fromJson(JsonVariantConst src) {
    return Complex(src["real"], src["imag"]);
  }

  static bool checkJson(JsonVariantConst src) {
    return src["real"].is<double>() && src["imag"].is<double>();
  }
};
}  // namespace ArduinoJson

TEST_CASE("Custom converter with specialization") {
  DynamicJsonDocument doc(4096);

  SECTION("convert JSON to Complex") {
    doc["value"]["real"] = 2;
    doc["value"]["imag"] = 3;

    Complex value = doc["value"];

    REQUIRE(value.real() == 2);
    REQUIRE(value.imag() == 3);
  }

  SECTION("is<Complex>() returns true") {
    doc["value"]["real"] = 2;
    doc["value"]["imag"] = 3;

    REQUIRE(doc["value"].is<Complex>());
  }

  SECTION("is<Complex>() returns false") {
    doc["value"]["real"] = 2;
    doc["value"]["imag"] = "3";

    REQUIRE(doc["value"].is<Complex>() == false);
  }

  SECTION("convert value to JSON") {
    doc["value"] = Complex(19, 3);

    REQUIRE(doc["value"]["real"] == 19);
    REQUIRE(doc["value"]["imag"] == 3);
  }
}

TEST_CASE("ConverterNeedsWriteableRef") {
  using namespace ArduinoJson::detail;
  CHECK(ConverterNeedsWriteableRef<int>::value == false);
  CHECK(ConverterNeedsWriteableRef<float>::value == false);
  CHECK(ConverterNeedsWriteableRef<JsonVariant>::value == true);
  CHECK(ConverterNeedsWriteableRef<JsonVariantConst>::value == false);
  CHECK(ConverterNeedsWriteableRef<JsonObject>::value == true);
  CHECK(ConverterNeedsWriteableRef<JsonObjectConst>::value == false);
  CHECK(ConverterNeedsWriteableRef<JsonArray>::value == true);
  CHECK(ConverterNeedsWriteableRef<JsonArrayConst>::value == false);
}
