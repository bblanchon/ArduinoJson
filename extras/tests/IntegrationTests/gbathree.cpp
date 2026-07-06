// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("Gbathree") {
  JsonDocument doc;

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

  SUBCASE("Success") {
    REQUIRE(error == DeserializationError::Ok);
  }

  SUBCASE("ProtocolName") {
    REQUIRE("fluorescence" == root["protocol_name"]);
  }

  SUBCASE("Repeats") {
    REQUIRE(1 == root["repeats"]);
  }

  SUBCASE("Wait") {
    REQUIRE(0 == root["wait"]);
  }

  SUBCASE("Measurements") {
    REQUIRE(3 == root["measurements"]);
  }

  SUBCASE("Meas2_Light") {
    REQUIRE(15 == root["meas2_light"]);
  }

  SUBCASE("Meas1_Baseline") {
    REQUIRE(0 == root["meas1_baseline"]);
  }

  SUBCASE("Act_Light") {
    REQUIRE(20 == root["act_light"]);
  }

  SUBCASE("Pulsesize") {
    REQUIRE(25 == root["pulsesize"]);
  }

  SUBCASE("Pulsedistance") {
    REQUIRE(10000 == root["pulsedistance"]);
  }

  SUBCASE("Actintensity1") {
    REQUIRE(50 == root["actintensity1"]);
  }

  SUBCASE("Actintensity2") {
    REQUIRE(255 == root["actintensity2"]);
  }

  SUBCASE("Measintensity") {
    REQUIRE(255 == root["measintensity"]);
  }

  SUBCASE("Calintensity") {
    REQUIRE(255 == root["calintensity"]);
  }

  SUBCASE("Pulses") {
    // "pulses":[50,50,50]

    JsonArray array = root["pulses"];
    REQUIRE(array.isNull() == false);

    REQUIRE(3 == array.size());

    for (size_t i = 0; i < 3; i++) {
      REQUIRE(50 == array[i]);
    }
  }

  SUBCASE("Act") {
    // "act":[2,1,2,2]

    JsonArray array = root["act"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());
    REQUIRE(2 == array[0]);
    REQUIRE(1 == array[1]);
    REQUIRE(2 == array[2]);
    REQUIRE(2 == array[3]);
  }

  SUBCASE("Detectors") {
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

  SUBCASE("Alta") {
    // alta:[2,2,2,2]

    JsonArray array = root["alta"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }

  SUBCASE("Altb") {
    // altb:[2,2,2,2]

    JsonArray array = root["altb"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }

  SUBCASE("Measlights") {
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

  SUBCASE("Measlights2") {
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

  SUBCASE("Altc") {
    // altc:[2,2,2,2]

    JsonArray array = root["altc"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }

  SUBCASE("Altd") {
    // altd:[2,2,2,2]

    JsonArray array = root["altd"];
    REQUIRE(array.isNull() == false);

    REQUIRE(4 == array.size());

    for (size_t i = 0; i < 4; i++) {
      REQUIRE(2 == array[i]);
    }
  }
}
