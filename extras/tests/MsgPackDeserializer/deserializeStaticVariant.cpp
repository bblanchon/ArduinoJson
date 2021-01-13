// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <size_t Capacity>
static void check(const char* input, DeserializationError expected) {
  StaticJsonDocument<Capacity> doc;

  DeserializationError error = deserializeMsgPack(doc, input);

  CAPTURE(input);
  REQUIRE(error == expected);
}

template <size_t Size>
static void checkString(const char* input, DeserializationError expected) {
  check<Size>(input, expected);
}

TEST_CASE("deserializeMsgPack(StaticJsonDocument&)") {
  SECTION("single values always fit") {
    check<0>("\xc0", DeserializationError::Ok);                  // nil
    check<0>("\xc2", DeserializationError::Ok);                  // false
    check<0>("\xc3", DeserializationError::Ok);                  // true
    check<0>("\xcc\x00", DeserializationError::Ok);              // uint 8
    check<0>("\xcd\x30\x39", DeserializationError::Ok);          // uint 16
    check<0>("\xCE\x12\x34\x56\x78", DeserializationError::Ok);  // uint 32
  }

  SECTION("fixstr") {
    checkString<8>("\xA0", DeserializationError::Ok);
    checkString<8>("\xA7ZZZZZZZ", DeserializationError::Ok);
    checkString<8>("\xA8ZZZZZZZZ", DeserializationError::NoMemory);
    checkString<16>("\xAFZZZZZZZZZZZZZZZ", DeserializationError::Ok);
    checkString<16>("\xB0ZZZZZZZZZZZZZZZZ", DeserializationError::NoMemory);
  }

  SECTION("str 8") {
    checkString<8>("\xD9\x00", DeserializationError::Ok);
    checkString<8>("\xD9\x07ZZZZZZZ", DeserializationError::Ok);
    checkString<8>("\xD9\x08ZZZZZZZZ", DeserializationError::NoMemory);
    checkString<16>("\xD9\x0FZZZZZZZZZZZZZZZ", DeserializationError::Ok);
    checkString<16>("\xD9\x10ZZZZZZZZZZZZZZZZ", DeserializationError::NoMemory);
  }

  SECTION("str 16") {
    checkString<8>("\xDA\x00\x00", DeserializationError::Ok);
    checkString<8>("\xDA\x00\x07ZZZZZZZ", DeserializationError::Ok);
    checkString<8>("\xDA\x00\x08ZZZZZZZZ", DeserializationError::NoMemory);
    checkString<16>("\xDA\x00\x0FZZZZZZZZZZZZZZZ", DeserializationError::Ok);
    checkString<16>("\xDA\x00\x10ZZZZZZZZZZZZZZZZ",
                    DeserializationError::NoMemory);
  }

  SECTION("str 32") {
    checkString<8>("\xDB\x00\x00\x00\x00", DeserializationError::Ok);
    checkString<8>("\xDB\x00\x00\x00\x07ZZZZZZZ", DeserializationError::Ok);
    checkString<8>("\xDB\x00\x00\x00\x08ZZZZZZZZ",
                   DeserializationError::NoMemory);
    checkString<16>("\xDB\x00\x00\x00\x0FZZZZZZZZZZZZZZZ",
                    DeserializationError::Ok);
    checkString<16>("\xDB\x00\x00\x00\x10ZZZZZZZZZZZZZZZZ",
                    DeserializationError::NoMemory);
  }

  SECTION("fixarray") {
    check<JSON_ARRAY_SIZE(0)>("\x90", DeserializationError::Ok);  // []
    check<JSON_ARRAY_SIZE(0)>("\x91\x01",
                              DeserializationError::NoMemory);        // [1]
    check<JSON_ARRAY_SIZE(1)>("\x91\x01", DeserializationError::Ok);  // [1]
    check<JSON_ARRAY_SIZE(1)>("\x92\x01\x02",
                              DeserializationError::NoMemory);  // [1,2]
  }

  SECTION("array 16") {
    check<JSON_ARRAY_SIZE(0)>("\xDC\x00\x00", DeserializationError::Ok);
    check<JSON_ARRAY_SIZE(0)>("\xDC\x00\x01\x01",
                              DeserializationError::NoMemory);
    check<JSON_ARRAY_SIZE(1)>("\xDC\x00\x01\x01", DeserializationError::Ok);
    check<JSON_ARRAY_SIZE(1)>("\xDC\x00\x02\x01\x02",
                              DeserializationError::NoMemory);
  }

  SECTION("array 32") {
    check<JSON_ARRAY_SIZE(0)>("\xDD\x00\x00\x00\x00", DeserializationError::Ok);
    check<JSON_ARRAY_SIZE(0)>("\xDD\x00\x00\x00\x01\x01",
                              DeserializationError::NoMemory);
    check<JSON_ARRAY_SIZE(1)>("\xDD\x00\x00\x00\x01\x01",
                              DeserializationError::Ok);
    check<JSON_ARRAY_SIZE(1)>("\xDD\x00\x00\x00\x02\x01\x02",
                              DeserializationError::NoMemory);
  }

  SECTION("fixmap") {
    SECTION("{}") {
      check<JSON_OBJECT_SIZE(0)>("\x80", DeserializationError::Ok);
    }
    SECTION("{H:1}") {
      check<JSON_OBJECT_SIZE(0)>("\x81\xA1H\x01",
                                 DeserializationError::NoMemory);
      check<JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2)>(
          "\x81\xA1H\x01", DeserializationError::Ok);
    }
    SECTION("{H:1,W:2}") {
      check<JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2)>(
          "\x82\xA1H\x01\xA1W\x02", DeserializationError::NoMemory);
      check<JSON_OBJECT_SIZE(2) + 2 * JSON_STRING_SIZE(2)>(
          "\x82\xA1H\x01\xA1W\x02", DeserializationError::Ok);
    }
  }

  SECTION("map 16") {
    SECTION("{}") {
      check<JSON_OBJECT_SIZE(0)>("\xDE\x00\x00", DeserializationError::Ok);
    }
    SECTION("{H:1}") {
      check<JSON_OBJECT_SIZE(0)>("\xDE\x00\x01\xA1H\x01",
                                 DeserializationError::NoMemory);
      check<JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2)>(
          "\xDE\x00\x01\xA1H\x01", DeserializationError::Ok);
    }
    SECTION("{H:1,W:2}") {
      check<JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2)>(
          "\xDE\x00\x02\xA1H\x01\xA1W\x02", DeserializationError::NoMemory);
      check<JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(1)>(
          "\xDE\x00\x02\xA1H\x01\xA1W\x02", DeserializationError::Ok);
    }
  }

  SECTION("map 32") {
    SECTION("{}") {
      check<JSON_OBJECT_SIZE(0)>("\xDF\x00\x00\x00\x00",
                                 DeserializationError::Ok);
    }
    SECTION("{H:1}") {
      check<JSON_OBJECT_SIZE(0)>("\xDF\x00\x00\x00\x01\xA1H\x01",
                                 DeserializationError::NoMemory);
      check<JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2)>(
          "\xDF\x00\x00\x00\x01\xA1H\x01", DeserializationError::Ok);
    }
    SECTION("{H:1,W:2}") {
      check<JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2)>(
          "\xDF\x00\x00\x00\x02\xA1H\x01\xA1W\x02",
          DeserializationError::NoMemory);
      check<JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(1)>(
          "\xDF\x00\x00\x00\x02\xA1H\x01\xA1W\x02", DeserializationError::Ok);
    }
  }
}
