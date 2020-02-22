// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("ElementProxy::add()") {
  DynamicJsonDocument doc(4096);
  doc.addElement();
  ElementProxy<JsonDocument&> ep = doc[0];

  SECTION("add(int)") {
    ep.add(42);

    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("add(const char*)") {
    ep.add("world");

    REQUIRE(doc.as<std::string>() == "[[\"world\"]]");
  }

  SECTION("set(char[])") {
    char s[] = "world";
    ep.add(s);
    strcpy(s, "!!!!!");

    REQUIRE(doc.as<std::string>() == "[[\"world\"]]");
  }
}
