/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "Print.h"
#include "IndentedPrintDecorator.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class PrettyPrintDecorator : public Print
        {
        public:

            PrettyPrintDecorator(IndentedPrintDecorator& p)
                : sink(p)
            {
                previousChar = 0;
                inString = false;
            }

            virtual size_t write(uint8_t);

        private:            
            uint8_t previousChar;
            IndentedPrintDecorator& sink;
            bool inString;

            bool inEmptyBlock()
            {
                return previousChar == '{' || previousChar == '[';
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