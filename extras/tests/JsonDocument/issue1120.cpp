#include <ArduinoJson.h>

#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("Issue #1120") {
  JsonDocument doc;
  constexpr char str[] =
      "{\"contents\":[{\"module\":\"Packet\"},{\"module\":\"Analog\"}]}";
  deserializeJson(doc, str);

  SECTION("MemberProxy<std::string>::isNull()") {
    SECTION("returns false") {
      auto value = doc["contents"_s];
      CHECK(value.isNull() == false);
    }

    SECTION("returns true") {
      auto value = doc["zontents"_s];
      CHECK(value.isNull() == true);
    }
  }

  SECTION("ElementProxy<MemberProxy<const char*> >::isNull()") {
    SECTION("returns false") {  // Issue #1120
      auto value = doc["contents"][1];
      CHECK(value.isNull() == false);
    }

    SECTION("returns true") {
      auto value = doc["contents"][2];
      CHECK(value.isNull() == true);
    }
  }

  SECTION("MemberProxy<ElementProxy<MemberProxy>, const char*>::isNull()") {
    SECTION("returns false") {
      auto value = doc["contents"][1]["module"];
      CHECK(value.isNull() == false);
    }

    SECTION("returns true") {
      auto value = doc["contents"][1]["zodule"];
      CHECK(value.isNull() == true);
    }
  }

  SECTION("MemberProxy<ElementProxy<MemberProxy>, std::string>::isNull()") {
    SECTION("returns false") {
      auto value = doc["contents"][1]["module"_s];
      CHECK(value.isNull() == false);
    }

    SECTION("returns true") {
      auto value = doc["contents"][1]["zodule"_s];
      CHECK(value.isNull() == true);
    }
  }
}
