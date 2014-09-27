/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonObject.h"

using namespace ArduinoJson::Parser;

DEPRECATED JsonObject JsonArray::getHashTable(int index)
{
    return operator[](index);
}
