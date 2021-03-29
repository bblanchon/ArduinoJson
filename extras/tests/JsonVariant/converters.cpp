// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
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

bool convertToJson(JsonVariant variant, const Date& date) {
  variant["day"] = date.day;
  variant["month"] = date.month;
  variant["year"] = date.year;
  return true;
}

void convertFromJson(Date& date, JsonVariantConst variant) {
  date.day = variant["day"];
  date.month = variant["month"];
  date.year = variant["year"];
}

bool canConvertFromJson(Date&, JsonVariantConst variant) {
  return variant["day"].is<int>() && variant["month"].is<int>() &&
         variant["year"].is<int>();
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

namespace ARDUINOJSON_NAMESPACE {
template <>
struct Converter<Complex> {
  static bool toJson(VariantRef variant, const Complex& value) {
    variant["real"] = value.real();
    variant["imag"] = value.imag();
    return true;
  }

  static Complex fromJson(VariantConstRef variant) {
    return Complex(variant["real"], variant["imag"]);
  }

  static bool checkJson(VariantConstRef variant) {
    return variant["real"].is<double>() && variant["imag"].is<double>();
  }
};
}  // namespace ARDUINOJSON_NAMESPACE

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
