// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::remove()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();
  obj["a"] = 0;
  obj["b"] = 1;
  obj["c"] = 2;
  std::string result;

  SECTION("remove(key)") {
    SECTION("Remove first") {
      obj.remove("a");
      serializeJson(obj, result);
      REQUIRE("{\"b\":1,\"c\":2}" == result);
    }

    SECTION("Remove middle") {
      obj.remove("b");
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"c\":2}" == result);
    }

    SECTION("Remove last") {
      obj.remove("c");
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"b\":1}" == result);
    }
  }

  SECTION("remove(iterator)") {
    JsonObject::iterator it = obj.begin();

    SECTION("Remove first") {
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"b\":1,\"c\":2}" == result);
    }

    SECTION("Remove middle") {
      ++it;
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"c\":2}" == result);
    }

    SECTION("Remove last") {
      it += 2;
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"b\":1}" == result);
    }
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a vla") {
    int i = 16;
    char vla[i];
    strcpy(vla, "b");
    obj.remove(vla);

    serializeJson(obj, result);
    REQUIRE("{\"a\":0,\"c\":2}" == result);
  }
#endif

  SECTION("remove by key on unbound reference") {
    JsonObject unboundObject;
    unboundObject.remove("key");
  }

  SECTION("remove by iterator on unbound reference") {
    JsonObject unboundObject;
    unboundObject.remove(unboundObject.begin());
  }
}
