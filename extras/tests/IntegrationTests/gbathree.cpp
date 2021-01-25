// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Gbathree") {
  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeJson(
      doc,
      "{\"protocol_name\":\"fluorescence\",\"repeats\":1,\"wait\":0,"
      "\"averages\":1,\"measurements\":3,\"meas2_light\":15,\"meas1_"
      "baseline\":0,\"act_light\":20,\"pulsesize\":25,\"pulsedistance\":"
      "10000,\"actintensity1\":50,\"actintensity2\":255,\"measintensity\":"
      "255,\"calintensity\":255,\"pulses\":[50,50,50],\"act\":[2,1,2,2],"
      "\"red\":[2,2,2,2],\"detectors\":[[34,34,34,34],[34,34,34,34],[34,"
      "34,34,34],[34,34,34,34]],\"alta\":[2,2,2,2],\"altb\":[2,2,2,2],"
      "\"measlights\":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,"
      "15,15]],\"measlights2\":[[15,15,15,15],[15,15,15,15],[15,15,15,15],"
      "[15,15,15,15]],\"altc\":[2,2,2,2],\"altd\":[2,2,2,2]}");
  JsonObject root = doc.as<JsonObject>();

  SECTION("Success") {
    REQUIRE(error == DeserializationError::Ok);
  }

  SECTION("ProtocolName") {
    REQUIRE("fluorescence" == root["protocol_name"]);
  }

  SECTION("Repeats") {
    REQUIRE(1 == root["repeats"]);
  }

  SECTION("Wait") {
    REQUIRE(0 == root["wait"]);
  }

  SECTION("Measurements") {
    REQUIRE(3 == root["measurements"]);
  }

  SECTION("Meas2_Light") {
    REQUIRE(15 == root["meas2_light"]);
  }

  SECTION("Meas1_Baseline") {
    REQUIRE(0 == root["meas1_baseline"]);
  }

  SECTION("Act_Light") {
    REQUIRE(20 == root["act_light"]);
  }

  SECTION("Pulsesize") {
    REQUIRE(25 == root["pulsesize"]);
  }

  SECTION("Pulsedistance") {
    REQUIRE(10000 == root["pulsedistance"]);
  }

  SECTION("Actintensity1") {
    REQUIRE(50 == root["actintensity1"]);
  }

  SECTION("Actintensity2") {
    REQUIRE(255 == root["actintensity2"]);
  }

  SECTION("Measintensity") {
    REQUIRE(255 == root["measintensity"]);
  }

  SECTION("Calintensity") {
    REQUIRE(255 == root["calintensity"]);
  }

  SECTION("Pulses") {
    // "pulses":[50,50,50]

    JsonArray array = root["pulses"];
    REQUIRE(array.isNull() == false);

    REQUIRE(3 == array.size());

    for (size_t i = 0; i < 3; i++) {
      REQUIRE(50 == array[i]);
    }
  }

  SECTION("Act") {
    // "act":[2,1,2,2]

    JsonArray array = root["act"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());
    REQUIRE(2 == array[0]);
    REQUIRE(1 == array[1]);
    REQUIRE(2 == array[2]);
    REQUIRE(2 == array[3]);
  }

  SECTION("Detectors") {
    // "detectors":[[34,34,34,34],[34,34,34,34],[34,34,34,34],[34,34,34,34]]

    JsonArray array = root["detectors"];
    REQUIRE(array.isNull() == false);
    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      JsonArray nestedArray = array[i];
      REQUIRE(4 == nestedArray.size());

      for (size_t j = 0; j < 4; j++) {
        REQUIRE(34 == nestedArray[j]);
      }
    }
  }

  SECTION("Alta") {
    // alta:[2,2,2,2]

    JsonArray array = root["alta"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }

  SECTION("Altb") {
    // altb:[2,2,2,2]

    JsonArray array = root["altb"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }

  SECTION("Measlights") {
    // "measlights":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

    JsonArray array = root["measlights"];
    REQUIRE(array.isNull() == false);
    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      JsonArray nestedArray = array[i];

      REQUIRE(4 == nestedArray.size());

      for (size_t j = 0; j < 4; j++) {
        REQUIRE(15 == nestedArray[j]);
      }
    }
  }

  SECTION("Measlights2") {
    // "measlights2":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

    JsonArray array = root["measlights2"];
    REQUIRE(array.isNull() == false);
    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      JsonArray nestedArray = array[i];
      REQUIRE(4 == nestedArray.size());

      for (size_t j = 0; j < 4; j++) {
        REQUIRE(15 == nestedArray[j]);
      }
    }
  }

  SECTION("Altc") {
    // altc:[2,2,2,2]

    JsonArray array = root["altc"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }

  SECTION("Altd") {
    // altd:[2,2,2,2]

    JsonArray array = root["altd"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }
}
