// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdint.h>
#include "../Data/Encoding.hpp"
#include "../Data/JsonInteger.hpp"
#include "../Polyfills/attributes.hpp"
#include "../Serialization/FloatParts.hpp"
#include "../TypeTraits/IsSame.hpp"
#include "../TypeTraits/IsSignedIntegral.hpp"


namespace ArduinoJson {
    namespace Internals {

// Writes the JSON tokens to a Print implementation
// This class is used by:
// - JsonArray::writeTo()
// - JsonObject::writeTo()
// - JsonVariant::writeTo()
// Its derived by PrettyJsonWriter that overrides some members to add
// indentation.
        template<typename Print>
        class JsonWriter {
        public:
            explicit JsonWriter(Print &sink) : _sink(sink), _length(0) {}

            // Returns the number of bytes sent to the Print implementation.
            // This is very handy for implementations of printTo() that must return the
            // number of bytes written.
            size_t bytesWritten() const {
                return _length;
            }

            void beginArray() {
                writeRaw('[');
            }

            void beginMessagepackArray(uint16_t size) {
                if (size <= 15) {
                    writeRaw(MESSAGEPACK_ARRAY_FIX_MIN + size);
                } else {
                    writeRaw(MESSAGEPACK_ARRAY_16);
                    writeRaw(size >> 8);
                    writeRaw(size & 0xFF);
                }
            }

            void beginMessagepackObject(uint16_t size) {
                if (size <= 15) {
                    writeRaw(MESSAGEPACK_MAP_FIX_MIN + size);
                } else {
                    writeRaw(MESSAGEPACK_MAP_16);
                    writeRaw(size >> 8);
                    writeRaw(size & 0xFF);

                }
            }

            void endArray() {
                writeRaw(']');
            }

            void beginObject() {
                writeRaw('{');
            }

            void endObject() {
                writeRaw('}');
            }

            void writeColon() {
                writeRaw(':');
            }

            void writeComma() {
                writeRaw(',');
            }

            void writeBoolean(bool value) {
                writeRaw(value ? "true" : "false");
            }

            void writeMessagepackBoolean(bool value)
            {
            if(value)
                writeRaw(MESSAGEPACK_TRUE);
            else
                writeRaw(MESSAGEPACK_FALSE);
            }

            void writeMessagepackNull()
            {
                writeRaw(MESSAGEPACK_NULL);
            }

            void writeString(const char *value) {
                if (!value) {
                    writeRaw("null");
                } else {
                    writeRaw('\"');
                    while (*value) writeChar(*value++);
                    writeRaw('\"');
                }
            }

            void writeMessagepackString(const char *value) {
                uint16_t stringSize = 0;
                if (!value)
                {
                    writeRaw(MESSAGEPACK_STRING_FIX_MIN);
                }
                else
                {
                    stringSize = strlen(value);

                    if(stringSize<=31)
                    {
                        writeRaw(MESSAGEPACK_STRING_FIX_MIN+stringSize);
                    }
                    else if (stringSize <= 255)  //8 bits string size
                    {
                        writeRaw(MESSAGEPACK_STRING_8);
                        writeRaw(stringSize);
                    }
                    else                        //16 bits string size
                    {
                        writeRaw(MESSAGEPACK_STRING_16);
                        writeRaw(stringSize >> 8);
                        writeRaw(stringSize & 0xFF);
                    }


                    while (*value) writeRaw(*value++); //write string until null character
                }
            }

            void writeChar(char c) {
                char specialChar = Encoding::escapeChar(c);
                if (specialChar) {
                    writeRaw('\\');
                    writeRaw(specialChar);
                } else {
                    writeRaw(c);
                }
            }

            template<typename TFloat>
            void writeFloat(TFloat value) {


                if (isNaN(value)) return writeRaw("NaN");

                if (value < 0.0) {
                    writeRaw('-');
                    value = -value;
                }

                if (isInfinity(value)) return writeRaw("Infinity");

                FloatParts<TFloat> parts(value);

                writeInteger(parts.integral);
                if (parts.decimalPlaces) writeDecimals(parts.decimal, parts.decimalPlaces);

                if (parts.exponent < 0) {
                    writeRaw("e-");
                    writeInteger(-parts.exponent);
                }

                if (parts.exponent > 0) {
                    writeRaw('e');
                    writeInteger(parts.exponent);
                }
            }

            template<typename TFloat>
            void writeMessagepackFloat(TFloat value) {
                union {
                    uint8_t bytes[sizeof(uint64_t)];
                    float valueFloat;
                    double valueDouble;
                } convertFloat;

                uint8_t floatSize = sizeof(TFloat);

            switch (floatSize){
                case sizeof(uint32_t):
                    convertFloat.valueFloat = value;
                    writeRaw(MESSAGEPACK_FLOAT_32);
                    break;
                case sizeof(uint64_t):
                    convertFloat.valueDouble = value;
                    writeRaw(MESSAGEPACK_FLOAT_64);
                    break;
            }

                while(floatSize--)
                {
                    writeRaw(convertFloat.bytes[floatSize]);
                }

            }

