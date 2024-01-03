// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Compare JsonArray with JsonArray") {
  JsonDocument doc;

  SECTION("Compare with unbound") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");
    JsonArray unbound;

    CHECK(array != unbound);
    CHECK_FALSE(array == unbound);
    CHECK_FALSE(array <= unbound);
    CHECK_FALSE(array >= unbound);
    CHECK_FALSE(array > unbound);
    CHECK_FALSE(array < unbound);

    CHECK(unbound != array);
    CHECK_FALSE(unbound == array);
    CHECK_FALSE(unbound <= array);
    CHECK_FALSE(unbound >= array);
    CHECK_FALSE(unbound > array);
    CHECK_FALSE(unbound < array);
  }

  SECTION("Compare with self") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");

    CHECK(array == array);
    CHECK(array <= array);
    CHECK(array >= array);
    CHECK_FALSE(array != array);
    CHECK_FALSE(array > array);
    CHECK_FALSE(array < array);
  }

  SECTION("Compare with identical array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello");
    array1.add<JsonObject>();

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello");
    array2.add<JsonObject>();

    CHECK(array1 == array2);
    CHECK(array1 <= array2);
    CHECK(array1 >= array2);
    CHECK_FALSE(array1 != array2);
    CHECK_FALSE(array1 > array2);
    CHECK_FALSE(array1 < array2);
  }

  SECTION("Compare with different array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello1");
    array1.add<JsonObject>();

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello2");
    array2.add<JsonObject>();

    CHECK(array1 != array2);
    CHECK_FALSE(array1 == array2);
    CHECK_FALSE(array1 > array2);
    CHECK_FALSE(array1 < array2);
    CHECK_FALSE(array1 <= array2);
    CHECK_FALSE(array1 >= array2);
  }
}

TEST_CASE("Compare JsonArray with JsonVariant") {
  JsonDocument doc;

  SECTION("Compare with self") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");

    JsonVariant variant = array;

    CHECK(array == variant);
    CHECK(array <= variant);
    CHECK(array >= variant);
    CHECK_FALSE(array != variant);
    CHECK_FALSE(array > variant);
    CHECK_FALSE(array < variant);

    CHECK(variant == array);
    CHECK(variant <= array);
    CHECK(variant >= array);
    CHECK_FALSE(variant != array);
    CHECK_FALSE(variant > array);
    CHECK_FALSE(variant < array);
  }

  SECTION("Compare with identical array") {
    JsonArray array = doc.add<JsonArray>();
    array.add(1);
    array.add("hello");
    array.add<JsonObject>();

    JsonVariant variant = doc.add<JsonArray>();
    variant.add(1);
    variant.add("hello");
    variant.add<JsonObject>();

    CHECK(array == variant);
    CHECK(array <= variant);
    CHECK(array >= variant);
    CHECK_FALSE(array != variant);
    CHECK_FALSE(array > variant);
    CHECK_FALSE(array < variant);

    CHECK(variant == array);
    CHECK(variant <= array);
    CHECK(variant >= array);
    CHECK_FALSE(variant != array);
    CHECK_FALSE(variant > array);
    CHECK_FALSE(variant < array);
  }

  SECTION("Compare with different array") {
    JsonArray array = doc.add<JsonArray>();
    array.add(1);
    array.add("hello1");
    array.add<JsonObject>();

    JsonVariant variant = doc.add<JsonArray>();
    variant.add(1);
    variant.add("hello2");
    variant.add<JsonObject>();

    CHECK(array != variant);
    CHECK_FALSE(array == variant);
    CHECK_FALSE(array > variant);
    CHECK_FALSE(array < variant);
    CHECK_FALSE(array <= variant);
    CHECK_FALSE(array >= variant);
  }
}

