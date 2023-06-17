// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/VariantPoolImpl.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

static const size_t poolCapacity = 512;

TEST_CASE("ResourceManager::clear()") {
  ResourceManager resources(poolCapacity);

  SECTION("Discards allocated variants") {
    resources.allocVariant();

    resources.clear();
    REQUIRE(resources.size() == 0);
  }

  SECTION("Discards allocated strings") {
    resources.saveString(adaptString("123456789"));
    REQUIRE(resources.size() == sizeofString(9));

    resources.clear();

    REQUIRE(resources.size() == 0);
  }
}