            template<typename UInt>
            void writeInteger(UInt value) {
                char buffer[22];
                char *end = buffer + sizeof(buffer) - 1;
                char *ptr = end;

                *ptr = 0;
                do {
                    *--ptr = char(value % 10 + '0');
                    value = UInt(value / 10);
                } while (value);

                writeRaw(ptr);
            }



            template<typename UInt>
            void writeMessagepackInteger(UInt value) {

                union {
                    uint8_t bytes[sizeof(uint64_t)];
                    UInt valueUInt;
                } convertInt;

                uint8_t isSigned = IsSignedIntegral<UInt>::value;
                uint8_t intSize = sizeof(UInt);

                if(isSigned)            //signed values
                {
                    if(value>=-31 && value <= 127)
                    {
                        writeRaw(value);
                        return;
                    }

                    convertInt.valueUInt = value;

                    for(int8_t i = intSize - 1, state = 0; i >= 0; i--)  //search for empty bytes or full negative bytes
                    {
                        switch (state){
                            case 0:
                                if(convertInt.bytes[i]==0) //if empty byte
                                {
                                    intSize--;
                                }
                                else if(convertInt.bytes[i]==0xFF) //if negative byte
                                {
                                    intSize--;
                                    state = 1;
                                }
                                break;
                            case 1:
                                if(convertInt.bytes[i]==0xFF) //if negative byte
                                    intSize--;
                                else
                                    goto INTEGER_SIZE_COMPLETED; //if different value, exit loop
                        }
                    }
                    INTEGER_SIZE_COMPLETED:
                    intSize = roundSize(intSize); //round real size to next powr of 2

                    switch (intSize)
                    {

                        case sizeof(int8_t):

                            writeRaw(MESSAGEPACK_INT_S8);
                            break;
                        case sizeof(int16_t):

                            writeRaw(MESSAGEPACK_INT_S16);
                            break;
                        case sizeof(int32_t):

                            writeRaw(MESSAGEPACK_INT_S32);
                            break;
                        case sizeof(int64_t):

                            writeRaw(MESSAGEPACK_INT_S64);
                            break;
                    }

                }
                else                //unsigned values
                {

                    if(value <= 127)
                    {
                        writeRaw(value);
                        return;
                    }

                    convertInt.valueUInt = value;

                    for(int8_t i = intSize - 1;i >= 0; i--)  //search for empty bytes
                    {
                        if(convertInt.bytes[i]==0)  intSize--; //count effective size
                        else break;
                    }

                    intSize = roundSize(intSize); //round real size to next powr of 2

                    switch (intSize)
                    {
                        case sizeof(uint8_t):
                            writeRaw(MESSAGEPACK_INT_U8);
                            break;
                        case sizeof(uint16_t):
                            writeRaw(MESSAGEPACK_INT_U16);
                            break;
                        case sizeof(uint32_t):
                            writeRaw(MESSAGEPACK_INT_U32);
                            break;
                        case sizeof(uint64_t):
                            writeRaw(MESSAGEPACK_INT_U64);
                            break;
                    }
                }

                while(intSize--)
                {
                    writeRaw(convertInt.bytes[intSize]);
                }

            }

            void writeDecimals(uint32_t value, int8_t width) {
                // buffer should be big enough for all digits, the dot and the null
                // terminator
                char buffer[16];
                char *ptr = buffer + sizeof(buffer) - 1;

                // write the string in reverse order
                *ptr = 0;
                while (width--) {
                    *--ptr = char(value % 10 + '0');
                    value /= 10;
                }
                *--ptr = '.';

                // and dump it in the right order
                writeRaw(ptr);
            }

            void writeRaw(const char *s) {
                _length += _sink.print(s);
            }

            void writeRaw(char c) {
                _length += _sink.print(c);

            }

        protected:
            Print &_sink;
            size_t _length;

        private:
            JsonWriter &operator=(const JsonWriter &);  // cannot be assigned

            static inline uint8_t roundSize(uint8_t intSize)
            {
                uint8_t intM;

                if(intSize > sizeof(uint16_t))
                {
                    intM = intSize % sizeof(uint32_t); //gets the remainder of division by 4

                    if(intM!=0) //if real size not multiple of 4
                    {
                        intSize -= (intSize < sizeof(uint32_t)? sizeof(uint32_t) - intM : intM );
                        intSize <<= 1; //multiply by 2
                    }
                }

                return intSize;
            }
        };
    }
}
