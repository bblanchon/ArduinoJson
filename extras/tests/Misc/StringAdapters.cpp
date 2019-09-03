// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include "custom_string.hpp"

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ConstRamStringAdapter") {
  SECTION("null") {
    ConstRamStringAdapter adapter(NULL);

    REQUIRE(adapter.compare("bravo") < 0);
    REQUIRE(adapter.compare(NULL) == 0);

    REQUIRE(adapter.equals(NULL));
    REQUIRE_FALSE(adapter.equals("charlie"));
  }

  SECTION("non-null") {
    ConstRamStringAdapter adapter("bravo");

    REQUIRE(adapter.compare(NULL) > 0);
    REQUIRE(adapter.compare("alpha") > 0);
    REQUIRE(adapter.compare("bravo") == 0);
    REQUIRE(adapter.compare("charlie") < 0);

    REQUIRE(adapter.equals("bravo"));
    REQUIRE_FALSE(adapter.equals("charlie"));
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
}
