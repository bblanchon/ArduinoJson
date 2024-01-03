// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

template <typename T>
static void checkValue(const char* input, T expected) {
  JsonDocument doc;

  DeserializationError error = deserializeMsgPack(doc, input);

  REQUIRE(error == DeserializationError::Ok);
  REQUIRE(doc.is<T>());
  REQUIRE(doc.as<T>() == expected);
}

static void checkError(size_t timebombCountDown, const char* input,
                       DeserializationError expected) {
  TimebombAllocator timebomb(timebombCountDown);
  JsonDocument doc(&timebomb);

  DeserializationError error = deserializeMsgPack(doc, input);

  CAPTURE(input);
  REQUIRE(error == expected);
}

TEST_CASE("deserialize MsgPack value") {
  SECTION("nil") {
    checkValue("\xc0", nullptr);
  }

  SECTION("bool") {
    checkValue<bool>("\xc2", false);
    checkValue<bool>("\xc3", true);
  }

  SECTION("positive fixint") {
    checkValue<int>("\x00", 0);
    checkValue<int>("\x7F", 127);
  }

  SECTION("negative fixint") {
    checkValue<int>("\xe0", -32);
    checkValue<int>("\xff", -1);
  }

  SECTION("uint 8") {
    checkValue<int>("\xcc\x00", 0);
    checkValue<int>("\xcc\xff", 255);
  }

  SECTION("uint 16") {
    checkValue<int>("\xcd\x00\x00", 0);
    checkValue<int>("\xcd\xFF\xFF", 65535);
    checkValue<int>("\xcd\x30\x39", 12345);
  }

  SECTION("uint 32") {
    checkValue<uint32_t>("\xCE\x00\x00\x00\x00", 0x00000000U);
    checkValue<uint32_t>("\xCE\xFF\xFF\xFF\xFF", 0xFFFFFFFFU);
    checkValue<uint32_t>("\xCE\x12\x34\x56\x78", 0x12345678U);
  }

  SECTION("uint 64") {
#if ARDUINOJSON_USE_LONG_LONG
    checkValue<uint64_t>("\xCF\x00\x00\x00\x00\x00\x00\x00\x00", 0U);
    checkValue<uint64_t>("\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
                         0xFFFFFFFFFFFFFFFFU);
    checkValue<uint64_t>("\xCF\x12\x34\x56\x78\x9A\xBC\xDE\xF0",
                         0x123456789ABCDEF0U);
#else
    checkValue("\xCF\x00\x00\x00\x00\x00\x00\x00\x00", nullptr);
    checkValue("\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", nullptr);
    checkValue("\xCF\x12\x34\x56\x78\x9A\xBC\xDE\xF0", nullptr);
#endif
  }

  SECTION("int 8") {
    checkValue<int>("\xd0\x00", 0);
    checkValue<int>("\xd0\xff", -1);
  }

  SECTION("int 16") {
    checkValue<int>("\xD1\x00\x00", 0);
    checkValue<int>("\xD1\xFF\xFF", -1);
    checkValue<int>("\xD1\xCF\xC7", -12345);
  }

  SECTION("int 32") {
    checkValue<int>("\xD2\x00\x00\x00\x00", 0);
    checkValue<int>("\xD2\xFF\xFF\xFF\xFF", -1);
    checkValue<int>("\xD2\xB6\x69\xFD\x2E", -1234567890);
  }

  SECTION("int 64") {
#if ARDUINOJSON_USE_LONG_LONG
    checkValue<int64_t>("\xD3\x00\x00\x00\x00\x00\x00\x00\x00", int64_t(0U));
    checkValue<int64_t>("\xD3\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
                        int64_t(0xFFFFFFFFFFFFFFFFU));
    checkValue<int64_t>("\xD3\x12\x34\x56\x78\x9A\xBC\xDE\xF0",
                        int64_t(0x123456789ABCDEF0));
#else
    checkValue("\xD3\x00\x00\x00\x00\x00\x00\x00\x00", nullptr);
    checkValue("\xD3\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", nullptr);
    checkValue("\xD3\x12\x34\x56\x78\x9A\xBC\xDE\xF0", nullptr);
#endif
  }

  SECTION("float 32") {
    checkValue<double>("\xCA\x00\x00\x00\x00", 0.0f);
    checkValue<double>("\xCA\x40\x48\xF5\xC3", 3.14f);
  }

  SECTION("float 64") {
    checkValue<double>("\xCB\x00\x00\x00\x00\x00\x00\x00\x00", 0.0);
    checkValue<double>("\xCB\x40\x09\x21\xCA\xC0\x83\x12\x6F", 3.1415);
  }

  SECTION("fixstr") {
    checkValue<std::string>("\xA0", std::string(""));
    checkValue<std::string>("\xABhello world", std::string("hello world"));
    checkValue<std::string>("\xBFhello world hello world hello !",
                            std::string("hello world hello world hello !"));
  }

  SECTION("str 8") {
    checkValue<std::string>("\xd9\x05hello", std::string("hello"));
  }

  SECTION("str 16") {
    checkValue<std::string>("\xda\x00\x05hello", std::string("hello"));
  }

  SECTION("str 32") {
    checkValue<std::string>("\xdb\x00\x00\x00\x05hello", std::string("hello"));
  }
}

