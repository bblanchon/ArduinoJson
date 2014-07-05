/*
 * Arduino JSON library - Generator example
 * Benoit Blanchon 2014 - MIT License
 */

#include <JsonGenerator.h>

using namespace ArduinoJson::Generator;

void setup()
{
    Serial.begin(9600);

    JsonHashTable<3> json;

    json.add("sensor", "gps");
    json.add("time", 1351824120);

    JsonArray<3> array;
    array.add(48.756080, 6);
    array.add(2.302038, 6);
    json.add("data", array);

    Serial.print(json); // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
}

void loop()
{

}