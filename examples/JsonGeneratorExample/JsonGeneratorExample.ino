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
    array.add<6>(48.756080); // 6 is the number of decimals to print
    array.add<6>(2.302038);  // if not specified, 2 digits are printed

    JsonObject<3> root; 
    root["sensor"] = "gps";
    root["time"] = 1351824120;
    root["data"] = array;

    Serial.print(root); // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

    Serial.println();
    root.prettyPrintTo(Serial); // same string indented
}

void loop()
{

}