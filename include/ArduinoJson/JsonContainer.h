#pragma once

#include "Arduino/Printable.h"
#include "Internals/JsonNodeIterator.h"
#include "Internals/JsonNode.h"
#include "Internals/IndentedPrint.h"
#include "Internals/JsonNodeWrapper.h"

namespace ArduinoJson
{
    class JsonArray;
    class JsonObject;
    class JsonValue;

    class JsonContainer : public Printable, public Internals::JsonNodeWrapper
    {
        friend class JsonArray;

    public:
        JsonContainer() {}

        explicit JsonContainer(Internals::JsonNode* node)
        : JsonNodeWrapper(node)
        {
        }

        size_t size() const;

        bool operator==(JsonContainer const& other) const;

        size_t printTo(char* buffer, size_t bufferSize) const;
        virtual size_t printTo(Print& print) const;

        size_t prettyPrintTo(char* buffer, size_t bufferSize) const;
        size_t prettyPrintTo(ArduinoJson::Internals::IndentedPrint& print) const;
        size_t prettyPrintTo(Print& print) const;

    protected:

        Internals::JsonNodeIterator beginChildren() const
        {
            return Internals::JsonNodeIterator(_node ? _node->getContainerChild() : 0);
        }

        Internals::JsonNodeIterator endChildren() const
        {
            return Internals::JsonNodeIterator(0);
        }

        void addChild(Internals::JsonNode*);
        void removeChild(Internals::JsonNode*);
        Internals::JsonNode* createNode();
    };
}