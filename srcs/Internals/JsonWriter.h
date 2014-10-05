#pragma once

#include "../Arduino/Print.h"

class JsonWriter
{
public:
    explicit JsonWriter(Print& sink)
        : _sink(sink), _length(0), _isCommaNeeded(false)
    {
    }

    size_t bytesWritten()
    {
        return _length;
    }

    void beginArray();
    void endArray();

    void beginObject();
    void endObject();

    void writeKey(const char* key);

    void writeValue(const char* value);
    void writeValue(long value);
    void writeValue(bool value);
    void writeValue(double value, int decimals);

private:
    Print& _sink;
    size_t _length;
    bool _isCommaNeeded;

    void writeCommaIfNeeded()
    {
        if (_isCommaNeeded)
            _length += _sink.write(',');
    }
};

