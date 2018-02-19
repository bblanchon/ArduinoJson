// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdlib.h>

#include "../Configuration.hpp"
#include "./ctype.hpp"

namespace ArduinoJson {
    namespace Internals {

        // Interger Json parser
        template <typename T>
        T parseInteger(const char *s) {
            if (!s) return 0;  // NULL

            if (*s == 't') return 1;  // "true"

            T result = 0;
            bool negative_result = false;

            switch (*s) {
                case '-':
                    negative_result = true;
                    s++;
                    break;
                case '+':
                    s++;
                    break;
            }

            while (isdigit(*s)) {
                result = T(result * 10 + T(*s - '0'));
                s++;
            }

            return negative_result ? T(~result + 1) : result;
        }


        // Integer messagepack parser
        template<typename T>
        T parseBinaryInteger(const char *s) {
            uint8_t intType;
            T result = 0;
            int8_t firstValue = *(s++);

            union {
                uint8_t array[sizeof(T)];
                T result;
            } convertArray;

            if (firstValue <= MESSAGEPACK_INT_U7_MAX &&
                firstValue >= MESSAGEPACK_INT_S5_MIN) //7-bit positive integer or 5-bit signed integer
            {
                result = firstValue;
            } else {
                switch (intType = (uint8_t) firstValue) {
                    case MESSAGEPACK_INT_U8: //8-bit unsigned
                        result = (uint8_t) *s;
                        break;
                    case MESSAGEPACK_INT_S8: //8-bit signed
                        result = *s;
                        break;
                    case MESSAGEPACK_INT_U16: //16-bit unsigned
                    case MESSAGEPACK_INT_S16: //16-bit signed
                        convertArray.array[1] = *(s++);
                        convertArray.array[0] = *(s);
                        result = convertArray.result;
                        if (intType == 0xCD) result = (uint16_t) result;
                        else result = (int16_t) result;
                        break;
                    case MESSAGEPACK_INT_U32: //32-bit unsigned
                    case MESSAGEPACK_INT_S32: //32-bit signed
                        convertArray.array[3] = *(s++);
                        convertArray.array[2] = *(s++);
                        convertArray.array[1] = *(s++);
                        convertArray.array[0] = *(s);
                        result = convertArray.result;
                        if (intType == 0xCE) result = (uint32_t) result;
                        else result = (int32_t) result;
                        break;
                    case MESSAGEPACK_INT_U64: //64-bit unsigned
                    case MESSAGEPACK_INT_S64: //64-bit signed

                        convertArray.array[7] = *(s++);
                        convertArray.array[6] = *(s++);
                        convertArray.array[5] = *(s++);
                        convertArray.array[4] = *(s++);
                        convertArray.array[3] = *(s++);
                        convertArray.array[2] = *(s++);
                        convertArray.array[1] = *(s++);
                        convertArray.array[0] = *(s);
                        result = convertArray.result;
                        if (intType == 0xCF) result = (uint64_t) result;
                        else result = (int64_t) result;
                        break;
                }
            }

            return result;
        }
    }
}