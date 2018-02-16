// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonBuffer::parseObject()") {
  DynamicJsonBuffer jb;

  SECTION("An empty object") {
    JsonObject& obj = jb.parseObject("{}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 0);
  }

  SECTION("Quotes") {
    SECTION("Double quotes") {
      JsonObject& obj = jb.parseObject("{\"key\":\"value\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Single quotes") {
      JsonObject& obj = jb.parseObject("{'key':'value'}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("No quotes") {
      JsonObject& obj = jb.parseObject("{key:value}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("No quotes, allow underscore in key") {
      JsonObject& obj = jb.parseObject("{_k_e_y_:42}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["_k_e_y_"] == 42);
    }
  }

  SECTION("Spaces") {
    SECTION("Before the key") {
      JsonObject& obj = jb.parseObject("{ \"key\":\"value\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("After the key") {
      JsonObject& obj = jb.parseObject("{\"key\" :\"value\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Before the value") {
      JsonObject& obj = jb.parseObject("{\"key\": \"value\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("After the value") {
      JsonObject& obj = jb.parseObject("{\"key\":\"value\" }");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Before the colon") {
      JsonObject& obj =
          jb.parseObject("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }

    SECTION("After the colon") {
      JsonObject& obj =
          jb.parseObject("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }
  }

  SECTION("Values types") {
    SECTION("String") {
      JsonObject& obj =
          jb.parseObject("{\"key1\":\"value1\",\"key2\":\"value2\"}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }

    SECTION("Integer") {
      JsonObject& obj = jb.parseObject("{\"key1\":42,\"key2\":-42}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == 42);
      REQUIRE(obj["key2"] == -42);
    }

    SECTION("Double") {
      JsonObject& obj = jb.parseObject("{\"key1\":12.345,\"key2\":-7E89}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == 12.345);
      REQUIRE(obj["key2"] == -7E89);
    }

    SECTION("Booleans") {
      JsonObject& obj = jb.parseObject("{\"key1\":true,\"key2\":false}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == true);
      REQUIRE(obj["key2"] == false);
    }

    SECTION("Null") {
      JsonObject& obj = jb.parseObject("{\"key1\":null,\"key2\":null}");
      REQUIRE(obj.success());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"].as<char*>() == 0);
      REQUIRE(obj["key2"].as<char*>() == 0);
    }
  }

  SECTION("Misc") {
    SECTION("The opening brace is missing") {
      JsonObject& obj = jb.parseObject("}");
      REQUIRE_FALSE(obj.success());
    }

    SECTION("The closing brace is missing") {
      JsonObject& obj = jb.parseObject("{");
      REQUIRE_FALSE(obj.success());
    }

    SECTION("A quoted key without value") {
      JsonObject& obj = jb.parseObject("{\"key\"}");
      REQUIRE_FALSE(obj.success());
    }

    SECTION("A non-quoted key without value") {
      JsonObject& obj = jb.parseObject("{key}");
      REQUIRE_FALSE(obj.success());
    }

    SECTION("A dangling comma") {
      JsonObject& obj = jb.parseObject("{\"key1\":\"value1\",}");
      REQUIRE_FALSE(obj.success());
      REQUIRE(obj.size() == 0);
    }

    SECTION("null as a key") {
      JsonObject& obj = jb.parseObject("null:\"value\"}");
      REQUIRE_FALSE(obj.success());
    }
  }
}
