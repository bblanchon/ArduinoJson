// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#define SHOULD_WORK(expression) REQUIRE(true == expression.success());
#define SHOULD_FAIL(expression) REQUIRE(false == expression.success());

TEST_CASE("JsonParser nestingLimit") {
  DynamicJsonBuffer jb;

  SECTION("parseArray()") {
    SECTION("limit = 0") {
      SHOULD_FAIL(jb.parseArray("[]", 0));
    }

    SECTION("limit = 1") {
      SHOULD_WORK(jb.parseArray("[]", 1));
      SHOULD_FAIL(jb.parseArray("[[]]", 1));
    }

    SECTION("limit = 2") {
      SHOULD_WORK(jb.parseArray("[[]]", 2));
      SHOULD_FAIL(jb.parseArray("[[[]]]", 2));
    }
  }

  SECTION("parseObject()") {
    SECTION("limit = 0") {
      SHOULD_FAIL(jb.parseObject("{}", 0));
    }

    SECTION("limit = 1") {
      SHOULD_WORK(jb.parseObject("{\"key\":42}", 1));
      SHOULD_FAIL(jb.parseObject("{\"key\":{\"key\":42}}", 1));
    }

    SECTION("limit = 2") {
      SHOULD_WORK(jb.parseObject("{\"key\":{\"key\":42}}", 2));
      SHOULD_FAIL(jb.parseObject("{\"key\":{\"key\":{\"key\":42}}}", 2));
    }
  }

  SECTION("parse()") {
    SECTION("limit = 0") {
      SHOULD_WORK(jb.parse("\"toto\"", 0));
      SHOULD_WORK(jb.parse("123", 0));
      SHOULD_WORK(jb.parse("true", 0));
      SHOULD_FAIL(jb.parse("[]", 0));
      SHOULD_FAIL(jb.parse("{}", 0));
      SHOULD_FAIL(jb.parse("[\"toto\"]", 0));
      SHOULD_FAIL(jb.parse("{\"toto\":1}", 0));
    }

    SECTION("limit = 1") {
      SHOULD_WORK(jb.parse("[\"toto\"]", 1));
      SHOULD_WORK(jb.parse("{\"toto\":1}", 1));
      SHOULD_FAIL(jb.parse("{\"toto\":{}}", 1));
      SHOULD_FAIL(jb.parse("{\"toto\":[]}", 1));
      SHOULD_FAIL(jb.parse("[[\"toto\"]]", 1));
      SHOULD_FAIL(jb.parse("[{\"toto\":1}]", 1));
    }
  }
}
