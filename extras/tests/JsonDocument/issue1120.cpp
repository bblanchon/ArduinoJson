#include <ArduinoJson.h>

#include <doctest.h>

#include "Literals.hpp"

TEST_CASE("Issue #1120") {
  JsonDocument doc;
  constexpr char str[] =
      "{\"contents\":[{\"module\":\"Packet\"},{\"module\":\"Analog\"}]}";
  deserializeJson(doc, str);

  SUBCASE("MemberProxy<std::string>::isNull()") {
    SUBCASE("returns false") {
      CHECK(doc["contents"_s].isNull() == false);
    }

    SUBCASE("returns true") {
      CHECK(doc["zontents"_s].isNull() == true);
    }
  }

  SUBCASE("ElementProxy<MemberProxy<const char*> >::isNull()") {
    SUBCASE("returns false") {  // Issue #1120
      CHECK(doc["contents"][1].isNull() == false);
    }

    SUBCASE("returns true") {
      CHECK(doc["contents"][2].isNull() == true);
    }
  }

  SUBCASE("MemberProxy<ElementProxy<MemberProxy>, const char*>::isNull()") {
    SUBCASE("returns false") {
      CHECK(doc["contents"][1]["module"].isNull() == false);
    }

    SUBCASE("returns true") {
      CHECK(doc["contents"][1]["zodule"].isNull() == true);
    }
  }

  SUBCASE("MemberProxy<ElementProxy<MemberProxy>, std::string>::isNull()") {
    SUBCASE("returns false") {
      CHECK(doc["contents"][1]["module"_s].isNull() == false);
    }

    SUBCASE("returns true") {
      CHECK(doc["contents"][1]["zodule"_s].isNull() == true);
    }
  }
}
