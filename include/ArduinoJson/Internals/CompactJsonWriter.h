#pragma once

#include "ArduinoJson/Internals/JsonWriter.h"

namespace ArduinoJson
{
    namespace Internals
    {
        class CompactJsonWriter : public JsonWriter
        {
        public:
            explicit CompactJsonWriter(Print* sink)
            : JsonWriter(sink)
            {
            }

            virtual void beginArray()
            {
                _length += _sink->write('[');
            }

            virtual void endArray()
            {
                _length += _sink->write(']');
            }

            virtual void writeColon()
            {
                _length += _sink->write(':');
            }

            virtual void writeComma()
            {
                _length += _sink->write(',');
            }

            virtual void beginObject()
            {
                _length += _sink->write('{');
            }

            virtual void endObject()
            {
                _length += _sink->write('}');
            }
        };       
    }
}