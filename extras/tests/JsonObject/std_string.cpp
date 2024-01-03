// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

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

    REQUIRE(std::string("value") == obj[std::string("key")]);
  }

  SECTION("operator[] const") {
    char json[] = "{\"key\":\"value\"}";

    deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();

    REQUIRE(std::string("value") == obj[std::string("key")]);
  }

  SECTION("containsKey()") {
    char json[] = "{\"key\":\"value\"}";
    deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();
    REQUIRE(true == obj.containsKey(std::string("key")));
  }

  SECTION("remove()") {
    JsonObject obj = doc.to<JsonObject>();
    obj["key"] = "value";

    obj.remove(std::string("key"));

    REQUIRE(0 == obj.size());
  }

  SECTION("operator[], set key") {
    std::string key("hello");
    JsonObject obj = doc.to<JsonObject>();
    obj[key] = "world";
    eraseString(key);
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("operator[], set value") {
    std::string value("world");
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = value;
    eraseString(value);
    REQUIRE(std::string("world") == obj["hello"]);
  }
}
