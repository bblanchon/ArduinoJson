#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <doctest.h>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument::set()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SUBCASE("nullptr") {
    doc.set(nullptr);

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SUBCASE("integer&") {
    int toto = 42;
    doc.set(toto);

    REQUIRE(doc.as<std::string>() == "42");
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SUBCASE("integer") {
    doc.set(42);

    REQUIRE(doc.as<std::string>() == "42");
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SUBCASE("string literal") {
    doc.set("example");

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("const char*") {
    const char* value = "example";
    doc.set(value);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("std::string") {
    doc.set("example"_s);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("char*") {
    char value[] = "example";
    doc.set(value);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("Arduino String") {
    doc.set(String("example"));

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("Flash string") {
    doc.set(F("example"));

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("Flash tiny string") {  // issue #2170
    doc.set(F("abc"));

    REQUIRE(doc.as<const char*>() == "abc"_s);
    REQUIRE(spy.log() == AllocatorLog{});
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "example");

    doc.set(vla);

    REQUIRE(doc.as<const char*>() == "example"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("example")),
                         });
  }
#endif
}
