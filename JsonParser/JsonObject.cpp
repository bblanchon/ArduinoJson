/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include <string.h> // for strcmp()
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

DEPRECATED JsonArray JsonObject::getArray(const char* key)
{
    return operator[](key);
}

/*
* Returns the token for the value associated with the specified key
*/
JsonValue JsonObject::operator[](const char* desiredKey)
{
    // sanity check
    if (desiredKey == 0 || !isObject())
        return null();

    // skip first token, it's the whole object
    JsonToken runningToken = firstChild();

    // scan each keys
    for (int i = 0; i < childrenCount() / 2; i++)
    {
        // get 'key' token string
        char* key = runningToken.getText();

        // move to the 'value' token
        runningToken = runningToken.nextSibling();

        // compare with desired name
        if (strcmp(desiredKey, key) == 0)
        {
            // return the value token that follows the key token
            return runningToken;
        }

        // skip nested tokens
        runningToken = runningToken.nextSibling();
    }

    // nothing found, return NULL
    return null();
}