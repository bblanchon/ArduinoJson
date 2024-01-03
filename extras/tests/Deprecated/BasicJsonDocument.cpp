// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <string>

using ArduinoJson::detail::is_base_of;

static std::string allocatorLog;

struct CustomAllocator {
  CustomAllocator() {
    allocatorLog = "";
  }

  void* allocate(size_t n) {
    allocatorLog += "A";
    return malloc(n);
  }

  void deallocate(void* p) {
    free(p);
    allocatorLog += "D";
  }

  void* reallocate(void* p, size_t n) {
    allocatorLog += "R";
    return realloc(p, n);
  }
};

TEST_CASE("BasicJsonDocument") {
  allocatorLog.clear();

  SECTION("is a JsonDocument") {
    REQUIRE(
        is_base_of<JsonDocument, BasicJsonDocument<CustomAllocator>>::value ==
        true);
  }

  SECTION("deserialize / serialize") {
    BasicJsonDocument<CustomAllocator> doc(256);
    deserializeJson(doc, "{\"hello\":\"world\"}");
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
    doc.clear();
    REQUIRE(allocatorLog == "ARAARDDD");
  }

  SECTION("copy") {
    BasicJsonDocument<CustomAllocator> doc(256);
    doc["hello"] = "world";
    auto copy = doc;
    REQUIRE(copy.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(allocatorLog == "AA");
  }

  SECTION("capacity") {
    BasicJsonDocument<CustomAllocator> doc(256);
    REQUIRE(doc.capacity() == 256);
  }

  SECTION("garbageCollect()") {
    BasicJsonDocument<CustomAllocator> doc(256);
    doc.garbageCollect();
  }
}
