/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "Print.h"

namespace ArduinoJson
{
    namespace Generator
    {
        // Decorator on top of Print to allow indented output.
        // This class is used by JsonPrintable::prettyPrintTo() but can also be used
        // for your own purpose, like logging.
        class IndentedPrint : public Print
        {
        public:

            IndentedPrint(Print& p)
                : _sink(p)
            {
                _level = 0;
                _tabSize = 2;
                _isNewLine = true;
            }

            virtual size_t write(uint8_t);

            // Adds one level of indentation
            void indent();

            // Removes one level of indentation
            void unindent();

            // Set the number of space printed for each level of indentation
            void setTabSize(uint8_t n);

        private:
            Print& _sink;
            uint8_t _level : 4;
            uint8_t _tabSize : 3;
            bool _isNewLine : 1;
            
            size_t writeTabs();

            static const int MAX_LEVEL = 15; // because it's only 4 bits
            static const int MAX_TAB_SIZE = 7; // because it's only 3 bits
        };
    }
}

