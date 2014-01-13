# A malloc-free JSON parser for Arduino

The library is an convenient and efficient wrapper around the *jsmn* tokenizer: http://zserge.com/jsmn.html

It works without any allocation on the heap (no malloc) and supports nested objects.

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library on any other C project.

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

## How to  use ?

### 1. Install the the library

Download the library and extract it to:

    <your Arduino Sketch folder>/libraries/ArduinoJonsParser

### 2. Import in your sketch

Just add the following line on the to of your `.ino` file:

    #include <JonsParser.h>
    
### 3. Create a parser

To extract data from the JSON string, you need to instanciate a `JsonParser`, and specify the number of byte your allocate for the parser itself:

    JsonParser<128> parser;
    
#### How to choose the size ?

The more bytes you give to the parser, the more complex the JSON can be, so if you have free space on your stack you should increase the size of the parser.
Sizes from 128 to 256 are usually good.

Behind the scenes, all these bytes are uses to store *jsmn* tokens.
A token is 8 bytes long, so 128 to 256, allows to parse from 16 to 32 tokens.
As an example the `char* json` on the top of this page requires 12 tokens.

### 4. Extract data

To use this library, you need to know beforehand what is the type of data contained in the JSON string, which is extremely likely.

#### Hash table

Consider we have a `char* json` pointing to the following JSON string:

    {
        "Name":"Blanchon",
        "Skills":[
            "C",
            "C++",
            "C#"],
        "Age":32,
        "Online":true
    }

In this case the root object of the JSON string is a hash table, so you need to extract a `JsonHashTable`:
   
    JsonHashTable root = parser.parseHashTable(json);
    
To check if the parsing was successful, you must check:

   if (!root.success())
   {
       // Parsing fail: could be an invalid JSON, or too many tokens
   }
    
And then extract the member you need:
    
    char* name = hashTable.getString("Name");

    JsonArray skills = hashTable.getArray("Skills");

    int age = hashTable.getLong("Age");

    bool online = hashTable.getBool("Online");
    
#### Array

Consider we have a `char* json` pointing to the following JSON string:

    [
        [ 1.2, 3.4 ],
        [ 5.6, 7.8 ]               
    ]

In this case the root object of the JSON string is an array, so you need to extract a `JsonArray`:
   
    JsonArray root = parser.parseArray(json);
    
To check if the parsing was successful, you must check:

   if (!root.success())
   {
       // Parsing fail: could be an invalid JSON, or too many tokens
   }
    
And then extract the content by its index in the array:
    
    JsonArray row0 = root.getArray(0);
    double a = row0.getDouble(0);
    
or simply:

    double a = root.getArray(0).getDouble(0);
    