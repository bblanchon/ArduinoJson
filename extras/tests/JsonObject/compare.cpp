// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Compare JsonObject with JsonObject") {
  StaticJsonDocument<512> doc;

  SECTION("Compare with unbound") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";
    JsonObject unbound;

    CHECK(object != unbound);
    CHECK_FALSE(object == unbound);
    CHECK_FALSE(object <= unbound);
    CHECK_FALSE(object >= unbound);
    CHECK_FALSE(object > unbound);
    CHECK_FALSE(object < unbound);

    CHECK(unbound != object);
    CHECK_FALSE(unbound == object);
    CHECK_FALSE(unbound <= object);
    CHECK_FALSE(unbound >= object);
    CHECK_FALSE(unbound > object);
    CHECK_FALSE(unbound < object);
  }

  SECTION("Compare with self") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";

    CHECK(object == object);
    CHECK(object <= object);
    CHECK(object >= object);
    CHECK_FALSE(object != object);
    CHECK_FALSE(object > object);
    CHECK_FALSE(object < object);
  }

  SECTION("Compare with identical object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello";
    object1["c"][0] = false;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello";
    object2["c"][0] = false;

    CHECK(object1 == object2);
    CHECK(object1 <= object2);
    CHECK(object1 >= object2);
    CHECK_FALSE(object1 != object2);
    CHECK_FALSE(object1 > object2);
    CHECK_FALSE(object1 < object2);
  }

  SECTION("Compare with different object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello1";
    object1["c"][0] = false;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello2";
    object2["c"][0] = false;

    CHECK(object1 != object2);
    CHECK_FALSE(object1 == object2);
    CHECK_FALSE(object1 > object2);
    CHECK_FALSE(object1 < object2);
    CHECK_FALSE(object1 <= object2);
    CHECK_FALSE(object1 >= object2);
  }
}

TEST_CASE("Compare JsonObject with JsonVariant") {
  StaticJsonDocument<512> doc;

  SECTION("Compare with self") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";

    JsonVariant variant = object;

    CHECK(object == variant);
    CHECK(object <= variant);
    CHECK(object >= variant);
    CHECK_FALSE(object != variant);
    CHECK_FALSE(object > variant);
    CHECK_FALSE(object < variant);

    CHECK(variant == object);
    CHECK(variant <= object);
    CHECK(variant >= object);
    CHECK_FALSE(variant != object);
    CHECK_FALSE(variant > object);
    CHECK_FALSE(variant < object);
  }

  SECTION("Compare with identical object") {
    JsonObject object = doc.createNestedObject();
    object["a"] = 1;
    object["b"] = "hello";
    object["c"][0] = false;

    JsonVariant variant = doc.createNestedObject();
    variant["a"] = 1;
    variant["b"] = "hello";
    variant["c"][0] = false;

    CHECK(object == variant);
    CHECK(object <= variant);
    CHECK(object >= variant);
    CHECK_FALSE(object != variant);
    CHECK_FALSE(object > variant);
    CHECK_FALSE(object < variant);

    CHECK(variant == object);
    CHECK(variant <= object);
    CHECK(variant >= object);
    CHECK_FALSE(variant != object);
    CHECK_FALSE(variant > object);
    CHECK_FALSE(variant < object);
  }

  SECTION("Compare with different object") {
    JsonObject object = doc.createNestedObject();
    object["a"] = 1;
    object["b"] = "hello1";
    object["c"][0] = false;

    JsonVariant variant = doc.createNestedObject();
    variant["a"] = 1;
    variant["b"] = "hello2";
    variant["c"][0] = false;

    CHECK(object != variant);
    CHECK_FALSE(object == variant);
    CHECK_FALSE(object > variant);
    CHECK_FALSE(object < variant);
    CHECK_FALSE(object <= variant);
    CHECK_FALSE(object >= variant);
  }
}

