// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

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

TEST_CASE("StlString") {
  std::string str("bravo");
  StlStringAdapter adapter(str);

  REQUIRE(adapter.compare(NULL) > 0);
  REQUIRE(adapter.compare("alpha") > 0);
  REQUIRE(adapter.compare("bravo") == 0);
  REQUIRE(adapter.compare("charlie") < 0);

  REQUIRE(adapter.equals("bravo"));
  REQUIRE_FALSE(adapter.equals("charlie"));
}