TEST_CASE("Compare JsonArray with JsonVariantConst") {
  JsonDocument doc;

  SECTION("Compare with unbound") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");
    JsonVariantConst unbound;

    CHECK(array != unbound);
    CHECK_FALSE(array == unbound);
    CHECK_FALSE(array <= unbound);
    CHECK_FALSE(array >= unbound);
    CHECK_FALSE(array > unbound);
    CHECK_FALSE(array < unbound);

    CHECK(unbound != array);
    CHECK_FALSE(unbound == array);
    CHECK_FALSE(unbound <= array);
    CHECK_FALSE(unbound >= array);
    CHECK_FALSE(unbound > array);
    CHECK_FALSE(unbound < array);
  }

  SECTION("Compare with self") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");

    JsonVariantConst variant = array;

    CHECK(array == variant);
    CHECK(array <= variant);
    CHECK(array >= variant);
    CHECK_FALSE(array != variant);
    CHECK_FALSE(array > variant);
    CHECK_FALSE(array < variant);

    CHECK(variant == array);
    CHECK(variant <= array);
    CHECK(variant >= array);
    CHECK_FALSE(variant != array);
    CHECK_FALSE(variant > array);
    CHECK_FALSE(variant < array);
  }

  SECTION("Compare with identical array") {
    JsonArray array = doc.add<JsonArray>();
    array.add(1);
    array.add("hello");
    array.add<JsonObject>();

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello");
    array2.add<JsonObject>();
    JsonVariantConst variant = array2;

    CHECK(array == variant);
    CHECK(array <= variant);
    CHECK(array >= variant);
    CHECK_FALSE(array != variant);
    CHECK_FALSE(array > variant);
    CHECK_FALSE(array < variant);

    CHECK(variant == array);
    CHECK(variant <= array);
    CHECK(variant >= array);
    CHECK_FALSE(variant != array);
    CHECK_FALSE(variant > array);
    CHECK_FALSE(variant < array);
  }

  SECTION("Compare with different array") {
    JsonArray array = doc.add<JsonArray>();
    array.add(1);
    array.add("hello1");
    array.add<JsonObject>();

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello2");
    array2.add<JsonObject>();
    JsonVariantConst variant = array2;

    CHECK(array != variant);
    CHECK_FALSE(array == variant);
    CHECK_FALSE(array > variant);
    CHECK_FALSE(array < variant);
    CHECK_FALSE(array <= variant);
    CHECK_FALSE(array >= variant);
  }
}

TEST_CASE("Compare JsonArray with JsonArrayConst") {
  JsonDocument doc;

  SECTION("Compare with unbound") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");
    JsonArrayConst unbound;

    CHECK(array != unbound);
    CHECK_FALSE(array == unbound);
    CHECK_FALSE(array <= unbound);
    CHECK_FALSE(array >= unbound);
    CHECK_FALSE(array > unbound);
    CHECK_FALSE(array < unbound);

    CHECK(unbound != array);
    CHECK_FALSE(unbound == array);
    CHECK_FALSE(unbound <= array);
    CHECK_FALSE(unbound >= array);
    CHECK_FALSE(unbound > array);
    CHECK_FALSE(unbound < array);
  }

  SECTION("Compare with self") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");
    JsonArrayConst carray = array;

    CHECK(array == carray);
    CHECK(array <= carray);
    CHECK(array >= carray);
    CHECK_FALSE(array != carray);
    CHECK_FALSE(array > carray);
    CHECK_FALSE(array < carray);

    CHECK(carray == array);
    CHECK(carray <= array);
    CHECK(carray >= array);
    CHECK_FALSE(carray != array);
    CHECK_FALSE(carray > array);
    CHECK_FALSE(carray < array);
  }

  SECTION("Compare with identical array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello");
    array1.add<JsonObject>();

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello");
    array2.add<JsonObject>();
    JsonArrayConst carray2 = array2;

    CHECK(array1 == carray2);
    CHECK(array1 <= carray2);
    CHECK(array1 >= carray2);
    CHECK_FALSE(array1 != carray2);
    CHECK_FALSE(array1 > carray2);
    CHECK_FALSE(array1 < carray2);

    CHECK(carray2 == array1);
    CHECK(carray2 <= array1);
    CHECK(carray2 >= array1);
    CHECK_FALSE(carray2 != array1);
    CHECK_FALSE(carray2 > array1);
    CHECK_FALSE(carray2 < array1);
  }

  SECTION("Compare with different array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello1");
    array1.add<JsonObject>();

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello2");
    array2.add<JsonObject>();
    JsonArrayConst carray2 = array2;

    CHECK(array1 != carray2);
    CHECK_FALSE(array1 == carray2);
    CHECK_FALSE(array1 > carray2);
    CHECK_FALSE(array1 < carray2);
    CHECK_FALSE(array1 <= carray2);
    CHECK_FALSE(array1 >= carray2);

    CHECK(carray2 != array1);
    CHECK_FALSE(carray2 == array1);
    CHECK_FALSE(carray2 > array1);
    CHECK_FALSE(carray2 < array1);
    CHECK_FALSE(carray2 <= array1);
    CHECK_FALSE(carray2 >= array1);
  }
}

