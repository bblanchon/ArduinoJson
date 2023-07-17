#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("Issue #1120") {
  JsonDocument doc;
  constexpr char str[] =
      "{\"contents\":[{\"module\":\"Packet\"},{\"module\":\"Analog\"}]}";
  deserializeJson(doc, str);

  SECTION("MemberProxy<std::string>::isNull()") {
    SECTION("returns false") {
      auto value = doc[std::string("contents")];
      CHECK(value.isNull() == false);
    }

    SECTION("returns true") {
      auto value = doc[std::string("zontents")];
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
      auto value = doc["contents"][1][std::string("module")];
      CHECK(value.isNull() == false);
    }

    SECTION("returns true") {
      auto value = doc["contents"][1][std::string("zodule")];
      CHECK(value.isNull() == true);
    }
  }
}
