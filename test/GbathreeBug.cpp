// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>
#include "Printers.hpp"

class GbathreeBug : public testing::Test {
 public:
  GbathreeBug() : object(buffer.parseObject(getJson())) {}

 protected:
  char json[1024];
  StaticJsonBuffer<10000> buffer;
  const JsonObject& object;

 private:
  char* getJson() {
    strcpy(json,
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
    return json;
  }
};

TEST_F(GbathreeBug, Success) { EXPECT_TRUE(object.success()); }

TEST_F(GbathreeBug, ProtocolName) {
  EXPECT_STREQ("fluorescence", object.at("protocol_name").asString());
}

TEST_F(GbathreeBug, Repeats) { EXPECT_EQ(1, object["repeats"]); }

TEST_F(GbathreeBug, Wait) { EXPECT_EQ(0, object["wait"]); }

TEST_F(GbathreeBug, Measurements) { EXPECT_EQ(3, object["measurements"]); }

TEST_F(GbathreeBug, Meas2_Light) { EXPECT_EQ(15, object["meas2_light"]); }

TEST_F(GbathreeBug, Meas1_Baseline) { EXPECT_EQ(0, object["meas1_baseline"]); }

TEST_F(GbathreeBug, Act_Light) { EXPECT_EQ(20, object["act_light"]); }

TEST_F(GbathreeBug, Pulsesize) { EXPECT_EQ(25, object["pulsesize"]); }

TEST_F(GbathreeBug, Pulsedistance) {
  EXPECT_EQ(10000, object["pulsedistance"]);
}

TEST_F(GbathreeBug, Actintensity1) { EXPECT_EQ(50, object["actintensity1"]); }

TEST_F(GbathreeBug, Actintensity2) { EXPECT_EQ(255, object["actintensity2"]); }

TEST_F(GbathreeBug, Measintensity) { EXPECT_EQ(255, object["measintensity"]); }

TEST_F(GbathreeBug, Calintensity) { EXPECT_EQ(255, object["calintensity"]); }

TEST_F(GbathreeBug, Pulses) {
  // "pulses":[50,50,50]

  JsonArray& array = object.at("pulses");
  EXPECT_TRUE(array.success());

  EXPECT_EQ(3, array.size());

  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(50, array[i]);
  }
}

TEST_F(GbathreeBug, Act) {
  // "act":[2,1,2,2]

  JsonArray& array = object.at("act");
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());
  EXPECT_EQ(2, array[0]);
  EXPECT_EQ(1, array[1]);
  EXPECT_EQ(2, array[2]);
  EXPECT_EQ(2, array[3]);
}

TEST_F(GbathreeBug, Detectors) {
  // "detectors":[[34,34,34,34],[34,34,34,34],[34,34,34,34],[34,34,34,34]]

  JsonArray& array = object.at("detectors");
  EXPECT_TRUE(array.success());
  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    JsonArray& nestedArray = array[i];
    EXPECT_EQ(4, nestedArray.size());

    for (int j = 0; j < 4; j++) EXPECT_EQ(34, nestedArray[j]);
  }
}

TEST_F(GbathreeBug, Alta) {
  // alta:[2,2,2,2]

  JsonArray& array = object.at("alta");
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}

TEST_F(GbathreeBug, Altb) {
  // altb:[2,2,2,2]

  JsonArray& array = object.at("altb");
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}

TEST_F(GbathreeBug, Measlights) {
  // "measlights":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

  JsonArray& array = object.at("measlights");
  EXPECT_TRUE(array.success());
  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    JsonArray& nestedArray = array[i];

    EXPECT_EQ(4, nestedArray.size());

    for (int j = 0; j < 4; j++) EXPECT_EQ(15, nestedArray[j]);
  }
}

TEST_F(GbathreeBug, Measlights2) {
  // "measlights2":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

  JsonArray& array = object.at("measlights2");
  EXPECT_TRUE(array.success());
  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    JsonArray& nestedArray = array[i];
    EXPECT_EQ(4, nestedArray.size());

    for (int j = 0; j < 4; j++) EXPECT_EQ(15, nestedArray[j]);
  }
}

TEST_F(GbathreeBug, Altc) {
  // altc:[2,2,2,2]

  JsonArray& array = object.at("altc");
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}

TEST_F(GbathreeBug, Altd) {
  // altd:[2,2,2,2]

  JsonArray& array = object.at("altd");
  EXPECT_TRUE(array.success());

  EXPECT_EQ(4, array.size());

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(2, array[i]);
  }
}
