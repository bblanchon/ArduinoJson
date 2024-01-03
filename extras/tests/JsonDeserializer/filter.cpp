// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>
#include <catch.hpp>

#include <sstream>
#include <string>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

TEST_CASE("Filtering") {
  struct TestCase {
    const char* description;
    const char* input;
    const char* filter;
    uint8_t nestingLimit;
    DeserializationError error;
    const char* output;
    size_t memoryUsage;
  };

  TestCase testCases[] = {
      {
          "Input is object, filter is null",  // description
          "{\"hello\":\"world\"}",            // input
          "null",                             // filter
          10,                                 // nestingLimit
          DeserializationError::Ok,           // error
          "null",                             // output
          0,                                  // memoryUsage
      },
      {
          "Input is object, filter is false",
          "{\"hello\":\"world\"}",
          "false",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Input is object, filter is true",
          "{\"abcdefg\":\"hijklmn\"}",
          "true",
          10,
          DeserializationError::Ok,
          "{\"abcdefg\":\"hijklmn\"}",
          sizeofObject(1) + sizeofString("abcdefg") + sizeofString("hijklmn"),
      },
      {
          "Input is object, filter is empty object",
          "{\"hello\":\"world\"}",
          "{}",
          10,
          DeserializationError::Ok,
          "{}",
          sizeofObject(0),
      },
      {
          "Input in an object, but filter wants an array",
          "{\"hello\":\"world\"}",
          "[]",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Member is a string, but filter wants an array",
          "{\"example\":\"example\"}",
          "{\"example\":[true]}",
          10,
          DeserializationError::Ok,
          "{\"example\":null}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Member is a number, but filter wants an array",
          "{\"example\":42}",
          "{\"example\":[true]}",
          10,
          DeserializationError::Ok,
          "{\"example\":null}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Input is an array, but filter wants an object",
          "[\"hello\",\"world\"]",
          "{}",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Input is a bool, but filter wants an object",
          "true",
          "{}",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Input is a string, but filter wants an object",
          "\"hello\"",
          "{}",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Skip an integer",
          "{\"an_integer\":666,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip a float",
          "{\"a_float\":12.34e-6,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip false",
          "{\"a_bool\":false,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip true",
          "{\"a_bool\":true,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip null",
          "{\"a_bool\":null,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip a double-quoted string",
          "{\"a_double_quoted_string\":\"hello\",example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip a single-quoted string",
          "{\"a_single_quoted_string\":'hello',example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an empty array",
          "{\"an_empty_array\":[],example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an empty array with spaces in it",
          "{\"an_empty_array\":[\t],example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an array",
          "{\"an_array\":[1,2,3],example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an array with spaces in it",
          "{\"an_array\": [ 1 , 2 , 3 ] ,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an empty nested object",
          "{\"an_empty_object\":{},example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an empty nested object with spaces in it",
          "{\"an_empty_object\":{    },example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip a nested object",
          "{\"an_object\":{a:1,'b':2,\"c\":3},example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip an object with spaces in it",
          "{\"an_object\" : { a : 1 , 'b' : 2 , \"c\" : 3 } ,example:42}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{\"example\":42}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "Skip a string in a nested object",
          "{\"an_integer\": 0,\"example\":{\"type\":\"int\",\"outcome\":42}}",
          "{\"example\":{\"outcome\":true}}",
          10,
          DeserializationError::Ok,
          "{\"example\":{\"outcome\":42}}",
          2 * sizeofObject(1) + 2 * sizeofString("example"),
      },
      {
          "wildcard",
          "{\"example\":{\"type\":\"int\",\"outcome\":42}}",
          "{\"*\":{\"outcome\":true}}",
          10,
          DeserializationError::Ok,
          "{\"example\":{\"outcome\":42}}",
          2 * sizeofObject(1) + 2 * sizeofString("example"),
      },
      {
          "exclusion filter (issue #1628)",
          "{\"example\":1,\"ignored\":2}",
          "{\"*\":true,\"ignored\":false}",
          10,
          DeserializationError::Ok,
          "{\"example\":1}",
          sizeofObject(1) + sizeofString("example"),
      },
      {
          "only the first element of array counts",
          "[1,2,3]",
          "[true, false]",
          10,
          DeserializationError::Ok,
          "[1,2,3]",
          sizeofArray(3),
      },
      {
          "only the first element of array counts",
          "[1,2,3]",
          "[false, true]",
          10,
          DeserializationError::Ok,
          "[]",
          sizeofArray(0),
      },
      {
          "filter members of object in array",
          "[{\"example\":1,\"ignore\":2},{\"example\":3,\"ignore\":4}]",
          "[{\"example\":true}]",
          10,
          DeserializationError::Ok,
          "[{\"example\":1},{\"example\":3}]",
          sizeofArray(2) + 2 * sizeofObject(1) + sizeofString("example"),
      },
      {
          "Unclosed single quote in skipped element",
          "[',2,3]",
          "[false,true]",
          10,
          DeserializationError::IncompleteInput,
          "[]",
          sizeofArray(0),
      },
      {
          "Unclosed double quote in skipped element",
          "[\",2,3]",
          "[false,true]",
          10,
          DeserializationError::IncompleteInput,
          "[]",
          sizeofArray(0),
      },
      {
          "Detect errors in skipped value",
          "[!,2,\\]",
          "[false]",
          10,
          DeserializationError::InvalidInput,
          "[]",
          sizeofArray(0),
      },
      {
          "Detect incomplete string event if it's skipped",
          "\"ABC",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Detect incomplete string event if it's skipped",
          "'ABC",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Handle escaped quotes",
          "'A\\'BC'",
          "false",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Handle escaped quotes",
          "\"A\\\"BC\"",
          "false",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Detect incomplete string in presence of escaped quotes",
          "'A\\'BC",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Detect incomplete string in presence of escaped quotes",
          "\"A\\\"BC",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "skip empty array",
          "[]",
          "false",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Skip empty array with spaces",
          " [ ] ",
          "false",
          10,
          DeserializationError::Ok,
          "null",
          0,
      },
      {
          "Bubble up element error even if array is skipped",
          "[1,'2,3]",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Bubble up member error even if object is skipped",
          "{'hello':'worl}",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Bubble up colon error even if object is skipped",
          "{'hello','world'}",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Bubble up key error even if object is skipped",
          "{'hello:1}",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Detect invalid value in skipped object",
          "{'hello':!}",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Ignore invalid value in skipped object",
          "{'hello':\\}",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Check nesting limit even for ignored objects",
          "{}",
          "false",
          0,
          DeserializationError::TooDeep,
          "null",
          0,
      },
      {
          "Check nesting limit even for ignored objects",
          "{'hello':{}}",
          "false",
          1,
          DeserializationError::TooDeep,
          "null",
          0,
      },
      {
          "Check nesting limit even for ignored values in objects",
          "{'hello':{}}",
          "{}",
          1,
          DeserializationError::TooDeep,
          "{}",
          sizeofObject(0),
      },
      {
          "Check nesting limit even for ignored arrays",
          "[]",
          "false",
          0,
          DeserializationError::TooDeep,
          "null",
          0,
      },
      {
          "Check nesting limit even for ignored arrays",
          "[[]]",
          "false",
          1,
          DeserializationError::TooDeep,
          "null",
          0,
      },
      {
          "Check nesting limit even for ignored values in arrays",
          "[[]]",
          "[]",
          1,
          DeserializationError::TooDeep,
          "[]",
          sizeofArray(0),
      },
      {
          "Supports back-slash at the end of skipped string",
          "\"hell\\",
          "false",
          1,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Invalid comment at after an element in a skipped array",
          "[1/]",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Incomplete comment at after an element in a skipped array",
          "[1/*]",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Missing comma in a skipped array",
          "[1 2]",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Invalid comment at the beginning of array",
          "[/1]",
          "[false]",
          10,
          DeserializationError::InvalidInput,
          "[]",
          sizeofArray(0),
      },
      {
          "Incomplete comment at the begining of an array",
          "[/*]",
          "[false]",
          10,
          DeserializationError::IncompleteInput,
          "[]",
          sizeofArray(0),
      },
      {
          "Invalid comment before key",
          "{/1:2}",
          "{}",
          10,
          DeserializationError::InvalidInput,
          "{}",
          sizeofObject(0),
      },
      {
          "Incomplete comment before key",
          "{/*:2}",
          "{}",
          10,
          DeserializationError::IncompleteInput,
          "{}",
          sizeofObject(0),
      },
      {
          "Invalid comment after key",
          "{\"example\"/1:2}",
          "{}",
          10,
          DeserializationError::InvalidInput,
          "{}",
          sizeofObject(0),
      },
      {
          "Incomplete comment after key",
          "{\"example\"/*:2}",
          "{}",
          10,
          DeserializationError::IncompleteInput,
          "{}",
          sizeofObject(0),
      },
      {
          "Invalid comment after colon",
          "{\"example\":/12}",
          "{}",
          10,
          DeserializationError::InvalidInput,
          "{}",
          sizeofObject(0),
      },
      {
          "Incomplete comment after colon",
          "{\"example\":/*2}",
          "{}",
          10,
          DeserializationError::IncompleteInput,
          "{}",
          sizeofObject(0),
      },
      {
          "Comment next to an integer",
          "{\"ignore\":1//,\"example\":2\n}",
          "{\"example\":true}",
          10,
          DeserializationError::Ok,
          "{}",
          sizeofObject(0),
      },
      {
          "Invalid comment after opening brace of a skipped object",
          "{/1:2}",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Incomplete after opening brace of a skipped object",
          "{/*:2}",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Invalid comment after key of a skipped object",
          "{\"example\"/:2}",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Incomplete comment after key of a skipped object",
          "{\"example\"/*:2}",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Invalid comment after value in a skipped object",
          "{\"example\":2/}",
          "false",
          10,
          DeserializationError::InvalidInput,
          "null",
          0,
      },
      {
          "Incomplete comment after value of a skipped object",
          "{\"example\":2/*}",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
      {
          "Incomplete comment after comma in skipped object",
          "{\"example\":2,/*}",
          "false",
          10,
          DeserializationError::IncompleteInput,
          "null",
          0,
      },
  };

  for (auto& tc : testCases) {
    SECTION(tc.description) {
      SpyingAllocator spy;
      JsonDocument filter;
      JsonDocument doc(&spy);

      REQUIRE(deserializeJson(filter, tc.filter) == DeserializationError::Ok);

      CHECK(deserializeJson(
                doc, tc.input, DeserializationOption::Filter(filter),
                DeserializationOption::NestingLimit(tc.nestingLimit)) ==
            tc.error);

      CHECK(doc.as<std::string>() == tc.output);

      doc.shrinkToFit();
      CHECK(spy.allocatedBytes() == tc.memoryUsage);
    }
  }
}

TEST_CASE("Overloads") {
  JsonDocument doc;
  JsonDocument filter;

  using namespace DeserializationOption;

  // deserializeJson(..., Filter)

  SECTION("const char*, Filter") {
    deserializeJson(doc, "{}", Filter(filter));
  }

  SECTION("const char*, size_t, Filter") {
    deserializeJson(doc, "{}", 2, Filter(filter));
  }

  SECTION("const std::string&, Filter") {
    deserializeJson(doc, std::string("{}"), Filter(filter));
  }

  SECTION("std::istream&, Filter") {
    std::stringstream s("{}");
    deserializeJson(doc, s, Filter(filter));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("char[n], Filter") {
    size_t i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeJson(doc, vla, Filter(filter));
  }
#endif

  // deserializeJson(..., Filter, NestingLimit)

  SECTION("const char*, Filter, NestingLimit") {
    deserializeJson(doc, "{}", Filter(filter), NestingLimit(5));
  }

  SECTION("const char*, size_t, Filter, NestingLimit") {
    deserializeJson(doc, "{}", 2, Filter(filter), NestingLimit(5));
  }

  SECTION("const std::string&, Filter, NestingLimit") {
    deserializeJson(doc, std::string("{}"), Filter(filter), NestingLimit(5));
  }

  SECTION("std::istream&, Filter, NestingLimit") {
    std::stringstream s("{}");
    deserializeJson(doc, s, Filter(filter), NestingLimit(5));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("char[n], Filter, NestingLimit") {
    size_t i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeJson(doc, vla, Filter(filter), NestingLimit(5));
  }
#endif

  // deserializeJson(..., NestingLimit, Filter)

  SECTION("const char*, NestingLimit, Filter") {
    deserializeJson(doc, "{}", NestingLimit(5), Filter(filter));
  }

  SECTION("const char*, size_t, NestingLimit, Filter") {
    deserializeJson(doc, "{}", 2, NestingLimit(5), Filter(filter));
  }

  SECTION("const std::string&, NestingLimit, Filter") {
    deserializeJson(doc, std::string("{}"), NestingLimit(5), Filter(filter));
  }

  SECTION("std::istream&, NestingLimit, Filter") {
    std::stringstream s("{}");
    deserializeJson(doc, s, NestingLimit(5), Filter(filter));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("char[n], NestingLimit, Filter") {
    size_t i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeJson(doc, vla, NestingLimit(5), Filter(filter));
  }
#endif
}

TEST_CASE("shrink filter") {
  JsonDocument doc;
  SpyingAllocator spy;
  JsonDocument filter(&spy);
  filter["a"] = true;
  spy.clearLog();

  deserializeJson(doc, "{}", DeserializationOption::Filter(filter));

  REQUIRE(spy.log() == AllocatorLog{
                           Reallocate(sizeofPool(), sizeofObject(1)),
                       });
}
