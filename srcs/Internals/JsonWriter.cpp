#include "JsonWriter.h"
#include "EscapedString.h"

using namespace ArduinoJson::Internals;

void JsonWriter::beginArray()
{
    writeCommaIfNeeded();
    _length += _sink.write('[');
    _isCommaNeeded = false;
}

void JsonWriter::endArray()
{
    _length += _sink.write(']');
    _isCommaNeeded = true;
}

void JsonWriter::beginObject()
{
    writeCommaIfNeeded();
    _length += _sink.write('{');
    _isCommaNeeded = false;
}

void JsonWriter::endObject()
{
    _length += _sink.write('}');
    _isCommaNeeded = true;
}

void JsonWriter::writeKey(char const* key)
{
    writeCommaIfNeeded();
    _length += EscapedString::printTo(key, _sink);
    _length += _sink.write(':');
    _isCommaNeeded = false;
}

void JsonWriter::writeValue(char const* value)
{
    writeCommaIfNeeded();
    _length += EscapedString::printTo(value, _sink);
    _isCommaNeeded = true;
}

void JsonWriter::writeValue(long value)
{
    writeCommaIfNeeded();
    _length += _sink.print(value);
    _isCommaNeeded = true;
}

void JsonWriter::writeValue(bool value)
{
    writeCommaIfNeeded();
    _length += _sink.print(value ? "true" : "false");
    _isCommaNeeded = true;
}

void JsonWriter::writeValue(double value, int decimals)
{
    writeCommaIfNeeded();
    _length += _sink.print(value, decimals);
    _isCommaNeeded = true;
}