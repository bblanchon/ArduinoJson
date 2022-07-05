// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::shallowCopy()") {
  StaticJsonDocument<1024> doc1, doc2;
  JsonVariant variant = doc1.to<JsonVariant>();

  SECTION("JsonVariant::shallowCopy(JsonDocument&)") {
    doc2["hello"] = "world";

    variant.shallowCopy(doc2);

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");

    // altering the linked document should change the result
    doc2["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }

  SECTION("JsonVariant::shallowCopy(MemberProxy)") {
    doc2["obj"]["hello"] = "world";

    variant.shallowCopy(doc2["obj"]);

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");

    // altering the linked document should change the result
    doc2["obj"]["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }

  SECTION("JsonVariant::shallowCopy(ElementProxy)") {
    doc2[0]["hello"] = "world";

    variant.shallowCopy(doc2[0]);

    CHECK(variant.as<std::string>() == "{\"hello\":\"world\"}");

    // altering the linked document should change the result
    doc2[0]["hello"] = "WORLD!";

    CHECK(variant.as<std::string>() == "{\"hello\":\"WORLD!\"}");
  }

  SECTION("target is unbound") {
    JsonVariant unbound;
    variant["hello"] = "world";

    variant.shallowCopy(unbound);

    CHECK(variant.isUnbound() == false);
    CHECK(variant.isNull() == true);
    CHECK(variant.memoryUsage() == 0);
    CHECK(variant.size() == 0);
  }

  SECTION("variant is unbound") {
    JsonVariant unbound;
    doc2["hello"] = "world";

    unbound.shallowCopy(doc2);

    CHECK(unbound.isUnbound() == true);
    CHECK(unbound.isNull() == true);
    CHECK(unbound.memoryUsage() == 0);
    CHECK(unbound.size() == 0);
  }

  SECTION("preserves owned key bit") {
    doc2.set(42);

    doc1["a"].shallowCopy(doc2);
    doc1[std::string("b")].shallowCopy(doc2);

    JsonObject::iterator it = doc1.as<JsonObject>().begin();

    CHECK(it->key().isLinked() == true);
    ++it;
    CHECK(it->key().isLinked() == false);
  }
}
