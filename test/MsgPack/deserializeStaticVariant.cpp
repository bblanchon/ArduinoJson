// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static const size_t epsilon = sizeof(void*);

template <size_t Capacity>
static void check(const char* input, MsgPackError expected) {
  StaticJsonDocument<Capacity> variant;

  MsgPackError error = deserializeMsgPack(variant, input);

  REQUIRE(error == expected);
}

TEST_CASE("deserializeMsgPack(StaticJsonDocument&)") {
  SECTION("single values always fit") {
    check<0>("\xc0", MsgPackError::Ok);                  // nil
    check<0>("\xc2", MsgPackError::Ok);                  // false
    check<0>("\xc3", MsgPackError::Ok);                  // true
    check<0>("\xcc\x00", MsgPackError::Ok);              // uint 8
    check<0>("\xcd\x30\x39", MsgPackError::Ok);          // uint 16
    check<0>("\xCE\x12\x34\x56\x78", MsgPackError::Ok);  // uint 32
  }

  SECTION("fixstr") {
    check<0>("\xA0", MsgPackError::Ok);
    check<0>("\xA1H", MsgPackError::NoMemory);
    check<4>("\xA1H", MsgPackError::Ok);
    check<4>("\xA5Hello", MsgPackError::NoMemory);
  }

  SECTION("str 8") {
    check<0>("\xD9\x00", MsgPackError::Ok);
    check<0>("\xD9\x01H", MsgPackError::NoMemory);
    check<4>("\xD9\x01H", MsgPackError::Ok);
    check<4>("\xD9\x05Hello", MsgPackError::NoMemory);
  }

  SECTION("str 16") {
    check<0>("\xDA\x00\x00", MsgPackError::Ok);
    check<0>("\xDA\x00\x01H", MsgPackError::NoMemory);
    check<4>("\xDA\x00\x01H", MsgPackError::Ok);
    check<4>("\xDA\x00\x05Hello", MsgPackError::NoMemory);
  }

  SECTION("str 32") {
    check<0>("\xDB\x00\x00\x00\x00", MsgPackError::Ok);
    check<0>("\xDB\x00\x00\x00\x01H", MsgPackError::NoMemory);
    check<4>("\xDB\x00\x00\x00\x01H", MsgPackError::Ok);
    check<4>("\xDB\x00\x00\x00\x05Hello", MsgPackError::NoMemory);
  }

  SECTION("fixarray") {
    check<JSON_ARRAY_SIZE(0)>("\x90", MsgPackError::Ok);                // []
    check<JSON_ARRAY_SIZE(0)>("\x91\x01", MsgPackError::NoMemory);      // [1]
    check<JSON_ARRAY_SIZE(1)>("\x91\x01", MsgPackError::Ok);            // [1]
    check<JSON_ARRAY_SIZE(1)>("\x92\x01\x02", MsgPackError::NoMemory);  // [1,2]
  }

  SECTION("array 16") {
    check<JSON_ARRAY_SIZE(0)>("\xDC\x00\x00", MsgPackError::Ok);
    check<JSON_ARRAY_SIZE(0)>("\xDC\x00\x01\x01", MsgPackError::NoMemory);
    check<JSON_ARRAY_SIZE(1)>("\xDC\x00\x01\x01", MsgPackError::Ok);
    check<JSON_ARRAY_SIZE(1)>("\xDC\x00\x02\x01\x02", MsgPackError::NoMemory);
  }

  SECTION("array 32") {
    check<JSON_ARRAY_SIZE(0)>("\xDD\x00\x00\x00\x00", MsgPackError::Ok);
    check<JSON_ARRAY_SIZE(0)>("\xDD\x00\x00\x00\x01\x01",
                              MsgPackError::NoMemory);
    check<JSON_ARRAY_SIZE(1)>("\xDD\x00\x00\x00\x01\x01", MsgPackError::Ok);
    check<JSON_ARRAY_SIZE(1)>("\xDD\x00\x00\x00\x02\x01\x02",
                              MsgPackError::NoMemory);
  }

  SECTION("fixmap") {
    SECTION("{}") {
      check<JSON_OBJECT_SIZE(0)>("\x80", MsgPackError::Ok);
    }
    SECTION("{H:1}") {
      check<JSON_OBJECT_SIZE(0)>("\x81\xA1H\x01", MsgPackError::NoMemory);
      check<JSON_OBJECT_SIZE(1) + epsilon>("\x81\xA1H\x01", MsgPackError::Ok);
    }
    SECTION("{H:1,W:2}") {
      check<JSON_OBJECT_SIZE(1) + epsilon>("\x82\xA1H\x01\xA1W\x02",
                                           MsgPackError::NoMemory);
      check<JSON_OBJECT_SIZE(2) + 2 * epsilon>("\x82\xA1H\x01\xA1W\x02",
                                               MsgPackError::Ok);
    }
  }

  SECTION("map 16") {
    SECTION("{}") {
      check<JSON_OBJECT_SIZE(0)>("\xDE\x00\x00", MsgPackError::Ok);
    }
    SECTION("{H:1}") {
      check<JSON_OBJECT_SIZE(0)>("\xDE\x00\x01\xA1H\x01",
                                 MsgPackError::NoMemory);
      check<JSON_OBJECT_SIZE(1) + epsilon>("\xDE\x00\x01\xA1H\x01",
                                           MsgPackError::Ok);
    }
    SECTION("{H:1,W:2}") {
      check<JSON_OBJECT_SIZE(1) + epsilon>("\xDE\x00\x02\xA1H\x01\xA1W\x02",
                                           MsgPackError::NoMemory);
      check<JSON_OBJECT_SIZE(2) + 2 * epsilon>("\xDE\x00\x02\xA1H\x01\xA1W\x02",
                                               MsgPackError::Ok);
    }
  }

  SECTION("map 32") {
    SECTION("{}") {
      check<JSON_OBJECT_SIZE(0)>("\xDF\x00\x00\x00\x00", MsgPackError::Ok);
    }
    SECTION("{H:1}") {
      check<JSON_OBJECT_SIZE(0)>("\xDF\x00\x00\x00\x01\xA1H\x01",
                                 MsgPackError::NoMemory);
      check<JSON_OBJECT_SIZE(1) + epsilon>("\xDF\x00\x00\x00\x01\xA1H\x01",
                                           MsgPackError::Ok);
    }
    SECTION("{H:1,W:2}") {
      check<JSON_OBJECT_SIZE(1) + epsilon>(
          "\xDF\x00\x00\x00\x02\xA1H\x01\xA1W\x02", MsgPackError::NoMemory);
      check<JSON_OBJECT_SIZE(2) + 2 * epsilon>(
          "\xDF\x00\x00\x00\x02\xA1H\x01\xA1W\x02", MsgPackError::Ok);
    }
  }
}
