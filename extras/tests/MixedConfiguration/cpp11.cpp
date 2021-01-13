#include <ArduinoJson.h>

#include <catch.hpp>

#if __cplusplus >= 201103L

TEST_CASE("nullptr") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("JsonVariant == nullptr") {
    REQUIRE((variant == nullptr));
    REQUIRE_FALSE((variant != nullptr));
  }

  SECTION("JsonVariant != nullptr") {
    variant.set(42);

    REQUIRE_FALSE((variant == nullptr));
    REQUIRE((variant != nullptr));
  }

  SECTION("JsonVariant.set(nullptr)") {
    variant.set(42);
    variant.set(nullptr);

    REQUIRE(variant.isNull());
  }

  SECTION("JsonVariant.is<nullptr_t>()") {
    variant.set(42);
    REQUIRE(variant.is<std::nullptr_t>() == false);

    variant.clear();
    REQUIRE(variant.is<std::nullptr_t>() == true);
  }
}

TEST_CASE("Issue #1120") {
  StaticJsonDocument<500> doc;
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

#endif
