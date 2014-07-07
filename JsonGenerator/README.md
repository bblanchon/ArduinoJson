Arduino JSON library - Generator
================================

This library is a simple JSON encoder for embedded systems.

It's design to be very lightweight, works without any allocation on the heap (no malloc) and supports nested objects.

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.


Features
--------

* Supports nested objects
* Elegant API, very easy to use 
* Fixed memory allocation (no malloc)
* Small footprint
* Implements Arduino's Printable interface
* MIT License


Example
-------

    JsonArray<2> array;
    array.add<6>(48.756080);
    array.add<6>(2.302038);

    JsonHashTable<3> root;
    root.add("sensor", "gps");
    root.add("time", 1351824120);
    root.add("data", array);

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
   
### 3. Create object tree

In order to generate a JSON string, you need to build the equivalent object tree. You usually start by the root which can be either an array or an hash-table.

#### Array

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

There are two syntaxes for the floating point values:

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

    JsonHashTable<8> nestedHash;
    array.add(nestedHash);

#### Hash-table

You create a hash-table with the following line:

    JsonHashTable<8> hash;

Like with the array class, there is a template parameter that gives the capacity of the hash-table.

Then you can add strings, integer, booleans, etc: 

    hash.add("key1", "bazinga!");
    hash.add("key2", 42);
    hash.add("key3", true);

As for the arrays, there are two syntaxes for the floating point values:

	array.add<4>("key4", 3.1415);  // 4 digits: "3.1415" 
    array.add("key5", 3.14);	   // 2 digits: "3.14"

Finally you can add nested objects:

    JsonArray<8> nestedArray;
    hash.add("key6", nestedArray);

or

    JsonHashTable<8> nestedHash;
    hash.add("key7", nestedHash);

### 4. Get the JSON string

There are two ways tho get the resulting JSON string.

Depending on your project, you may need to dump the string in a classic `char[]` or send it to a stream like `Serial` or `EthernetClient `.

Both ways are the easy way :-)

#### Use a classic `char[]`

Whether you have a `JsonArray` or a `JsonHashTable`, simply call `printTo()` with the destination buffer, like so:

	char buffer[256];
    array.printTo(buffer, sizeof(buffer));

#### Send to a stream

It's very likely that the generated JSON will end up in a stream like `Serial` or `EthernetClient `, so you can save some time and memory by doing this:

	Serial.print(array);

or
  
    array.printTo(Serial);

> ##### About the Printable interface
> `JsonArray` and `JsonHashTable` implement Arduino's `Printable` interface.
> This is why you can call `Serial.print()` like in the example above.
> You can do the same with any other implementation of `Print`:  `HardwareSerial`,  `SoftwareSerial`, `LiquidCrystal`, `EthernetClient`, `WiFiClient`...
   

Memory usage
------------

Here are the size of the main classes of the library.

This table is for an 8-bit Arduino, types would be bigger on a 32-bit processor.

| Type                   | Size in bytes |
| ---------------------- | ------------- |
| JsonArray&lt;N&gt;     | 4 + 6 x N     |
| JsonHashTable&lt;N&gt; | 4 + 8 x N     |
