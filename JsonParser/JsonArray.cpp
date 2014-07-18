/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonArray.h"
#include "JsonObject.h"

using namespace ArduinoJson::Parser;
using namespace ArduinoJson::Internal;

DEPRECATED JsonObject JsonArray::getHashTable(int index)
{
    return (JsonObject) (*this)[index];
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
    JsonToken runningToken = token.firstChild();

    // skip all tokens before the specified index
    for (int i = 0; i < index; i++)
    {
        // move forward: current + nested tokens
        runningToken = runningToken.nextSibling();
    }

    return JsonValue(json, runningToken);
}