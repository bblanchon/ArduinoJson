// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("deserialize JSON array") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("An empty array") {
    DeserializationError err = deserializeJson(doc, "[]");
    JsonArray arr = doc.as<JsonArray>();

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(0 == arr.size());
  }

  SECTION("Spaces") {
    SECTION("Before the opening bracket") {
      DeserializationError err = deserializeJson(doc, "  []");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(0 == arr.size());
    }

    SECTION("Before first value") {
      DeserializationError err = deserializeJson(doc, "[ \t\r\n42]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }

    SECTION("After first value") {
      DeserializationError err = deserializeJson(doc, "[42 \t\r\n]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }
  }

  SECTION("Values types") {
    SECTION("On integer") {
      DeserializationError err = deserializeJson(doc, "[42]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }

    SECTION("Two integers") {
      DeserializationError err = deserializeJson(doc, "[42,84]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == 42);
      REQUIRE(arr[1] == 84);
    }

    SECTION("Double") {
      DeserializationError err = deserializeJson(doc, "[4.2,1e2]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == 4.2);
      REQUIRE(arr[1] == 1e2);
    }

    SECTION("Unsigned long") {
      DeserializationError err = deserializeJson(doc, "[4294967295]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 4294967295UL);
    }

    SECTION("Boolean") {
      DeserializationError err = deserializeJson(doc, "[true,false]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == true);
      REQUIRE(arr[1] == false);
    }

    SECTION("Null") {
      DeserializationError err = deserializeJson(doc, "[null,null]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0].as<const char*>() == 0);
      REQUIRE(arr[1].as<const char*>() == 0);
    }
  }

  SECTION("Quotes") {
    SECTION("Double quotes") {
      DeserializationError err =
          deserializeJson(doc, "[ \"hello\" , \"world\" ]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Single quotes") {
      DeserializationError err = deserializeJson(doc, "[ 'hello' , 'world' ]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("No quotes") {
      DeserializationError err = deserializeJson(doc, "[ hello , world ]");
      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("Double quotes (empty strings)") {
      DeserializationError err = deserializeJson(doc, "[\"\",\"\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("Single quotes (empty strings)") {
      DeserializationError err = deserializeJson(doc, "[\'\',\'\']");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("No quotes (empty strings)") {
      DeserializationError err = deserializeJson(doc, "[,]");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("Closing single quotes missing") {
      DeserializationError err = deserializeJson(doc, "[\"]");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("Closing double quotes missing") {
      DeserializationError err = deserializeJson(doc, "[\']");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SECTION("Premature null-terminator") {
    SECTION("After opening bracket") {
      DeserializationError err = deserializeJson(doc, "[");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("After value") {
      DeserializationError err = deserializeJson(doc, "[1");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("After comma") {
      DeserializationError err = deserializeJson(doc, "[1,");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SECTION("Premature end of input") {
    const char* input = "[1,2]";

    SECTION("After opening bracket") {
      DeserializationError err = deserializeJson(doc, input, 1);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("After value") {
      DeserializationError err = deserializeJson(doc, input, 2);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("After comma") {
      DeserializationError err = deserializeJson(doc, input, 3);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SECTION("Misc") {
    SECTION("Nested objects") {
      char jsonString[] =
          " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";

      DeserializationError err = deserializeJson(doc, jsonString);
      JsonArray arr = doc.as<JsonArray>();

      JsonObject object1 = arr[0];
      const JsonObject object2 = arr[1];
      JsonObject object3 = arr[2];

      REQUIRE(err == DeserializationError::Ok);

      REQUIRE(object1.isNull() == false);
      REQUIRE(object2.isNull() == false);
      REQUIRE(object3.isNull() == true);

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

  SECTION("Should clear the JsonArray") {
    deserializeJson(doc, "[1,2,3,4]");
    spy.clearLog();

    deserializeJson(doc, "[]");

    JsonArray arr = doc.as<JsonArray>();
    REQUIRE(arr.size() == 0);
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofArray(4)),
                         });
  }
}

TEST_CASE("deserialize JSON array under memory constraints") {
  TimebombAllocator timebomb(100);
  SpyingAllocator spy(&timebomb);
  JsonDocument doc(&spy);

  SECTION("empty array requires no allocation") {
    timebomb.setCountdown(0);
    char input[] = "[]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("allocation of pool list fails") {
    timebomb.setCountdown(0);
    char input[] = "[1]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
    REQUIRE(doc.as<std::string>() == "[]");
  }

  SECTION("allocation of pool fails") {
    timebomb.setCountdown(0);
    char input[] = "[1]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
    REQUIRE(doc.as<std::string>() == "[]");
  }

  SECTION("allocation of string fails in array") {
    timebomb.setCountdown(1);
    char input[] = "[0,\"hi!\"]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
    REQUIRE(doc.as<std::string>() == "[0,null]");
  }

  SECTION("don't store space characters") {
    deserializeJson(doc, "  [ \"1234567\" ] ");

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("1234567")),
                Reallocate(sizeofPool(), sizeofArray(1)),
            });
  }
}
