#pragma once

#include "JsonNode.hpp"

namespace ArduinoJson
{
    class JsonBuffer;

    namespace Internals
    {
        class JsonNode;

        class JsonParser
        {
        public:
            JsonParser(JsonBuffer* buffer, char* json)
            : _buffer(buffer), _ptr(json)
            {

            }

            JsonNode* parseAnything();

        private:
            JsonBuffer* _buffer;
            char* _ptr;

            inline bool isArrayStart();
            inline bool isArrayStop();
            inline bool isBoolean();
            inline bool isComma();
            inline bool isDouble();
            inline bool isEnd();
            inline bool isLong();
            inline bool isNull();
            inline bool isSpace();

            inline void skipOneChar();
            inline void skipSpaces();

            inline JsonNode* parseArray();
            inline JsonNode* parseBoolean();
            inline JsonNode* parseLong();
            inline JsonNode* parseNull();
            inline JsonNode* parseString();

            JsonNode *parseDouble();
        };
    }
}