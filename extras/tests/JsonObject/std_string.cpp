// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

static void eraseString(std::string& str) {
  char* p = const_cast<char*>(str.c_str());
  while (*p)
    *p++ = '*';
}

TEST_CASE("std::string") {
  JsonDocument doc;

  SECTION("operator[]") {
    char json[] = "{\"key\":\"value\"}";

    deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();

    REQUIRE("value"_s == obj["key"_s]);
  }

  SECTION("operator[] const") {
    char json[] = "{\"key\":\"value\"}";

    deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();

    REQUIRE("value"_s == obj["key"_s]);
  }

  SECTION("containsKey()") {
    char json[] = "{\"key\":\"value\"}";
    deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();
    REQUIRE(true == obj.containsKey("key"_s));
  }

  SECTION("remove()") {
    JsonObject obj = doc.to<JsonObject>();
    obj["key"] = "value";

    obj.remove("key"_s);

    REQUIRE(0 == obj.size());
  }

  SECTION("operator[], set key") {
    std::string key("hello");
    JsonObject obj = doc.to<JsonObject>();
    obj[key] = "world";
    eraseString(key);
    REQUIRE("world"_s == obj["hello"]);
  }

  SECTION("operator[], set value") {
    std::string value("world");
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = value;
    eraseString(value);
    REQUIRE("world"_s == obj["hello"]);
  }
}
