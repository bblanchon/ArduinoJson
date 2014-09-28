/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "Print.h"
#include "IndentedPrint.h"

namespace ArduinoJson
{
    namespace Generator
    {
        // Converts a compact JSON string into an indented one.
        class JsonPrettyPrint : public Print
        {
        public:

            JsonPrettyPrint(IndentedPrint& p)
                : _sink(p)
            {
                _previousChar = 0;
                _inString = false;
            }

            virtual size_t write(uint8_t);

        private:            
            uint8_t _previousChar;
            IndentedPrint& _sink;
            bool _inString;

            bool inEmptyBlock()
            {
                return _previousChar == '{' || _previousChar == '[';
            }

            size_t handleStringChar(uint8_t);
            size_t handleMarkupChar(uint8_t);

            size_t handleBlockClose(uint8_t);
            size_t handleBlockOpen(uint8_t);
            size_t handleColumn();
            size_t handleComma();
            size_t handleQuoteOpen();
            size_t handleNormalChar(uint8_t);
            size_t indentIfNeeded();
            size_t unindentIfNeeded();
        };
    }
}
