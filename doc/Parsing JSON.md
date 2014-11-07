Parsing JSON with Arduino JSON
==============================

## Example

Here an example that parse the string `{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}`:

    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    
    //
    // Step 1: Reserve memory space
    //
    StaticJsonBuffer<200> jsonBuffer;
    
    //
    // Step 2: Deserialize the JSON string
    //
    JsonObject& root = jsonBuffer.parseObject(json);

    if (!root.success()) 
    {
      Serial.println("parseObject() failed");
      return;
    }

    //
    // Step 3: Retrieve the values
    //
    const char* sensor    = root["sensor"];
    long        time      = root["time"];
    double      latitude  = root["data"][0];
    double      longitude = root["data"][1];

## Step 1: Reserve memory space

Arduino JSON uses a preallocated memory pool to store the object tree, this is done by the `StaticJsonBuffer`.

Before continuing please read the page [Arduino JSON memory model](Memory model.md) that explains everything you need to know about `StaticJsonBuffer`.

## Step 2: Parse the JSON string

You call the JSON parser through the instance of `StaticJsonBuffer`.
It exposes two function for parsing JSON:

1. parseArray() that returns a reference to a `JsonArray`
2. parseObject() that returns a reference to a `JsonObject`

Let's see an example.
Say we want to parse `{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}`, it's an object so we call `parseObject` as follows:

    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

    JsonObject& root = jsonBuffer.parseObject(json);

As you can see `parseObject()` takes a `char*` as a parameter.
Be careful, it's not a `const char*`, the memory must be writable.
Indeed, the parser will modify the string in two cases:

1. to insert string endings (character `\0`),
2. to translate escaped characters (like `\n` or `\t`).

Another thing that you must keep in mind is that the string (`char json[]` in the example above) must stay in memory during the whole parsing process.
That is because the in memory object tree will store pointer to chunks of the string, so as to avoid any memory duplication. 

Now, to check if the parsing was successful, you can call `JsonObject::success()`:

    if (!root.success())
    {
        // Parsing fail
    }

The result can be `false` for tree reasons:

1. the JSON string is invalid,
2. the JSON string doesn't represent an object,
3. the `StaticJsonBuffer` is too small.

We just saw how to parse an object, there is nothing more to say for arrays, the procedure is exactly the same.

## Step 3: Retrieve the values

Now that the object or array is in memory, you can extract the data very easily.

In this section, we'll see how to do it with a `JsonObject`.
Once again, there is nothing more to say about arrays, `JsonArray` works exactly the same as `JsonObject`.

#### Subscript operator

The simplest way is to use the subscript operator of `JsonObject`:
    
    const char* sensor = root["sensor"];
    long time = root["time"];
 
You can chain the subscript operator if you have nested arrays or objects:

    double latitude  = root["data"][0];
    double longitude = root["data"][1];

But alternatively, you can get a reference to the nested array:

    JsonArray& nestedArray = root["data"];

#### Casting values

In the previous examples, the values were implicitly casted to the target type.
You can also do this explicitly

    const char* sensor = root["sensor"].asString();
    long time = root["time"].as<long>();
    JsonArray& nestedArray = root["data"].asArray();

If the actual value doesn't match the target type, a default value will be return:

1. `false` for boolean values
2. `0` for integer values
3. `NULL` for string values
4. `JsonArray::invalid()` for nested arrays
5. `JsonObject::invalid()` for nested object

#### Check values

If you want to know if some value is present, call `containsKey()`:

    if (root.contains("extra")) 
    {
        // root["extra"] is valid
    }

If you want to check the type value has a certain type, call `is<T>()`:

    if (root["extra"].is<JsonArray&>()) 
    {
        // root["extra"] is an array
    }

You can also iterate through the key-value pairs of the object:

    for (JsonObject::itertor it=root.begin(); it!=root.end(); ++it)
    {
      Serial.println(it->key);
      Serial.println(i->value.asString());
    }
