Generating JSON with Arduino JSON
=================================

Before writing any code, don't forget to include the header:

    #include <ArduinoJson.h>

If your not using the Arduino IDE, please read [Using the library without Arduino](Using the library without Arduino.md).

## Example

Here is an example to generate `{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}`

    //
    // Step 1: Reserve memory space
    //
    StaticJsonBuffer<200> jsonBuffer;

    //
    // Step 2: Build object tree in memory
    //
    JsonObject& root = jsonBuffer.createObject();
    root["sensor"] = "gps";
    root["time"] = 1351824120;

    JsonArray& data = root.createNestedArray("data");
    data.add(48.756080, 6);  // 6 is the number of decimals to print
    data.add(2.302038, 6);   // if not specified, 2 digits are printed

    //
    // Step 3: Generate the JSON string
    //
    root.printTo(Serial);

## Step 1: Reserve memory space

Arduino JSON uses a preallocated memory pool to store the object tree, this is done by the `StaticJsonBuffer`.

Before continuing please read the page [Arduino JSON memory model](Memory model.md) that explains everything you need to know about `StaticJsonBuffer`.

## Step 2: Build object tree in memory

Now that you have enough memory hold by the `StaticJsonBuffer`, you can use it to build your in-memory representation of the JSON string.

#### Arrays

You create an array like this:

    JsonArray& array = jsonBuffer.createArray();

Don't forget the `&` after `JsonArray`, it needs to be a reference to the array.

Then you can add strings, integer, booleans, etc: 

    array.add("bazinga!");
    array.add(42);
    array.add(true);

There are two syntaxes for floating point values:

	array.add<4>(3.1415);  // 4 digits: "3.1415" 
    array.add(3.1415);	   // 2 digits: "3.14"

> ##### About floating point precision
> The overload of `add()` with 2 parameters allows you to specify the number of decimals to save in the JSON string. 
> When you use the overload with one parameter, you use the default number of decimals which is two.
> Note that this behavior is the exact same as Arduino's `Print::print(double,int);` which is implemented by `Serial`. 
> So you may already be familiar with it.

You can add a nested array or object if you have a reference to it.
Or simpler, you can create nested array or nested objects from the array:

    JsonArray&  nestedArray  = array.createNestedArray();
    JsonObject& nestedObject = array.createNestedObject();

#### Objects

You create an array like this:

    JsonObject& object = jsonBuffer.createObject();

Again, don't forget the `&` after `JsonObject`, it needs to be a reference to the object.

Then you can add strings, integer, booleans, etc: 

    object["key1"] = "bazinga!";
    object["key2"] = 42;
    object["key3"] = true;

As for the arrays, there are two syntaxes for the floating point values:

	object["key4"].set<4>(3.1415);  // 4 digits "3.1415" 
    object["key5"] = 3.1415;	    // default: 2 digits "3.14"

You can add a nested array or object if you have a reference to it.
Or simpler, you can create nested array or nested objects from the object:

    JsonArray&  nestedArray  = object.createNestedArray("key6");
    JsonObject& nestedObject = object.createNestedObject("key7");

> ##### Other JsonObject functions
> * `object.add(key, value)` is a synonym for `object[key] = value`
> * `object.containsKey(key)` returns `true` is the `key` is present in `object`
> * `object.remove(key)` removes the `value` associated with `key`

## Step 3: Generate the JSON string

There are two ways tho get the resulting JSON string.

Depending on your project, you may need to dump the string in a classic `char[]` or send it to a `Print` implementation like `Serial` or `EthernetClient `.

Both ways are the easy way :-)

#### Use a classic `char[]`

Whether you have a `JsonArray&` or a `JsonObject&`, simply call `printTo()` with the destination buffer, like so:

	char buffer[256];
    array.printTo(buffer, sizeof(buffer));

> ##### Want an indented output?
> By default the generated JSON is as small as possible. It contains no extra space, nor line break.
> But if you want an indented, more readable output, you can.
> Simply call `prettyPrintTo` instead of `printTo()`:
> 
>     array.prettyPrintTo(buffer, sizeof(buffer));

#### Send to a `Print` implementation

It's very likely that the generated JSON will end up in a stream like `Serial` or `EthernetClient `, so you can save some time and memory by doing this:

    array.printTo(Serial);

And, of course if you need an indented JSON string:

    array.prettyPrintTo(Serial);

> ##### About the Print interface
> The library is designed to send the JSON string to an implementation of the `Print` interface that is part of Arduino. 
> In the example above we used `Serial`, but they are many other implementation that would work as well, including: `HardwareSerial`,  `SoftwareSerial`, `LiquidCrystal`, `EthernetClient`, `WiFiClient`, `Wire`...
> When you use this library out of the Arduino environment, it will use it's own implementation of `Print` and everything will be the same.