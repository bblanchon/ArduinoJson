// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

class CustomWriter {
 public:
  CustomWriter() {}
  CustomWriter(const CustomWriter&) = delete;
  CustomWriter& operator=(const CustomWriter&) = delete;

  size_t write(uint8_t c) {
    str_.append(1, static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    str_.append(reinterpret_cast<const char*>(s), n);
    return n;
  }

  const std::string& str() const {
    return str_;
  }

 private:
  std::string str_;
};

TEST_CASE("CustomWriter") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  array.add(4);
  array.add(2);

  SUBCASE("serializeJson()") {
    CustomWriter writer;
    serializeJson(array, writer);

    REQUIRE("[4,2]" == writer.str());
  }

  SUBCASE("serializeJsonPretty") {
    CustomWriter writer;
    serializeJsonPretty(array, writer);

    REQUIRE("[\r\n  4,\r\n  2\r\n]" == writer.str());
  }
}
