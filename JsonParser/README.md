An efficient JSON parser for Arduino
====================================

This library is an thin C++ wrapper around the *jsmn* tokenizer: http://zserge.com/jsmn.html

It's design to be very lightweight, works without any allocation on the heap (no malloc) and supports nested objects.

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library on any other C++ project.


Features
--------

* Based on the well-proven [jsmn](http://zserge.com/jsmn.html) tokenizer
* Supports nested objects
* Works with fixed memory allocation : no `malloc()`
* Low footprint
* MIT License


Example
-------

    char json[] = "{\"Name\":\"Blanchon\",\"Skills\":[\"C\",\"C++\",\"C#\"],\"Age\":32,\"Online\":true}";

    JsonParser<32> parser;

    JsonHashTable hashTable = parser.parseHashTable(json);

    if (!hashTable.success())
    {
        return;
    }

    char* name = hashTable.getString("Name");

    JsonArray skills = hashTable.getArray("Skills");

    int age = hashTable.getLong("Age");

    bool online = hashTable.getBool("Online");



How to  use ?
-------------

### 1. Install the library

Download the library and extract it to:

    <your Arduino Sketch folder>/libraries/ArduinoJsonParser

### 2. Import in your sketch

Just add the following line on the top of your `.ino` file:

    #include <JsonParser.h>
    
### 3. Create a parser

To extract data from the JSON string, you need to create a `JsonParser`, and specify the number of token you allocate for the parser itself:

    JsonParser<32> parser;
    
> #### How to choose the number of tokens ?

> A token is an element of the JSON object: either a key, a value, an hash-table or an array.
> As an example the `char json[]` on the top of this page contains 12 tokens (don't forget to count 1 for the whole object and 1 more for the array itself).

> The more tokens you allocate, the more complex the JSON can be, but also the more memory is occupied.
> Each token takes 8 bytes, so `sizeof(JsonParser<32>)` is 256 bytes which is quite big in an Arduino with only 2KB of RAM.
> Don't forget that you also have to store the JSON string in RAM and it's probably big.

> 32 tokens may seem small, but it's very decent for an 8-bit processor, you wouldn't get better results with other JSON libraries.

### 4. Extract data

To use this library, you need to know beforehand what is the type of data contained in the JSON string, which is very likely.

The root object has to be either a hash-table (like `{"key":"value"}`) or an array (like `[1,2]`). 

The nested objects can be either arrays, booleans, hash-tables, numbers or strings.
If you need other type, you can get the string value and parse it yourself.

#### Hash-table

Consider we have a `char json[]` containing to the following JSON string:

    {
        "Name":"Blanchon",
        "Skills":[
            "C",
            "C++",
            "C#"],
        "Age":32,
        "Online":true
    }

In this case the root object of the JSON string is a hash-table, so you need to extract a `JsonHashTable`:
   
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

Consider we have a `char json[]` containing to the following JSON string:

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


Common pitfalls
---------------

### 1. Not enough tokens

By design, the library has no way to tell you why `JsonParser::parseArray()` or `JsonParser::parseHashTable()` failed.

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

To reduce the memory consumption, `JsonArray` and `JsonHashTable` contains pointer to the token that are inside the `JsonParser`. This can only work if the `JsonParser` is still in memory.

For example, don't do this:

    JsonArray getArray(char* json)
    {
        JsonParser<16> parser;
        return parser.parseArray(parser); 
    }

because the local variable `parser` will be *removed* from memory when the function `getArray()` returns, and the pointer inside `JsonArray` will point to an invalid location.

### 4. JSON string is altered

This will probably never be an issue, but you need to be aware of this feature.

When you pass a `char[]` to `JsonParser::parseArray()` or `JsonParser::parseHashTable()`, the content of the string will be altered to add `\0` at the end of the tokens.

This is because we want functions like `JsonArray::getString()` to return a null-terminating string without any memory allocation.
   

Memory usage
------------

Here are the size of the main classes of the library.

This table is for an 8-bit Arduino, types would be bigger on a 32-bit processor.

<table>
    <tr>
        <th>Type</th>
        <th>Size in bytes</th>
    </tr>
    <tr>
        <td>Parser&lt;N&gt;</td>
        <td>8 x N</td>
    </tr>
    <tr>
        <td>JsonArray</td>
        <td>4</td>
    </tr>
    <tr>
        <td>JsonHashTable</td>
        <td>4</td>
    </tr>
</table> 


Code size
---------

Theses tables has been created by analyzing the map file generated by AVR-GCC after adding `-Wl,-Map,foo.map` to the command line.

As you'll see the code size if between 1680 and 3528 bytes, depending on the features you use.

### Minimum setup

<table>
    <tr>
        <th>Function</th>
        <th>Size in bytes</th>
    </tr>
    <tr>
        <td>strcmp(char*,char*)</td>
        <td>18</td>
    </tr>
    <tr>
        <td>jsmn_init(jsmn_parser*)</td>
        <td>20</td>
    </tr>
    <tr>
        <td>jsmn_parse(jsmn_parser*, char const*, jsmntok_t*, unsigned int)</td>
        <td>960</td>
    </tr>
    <tr>
        <td>JsonParser::parse(char*)</td>
        <td>106</td>
    </tr>
    <tr>
        <td>JsonObjectBase::getNestedTokenCount(jsmntok_t*)</td>
        <td>84</td>        
    </tr>
    <tr>
        <td>JsonObjectBase::getStringFromToken(jsmntok_t*)</td>
        <td>68</td>        
    </tr>
    <tr>
        <td>JsonArray::JsonArray(char*, jsmntok_t*)</td>
        <td>42</td>        
    </tr>
    <tr>
        <td>JsonArray::getToken(int)</td>
        <td>112</td>        
    </tr>
    <tr>
        <td>JsonArray::getString(int)</td>
        <td>18</td>
    </tr>
    <tr>
        <td>JsonHashTable::JsonHashTable(char*, jsmntok_t*)</td>
        <td>42</td>        
    </tr>
    <tr>
        <td>JsonHashTable::getToken(char*)</td>
        <td>180</td>        
    </tr>
    <tr>
        <td>JsonHashTable::getString(char*)</td>
        <td>18</td>
    </tr>
    <tr>
        <td>TOTAL</td>
        <td>1680</td>
    </tr>
</table>

### Additional space to parse nested  objects

<table>
    <tr>
        <th>Function</th>
        <th>Size in bytes</th>
    </tr>
    <tr>
        <td>JsonArray::getArray(int)</td>
        <td>42</td>
    </tr>    
    <tr>
        <td>JsonArray::getHashTable(int)</td>
        <td>64</td>        
    </tr>
    <tr>
        <td>JsonHashTable::getArray(char*)</td>
        <td>64</td>
    </tr>
    <tr>
        <td>JsonHashTable::getHashTable(char*)</td>
        <td>42</td>
    </tr>
    <tr>
        <td>TOTAL</td>
        <td>212</td>
    </tr>
</table>

### Additional space to parse `bool` values

<table>
    <tr>
        <th>Function</th>
        <th>Size in bytes</th>
    </tr>
    <tr>
        <td>JsonObjectBase::getBoolFromToken(jsmntok_t*)</td>
        <td>82</td>
    </tr>    
    <tr>
        <td>JsonArray::getBool(int)</td>
        <td>18</td>        
    </tr>
    <tr>
        <td>JsonHashTable::getBool(char*)</td>
        <td>18</td>
    </tr>
    <tr>
        <td>TOTAL</td>
        <td>130</td>
    </tr>
</table>

### Additional space to parse `double` values

<table>
    <tr>
        <th>Function</th>
        <th>Size in bytes</th>
    </tr>
    <tr>
        <td>strtod(char*,int)</td>
        <td>704</td>
    </tr>    
    <tr>
        <td>JsonObjectBase::getDoubleFromToken(jsmntok_t*)</td>
        <td>44</td>
    </tr>    
    <tr>
        <td>JsonArray::getDouble(int)</td>
        <td>18</td>        
    </tr>
    <tr>
        <td>JsonHashTable::getDouble(char*)</td>
        <td>18</td>
    </tr>
    <tr>
        <td>TOTAL</td>
        <td>796</td>
    </tr>
</table>

### Additional space to parse `long` values

<table>
    <tr>
        <th>Function</th>
        <th>Size in bytes</th>
    </tr>
    <tr>
        <td>strtol(char*,char**,int)</td>
        <td>606</td>
    </tr>    
    <tr>
        <td>JsonObjectBase::getLongFromToken(jsmntok_t*)</td>
        <td>56</td>
    </tr>    
    <tr>
        <td>JsonArray::getLong(int)</td>
        <td>18</td>        
    </tr>
    <tr>
        <td>JsonHashTable::getLong(char*)</td>
        <td>18</td>
    </tr>
    <tr>
        <td>TOTAL</td>
        <td>710</td>
    </tr>
</table>


Links
-----

* [The project for which I made me this library](http://blog.benoitblanchon.fr/rfid-payment-terminal/)
* [Blog post on the motivation for this library](http://blog.benoitblanchon.fr/arduino-json-parser/)