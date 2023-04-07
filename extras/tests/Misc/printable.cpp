// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <Arduino.h>
#include <catch.hpp>

#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#include <ArduinoJson.h>

struct PrintOneCharacterAtATime {
  static size_t printStringTo(const std::string& s, Print& p) {
    size_t result = 0;
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
      size_t n = p.write(uint8_t(*it));
      if (n == 0)
        break;
      result += n;
    }
    return result;
  }
};

struct PrintAllAtOnce {
  static size_t printStringTo(const std::string& s, Print& p) {
    return p.write(s.data(), s.size());
  }
};

template <typename PrintPolicy>
struct PrintableString : public Printable {
  PrintableString(const char* s) : str_(s), total_(0) {}

  virtual size_t printTo(Print& p) const {
    size_t result = PrintPolicy::printStringTo(str_, p);
    total_ += result;
    return result;
  }

  size_t totalBytesWritten() const {
    return total_;
  }

 private:
  std::string str_;
  mutable size_t total_;
};

TEST_CASE("Printable") {
  SECTION("Doesn't overflow") {
    StaticJsonDocument<8> doc;
    const char* value = "example";  // == 7 chars

    doc.set(666);  // to make sure we override the value

    SECTION("Via Print::write(char)") {
      PrintableString<PrintOneCharacterAtATime> printable(value);
      CHECK(doc.set(printable) == true);
      CHECK(doc.as<std::string>() == value);
      CHECK(printable.totalBytesWritten() == 7);
      CHECK(doc.overflowed() == false);
      CHECK(doc.memoryUsage() == 8);
      CHECK(doc.as<JsonVariant>().memoryUsage() == 8);
    }

    SECTION("Via Print::write(const char* size_t)") {
      PrintableString<PrintAllAtOnce> printable(value);
      CHECK(doc.set(printable) == true);
      CHECK(doc.as<std::string>() == value);
      CHECK(printable.totalBytesWritten() == 7);
      CHECK(doc.overflowed() == false);
      CHECK(doc.memoryUsage() == 8);
      CHECK(doc.as<JsonVariant>().memoryUsage() == 8);
    }
  }

  SECTION("Overflows early") {
    StaticJsonDocument<8> doc;
    const char* value = "hello world";  // > 8 chars

    doc.set(666);  // to make sure we override the value

    SECTION("Via Print::write(char)") {
      PrintableString<PrintOneCharacterAtATime> printable(value);
      CHECK(doc.set(printable) == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 8);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
    }

    SECTION("Via Print::write(const char*, size_t)") {
      PrintableString<PrintAllAtOnce> printable(value);
      CHECK(doc.set(printable) == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 0);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
    }
  }

  SECTION("Overflows adding terminator") {
    StaticJsonDocument<8> doc;
    const char* value = "overflow";  // == 8 chars

    doc.set(666);  // to make sure we override the value

    SECTION("Via Print::write(char)") {
      PrintableString<PrintOneCharacterAtATime> printable(value);
      CHECK(doc.set(printable) == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 8);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
    }

    SECTION("Via Print::write(const char*, size_t)") {
      PrintableString<PrintAllAtOnce> printable(value);
      CHECK(doc.set(printable) == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 0);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
    }
  }

  SECTION("Null variant") {
    JsonVariant var;
    PrintableString<PrintOneCharacterAtATime> printable = "Hello World!";
    CHECK(var.set(printable) == false);
    CHECK(var.isNull());
    CHECK(printable.totalBytesWritten() == 0);
  }

  SECTION("String deduplication") {
    StaticJsonDocument<128> doc;
    doc.add(PrintableString<PrintOneCharacterAtATime>("Hello World!"));
    doc.add(PrintableString<PrintAllAtOnce>("Hello World!"));
    REQUIRE(doc.size() == 2);
    CHECK(doc[0] == "Hello World!");
    CHECK(doc[1] == "Hello World!");
    CHECK(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 13);
  }
}
