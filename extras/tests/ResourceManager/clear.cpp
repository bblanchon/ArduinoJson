// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>

#include <doctest.h>

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::clear()") {
  ResourceManager resources;

  SUBCASE("Discards allocated variants") {
    resources.allocVariant();

    resources.clear();
    REQUIRE(resources.size() == 0);
  }

  SUBCASE("Discards allocated strings") {
    resources.saveString(adaptString("123456789"));
    REQUIRE(resources.size() == sizeofString(9));

    resources.clear();

    REQUIRE(resources.size() == 0);
  }
}
