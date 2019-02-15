// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonBuffer nested objects") {
  SECTION("ArrayNestedInObject") {
    DynamicJsonBuffer jsonBuffer;
    char jsonString[] = " { \"ab\" : [ 1 , 2 ] , \"cd\" : [ 3 , 4 ] } ";

    JsonObject &object = jsonBuffer.parseObject(jsonString);
    JsonArray &array1 = object["ab"];
    const JsonArray &array2 = object["cd"];
    JsonArray &array3 = object["ef"];

    REQUIRE(true == object.success());

    REQUIRE(true == array1.success());
    REQUIRE(true == array2.success());
    REQUIRE(false == array3.success());

    REQUIRE(2 == array1.size());
    REQUIRE(2 == array2.size());
    REQUIRE(0 == array3.size());

    REQUIRE(1 == array1[0].as<int>());
    REQUIRE(2 == array1[1].as<int>());

    REQUIRE(3 == array2[0].as<int>());
    REQUIRE(4 == array2[1].as<int>());

    REQUIRE(0 == array3[0].as<int>());
  }

  SECTION("ObjectNestedInArray") {
    DynamicJsonBuffer jsonBuffer;
    char jsonString[] =
        " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";

    JsonArray &array = jsonBuffer.parseArray(jsonString);
    JsonObject &object1 = array[0];
    const JsonObject &object2 = array[1];
    JsonObject &object3 = array[2];

    REQUIRE(true == array.success());

    REQUIRE(true == object1.success());
    REQUIRE(true == object2.success());
    REQUIRE(false == object3.success());

    REQUIRE(2 == object1.size());
    REQUIRE(2 == object2.size());
    REQUIRE(0 == object3.size());

    REQUIRE(1 == object1["a"].as<int>());
    REQUIRE(2 == object1["b"].as<int>());
    REQUIRE(3 == object2["c"].as<int>());
    REQUIRE(4 == object2["d"].as<int>());
    REQUIRE(0 == object3["e"].as<int>());
  }
}
