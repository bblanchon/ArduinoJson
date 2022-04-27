// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::clear()") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("size goes back to zero") {
    var.add(42);
    var.clear();

    REQUIRE(var.size() == 0);
  }

  SECTION("isNull() return true") {
    var.add("hello");
    var.clear();

    REQUIRE(var.isNull() == true);
  }

  SECTION("doesn't alter linked object") {
    StaticJsonDocument<128> doc2;
    doc2["hello"] = "world";
    var.link(doc2);

    var.clear();

    CHECK(var.isNull() == true);
    CHECK(doc2.as<std::string>() == "{\"hello\":\"world\"}");
  }
}