TEST_CASE("Compare JsonObject with JsonVariantConst") {
  StaticJsonDocument<512> doc;

  SECTION("Compare with unbound") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";
    JsonVariantConst unbound;

    CHECK(object != unbound);
    CHECK_FALSE(object == unbound);
    CHECK_FALSE(object <= unbound);
    CHECK_FALSE(object >= unbound);
    CHECK_FALSE(object > unbound);
    CHECK_FALSE(object < unbound);

    CHECK(unbound != object);
    CHECK_FALSE(unbound == object);
    CHECK_FALSE(unbound <= object);
    CHECK_FALSE(unbound >= object);
    CHECK_FALSE(unbound > object);
    CHECK_FALSE(unbound < object);
  }

  SECTION("Compare with self") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";

    JsonVariantConst variant = object;

    CHECK(object == variant);
    CHECK(object <= variant);
    CHECK(object >= variant);
    CHECK_FALSE(object != variant);
    CHECK_FALSE(object > variant);
    CHECK_FALSE(object < variant);

    CHECK(variant == object);
    CHECK(variant <= object);
    CHECK(variant >= object);
    CHECK_FALSE(variant != object);
    CHECK_FALSE(variant > object);
    CHECK_FALSE(variant < object);
  }

  SECTION("Compare with identical object") {
    JsonObject object = doc.createNestedObject();
    object["a"] = 1;
    object["b"] = "hello";
    object["c"][0] = false;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello";
    object2["c"][0] = false;
    JsonVariantConst variant = object2;

    CHECK(object == variant);
    CHECK(object <= variant);
    CHECK(object >= variant);
    CHECK_FALSE(object != variant);
    CHECK_FALSE(object > variant);
    CHECK_FALSE(object < variant);

    CHECK(variant == object);
    CHECK(variant <= object);
    CHECK(variant >= object);
    CHECK_FALSE(variant != object);
    CHECK_FALSE(variant > object);
    CHECK_FALSE(variant < object);
  }

  SECTION("Compare with different object") {
    JsonObject object = doc.createNestedObject();
    object["a"] = 1;
    object["b"] = "hello1";
    object["c"][0] = false;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello2";
    object2["c"][0] = false;
    JsonVariantConst variant = object2;

    CHECK(object != variant);
    CHECK_FALSE(object == variant);
    CHECK_FALSE(object > variant);
    CHECK_FALSE(object < variant);
    CHECK_FALSE(object <= variant);
    CHECK_FALSE(object >= variant);
  }
}

TEST_CASE("Compare JsonObject with JsonObjectConst") {
  StaticJsonDocument<512> doc;

  SECTION("Compare with unbound") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";
    JsonObjectConst unbound;

    CHECK(object != unbound);
    CHECK_FALSE(object == unbound);
    CHECK_FALSE(object <= unbound);
    CHECK_FALSE(object >= unbound);
    CHECK_FALSE(object > unbound);
    CHECK_FALSE(object < unbound);

    CHECK(unbound != object);
    CHECK_FALSE(unbound == object);
    CHECK_FALSE(unbound <= object);
    CHECK_FALSE(unbound >= object);
    CHECK_FALSE(unbound > object);
    CHECK_FALSE(unbound < object);
  }

  SECTION("Compare with self") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";
    JsonObjectConst cobject = object;

    CHECK(object == cobject);
    CHECK(object <= cobject);
    CHECK(object >= cobject);
    CHECK_FALSE(object != cobject);
    CHECK_FALSE(object > cobject);
    CHECK_FALSE(object < cobject);

    CHECK(cobject == object);
    CHECK(cobject <= object);
    CHECK(cobject >= object);
    CHECK_FALSE(cobject != object);
    CHECK_FALSE(cobject > object);
    CHECK_FALSE(cobject < object);
  }

  SECTION("Compare with identical object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello";
    object1["c"][0] = false;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello";
    object2["c"][0] = false;
    JsonObjectConst carray2 = object2;

    CHECK(object1 == carray2);
    CHECK(object1 <= carray2);
    CHECK(object1 >= carray2);
    CHECK_FALSE(object1 != carray2);
    CHECK_FALSE(object1 > carray2);
    CHECK_FALSE(object1 < carray2);

    CHECK(carray2 == object1);
    CHECK(carray2 <= object1);
    CHECK(carray2 >= object1);
    CHECK_FALSE(carray2 != object1);
    CHECK_FALSE(carray2 > object1);
    CHECK_FALSE(carray2 < object1);
  }

  SECTION("Compare with different object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello1";
    object1["c"][0] = false;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello2";
    object2["c"][0] = false;
    JsonObjectConst carray2 = object2;

    CHECK(object1 != carray2);
    CHECK_FALSE(object1 == carray2);
    CHECK_FALSE(object1 > carray2);
    CHECK_FALSE(object1 < carray2);
    CHECK_FALSE(object1 <= carray2);
    CHECK_FALSE(object1 >= carray2);

    CHECK(carray2 != object1);
    CHECK_FALSE(carray2 == object1);
    CHECK_FALSE(carray2 > object1);
    CHECK_FALSE(carray2 < object1);
    CHECK_FALSE(carray2 <= object1);
    CHECK_FALSE(carray2 >= object1);
  }
}

