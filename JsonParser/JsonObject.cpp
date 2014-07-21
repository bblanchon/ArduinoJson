/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

DEPRECATED JsonArray JsonObject::getArray(const char* key)
{
    return operator[](key);
}