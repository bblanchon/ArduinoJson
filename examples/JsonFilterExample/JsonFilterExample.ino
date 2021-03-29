// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License
//
// This example shows how to use DeserializationOpion::Filter
//
// https://arduinojson.org/v6/example/filter/

#include <ArduinoJson.h>

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // The huge input: an extract from OpenWeatherMap response
  const __FlashStringHelper* input_json = F(
      "{\"cod\":\"200\",\"message\":0,\"list\":[{\"dt\":1581498000,\"main\":{"
      "\"temp\":3.23,\"feels_like\":-3.63,\"temp_min\":3.23,\"temp_max\":4.62,"
      "\"pressure\":1014,\"sea_level\":1014,\"grnd_level\":1010,\"humidity\":"
      "58,\"temp_kf\":-1.39},\"weather\":[{\"id\":800,\"main\":\"Clear\","
      "\"description\":\"clear "
      "sky\",\"icon\":\"01d\"}],\"clouds\":{\"all\":0},\"wind\":{\"speed\":6."
      "19,\"deg\":266},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2020-02-12 "
      "09:00:00\"},{\"dt\":1581508800,\"main\":{\"temp\":6.09,\"feels_like\":-"
      "1.07,\"temp_min\":6.09,\"temp_max\":7.13,\"pressure\":1015,\"sea_"
      "level\":1015,\"grnd_level\":1011,\"humidity\":48,\"temp_kf\":-1.04},"
      "\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear "
      "sky\",\"icon\":\"01d\"}],\"clouds\":{\"all\":9},\"wind\":{\"speed\":6."
      "64,\"deg\":268},\"sys\":{\"pod\":\"d\"},\"dt_txt\":\"2020-02-12 "
      "12:00:00\"}],\"city\":{\"id\":2643743,\"name\":\"London\",\"coord\":{"
      "\"lat\":51.5085,\"lon\":-0.1257},\"country\":\"GB\",\"population\":"
      "1000000,\"timezone\":0,\"sunrise\":1581492085,\"sunset\":1581527294}}");

  // The filter: it contains "true" for each value we want to keep
  StaticJsonDocument<200> filter;
  filter["list"][0]["dt"] = true;
  filter["list"][0]["main"]["temp"] = true;

  // Deserialize the document
  StaticJsonDocument<400> doc;
  deserializeJson(doc, input_json, DeserializationOption::Filter(filter));

  // Print the result
  serializeJsonPretty(doc, Serial);
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any
// deserialization problem.
//
// The book "Mastering ArduinoJson" contains a tutorial on deserialization.
// It begins with a simple example, like the one above, and then adds more
// features like deserializing directly from a file or an HTTP request.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