TEST_CASE("Compare JsonObjectConst with JsonObjectConst") {
  StaticJsonDocument<512> doc;

  SECTION("Compare with unbound") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";

    JsonObjectConst cobject = object;
    JsonObjectConst unbound;

    CHECK(cobject != unbound);
    CHECK_FALSE(cobject == unbound);
    CHECK_FALSE(cobject <= unbound);
    CHECK_FALSE(cobject >= unbound);
    CHECK_FALSE(cobject > unbound);
    CHECK_FALSE(cobject < unbound);

    CHECK(unbound != cobject);
    CHECK_FALSE(unbound == cobject);
    CHECK_FALSE(unbound <= cobject);
    CHECK_FALSE(unbound >= cobject);
    CHECK_FALSE(unbound > cobject);
    CHECK_FALSE(unbound < cobject);
  }

  SECTION("Compare with self") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";
    JsonObjectConst cobject = object;

    CHECK(cobject == cobject);
    CHECK(cobject <= cobject);
    CHECK(cobject >= cobject);
    CHECK_FALSE(cobject != cobject);
    CHECK_FALSE(cobject > cobject);
    CHECK_FALSE(cobject < cobject);
  }

  SECTION("Compare with identical object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello";
    object1["c"][0] = false;
    JsonObjectConst carray1 = object1;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello";
    object2["c"][0] = false;
    JsonObjectConst carray2 = object2;

    CHECK(carray1 == carray2);
    CHECK(carray1 <= carray2);
    CHECK(carray1 >= carray2);
    CHECK_FALSE(carray1 != carray2);
    CHECK_FALSE(carray1 > carray2);
    CHECK_FALSE(carray1 < carray2);
  }

  SECTION("Compare with different object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello1";
    object1["c"][0] = false;
    JsonObjectConst carray1 = object1;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello2";
    object2["c"][0] = false;
    JsonObjectConst carray2 = object2;

    CHECK(carray1 != carray2);
    CHECK_FALSE(carray1 == carray2);
    CHECK_FALSE(carray1 > carray2);
    CHECK_FALSE(carray1 < carray2);
    CHECK_FALSE(carray1 <= carray2);
    CHECK_FALSE(carray1 >= carray2);
  }
}

TEST_CASE("Compare JsonObjectConst with JsonVariant") {
  StaticJsonDocument<512> doc;

  SECTION("Compare with self") {
    JsonObject object = doc.to<JsonObject>();
    object["a"] = 1;
    object["b"] = "hello";
    JsonObjectConst cobject = object;
    JsonVariant variant = object;

    CHECK(cobject == variant);
    CHECK(cobject <= variant);
    CHECK(cobject >= variant);
    CHECK_FALSE(cobject != variant);
    CHECK_FALSE(cobject > variant);
    CHECK_FALSE(cobject < variant);

    CHECK(variant == cobject);
    CHECK(variant <= cobject);
    CHECK(variant >= cobject);
    CHECK_FALSE(variant != cobject);
    CHECK_FALSE(variant > cobject);
    CHECK_FALSE(variant < cobject);
  }

  SECTION("Compare with identical object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello";
    object1["c"][0] = false;
    JsonObjectConst carray1 = object1;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello";
    object2["c"][0] = false;
    JsonVariant variant2 = object2;

    CHECK(carray1 == variant2);
    CHECK(carray1 <= variant2);
    CHECK(carray1 >= variant2);
    CHECK_FALSE(carray1 != variant2);
    CHECK_FALSE(carray1 > variant2);
    CHECK_FALSE(carray1 < variant2);

    CHECK(variant2 == carray1);
    CHECK(variant2 <= carray1);
    CHECK(variant2 >= carray1);
    CHECK_FALSE(variant2 != carray1);
    CHECK_FALSE(variant2 > carray1);
    CHECK_FALSE(variant2 < carray1);
  }

  SECTION("Compare with different object") {
    JsonObject object1 = doc.createNestedObject();
    object1["a"] = 1;
    object1["b"] = "hello1";
    object1["c"][0] = false;
    JsonObjectConst carray1 = object1;

    JsonObject object2 = doc.createNestedObject();
    object2["a"] = 1;
    object2["b"] = "hello2";
    object2["c"][0] = false;
    JsonVariant variant2 = object2;

    CHECK(carray1 != variant2);
    CHECK_FALSE(carray1 == variant2);
    CHECK_FALSE(carray1 > variant2);
    CHECK_FALSE(carray1 < variant2);
    CHECK_FALSE(carray1 <= variant2);
    CHECK_FALSE(carray1 >= variant2);

    CHECK(variant2 != carray1);
    CHECK_FALSE(variant2 == carray1);
    CHECK_FALSE(variant2 > carray1);
    CHECK_FALSE(variant2 < carray1);
    CHECK_FALSE(variant2 <= carray1);
    CHECK_FALSE(variant2 >= carray1);
  }
}
