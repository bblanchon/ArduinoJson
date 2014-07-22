/*
* Arduino JSON library - Parser Example
* Benoit Blanchon 2014 - MIT License
*/

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

void setup()
{
    Serial.begin(9600);

    char json [] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

    JsonParser<16> parser;

    JsonObject root = parser.parse(json);

    if (!root.success())
    {
        Serial.println("JsonParser.parse() failed");
        return;
    }

    char*  sensor    = root["sensor"];
    long   time      = root["time"];
    double latitude  = root["data"][0];
    double longitude = root["data"][1];

    Serial.println(sensor);
    Serial.println(time);
    Serial.println(latitude, 6);
    Serial.println(longitude, 6);
}

void loop()
{

}