TEST_CASE("Compare JsonArrayConst with JsonArrayConst") {
  JsonDocument doc;

  SECTION("Compare with unbound") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");

    JsonArrayConst carray = array;
    JsonArrayConst unbound;

    CHECK(carray != unbound);
    CHECK_FALSE(carray == unbound);
    CHECK_FALSE(carray <= unbound);
    CHECK_FALSE(carray >= unbound);
    CHECK_FALSE(carray > unbound);
    CHECK_FALSE(carray < unbound);

    CHECK(unbound != carray);
    CHECK_FALSE(unbound == carray);
    CHECK_FALSE(unbound <= carray);
    CHECK_FALSE(unbound >= carray);
    CHECK_FALSE(unbound > carray);
    CHECK_FALSE(unbound < carray);
  }

  SECTION("Compare with self") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");
    JsonArrayConst carray = array;

    CHECK(carray == carray);
    CHECK(carray <= carray);
    CHECK(carray >= carray);
    CHECK_FALSE(carray != carray);
    CHECK_FALSE(carray > carray);
    CHECK_FALSE(carray < carray);
  }

  SECTION("Compare with identical array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello");
    array1.add<JsonObject>();
    JsonArrayConst carray1 = array1;

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello");
    array2.add<JsonObject>();
    JsonArrayConst carray2 = array2;

    CHECK(carray1 == carray2);
    CHECK(carray1 <= carray2);
    CHECK(carray1 >= carray2);
    CHECK_FALSE(carray1 != carray2);
    CHECK_FALSE(carray1 > carray2);
    CHECK_FALSE(carray1 < carray2);
  }

  SECTION("Compare with different array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello1");
    array1.add<JsonObject>();
    JsonArrayConst carray1 = array1;

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello2");
    array2.add<JsonObject>();
    JsonArrayConst carray2 = array2;

    CHECK(carray1 != carray2);
    CHECK_FALSE(carray1 == carray2);
    CHECK_FALSE(carray1 > carray2);
    CHECK_FALSE(carray1 < carray2);
    CHECK_FALSE(carray1 <= carray2);
    CHECK_FALSE(carray1 >= carray2);
  }
}

TEST_CASE("Compare JsonArrayConst with JsonVariant") {
  JsonDocument doc;

  SECTION("Compare with self") {
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add("hello");
    JsonArrayConst carray = array;
    JsonVariant variant = array;

    CHECK(carray == variant);
    CHECK(carray <= variant);
    CHECK(carray >= variant);
    CHECK_FALSE(carray != variant);
    CHECK_FALSE(carray > variant);
    CHECK_FALSE(carray < variant);

    CHECK(variant == carray);
    CHECK(variant <= carray);
    CHECK(variant >= carray);
    CHECK_FALSE(variant != carray);
    CHECK_FALSE(variant > carray);
    CHECK_FALSE(variant < carray);
  }

  SECTION("Compare with identical array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello");
    array1.add<JsonObject>();
    JsonArrayConst carray1 = array1;

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello");
    array2.add<JsonObject>();
    JsonVariant variant2 = array2;

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

  SECTION("Compare with different array") {
    JsonArray array1 = doc.add<JsonArray>();
    array1.add(1);
    array1.add("hello1");
    array1.add<JsonObject>();
    JsonArrayConst carray1 = array1;

    JsonArray array2 = doc.add<JsonArray>();
    array2.add(1);
    array2.add("hello2");
    array2.add<JsonObject>();
    JsonVariant variant2 = array2;

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
