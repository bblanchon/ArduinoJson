// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename T>
class Nullable {
 public:
  Nullable() : value_{} {}
  Nullable(T value) : value_{value} {}

  operator T() const {
    return value_;
  }

  operator T&() {
    return value_;
  }

  bool is_valid() const {
    return value_ != invalid_value_;
  }

  T value() const {
    return value_;
  }

 private:
  T value_;
  static T invalid_value_;
};

template <>
float Nullable<float>::invalid_value_ = std::numeric_limits<float>::lowest();

template <typename T>
void convertToJson(const Nullable<T>& src, JsonVariant dst) {
  if (src.is_valid()) {
    dst.set(src.value());
  } else {
    dst.clear();
  }
}

TEST_CASE("Issue #2129") {
  Nullable<float> nullable_value = Nullable<float>{123.4f};

  JsonDocument doc;

  doc["value"] = nullable_value;

  REQUIRE(doc["value"].as<float>() == Approx(123.4f));
}
