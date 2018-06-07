// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../TypeTraits/FloatTraits.hpp"
#include "./ctype.hpp"
#include "./math.hpp"
#include "./convertDouble.hpp"

namespace ArduinoJson {
    namespace Internals {

        //Json float parser
        template <typename T>
        inline T parseFloat(const char* s) {
            typedef FloatTraits<T> traits;
            typedef typename traits::mantissa_type mantissa_t;
            typedef typename traits::exponent_type exponent_t;

            if (!s) return 0;  // NULL

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

            if (*s == 't') return 1;  // true
            if (*s == 'n' || *s == 'N') return traits::nan();
            if (*s == 'i' || *s == 'I')
                return negative_result ? -traits::inf() : traits::inf();

            mantissa_t mantissa = 0;
            exponent_t exponent_offset = 0;

            while (isdigit(*s)) {
                if (mantissa < traits::mantissa_max / 10)
                    mantissa = mantissa * 10 + (*s - '0');
                else
                    exponent_offset++;
                s++;
            }

            if (*s == '.') {
                s++;
                while (isdigit(*s)) {
                    if (mantissa < traits::mantissa_max / 10) {
                        mantissa = mantissa * 10 + (*s - '0');
                        exponent_offset--;
                    }
                    s++;
                }
            }

            int exponent = 0;
            if (*s == 'e' || *s == 'E') {
                s++;
                bool negative_exponent = false;
                if (*s == '-') {
                    negative_exponent = true;
                    s++;
                } else if (*s == '+') {
                    s++;
                }

                while (isdigit(*s)) {
                    exponent = exponent * 10 + (*s - '0');
                    if (exponent + exponent_offset > traits::exponent_max) {
                        if (negative_exponent)
                            return negative_result ? -0.0f : 0.0f;
                        else
                            return negative_result ? -traits::inf() : traits::inf();
                    }
                    s++;
                }
                if (negative_exponent) exponent = -exponent;
            }
            exponent += exponent_offset;

            T result = traits::make_float(static_cast<T>(mantissa), exponent);

            return negative_result ? -result : result;
        }



        // Float parser for message pack
        template<typename T>
        inline T parseBinaryFloat(const char *s) {
            uint8_t floatType = *s++; //s[0]

            union {
                uint8_t binary[sizeof(uint64_t)];
                float floatValue;
                double doubleValue;
                uint64_t value64;
                uint32_t value32;
            } value;

            value.doubleValue = 0;

            if (floatType == MESSAGEPACK_FLOAT_64) {

                value.binary[7] = *(s++); //s[1]
                value.binary[6] = *(s++); //s[2]
                value.binary[5] = *(s++); //s[3]
                value.binary[4] = *(s++); //s[4]
                value.binary[3] = *(s++); //s[5]
                value.binary[2] = *(s++); //s[6]
                value.binary[1] = *(s++); //s[7]
                value.binary[0] = *(s);   //s[8]
                if (sizeof(T) == sizeof(uint32_t)) //downcasting (compiler will probaly optimize this)
                {
                    if(sizeof(float)!=sizeof(double))
                    {
                        return (float) value.doubleValue;
                    }
                    else //device double tupe is only 4 bytes
                    {
                        float64_t d;
                        d.v = value.value64;
                        float32_t f = f64_to_f32(d);
                        value.value32 = f.v;
                        return value.floatValue;
                    }

                }
                else
                {
                    return value.doubleValue;
                }
            } else {
                value.binary[3] = *(s++); //s[1]
                value.binary[2] = *(s++); //s[2]
                value.binary[1] = *(s++); //s[3]
                value.binary[0] = *(s); //s[4]
                if (sizeof(T) == sizeof(uint64_t))
                    return (double) value.floatValue; //upcasting
            }

            return value.floatValue;
        }
    }
}
