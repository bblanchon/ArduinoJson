Avoiding common pitfalls in Arduino JSON
========================================

As `StaticJsonBuffer` is the corner stone of this library, you'll see that every pitfall listed here are related to a wrong understanding of the memory model.

Make sure you read [Arduino JSON memory model](Memory model.md) before going further.

## 1. Make `StaticJsonBuffer` big enough

By design, the library has no way to tell you why `parseArray()` or `parseObject()` failed.

There are basically two reasons why they may fail:

1. the JSON string is invalid,
2. the JSON string contains more values that the buffer can store.

So, if you are sure the JSON string is correct and you still can't parse it, you should try to increase the size of the `StaticJsonBuffer`.

## 2. Make sure everything fits in memory

You may go into unpredictable trouble if you allocate more memory than your processor really has.
It's a very common issue in embedded development.

To diagnose this, look at every big objects in you code and sum their size to check that they fit in RAM.

For example, don't do this:

    char json[1024];                // 1 KB
    StaticJsonBuffer<512> buffer;   // 514 B

because it may be too big for a processor with only 2 KB: you need free memory to store other variables and the call stack.

That is why an 8-bit processor is not able to parse long and complex JSON strings.

## 3. Keep the `StaticJsonBuffer` in memory long enough

Remember that `StaticJsonBuffer`'s function return references.
References don't contain data, they are just pointer to the actual.
So they can only work if the actual data is in memory.

For example, don't do this:

    JsonArray& getArray(char* json)
    {
        StaticJsonBuffer<200> buffer;
        return buffer.parseArray(json); 
    }

because the local variable `buffer` will be *removed* from memory when the function `parseArray()` returns, and the `JsonArray&` will point to an invalid location.

## 4. Don't reuse the same `StaticJsonBuffer`

During is lifetime a `StaticJsonBuffer` growth until it's discarded. If you try to reuse the same instance several time, it will rapidly get full.

For this reason, you should not use a global variable for your `StaticJsonBuffer`. I don't think there is any scenario in which a global `StaticJsonBuffer` would be a valid option.

The best practice is to declare it in a local scope, so that it's discarded as soon as possible. My advice it to declare it in a function which unique role is to handle the JSON serialization.

## 5. Keep the JSON string in memory long enough

The library never make memory duplication.
This has an important implication on string values, it means that the library will return pointer to chunks of the string.

For instance, let's imagine that you parse `["hello","world"]`, like this:

    char[] json = "[\"hello\",\"world\"]";
    StaticJsonBuffer<32> buffer;
    JsonArray& array = buffer.parseArray(json);

    const char* first = array[0];
    const char* second = array[1];

In that case, both `first` and `second` are pointers to the content of the original string `json`.
So this will only work if `json` is still in memory.

## 6. JSON string is altered

If you read carefully the previous section, you may have come to the conclusion that the JSON parser modifies the JSON string.

Indeed, the parser modifies the string for two reasons:

1. it inserts `\0` to terminate substrings,
2. it translate escaped characters like `\n` or `\t`.

Most of the time this wont be an issue, but there are some corner cases that can be problematic.

Let take the example bellow:

    char[] json = "[\"hello\",\"world\"]";
    StaticJsonBuffer<32> buffer;
    JsonArray& array = buffer.parseArray(json);

If you replace it by:

    char* json = "[\"hello\",\"world\"]";
    StaticJsonBuffer<32> buffer;
    JsonArray& array = buffer.parseArray(json);

Depending on your platform, you may have an exception because the parser tries to write at a location that is read-only.
In the first case `char json[]` declares an array of `char` initialized to the specified string.
In the second case `char* json` declares a pointer to a read-only string, in fact it should be a `const char*` instead of a `char*`.

