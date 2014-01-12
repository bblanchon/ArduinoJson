# A malloc-free JSON parser for Arduino

The library is an convenient and efficient wrapper around the *jsmn* tokenizer: http://zserge.com/jsmn.html

It works without any allocation on the heap (no malloc) and supports nested objects.

## Example

    char* json = "{\"Name\":\"Blanchon\",\"Skills\":[\"C\",\"C++\",\"C#\"],\"Age\":32,\"Online\":true}";

    JsonParser<256> parser;

    JsonHashTable hashTable = parser.parseHashTable(json);

    if (!hashTable.success())
    {
        return;
    }

    char* name = hashTable.getString("Name");

    JsonArray skills = hashTable.getArray("Skills");

    int age = hashTable.getLong("Age");

    bool online = hashTable.getBool("Online");

