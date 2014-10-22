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
            inline bool isBoolean();
            inline bool isDouble();
            inline bool isEnd();
            inline bool isLong();
            inline bool isNull();
            inline bool isObjectStart();

            bool skip(char charToSkip);
            void skipSpaces();

            inline JsonNode* parseArray();
            inline JsonNode* parseBoolean();
            inline JsonNode *parseDouble();
            inline JsonNode* parseObjectKeyValue();
            inline JsonNode* parseLong();
            inline JsonNode* parseNull();
            inline JsonNode* parseObject();
            inline JsonNode* parseString();
        };
    }
}