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

TEST(IntegrationTests, OpenWeatherMap) {
  StaticJsonBuffer<2048> json;
  const char* input =
      "{\"coord\":{\"lon\":-0.13,\"lat\":51.51},\"sys\":{\"type\":1,\"id\":"
      "5093,\"message\":0.1177,\"country\":\"GB\",\"sunrise\":1414824893,"
      "\"sunset\":1414859596},\"weather\":[{\"id\":802,\"main\":\"Clouds\","
      "\"description\":\"scattered clouds\",\"icon\":\"03d\"}],\"base\":\"cmc "
      "stations\",\"main\":{\"temp\":288.62,\"pressure\":1015,\"humidity\":87,"
      "\"temp_min\":287.15,\"temp_max\":290.15},\"wind\":{\"speed\":4.6,"
      "\"deg\":240},\"clouds\":{\"all\":40},\"dt\":1414825810,\"id\":2643743,"
      "\"name\":\"London\",\"cod\":200}";

  char inputBuffer[1024];

  strcpy(inputBuffer, input);

  JsonObject& o1 = json.parseObject(inputBuffer);

  char outputBuffer[1024];

  o1.printTo(outputBuffer, sizeof(outputBuffer));

  ASSERT_STREQ(input, outputBuffer);
}