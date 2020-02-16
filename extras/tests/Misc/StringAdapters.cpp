// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include "custom_string.hpp"
#include "progmem_emulation.hpp"

#include <ArduinoJson/Strings/ConstRamStringAdapter.hpp>
#include <ArduinoJson/Strings/FlashStringAdapter.hpp>
#include <ArduinoJson/Strings/StlStringAdapter.hpp>

#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ConstRamStringAdapter") {
  SECTION("null") {
    ConstRamStringAdapter adapter(NULL);

    REQUIRE(adapter.compare("bravo") < 0);
    REQUIRE(adapter.compare(NULL) == 0);

    REQUIRE(adapter.equals(NULL));
    REQUIRE_FALSE(adapter.equals("charlie"));

    REQUIRE(adapter.size() == 0);
  }

  SECTION("non-null") {
    ConstRamStringAdapter adapter("bravo");

    REQUIRE(adapter.compare(NULL) > 0);
    REQUIRE(adapter.compare("alpha") > 0);
    REQUIRE(adapter.compare("bravo") == 0);
    REQUIRE(adapter.compare("charlie") < 0);

    REQUIRE(adapter.equals("bravo"));
    REQUIRE_FALSE(adapter.equals("charlie"));

    REQUIRE(adapter.size() == 5);
  }
}

TEST_CASE("FlashStringAdapter") {
  SECTION("null") {
    FlashStringAdapter adapter(NULL);

    REQUIRE(adapter.compare("bravo") < 0);
    REQUIRE(adapter.compare(NULL) == 0);

    REQUIRE(adapter.equals(NULL));
    REQUIRE_FALSE(adapter.equals("charlie"));

    REQUIRE(adapter.size() == 0);
  }

  SECTION("non-null") {
    FlashStringAdapter adapter = adaptString(F("bravo"));

    REQUIRE(adapter.compare(NULL) > 0);
    REQUIRE(adapter.compare("alpha") > 0);
    REQUIRE(adapter.compare("bravo") == 0);
    REQUIRE(adapter.compare("charlie") < 0);

    REQUIRE(adapter.equals("bravo"));
    REQUIRE_FALSE(adapter.equals("charlie"));

    REQUIRE(adapter.size() == 5);
  }
}

TEST_CASE("std::string") {
  std::string str("bravo");
  StlStringAdapter<std::string> adapter = adaptString(str);

  REQUIRE(adapter.compare(NULL) > 0);
  REQUIRE(adapter.compare("alpha") > 0);
  REQUIRE(adapter.compare("bravo") == 0);
  REQUIRE(adapter.compare("charlie") < 0);

  REQUIRE(adapter.equals("bravo"));
  REQUIRE_FALSE(adapter.equals("charlie"));

  REQUIRE(adapter.size() == 5);
}

TEST_CASE("custom_string") {
  custom_string str("bravo");
  StlStringAdapter<custom_string> adapter = adaptString(str);

  REQUIRE(adapter.compare(NULL) > 0);
  REQUIRE(adapter.compare("alpha") > 0);
  REQUIRE(adapter.compare("bravo") == 0);
  REQUIRE(adapter.compare("charlie") < 0);

  REQUIRE(adapter.equals("bravo"));
  REQUIRE_FALSE(adapter.equals("charlie"));

  REQUIRE(adapter.size() == 5);
}

TEST_CASE("IsString<T>") {
  SECTION("std::string") {
    REQUIRE(IsString<std::string>::value == true);
  }

  SECTION("basic_string<wchar_t>") {
    REQUIRE(IsString<std::basic_string<wchar_t> >::value == false);
  }

  SECTION("custom_string") {
    REQUIRE(IsString<custom_string>::value == true);
  }

  SECTION("const __FlashStringHelper*") {
    REQUIRE(IsString<const __FlashStringHelper*>::value == true);
  }
}
