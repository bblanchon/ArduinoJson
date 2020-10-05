// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>
#include <catch.hpp>

#include <sstream>
#include <string>

TEST_CASE("Filtering") {
  struct TestCase {
    const char* input;
    const char* filter;
    uint8_t nestingLimit;
    DeserializationError error;
    const char* output;
    size_t memoryUsage;
  };

  // clang-format off
  TestCase testCases[] = {
    {
      "\x81\xA5\x68\x65\x6C\x6C\x6F\xA5\x77\x6F\x72\x6C\x64",	//{"hello": "world"}	:	1. input
      "null",                    // 2. filter
      10,                        // 3. nestingLimit
      DeserializationError::Ok,  // 4. error
      "null",                    // 5. output
      0                          // 6. memoryUsage
    },
    {
      "\x81\xA5\x68\x65\x6C\x6C\x6F\xA5\x77\x6F\x72\x6C\x64",	//{"hello": "world"}
      "false",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      "\x81\xA7\x61\x62\x63\x64\x65\x66\x67\xA7\x68\x69\x6A\x6B\x6C\x6D\x6E",	//{"abcdefg": "hijklmn"}
      "true",
      10,
      DeserializationError::Ok,
      "{\"abcdefg\":\"hijklmn\"}",
      JSON_OBJECT_SIZE(1) + 16
    },
    {
      "\x81\xA5\x68\x65\x6C\x6C\x6F\xA5\x77\x6F\x72\x6C\x64",	//{"hello": "world"}
      "{}",
      10,
      DeserializationError::Ok,
      "{}",
      JSON_OBJECT_SIZE(0)
    },
    {
      // Input in an object, but filter wants an array
      "\x81\xA5\x68\x65\x6C\x6C\x6F\xA5\x77\x6F\x72\x6C\x64",	//{"hello": "world"}
      "[]",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      // Input is an array, but filter wants an object
      "\x92\xA5\x68\x65\x6C\x6C\x6F\xA5\x77\x6F\x72\x6C\x64",	//["hello", "world"]
      "{}",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      // Input is a bool, but filter wants an object
      "\xC3",	//true
      "{}",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      // Input is a string, but filter wants an object
      "\xA5\x68\x65\x6C\x6C\x6F",	//"hello"
      "{}",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      // skip an integer
      "\x82\xAA\x61\x6E\x5F\x69\x6E\x74\x65\x67\x65\x72\xCD\x02\x9A\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_integer": 666, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // skip a float
      "\x82\xA7\x61\x5F\x66\x6C\x6F\x61\x74\xCB\x3E\xE9\xE0\xFC\xAF\x93\x80\xFC\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"a_float": 1.234e-05, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip a boolean
      "\x82\xA6\x61\x5F\x62\x6F\x6F\x6C\xC2\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"a_bool": false, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip a double-quoted string
      "\x82\xB6\x61\x5F\x64\x6F\x75\x62\x6C\x65\x5F\x71\x75\x6F\x74\x65\x64\x5F\x73\x74\x72\x69\x6E\x67\xA5\x68\x65\x6C\x6C\x6F\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"a_double_quoted_string": "hello", "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip an empty array
      "\x82\xAE\x61\x6E\x5F\x65\x6D\x70\x74\x79\x5F\x61\x72\x72\x61\x79\x90\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_empty_array": [], "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip an array
      "\x82\xA8\x61\x6E\x5F\x61\x72\x72\x61\x79\x93\x01\x02\x03\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_array": [1, 2, 3], "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip an array with spaces in it
      "\x82\xA8\x61\x6E\x5F\x61\x72\x72\x61\x79\x93\x01\x02\x03\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_array": [1, 2, 3], "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip an empty object
      "\x82\xAF\x61\x6E\x5F\x65\x6D\x70\x74\x79\x5F\x6F\x62\x6A\x65\x63\x74\x80\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_empty_object": {}, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip an empty object with spaces in it
      "\x82\xAF\x61\x6E\x5F\x65\x6D\x70\x74\x79\x5F\x6F\x62\x6A\x65\x63\x74\x80\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_empty_object": {}, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // can skip an object
      "\x82\xA9\x61\x6E\x5F\x6F\x62\x6A\x65\x63\x74\x83\xA1\x61\x01\xA1\x62\x02\xA1\x63\x03\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_object": {"a": 1, "b": 2, "c": 3}, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      // skip an object with spaces in it
      "\x82\xA9\x61\x6E\x5F\x6F\x62\x6A\x65\x63\x74\x83\xA1\x61\x01\xA1\x62\x02\xA1\x63\x03\xA7\x65\x78\x61\x6D\x70\x6C\x65\x2A",	//{"an_object": {"a": 1, "b": 2, "c": 3}, "example": 42}
      "{\"example\":true}",
      10,
      DeserializationError::Ok,
      "{\"example\":42}",
      JSON_OBJECT_SIZE(1) + 8
    },
    {
      "\x82\xAA\x61\x6E\x5F\x69\x6E\x74\x65\x67\x65\x72\x00\xA7\x65\x78\x61\x6D\x70\x6C\x65\x82\xA4\x74\x79\x70\x65\xA3\x69\x6E\x74\xA7\x6F\x75\x74\x63\x6F\x6D\x65\x2A",	//{"an_integer": 0, "example": {"type": "int", "outcome": 42}}
      "{\"example\":{\"outcome\":true}}",
      10,
      DeserializationError::Ok,
      "{\"example\":{\"outcome\":42}}",
      2 * JSON_OBJECT_SIZE(1) + 16
    },
    {
      // wildcard
      "\x81\xA7\x65\x78\x61\x6D\x70\x6C\x65\x82\xA4\x74\x79\x70\x65\xA3\x69\x6E\x74\xA7\x6F\x75\x74\x63\x6F\x6D\x65\x2A",	//{"example": {"type": "int", "outcome": 42}}
      "{\"*\":{\"outcome\":true}}",
      10,
      DeserializationError::Ok,
      "{\"example\":{\"outcome\":42}}",
      2 * JSON_OBJECT_SIZE(1) + 16
    },
    {
      // only the first element of array counts
      "\x93\x01\x02\x03",	//[1, 2, 3]
      "[true, false]",
      10,
      DeserializationError::Ok,
      "[1,2,3]",
      JSON_ARRAY_SIZE(3)
    },
    {
      // only the first element of array counts
      "\x93\x01\x02\x03",	//[1, 2, 3]
      "[false, true]",
      10,
      DeserializationError::Ok,
      "[]",
      JSON_ARRAY_SIZE(0)
    },
    {
      // filter members of object in array
      "\x92\x82\xA7\x65\x78\x61\x6D\x70\x6C\x65\x01\xA6\x69\x67\x6E\x6F\x72\x65\x02\x82\xA7\x65\x78\x61\x6D\x70\x6C\x65\x03\xA6\x69\x67\x6E\x6F\x72\x65\x04",	//[{"example": 1, "ignore": 2}, {"example": 3, "ignore": 4}]
      "[{\"example\":true}]",
      10,
      DeserializationError::Ok,
      "[{\"example\":1},{\"example\":3}]",
      JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 8
    },
    // {
    //   // handle escaped quotes
    //   .input= "\"A\\\"BC\"",
    //   "false",
    //   10,
    //   DeserializationError::Ok,
    //   "null",
    //   0
    // },
    {
      // skip empty array
      "\x90",	//[]
      "false",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      // skip empty array with spaces
      "\x90",	//[]
      "false",
      10,
      DeserializationError::Ok,
      "null",
      0
    },
    {
      // check nesting limit even for ignored objects
      "\x80",	//{}
      "false", 
      0,
      DeserializationError::TooDeep,
      "null", 
      0
    },
    {
      // check nesting limit even for ignored objects
      "\x81\xA5\x68\x65\x6C\x6C\x6F\x80",	//{"hello": {}}
      "false", 
      1,
      DeserializationError::TooDeep,
      "null", 
      0
    },
    {
      // check nesting limit even for ignored values in objects
      "\x81\xA5\x68\x65\x6C\x6C\x6F\x80",	//{"hello": {}}
      "{}", 
      1,
      DeserializationError::TooDeep,
      "{}", 
      JSON_OBJECT_SIZE(0)
    },
    {
      // check nesting limit even for ignored arrays
      "\x90",	//[]
      "false", 
      0,
      DeserializationError::TooDeep,
      "null", 
      0
    },
    {
      // check nesting limit even for ignored arrays
      "\x91\x90",	//[[]]
      "false", 
      1,
      DeserializationError::TooDeep,
      "null", 
      0
    },
    {
      // check nesting limit even for ignored values in arrays
      "\x91\x90",	//[[]]
      "[]", 
      1,
      DeserializationError::TooDeep,
      "[]", 
      JSON_ARRAY_SIZE(0)
    },
  };  // clang-format on

  for (size_t i = 0; i < sizeof(testCases) / sizeof(testCases[0]); i++) {
    CAPTURE(i);

    DynamicJsonDocument filter(256);
    DynamicJsonDocument doc(256);
    TestCase& tc = testCases[i];

    CAPTURE(tc.filter);
    REQUIRE(deserializeJson(filter, tc.filter) == DeserializationError::Ok);

    CAPTURE(tc.input);
    CAPTURE(tc.nestingLimit);
    CHECK(deserializeMsgPack(doc, tc.input, DeserializationOption::Filter(filter),
                          DeserializationOption::NestingLimit(
                              tc.nestingLimit)) == tc.error);

    CAPTURE(tc.output);
    CHECK(doc.as<std::string>() == tc.output);
    CAPTURE(tc.nestingLimit);
    CHECK(doc.memoryUsage() == tc.memoryUsage);
  }
}

TEST_CASE("Overloads") {
  StaticJsonDocument<256> doc;
  StaticJsonDocument<256> filter;

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
    int i = 4;
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
    int i = 4;
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
    int i = 4;
    char vla[i];
    strcpy(vla, "{}");
    deserializeJson(doc, vla, NestingLimit(5), Filter(filter));
  }
#endif
}
