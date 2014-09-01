Arduino JSON library - Generator
================================

*An elegant and efficient JSON encoder for embedded systems.*

It's design to have the most intuitive API, the smallest footprint and works without any allocation on the heap (no malloc).

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.


Features
--------

* Elegant API, very easy to use 
* Fixed memory allocation (no malloc)
* Small footprint
* Supports nested objects
* Supports indented output
* Implements Arduino's `Printable interface
* MIT License


Example
-------

    JsonArray<2> array;
    array.add<6>(48.756080);  // <6> specifies the number of digits in the output
    array.add<6>(2.302038);   // (the default is 2)

    JsonObject<3> root;
    root["sensor"] = "gps";
    root["time"] = 1351824120;
    root["data"] = array;

    Serial.print(root); // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}


How to  use?
------------

### 1. Install the library

Download the library and extract it to:

    <your Arduino Sketch folder>/libraries/ArduinoJson

### 2. Import in your sketch

Just add the following lines at the top of your `.ino` file:

    #include <JsonGenerator.h>

    using namespace ArduinoJson::Generator;

> ##### Having a namespace conflict?
> To be able to use both `ArduinoJson::Generator` and `ArduinoJson::Parser` in the same file, you need to do one of the followings:
> 
> * Put the `using` statements into different functions
> * `using namespace ArduinoJson`, then prefix the type names by `Generator::` or `Parser::`
> * Create aliases for the namespaces or the types (C++11 only)
   
### 3. Create object tree

In order to generate a JSON string, you need to build the equivalent object tree. You usually start by the root which can be either a JSON Array or a JSON Object.

#### JSON Array

You create an array with the following line:

    JsonArray<8> array;

See the little `<8>`? It's a template parameter that gives the capacity of the array, it's the maximum number of elements you can put in it. 

> ##### About the capacity
> As stated in the feature list, this library works with a fixed memory allocation.
> This means that the size of the object must be know at the compilation time, therefore you can **not** use a variable to set the capacity of the array.

Then you can add strings, integer, booleans, etc: 

    array.add("bazinga!");
    array.add(42);
    array.add(true);

There are two syntaxes for floating point values:

	array.add<4>(3.1415);  // 4 digits: "3.1415" 
    array.add(3.14);	   // 2 digits: "3.14"

> ##### About floating point precision
> The overload of `add()` with 2 parameters allows you to specify the number of decimals to save in the JSON string. 
> When you use the overload with one parameter, you use the default number of decimals which is two.
> Note that this behavior is the exact same as Arduino's `Print::print(double,int);` which is implemented by `Serial`. 
> So you may already be familiar with it. 

Finally you can add nested object to the array:

    JsonArray<8> nestedArray;
    array.add(nestedArray);

or

    JsonObject<8> nestedObject;
    array.add(nestedObject);

> ##### CAUTION! Nested objects must be in memory
> Calling `add()` makes the `JsonArray` store a pointer to the nested object.
> This is designed to avoid memory duplication.
> But it can only work if the object is in memory when `printTo()` is executed.
> For instance, don't do this:
> 
>     void addNestedObject()
>     {
>         JsonObject<2> nestedObject;
>         // ...
>         array.add(nestedObject); // <- DON'T !!
>         
>         // array now contains a pointer to a local variable that will be
>         // discarded as soon as the function exits 
>     } 
>     
> For the same reason, don't do this either:
> 
>     for( int i=0; i<100; i++)
>     {
>         JsonObject<2> nestedObject;
>         // ...
>         array.add(nestedObject); // <- DON'T !!
>     }  
>     // array now contains 100 pointers to the same a local variable
>     // that is out of the scope anyway

#### JSON Object

You create a JSON object (ie hash-table/dictionary) with the following line:

    JsonObject<8> object;

Like with the array class, there is a template parameter that gives the capacity of the object.

Then you can add strings, integer, booleans, etc: 

    object["key1"] = "bazinga!";
    object["key2"] = 42;
    object["key3"] = true;

As for the arrays, there are two syntaxes for the floating point values:

	object["key4"].set<4>(3.1415);  // 4 digits "3.1415" 
    object["key5"] = 3.1415;	    // default: 2 digits "3.14"

Finally you can add nested objects:

    JsonArray<8> nestedArray;
    object["key6"] = nestedArray;

or

    JsonObject<8> nestedObject;
    object["key7"] = nestedObject;

> ##### Other JsonObject functions
> * `object.add(key, value)` is a synonym for `object[key] = value`
> * `object.containsKey(key)` returns `true` is the `key` is present in `object`
> * `object.remove(key)` removes the `value` associated with `key`

### 4. Get the JSON string

There are two ways tho get the resulting JSON string.

Depending on your project, you may need to dump the string in a classic `char[]` or send it to a stream like `Serial` or `EthernetClient `.

Both ways are the easy way :-)

#### Use a classic `char[]`

Whether you have a `JsonArray` or a `JsonObject`, simply call `printTo()` with the destination buffer, like so:

	char buffer[256];
    array.printTo(buffer, sizeof(buffer));

> ##### Want an indented output?
> By default the generated JSON is as small as possible. It contains no extra space, nor line break.
> But if you want an indented, more readable output, you can.
> Simply call `prettyPrintTo` instead of `printTo()`:
> 
>     array.prettyPrintTo(buffer, sizeof(buffer));

#### Send to a stream

It's very likely that the generated JSON will end up in a stream like `Serial` or `EthernetClient `, so you can save some time and memory by doing this:

	Serial.print(array);

or
  
    array.printTo(Serial);

> ##### About the Printable interface
> `JsonArray` and `JsonObject` implement Arduino's `Printable` interface.
> This is why you can call `Serial.print()` like in the example above.
> You can do the same with any other implementation of `Print`:  `HardwareSerial`,  `SoftwareSerial`, `LiquidCrystal`, `EthernetClient`, `WiFiClient`, `Wire`...
   

Memory usage
------------

Here are the size of the main classes of the library.

This table is for an 8-bit Arduino, types would be bigger on a 32-bit processor.

| Type                | Size in bytes |
| --------------------| ------------- |
| JsonArray&lt;N&gt;  | 8 + 6 x N     |
| JsonObject&lt;N&gt; | 8 + 8 x N     |


Code size
---------

The following values has been obtained with Arduino IDE 1.0.5, targeting an Arduino Duelmilanove with an ATmega 328.

### Minimum setup

| Function                            | Size |
| ----------------------------------- | ---- |
| `JsonObjectBase::printTo()`         | 234  |
| `EscapedString::printTo()`          | 196  |
| `JsonArrayBase::printTo()`          | 164  |
| `Print::print(char const*)`         | 146  |
| `JsonObjectBase::operator[]`        | 114  |
| `JsonObjectBase::getMatchingPair()` | 72   |
| `JsonValue::printPrintableTo()`     | 40   |
| `JsonValue::printStringTo()`        | 12   |

### Additional space for integers

| Function                     | Size |
| ---------------------------- | ---- |
| `Print::print(long, int)`    | 328  |
| `JsonValue::printLongTo()`   | 22   |

### Additional space for floating point

| Function                       | Size |
| ------------------------------ | ---- |
| `Print::print(double, int)`    | 1548 |
| `JsonValue::printDouleTo<2>()` | 22   |