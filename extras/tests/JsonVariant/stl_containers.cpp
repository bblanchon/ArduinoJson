// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include <array>
#include <string>
#include <vector>

namespace ArduinoJson {
template <typename T>
struct Converter<std::vector<T>> {
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
    for (JsonVariantConst item : array)
      result &= item.is<T>();
    return result;
  }
};

template <typename T, size_t N>
struct Converter<std::array<T, N>> {
  static void toJson(const std::array<T, N>& src, JsonVariant dst) {
    JsonArray array = dst.to<JsonArray>();
    for (T item : src)
      array.add(item);
  }

  static std::array<T, N> fromJson(JsonVariantConst src) {
    std::array<T, N> dst;
    dst.fill(0);
    size_t idx = 0;
    for (T item : src.as<JsonArrayConst>())
      dst[idx++] = item;
    return dst;
  }

  static bool checkJson(JsonVariantConst src) {
    JsonArrayConst array = src;
    bool result = array;
    size_t size = 0;
    for (JsonVariantConst item : array) {
      result &= item.is<T>();
      size++;
    }
    return result && size == N;
  }
};
}  // namespace ArduinoJson

TEST_CASE("vector<int>") {
  SECTION("toJson") {
    std::vector<int> v = {1, 2};

    JsonDocument doc;
    doc.set(v);
    REQUIRE(doc.as<std::string>() == "[1,2]");
  }

  SECTION("fromJson") {
    JsonDocument doc;
    doc.add(1);
    doc.add(2);

    auto v = doc.as<std::vector<int>>();
    REQUIRE(v.size() == 2);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
  }

  SECTION("checkJson") {
    JsonDocument doc;
    CHECK(doc.is<std::vector<int>>() == false);

    doc.add(1);
    doc.add(2);
    CHECK(doc.is<std::vector<int>>() == true);

    doc.add("foo");
    CHECK(doc.is<std::vector<int>>() == false);
  }
}

TEST_CASE("array<int, 2>") {
  using array_type = std::array<int, 2>;

  SECTION("toJson") {
    array_type v;
    v[0] = 1;
    v[1] = 2;

    JsonDocument doc;
    doc.set(v);
    REQUIRE(doc.as<std::string>() == "[1,2]");
  }

  SECTION("fromJson") {
    JsonDocument doc;
    doc.add(1);
    doc.add(2);

    auto v = doc.as<array_type>();
    REQUIRE(v.size() == 2);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
  }

  SECTION("checkJson") {
    JsonDocument doc;
    CHECK(doc.is<array_type>() == false);

    doc.add(1);
    CHECK(doc.is<array_type>() == false);

    doc.add(2);
    CHECK(doc.is<array_type>() == true);

    doc[0] = "foo";
    CHECK(doc.is<array_type>() == false);
  }
}
