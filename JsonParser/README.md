Arduino JSON library - Parser
=============================

This library is an thin C++ wrapper around the *jsmn* tokenizer: http://zserge.com/jsmn.html

It's design to be very lightweight, works without any allocation on the heap (no malloc) and supports nested objects.

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.


Features
--------

* Based on the well-proven [jsmn](http://zserge.com/jsmn.html) tokenizer
* Supports nested objects
* Elegant API, very easy to use 
* Fixed memory allocation (no malloc)
* Small footprint
* MIT License


Example
-------

	JsonParser<32> parser;
    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";   

    JsonObject root = parser.parse(json);

    char*  sensor    = root["sensor"];
    long   time      = root["time"];
    double latitude  = root["data"][0];
    double longitude = root["data"][1];


How to  use ?
-------------

### 1. Install the library

Download the library and extract it to:

    <your Arduino Sketch folder>/libraries/ArduinoJson

### 2. Import in your sketch

Just add the following lines at the top of your `.ino` file:

    #include <JsonParser.h>

    using namespace ArduinoJson::Parser;

> ##### Having a namespace conflict?
> To be able to use both `ArduinoJson::Generator` and `ArduinoJson::Parser` in the same file, you need to do one of the followings:
> 
> * Put the `using` statements into different functions
> * `using namespace ArduinoJson`, then prefix the type names by `Generator::` or `Parser::`
> * Create aliases for the namespaces or the types (C++11 only)
    
### 3. Create a parser

To extract data from the JSON string, you need to create a `JsonParser`, and specify the number of token you allocate for the parser itself:

    JsonParser<32> parser;
    
> #### How to choose the number of tokens ?

> A token is an element of the JSON object: either a key, a value, an object or an array.
> As an example the `char json[]` on the top of this page contains 9 tokens (don't forget to count 1 for the whole object and 1 more for the array itself).

> The more tokens you allocate, the more complex the JSON can be, but also the more memory is occupied.
> Each token takes 8 bytes, so `sizeof(JsonParser<32>)` is 256 bytes which is quite big in an Arduino with only 2KB of RAM.
> Don't forget that you also have to store the JSON string in RAM and it's probably big.

> 32 tokens may seem small, but it's very decent for an 8-bit processor, you wouldn't get better results with other JSON libraries.

### 4. Extract data

To use this library, you need to know beforehand what is the type of data contained in the JSON string, which is very likely.

The root object has to be either an object (like `{"key":"value"}`) or an array (like `[1,2]`). 

The nested objects can be either arrays, booleans, objects, numbers or strings.
If you need other type, you can get the string value and parse it yourself.

#### Object

Consider we have a `char json[]` containing to the following JSON string:

    {
        "sensor":"gps",
		"time":1351824120,
		"data":[48.756080,2.302038]
    }

In this case the string contains a JSON object, so you need to extract a `JsonObject`:
   
    JsonObject root = parser.parse(json);
    
To check if the parsing was successful, you must check:

    if (!root.success())
    {
        // Parsing fail: could be an invalid JSON, or too many tokens
    }
    
And then extract the member you need:
    
    char*  sensor    = root["sensor"];
    long   time      = root["time"];
    double latitude  = root["data"][0];
    double longitude = root["data"][1];

You can also iterate through the key-value pairs of the object:

    for (JsonObjectIterator i=root.begin(); i!=root.end(); ++i)
    {
      Serial.println(i.key());
      Serial.println((char*)i.value());
    }
    
#### Array

Consider we have a `char json[]` containing to the following JSON string:

    [
        [ 1.2, 3.4 ],
        [ 5.6, 7.8 ]               
    ]

In this case the root object of the JSON string is an array, so you need to extract a `JsonArray`:
   
    JsonArray root = parser.parse(json);
    
To check if the parsing was successful, you must check:

    if (!root.success())
    {
        // Parsing fail: could be an invalid JSON, or too many tokens
    }
    
And then extract the content by its index in the array:
    
    double a = root[0][0];
	double b = root[0][1];
	double c = root[1][0];
	double d = root[1][1];

You can also iterate through the key-value pairs of the object:

    for (JsonArrayIterator i=array.begin(); i!=array.end(); ++i)
    {
      Serial.println((char*)*i);
    }
	
Common pitfalls
---------------

### 1. Not enough tokens

By design, the library has no way to tell you why `JsonParser::parse()` failed.

There are basically two reasons why they may fail:

1. the JSON string is invalid
2. the JSON string contains more tokens that the parser can store

So, if you are sure the JSON string is correct and you still can't parse it, you should slightly increase the number of token of the parser.

### 2. Not enough memory

You may go into unpredictable trouble if you allocate more memory than your processor really has.
It's a very common issue in embedded development. 

To diagnose this, look at every big objects in you code and sum their size to check that they fit in RAM.

For example, don't do this:

    char json[1024];        // 1 KB
    JsonParser<64> parser;    // 512 B

because it may be too big for a processor with only 2 KB: you need free memory to store other variables and the call stack.

That is why an 8-bit processor is not able to parse long and complex JSON strings.

### 3. JsonParser not in memory

To reduce the memory consumption, `JsonValue`, `JsonArray` and `JsonObject` contains pointer to the token that are inside the `JsonParser`. This can only work if the `JsonParser` is still in memory.

For example, don't do this:

    JsonArray getArray(char* json)
    {
        JsonParser<16> parser;
        return parser.parseArray(parser); 
    }

because the local variable `parser` will be *removed* from memory when the function `getArray()` returns, and the pointer inside `JsonArray` will point to an invalid location.

### 4. JSON string is altered

This will probably never be an issue, but you need to be aware of this feature.

When you pass a `char[]` to `JsonParser::parse()`, the content of the string will be altered to add `\0` at the end of the tokens.

This is because we want functions like `JsonArray::getString()` to return a null-terminating string without any memory allocation.
   

Memory usage
------------

Here are the size of the main classes of the library.

This table is for an 8-bit Arduino, types would be bigger on a 32-bit processor.

| Type         | Size in bytes |
| ------------ | ------------- |
| `Parser<N>`  | 4 + 8 x N     |
| `JsonArray`  | 4             |
| `JsonObject` | 4             |
| `JsonValue`  | 4             |

Code size
---------

The sizes have been obtained with Arduino IDE 1.0.5 for a Duemilanove.

### Minimum setup

| Function                             | Size |
| ------------------------------------ | ---- |
| `jsmn_parse()`                       | 962  |
| `JsonValue::operator[](char const*)` | 218  | 
| `JsonParserBase::parse()`            | 116  | 
| `JsonValue::operator[](int)`         | 108  |
| `strcmp()`                           | 18   | 

### Additional space for integers

| Function                     | Size |
| ---------------------------- | ---- |
| `strtol()`                   | 606  |
| `JsonValue::operator long()` | 94   |

### Additional space for floating points

| Function                         | Size  |
| -------------------------------- | ----- |
| `strtod()`                       | 1369  |
| `JsonValue::operator double()`   | 82    |
