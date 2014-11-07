Arduino JSON memory model
=========================

## Fixed memory allocation

### Introducing `StaticJsonBuffer`

Arduino JSON uses a preallocated memory pool to store the object tree, this is done by the `StaticJsonBuffer`.

Before using any function of the library you need to create a `StaticJsonBuffer`. Then you can use this instance to create arrays and objects, or parse a JSON string.

`StaticJsonBuffer` has a template parameter that determines the number of bytes that it contains. For example, the following line create a `StaticJsonBuffer` containing 200 bytes on the stack:

    StaticJsonBuffer<200> jsonBuffer;

The bigger the buffer is, the more complex the object tree can be, but also the more memory you need.

### How to determine the buffer size?

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

### Why choosing fixed allocation?

This fixed allocation approach may seem a bit strange, especially if your a desktop app developer used to dynamic allocation, but it make a lot of sense in an embedded context:

1. the code is smaller
2. it uses less memory
3. it doesn't create memory fragmentation
4. it predictable

Don't forget that, the memory is "freed" as soon as the `StaticJsonBuffer` is out of scope, like any other variable. It only hold the memory for a short amount of time.

For that reason, you should never use a `StaticJsonBuffer` as a **global variable** because it would hold a lot of memory for the whole life of the program.