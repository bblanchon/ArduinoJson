/*
* malloc-free JSON parser for Arduino
* Benoit Blanchon 2014 - MIT License
*/

#include <JsonParser.h>

void ParseAnObject()
{
    char json[] = "{\"Name\":\"Blanchon\",\"Skills\":[\"C\",\"C++\",\"C#\"],\"Age\":32,\"Online\":true}";

    JsonParser<32> parser;

    Serial.print("Parse ");
    Serial.println(json);

    JsonHashTable hashTable = parser.parseHashTable(json);

    if (!hashTable.success())
    {
        Serial.println("JsonParser.parseHashTable() failed");
        return;
    }

    char* name = hashTable.getString("Name");
    Serial.print("name=");
    Serial.println(name);

    JsonArray skills = hashTable.getArray("Skills");
    Serial.println("skills:");
    for (int i = 0; i < skills.getLength(); i++)
    {
        char* value = skills.getString(i);
        Serial.print(i);
        Serial.print(" ");
        Serial.println(value);
    }

    int age = hashTable.getLong("Age");
    Serial.print("age=");
    Serial.println(age);

    bool online = hashTable.getBool("Online");
    Serial.print("online=");
    Serial.println(online);
}

void ParseAnArray()
{
    char json[] = "[[1.2,3.4],[5.6,7.8]]";

    JsonParser<32> parser;

    Serial.print("Parse ");
    Serial.println(json);

    JsonArray array = parser.parseArray(json);

    if (!array.success())
    {
        Serial.println("JsonParser.parseArray() failed");
        return;
    }

    for (int i = 0; i < array.getLength(); i++)
    {
        Serial.println(i);

        JsonArray innerArray = array.getArray(i);

        for (int j = 0; j < innerArray.getLength(); j++)
        {
            double value = innerArray.getDouble(j);

            Serial.print("  ");
            Serial.print(j);
            Serial.print("=");
            Serial.println(value);
        }
    }
}

void setup()
{
    Serial.begin(9600);

    ParseAnObject();
    ParseAnArray();
}

void loop()
{

}
