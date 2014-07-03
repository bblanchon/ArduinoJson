/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <JsonGenerator.h>

void setup()
{
    Serial.begin(9600);

    JsonHashTable<3> h;    

    h.add("pi", 3.14159265358979323846, 4);

    JsonArray<3> a;
    a.add(1);
    a.add(2);
    a.add(3);
    h.add("list", a);

    h.add("hello", "world");

    Serial.print(h);
}

void loop()
{

}
