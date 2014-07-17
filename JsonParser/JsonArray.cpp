/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace ArduinoJson::Parser;

DEPRECATED JsonHashTable JsonArray::getHashTable(int index)
{
    return (JsonHashTable) (*this)[index];
}
