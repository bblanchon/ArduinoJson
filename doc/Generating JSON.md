Generating JSON with Arduino JSON
==============================

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

#### Introducing `StaticJsonBuffer`

Arduino JSON uses a preallocated memory pool to store the object tree, this is done by the `StaticJsonBuffer`.

Before using any function of the library you need to create a `StaticJsonBuffer`. Then you can use this instance to create arrays and objects, or parse a JSON string.

`StaticJsonBuffer` has a template parameter that determines the number of bytes that it contains. For example, the following line create a `StaticJsonBuffer` containing 200 bytes on the stack:

    StaticJsonBuffer<200> jsonBuffer;

The bigger the buffer is, the more complex the object tree can be, but also the more memory you need.

#### How to determine the buffer size?

So the big question you should have in mind right now is *How can I determine the size?*. 

There are basically two approaches here:

1. either you can predict the content of the object tree,
2. or, you know how much memory is available.

In the first case, you know some constraints on the object tree. For instance, let's say that your know in advance (and by that I mean "at compilation time") that you want to generate an object with 3 values, one of them being an array with 2 values, like the following:

    {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

To determine the memory usage of this object tree, you use the two macros `JSON_ARRAY_SIZE(n)` `JSON_OBJECT_SIZE(n)`, both take the number of elements as a parameter.
For the example above, it would be:

    const int BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2);
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

In the second case, let's say you dynamically generate a JSON object tree of a random complexity so you can't put a limit base on that. But on the other hand, you don't want your program to crash because the object tree doesn't fit in memory.
The solution here is to determine how much memory is available, or in other words how much memory you can afford for the JSON generation.

#### Why choosing fixed allocation?

This fixed allocation approach may seem a bit strange, especially if your a desktop app developer used to dynamic allocation, but it make a lot of sense in an embedded context:

1. the code is smaller
2. it uses less memory
3. it doesn't create memory fragmentation
4. it predictable

Don't forget that, the memory is "freed" as soon as the `StaticJsonBuffer` is out of scope, like any other variable. It only hold the memory for a short amount of time.

For that reason, you should never use a `StaticJsonBuffer` as a **global variable** because it would hold a lot of memory for the whole life of the program.

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