/*
 * Arduino JSON library - Generator example
 * Benoit Blanchon 2014 - MIT License
 */

#include <JsonGenerator.h>

using namespace ArduinoJson::Generator;

void setup()
{
    Serial.begin(9600);

    JsonArray<2> array;
    array.add(48.756080, 6);
    array.add(2.302038, 6);

    JsonHashTable<3> root; 
    root.add("sensor", "gps");
    root.add("time", 1351824120);
    root.add("data", array);

    Serial.print(root); // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
}

void loop()
{

}