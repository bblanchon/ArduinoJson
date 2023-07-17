// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <Arduino.h>
#include <catch.hpp>

#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#include <ArduinoJson.h>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofString;

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
    JsonDocument doc;
    const char* value = "example";

    doc.set(666);  // to make sure we override the value

    SECTION("Via Print::write(char)") {
      PrintableString<PrintOneCharacterAtATime> printable(value);
      CHECK(doc.set(printable) == true);
      CHECK(doc.as<std::string>() == value);
      CHECK(printable.totalBytesWritten() == 7);
      CHECK(doc.overflowed() == false);
      CHECK(doc.memoryUsage() == sizeofString(7));
      CHECK(doc.as<JsonVariant>().memoryUsage() == sizeofString(7));
    }

    SECTION("Via Print::write(const char* size_t)") {
      PrintableString<PrintAllAtOnce> printable(value);
      CHECK(doc.set(printable) == true);
      CHECK(doc.as<std::string>() == value);
      CHECK(printable.totalBytesWritten() == 7);
      CHECK(doc.overflowed() == false);
      CHECK(doc.memoryUsage() == sizeofString(7));
      CHECK(doc.as<JsonVariant>().memoryUsage() == sizeofString(7));
    }
  }

  SECTION("First allocation fails") {
    SpyingAllocator spyingAllocator(FailingAllocator::instance());
    JsonDocument doc(&spyingAllocator);
    const char* value = "hello world";

    doc.set(666);  // to make sure we override the value

    SECTION("Via Print::write(char)") {
      PrintableString<PrintOneCharacterAtATime> printable(value);

      bool success = doc.set(printable);

      CHECK(success == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 0);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
      CHECK(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::AllocateFail(sizeofString(31)));
    }

    SECTION("Via Print::write(const char*, size_t)") {
      PrintableString<PrintAllAtOnce> printable(value);

      bool success = doc.set(printable);

      CHECK(success == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 0);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
      CHECK(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::AllocateFail(sizeofString(31)));
    }
  }

  SECTION("Reallocation fails") {
    TimebombAllocator timebombAllocator(1);
    SpyingAllocator spyingAllocator(&timebombAllocator);
    JsonDocument doc(&spyingAllocator);
    const char* value = "Lorem ipsum dolor sit amet, cons";  // > 31 chars

    doc.set(666);  // to make sure we override the value

    SECTION("Via Print::write(char)") {
      PrintableString<PrintOneCharacterAtATime> printable(value);

      bool success = doc.set(printable);

      CHECK(success == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 31);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
      CHECK(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::ReallocateFail(sizeofString(31),
                                                           sizeofString(63))
                           << AllocatorLog::Deallocate(sizeofString(31)));
    }

    SECTION("Via Print::write(const char*, size_t)") {
      PrintableString<PrintAllAtOnce> printable(value);

      bool success = doc.set(printable);

      CHECK(success == false);
      CHECK(doc.isNull());
      CHECK(printable.totalBytesWritten() == 31);
      CHECK(doc.overflowed() == true);
      CHECK(doc.memoryUsage() == 0);
      CHECK(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::ReallocateFail(sizeofString(31),
                                                           sizeofString(63))
                           << AllocatorLog::Deallocate(sizeofString(31)));
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
    JsonDocument doc;
    doc.add(PrintableString<PrintOneCharacterAtATime>("Hello World!"));
    doc.add(PrintableString<PrintAllAtOnce>("Hello World!"));
    REQUIRE(doc.size() == 2);
    CHECK(doc[0] == "Hello World!");
    CHECK(doc[1] == "Hello World!");
    CHECK(doc.memoryUsage() == sizeofArray(2) + sizeofString(12));
  }
}
