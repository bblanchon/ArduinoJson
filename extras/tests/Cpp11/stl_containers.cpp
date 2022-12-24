// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include <string>
#include <vector>

namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct Converter<std::vector<T> > {
  static void toJson(const std::vector<T>& src, JsonVariant dst) {
    JsonArray array = dst.to<JsonArray>();
    for (T item : src)
      array.add(item);
  }

  static std::vector<T> fromJson(JsonVariantConst src) {
    std::vector<T> dst;
    for (T item : src.as<JsonArrayConst>())
      dst.push_back(item);
    return dst;
  }

  static bool checkJson(JsonVariantConst src) {
    JsonArrayConst array = src;
    bool result = array;
    for (JsonVariantConst item : array) {
      if (!result)
        break;
      result = item.is<T>();
    }
    return result;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE

TEST_CASE("vector<int>") {
  SECTION("toJson") {
    std::vector<int> v = {1, 2};

    StaticJsonDocument<128> doc;
    doc.set(v);
    REQUIRE(doc.as<std::string>() == "[1,2]");
  }

  SECTION("fromJson") {
    StaticJsonDocument<128> doc;
    doc.add(1);
    doc.add(2);

    auto v = doc.as<std::vector<int> >();
    REQUIRE(v.size() == 2);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
  }

  SECTION("checkJson") {
    StaticJsonDocument<128> doc;
    CHECK(doc.is<std::vector<int> >() == false);

    doc.add(1);
    doc.add(2);
    CHECK(doc.is<std::vector<int> >() == true);

    doc.add("foo");
    CHECK(doc.is<std::vector<int> >() == false);
  }
}
