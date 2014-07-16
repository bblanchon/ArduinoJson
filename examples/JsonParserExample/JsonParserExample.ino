/*
* Arduino JSON library - Parser Example
* Benoit Blanchon 2014 - MIT License
*/

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

void setup()
{
    Serial.begin(9600);

    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

    JsonParser<16> parser;

    JsonHashTable root = parser.parseHashTable(json);

    if (!root.success())
    {
        Serial.println("JsonParser.parseHashTable() failed");
        return;
    }

    char* sensor = root["sensor"];
    Serial.println(sensor);

    long time = root["time"];
    Serial.println(time);
    
    JsonArray coords = root["data"];
    
    for (int i = 0; i < coords.size(); i++)
    {
        double value = coords[i];
        Serial.println(value, 6);
    }
}

void loop()
{

}