TEST_CASE("deserializeMsgPack() under memory constaints") {
  SECTION("single values always fit") {
    checkError(0, "\xc0", DeserializationError::Ok);          // nil
    checkError(0, "\xc2", DeserializationError::Ok);          // false
    checkError(0, "\xc3", DeserializationError::Ok);          // true
    checkError(0, "\xcc\x00", DeserializationError::Ok);      // uint 8
    checkError(0, "\xcd\x30\x39", DeserializationError::Ok);  // uint 16
    checkError(0, "\xCE\x12\x34\x56\x78",
               DeserializationError::Ok);  // uint 32
  }

  SECTION("fixstr") {
    checkError(2, "\xA7ZZZZZZZ", DeserializationError::Ok);
    checkError(0, "\xA7ZZZZZZZ", DeserializationError::NoMemory);
  }

  SECTION("str 8") {
    checkError(2, "\xD9\x07ZZZZZZZ", DeserializationError::Ok);
    checkError(0, "\xD9\x07ZZZZZZZ", DeserializationError::NoMemory);
  }

  SECTION("str 16") {
    checkError(2, "\xDA\x00\x07ZZZZZZZ", DeserializationError::Ok);
    checkError(0, "\xDA\x00\x07ZZZZZZZ", DeserializationError::NoMemory);
  }

  SECTION("str 32") {
    checkError(2, "\xDB\x00\x00\x00\x07ZZZZZZZ", DeserializationError::Ok);
    checkError(0, "\xDB\x00\x00\x00\x07ZZZZZZZ",
               DeserializationError::NoMemory);
  }

  SECTION("fixarray") {
    checkError(0, "\x90", DeserializationError::Ok);  // []
    checkError(0, "\x91\x01",
               DeserializationError::NoMemory);  // [1]
    checkError(1, "\x91\x01",
               DeserializationError::Ok);  // [1]
  }

  SECTION("array 16") {
    checkError(0, "\xDC\x00\x00", DeserializationError::Ok);
    checkError(0, "\xDC\x00\x01\x01", DeserializationError::NoMemory);
    checkError(1, "\xDC\x00\x01\x01", DeserializationError::Ok);
  }

  SECTION("array 32") {
    checkError(0, "\xDD\x00\x00\x00\x00", DeserializationError::Ok);
    checkError(0, "\xDD\x00\x00\x00\x01\x01", DeserializationError::NoMemory);
    checkError(1, "\xDD\x00\x00\x00\x01\x01", DeserializationError::Ok);
  }

  SECTION("fixmap") {
    SECTION("{}") {
      checkError(0, "\x80", DeserializationError::Ok);
    }
    SECTION("{H:1}") {
      checkError(0, "\x81\xA1H\x01", DeserializationError::NoMemory);
      checkError(3, "\x81\xA1H\x01", DeserializationError::Ok);
    }
    SECTION("{H:1,W:2}") {
      checkError(3, "\x82\xA1H\x01\xA1W\x02", DeserializationError::NoMemory);
      checkError(5, "\x82\xA1H\x01\xA1W\x02", DeserializationError::Ok);
    }
  }

  SECTION("map 16") {
    SECTION("{}") {
      checkError(0, "\xDE\x00\x00", DeserializationError::Ok);
    }
    SECTION("{H:1}") {
      checkError(2, "\xDE\x00\x01\xA1H\x01", DeserializationError::NoMemory);
      checkError(3, "\xDE\x00\x01\xA1H\x01", DeserializationError::Ok);
    }
    SECTION("{H:1,W:2}") {
      checkError(3, "\xDE\x00\x02\xA1H\x01\xA1W\x02",
                 DeserializationError::NoMemory);
      checkError(5, "\xDE\x00\x02\xA1H\x01\xA1W\x02", DeserializationError::Ok);
    }
  }

  SECTION("map 32") {
    SECTION("{}") {
      checkError(0, "\xDF\x00\x00\x00\x00", DeserializationError::Ok);
    }
    SECTION("{H:1}") {
      checkError(2, "\xDF\x00\x00\x00\x01\xA1H\x01",
                 DeserializationError::NoMemory);
      checkError(3, "\xDF\x00\x00\x00\x01\xA1H\x01", DeserializationError::Ok);
    }
    SECTION("{H:1,W:2}") {
      checkError(3, "\xDF\x00\x00\x00\x02\xA1H\x01\xA1W\x02",
                 DeserializationError::NoMemory);
      checkError(5, "\xDF\x00\x00\x00\x02\xA1H\x01\xA1W\x02",
                 DeserializationError::Ok);
    }
  }
}
