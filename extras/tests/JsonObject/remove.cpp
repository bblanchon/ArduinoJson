// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <string>

TEST_CASE("JsonObject::remove()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["a"] = 0;
  obj["b"] = 1;
  obj["c"] = 2;
  std::string result;

  SUBCASE("remove(key)") {
    SUBCASE("Remove first") {
      obj.remove("a");
      serializeJson(obj, result);
      REQUIRE("{\"b\":1,\"c\":2}" == result);
    }

    SUBCASE("Remove middle") {
      obj.remove("b");
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"c\":2}" == result);
    }

    SUBCASE("Remove last") {
      obj.remove("c");
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"b\":1}" == result);
    }
  }

  SUBCASE("remove(iterator)") {
    JsonObject::iterator it = obj.begin();

    SUBCASE("Remove first") {
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"b\":1,\"c\":2}" == result);
    }

    SUBCASE("Remove middle") {
      ++it;
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"c\":2}" == result);
    }

    SUBCASE("Remove last") {
      ++it;
      ++it;
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"b\":1}" == result);
    }
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("key is a vla") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "b");
    obj.remove(vla);

    serializeJson(obj, result);
    REQUIRE("{\"a\":0,\"c\":2}" == result);
  }
#endif

  SUBCASE("remove by key on unbound reference") {
    JsonObject unboundObject;
    unboundObject.remove("key");
  }

  SUBCASE("remove by iterator on unbound reference") {
    JsonObject unboundObject;
    unboundObject.remove(unboundObject.begin());
  }

  SUBCASE("remove(JsonVariant)") {
    obj["key"] = "b";
    obj.remove(obj["key"]);
    REQUIRE("{\"a\":0,\"c\":2,\"key\":\"b\"}" == doc.as<std::string>());
  }
}
