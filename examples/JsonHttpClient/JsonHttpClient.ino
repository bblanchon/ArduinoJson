// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Example of an HTTP client parsing a JSON response.
//
// This program perform an HTTP GET of arduinojson.org/example.json
// Here is the expected response:
// {
//   "sensor": "gps",
//   "time": 1351824120,
//   "data": [
//     48.756080,
//     2.302038
//   ]
// }
// See http://arduinojson.org/assistant/ to compute the size of the buffer.
//
// Disclaimer: the code emphasize the communication between client and server,
// it doesn't claim to be a reference of good coding practices.

#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  echo("Initialize Ethernet library");
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  Ethernet.begin(mac) || die("Failed to configure Ethernet");
  delay(1000);

  echo("Connect to HTTP server");
  EthernetClient client;
  client.setTimeout(10000);
  client.connect("arduinojson.org", 80) || die("Connection failed");

  echo("Send HTTP request");
  client.println("GET /example.json HTTP/1.0");
  client.println("Host: arduinojson.org");
  client.println("Connection: close");
  client.println() || die("Failed to send request");

  echo("Check HTTP status");
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    echo(status);
    die("Unexpected HTTP response");
  }

  echo("Skip HTTP headers");
  char endOfHeaders[] = "\r\n\r\n";
  client.find(endOfHeaders) || die("Invalid response");

  echo("Allocate JsonBuffer");
  const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

  echo("Parse JSON object");
  JsonObject& root = jsonBuffer.parseObject(client);
  if (!root.success()) die("Parsing failed!");

  echo("Extract values");
  echo(root["sensor"].as<char*>());
  echo(root["time"].as<char*>());
  echo(root["data"][0].as<char*>());
  echo(root["data"][1].as<char*>());

  echo("Disconnect");
  client.stop();
}

void loop() {}

void echo(const char* message) {
  Serial.println(message);
}

bool die(const char* message) {
  Serial.println(message);
  while (true);  // loop forever
  return false;
}