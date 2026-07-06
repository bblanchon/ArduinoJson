// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <ArduinoJson/Variant/VariantImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::detail;

TEST_CASE("StringBuilder") {
  KillswitchAllocator killswitch;
  SpyingAllocator spyingAllocator(&killswitch);
  ResourceManager resources(&spyingAllocator);

  SECTION("Empty string") {
    StringBuilder str(&resources);
    VariantData data;

    str.startString();
    str.save(&data);

    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                     });
    REQUIRE(data.type == VariantType::TinyString);
  }

  SECTION("Tiny string") {
    StringBuilder str(&resources);

    str.startString();
    str.append("url");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "url");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                     });

    VariantData data;
    str.save(&data);

    REQUIRE(resources.overflowed() == false);
    REQUIRE(data.type == VariantType::TinyString);
    REQUIRE(data.asString() == "url");
  }

  SECTION("Short string fits in first allocation") {
    StringBuilder str(&resources);

    str.startString();
    str.append("hello");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "hello");
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                     });
  }

  SECTION("Long string needs reallocation") {
    StringBuilder str(&resources);
    const char* lorem =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.";

    str.startString();
    str.append(lorem);

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == lorem);
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer(1)),
                Reallocate(sizeofStringBuffer(1), sizeofStringBuffer(2)),
                Reallocate(sizeofStringBuffer(2), sizeofStringBuffer(3)),
            });
  }

  SECTION("Realloc fails") {
    StringBuilder str(&resources);

    str.startString();
    killswitch.on();
    str.append(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                ReallocateFail(sizeofStringBuffer(), sizeofStringBuffer(2)),
                Deallocate(sizeofStringBuffer()),
            });
    REQUIRE(str.isValid() == false);
    REQUIRE(resources.overflowed() == true);
  }

  SECTION("Initial allocation fails") {
    StringBuilder str(&resources);

    killswitch.on();
    str.startString();

    REQUIRE(str.isValid() == false);
    REQUIRE(resources.overflowed() == true);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         AllocateFail(sizeofStringBuffer()),
                                     });
  }
}

static VariantData saveString(StringBuilder& builder, const char* s) {
  VariantData data;
  builder.startString();
  builder.append(s);
  builder.save(&data);
  return data;
}

TEST_CASE("StringBuilder::save() deduplicates strings") {
  SpyingAllocator spy;
  ResourceManager resources(&spy);
  StringBuilder builder(&resources);

  SECTION("Basic") {
    auto s1 = saveString(builder, "hello");
    auto s2 = saveString(builder, "world");
    auto s3 = saveString(builder, "hello");

    REQUIRE(s1.asString() == "hello");
    REQUIRE(s2.asString() == "world");
    REQUIRE(+s1.asString().c_str() == +s3.asString().c_str());  // same address

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("hello")),
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("world")),
                Allocate(sizeofStringBuffer()),
            });
  }

  SECTION("Requires terminator") {
    auto s1 = saveString(builder, "hello world");
    auto s2 = saveString(builder, "hello");

    REQUIRE(s1.asString() == "hello world");
    REQUIRE(s2.asString() == "hello");
    REQUIRE(+s2.asString().c_str() !=
            +s1.asString().c_str());  // different address

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("hello world")),
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("hello")),
            });
  }

  SECTION("Don't overrun") {
    auto s1 = saveString(builder, "hello world");
    auto s2 = saveString(builder, "worl");

    REQUIRE(s1.asString() == "hello world");
    REQUIRE(s2.asString() == "worl");
    REQUIRE(s2.asString().c_str() !=
            s1.asString().c_str());  // different address

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("hello world")),
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("worl")),
            });
  }
}
