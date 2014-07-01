/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <JsonGenerator.h>

void setup()
{
    Serial.begin(9600);

    JsonHashTable<3> h;    

    h.add("pi", 3.14);

    JsonArray<3> a;
    a.add(1.0);
    a.add(2.0);
    a.add(3.0);
    h.add("list", a);

    h.add("hellow", "world");

    Serial.print(h);
}

void loop()
{

}
