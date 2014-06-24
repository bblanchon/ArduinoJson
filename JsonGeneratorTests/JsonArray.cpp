/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include <string.h>

#include "JsonArray.h"

void JsonArray::writeTo(char* buffer, size_t bufferSize)
{
    strncpy(buffer, "[]", bufferSize);

}