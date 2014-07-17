/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonHashTable.h"
#include "JsonArray.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

DEPRECATED JsonArray JsonHashTable::getArray(const char* key)
{
    return (JsonArray) (*this)[key];
}