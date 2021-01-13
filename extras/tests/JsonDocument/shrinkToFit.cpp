// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <stdlib.h>  // malloc, free
#include <string>

using ARDUINOJSON_NAMESPACE::addPadding;

class ArmoredAllocator {
 public:
  ArmoredAllocator() : _ptr(0), _size(0) {}

  void* allocate(size_t size) {
    _ptr = malloc(size);
    _size = size;
    return _ptr;
  }

  void deallocate(void* ptr) {
    REQUIRE(ptr == _ptr);
    free(ptr);
    _ptr = 0;
    _size = 0;
  }

  void* reallocate(void* ptr, size_t new_size) {
    REQUIRE(ptr == _ptr);
    // don't call realloc, instead alloc a new buffer and erase the old one
    // this way we make sure we support relocation
    void* new_ptr = malloc(new_size);
    memcpy(new_ptr, _ptr, std::min(new_size, _size));
    memset(_ptr, '#', _size);  // erase
    free(_ptr);
    _ptr = new_ptr;
    return new_ptr;
  }

 private:
  void* _ptr;
  size_t _size;
};

typedef BasicJsonDocument<ArmoredAllocator> ShrinkToFitTestDocument;

void testShrinkToFit(ShrinkToFitTestDocument& doc, std::string expected_json,
                     size_t expected_size) {
  // test twice: shrinkToFit() should be idempotent
  for (int i = 0; i < 2; i++) {
    doc.shrinkToFit();

    REQUIRE(doc.capacity() == expected_size);
    REQUIRE(doc.memoryUsage() == expected_size);

    std::string json;
    serializeJson(doc, json);
    REQUIRE(json == expected_json);
  }
}

TEST_CASE("BasicJsonDocument::shrinkToFit()") {
  ShrinkToFitTestDocument doc(4096);

  SECTION("null") {
    testShrinkToFit(doc, "null", 0);
  }

  SECTION("empty object") {
    deserializeJson(doc, "{}");
    testShrinkToFit(doc, "{}", JSON_OBJECT_SIZE(0));
  }

  SECTION("empty array") {
    deserializeJson(doc, "[]");
    testShrinkToFit(doc, "[]", JSON_ARRAY_SIZE(0));
  }

  SECTION("linked string") {
    doc.set("hello");
    testShrinkToFit(doc, "\"hello\"", 0);
  }

  SECTION("owned string") {
    doc.set(std::string("abcdefg"));
    testShrinkToFit(doc, "\"abcdefg\"", 8);
  }

  SECTION("linked raw") {
    doc.set(serialized("[{},123]"));
    testShrinkToFit(doc, "[{},123]", 0);
  }

  SECTION("owned raw") {
    doc.set(serialized(std::string("[{},12]")));
    testShrinkToFit(doc, "[{},12]", 8);
  }

  SECTION("linked key") {
    doc["key"] = 42;
    testShrinkToFit(doc, "{\"key\":42}", JSON_OBJECT_SIZE(1));
  }

  SECTION("owned key") {
    doc[std::string("abcdefg")] = 42;
    testShrinkToFit(doc, "{\"abcdefg\":42}", JSON_OBJECT_SIZE(1) + 8);
  }

  SECTION("linked string in array") {
    doc.add("hello");
    testShrinkToFit(doc, "[\"hello\"]", JSON_ARRAY_SIZE(1));
  }

  SECTION("owned string in array") {
    doc.add(std::string("abcdefg"));
    testShrinkToFit(doc, "[\"abcdefg\"]", JSON_ARRAY_SIZE(1) + 8);
  }

  SECTION("linked string in object") {
    doc["key"] = "hello";
    testShrinkToFit(doc, "{\"key\":\"hello\"}", JSON_OBJECT_SIZE(1));
  }

  SECTION("owned string in object") {
    doc["key"] = std::string("abcdefg");
    testShrinkToFit(doc, "{\"key\":\"abcdefg\"}", JSON_ARRAY_SIZE(1) + 8);
  }

  SECTION("unaligned") {
    doc.add(std::string("?"));  // two bytes in the string pool
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 2);

    doc.shrinkToFit();

    // the new capacity should be padded to align the pointers
    REQUIRE(doc.capacity() == JSON_OBJECT_SIZE(1) + sizeof(void*));
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 2);
    REQUIRE(doc[0] == "?");
  }
}

TEST_CASE("DynamicJsonDocument::shrinkToFit()") {
  DynamicJsonDocument doc(4096);

  deserializeJson(doc, "{\"hello\":[\"world\"]");

  doc.shrinkToFit();

  std::string json;
  serializeJson(doc, json);
  REQUIRE(json == "{\"hello\":[\"world\"]}");
}
