// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeJson(DynamicJsonObject&)") {
  DynamicJsonObject obj;

  SECTION("An empty object") {
    JsonError err = deserializeJson(obj, "{}");
    REQUIRE(err == JsonError::Ok);
    REQUIRE(obj.size() == 0);
  }

  SECTION("Quotes") {
    SECTION("Double quotes") {
      JsonError err = deserializeJson(obj, "{\"key\":\"value\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Single quotes") {
      JsonError err = deserializeJson(obj, "{'key':'value'}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("No quotes") {
      JsonError err = deserializeJson(obj, "{key:value}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("No quotes, allow underscore in key") {
      JsonError err = deserializeJson(obj, "{_k_e_y_:42}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["_k_e_y_"] == 42);
    }
  }

  SECTION("Spaces") {
    SECTION("Before the key") {
      JsonError err = deserializeJson(obj, "{ \"key\":\"value\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("After the key") {
      JsonError err = deserializeJson(obj, "{\"key\" :\"value\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Before the value") {
      JsonError err = deserializeJson(obj, "{\"key\": \"value\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("After the value") {
      JsonError err = deserializeJson(obj, "{\"key\":\"value\" }");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Before the colon") {
      JsonError err =
          deserializeJson(obj, "{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }

    SECTION("After the colon") {
      JsonError err =
          deserializeJson(obj, "{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }
  }

  SECTION("Values types") {
    SECTION("String") {
      JsonError err =
          deserializeJson(obj, "{\"key1\":\"value1\",\"key2\":\"value2\"}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }

    SECTION("Integer") {
      JsonError err = deserializeJson(obj, "{\"key1\":42,\"key2\":-42}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == 42);
      REQUIRE(obj["key2"] == -42);
    }

    SECTION("Double") {
      JsonError err = deserializeJson(obj, "{\"key1\":12.345,\"key2\":-7E89}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == 12.345);
      REQUIRE(obj["key2"] == -7E89);
    }

    SECTION("Booleans") {
      JsonError err = deserializeJson(obj, "{\"key1\":true,\"key2\":false}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == true);
      REQUIRE(obj["key2"] == false);
    }

    SECTION("Null") {
      JsonError err = deserializeJson(obj, "{\"key1\":null,\"key2\":null}");
      REQUIRE(err == JsonError::Ok);
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"].as<char *>() == 0);
      REQUIRE(obj["key2"].as<char *>() == 0);
    }

    SECTION("Array") {
      char jsonString[] = " { \"ab\" : [ 1 , 2 ] , \"cd\" : [ 3 , 4 ] } ";

      JsonError err = deserializeJson(obj, jsonString);

      JsonArray &array1 = obj["ab"];
      const JsonArray &array2 = obj["cd"];
      JsonArray &array3 = obj["ef"];

      REQUIRE(err == JsonError::Ok);

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
  }

  SECTION("Misc") {
    SECTION("The opening brace is missing") {
      JsonError err = deserializeJson(obj, "}");
      REQUIRE(err == JsonError::OpeningBraceExpected);
    }

    SECTION("The closing brace is missing") {
      JsonError err = deserializeJson(obj, "{\"hello\":\"world\"");
      REQUIRE(err == JsonError::ClosingBraceExpected);
    }

    SECTION("A quoted key without value") {
      JsonError err = deserializeJson(obj, "{\"key\"}");
      REQUIRE(err == JsonError::ColonExpected);
    }

    SECTION("A non-quoted key without value") {
      JsonError err = deserializeJson(obj, "{key}");
      REQUIRE(err == JsonError::ColonExpected);
    }

    SECTION("A dangling comma") {
      JsonError err = deserializeJson(obj, "{\"key1\":\"value1\",}");
      REQUIRE(err == JsonError::ColonExpected);
    }

    SECTION("null as a key") {
      JsonError err = deserializeJson(obj, "{null:\"value\"}");
      REQUIRE(err == JsonError::Ok);
    }
  }

  SECTION("Should clear the JsonObject") {
    deserializeJson(obj, "{\"hello\":\"world\"}");
    deserializeJson(obj, "{}");
    REQUIRE(obj.size() == 0);
    REQUIRE(obj.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
