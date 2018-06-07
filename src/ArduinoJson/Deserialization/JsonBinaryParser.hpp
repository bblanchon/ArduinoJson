// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"
#include "../TypeTraits/IsConst.hpp"
#include "StringWriter.hpp"



namespace ArduinoJson {
    namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
        template<typename TReader, typename TWriter>
        class JsonBinaryParser {
        public:
            JsonBinaryParser(JsonBuffer *buffer, TReader reader, TWriter writer,
                       uint8_t nestingLimit)
                    : _buffer(buffer),
                      _reader(reader),
                      _writer(writer),
                      _nestingLimit(nestingLimit) {}

            JsonBinaryParser(JsonBuffer *buffer, TReader reader, TWriter writer,
                       uint8_t nestingLimit,uint8_t binaryParserEnabled)
                    : _buffer(buffer),
                      _reader(reader),
                      _writer(writer),
                      _nestingLimit(nestingLimit) {}

            JsonArray &parseArray();

            JsonObject &parseObject();

            JsonVariant parseVariant() {
                JsonVariant result;
                parseAnythingTo(&result);
                return result;
            }

        private:
            JsonBinaryParser &operator=(const JsonBinaryParser &);  // non-copiable

            const char *parseString();

            bool parseAnythingTo(JsonVariant *destination);

            FORCE_INLINE bool parseAnythingToUnsafe(JsonVariant *destination);

            inline bool parseArrayTo(JsonVariant *destination);

            inline bool parseObjectTo(JsonVariant *destination);

            inline bool parseStringTo(JsonVariant *destination);

            static inline bool isQuote(char c) {
                return c == '\'' || c == '\"';
            }

            JsonBuffer *_buffer;
            TReader _reader;
            TWriter _writer;
            uint8_t _nestingLimit;

        };

        template<typename TJsonBuffer, typename TString, typename Enable = void>
        struct JsonBinaryParserBuilder {
            typedef typename StringTraits<TString>::Reader InputReader;
            typedef JsonBinaryParser<InputReader, TJsonBuffer &> TParser;

            static TParser makeBinaryParser(TJsonBuffer *buffer, TString &json,
                                      uint8_t nestingLimit) {
                return TParser(buffer, InputReader(json), *buffer, nestingLimit);
            }
        };

        template<typename TJsonBuffer, typename TChar>
        struct JsonBinaryParserBuilder<TJsonBuffer, TChar *,
                typename EnableIf<!IsConst<TChar>::value>::type> {
            typedef typename StringTraits<TChar *>::Reader TReader;
            typedef StringWriter<TChar> TWriter;
            typedef JsonBinaryParser<TReader, TWriter> TParser;

            static TParser makeBinaryParser(TJsonBuffer *buffer, TChar *json,
                                      uint8_t nestingLimit) {
                return TParser(buffer, TReader(json), TWriter(json), nestingLimit);
            }
        };

        template<typename TJsonBuffer, typename TString>
        inline typename JsonBinaryParserBuilder<TJsonBuffer, TString>::TParser makeBinaryParser(
                TJsonBuffer *buffer, TString &json, uint8_t nestingLimit) {
            return JsonBinaryParserBuilder<TJsonBuffer, TString>::makeBinaryParser(buffer, json,
                                                                       nestingLimit);
        }
    }
}
