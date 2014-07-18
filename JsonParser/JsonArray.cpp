/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace ArduinoJson::Parser;
using namespace ArduinoJson::Internal;

DEPRECATED JsonHashTable JsonArray::getHashTable(int index)
{
    return (JsonHashTable) (*this)[index];
}

/*
* Returns the token for the value at the specified index
*/
JsonValue JsonArray::operator[](int index)
{
    // sanity check
    if (index < 0 || !token.isArray() || index >= token.size())
        return JsonValue::null();

    // skip first token, it's the whole object
    JsonToken currentToken = token + 1;

    // skip all tokens before the specified index
    for (int i = 0; i < index; i++)
    {
        // move forward: current + nested tokens
        currentToken += 1 + currentToken.nestedTokenCount();
    }

    return JsonValue(json, currentToken);
}