// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonArray.hpp>
#include <ArduinoJson/JsonObject.hpp>

using namespace ArduinoJson;

class IntegrationTests : public testing::TestWithParam<const char*> {
 protected:
  virtual void SetUp() {
    _input = GetParam();
    strcpy(inputBuffer, _input);
  }

  const char* _input;
  char inputBuffer[2048];
  char outputBuffer[2048];
  char intermediateBuffer[2048];
  StaticJsonBuffer<2048> json;
};

TEST_P(IntegrationTests, ParseThenPrint) {
  json.parseObject(inputBuffer).printTo(outputBuffer, sizeof(outputBuffer));
  ASSERT_STREQ(_input, outputBuffer);
}

TEST_P(IntegrationTests, ParseThenPrettyPrintThenParseThenPrint) {
  json.parseObject(inputBuffer)
      .prettyPrintTo(intermediateBuffer, sizeof(intermediateBuffer));
  json.parseObject(intermediateBuffer)
      .printTo(outputBuffer, sizeof(outputBuffer));
  ASSERT_STREQ(_input, outputBuffer);
}

INSTANTIATE_TEST_CASE_P(
    OpenWeatherMap, IntegrationTests,
    testing::Values(
        "{\"coord\":{\"lon\":-0.13,\"lat\":51.51},\"sys\":{\"type\":1,\"id\":"
        "5093,\"message\":0.1177,\"country\":\"GB\",\"sunrise\":1414824893,"
        "\"sunset\":1414859596},\"weather\":[{\"id\":802,\"main\":\"Clouds\","
        "\"description\":\"scattered "
        "clouds\",\"icon\":\"03d\"}],\"base\":\"cmc "
        "stations\",\"main\":{\"temp\":288.62,\"pressure\":1015,\"humidity\":"
        "87,\"temp_min\":287.15,\"temp_max\":290.15},\"wind\":{\"speed\":4.6,"
        "\"deg\":240},\"clouds\":{\"all\":40},\"dt\":1414825810,\"id\":2643743,"
        "\"name\":\"London\",\"cod\":200}"));