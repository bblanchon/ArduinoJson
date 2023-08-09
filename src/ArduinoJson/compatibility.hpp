// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License
//
// clang-format off

#include <ArduinoJson/Namespace.hpp>

#ifdef __GNUC__

#define ARDUINOJSON_PRAGMA(x) _Pragma(#x)

#define ARDUINOJSON_COMPILE_ERROR(msg) ARDUINOJSON_PRAGMA(GCC error msg)

#define ARDUINOJSON_STRINGIFY(S) #S

#define ARDUINOJSON_DEPRECATION_ERROR(X, Y) \
  ARDUINOJSON_COMPILE_ERROR(ARDUINOJSON_STRINGIFY(X is a Y from ArduinoJson 5. Please see https:/\/arduinojson.org/upgrade to learn how to upgrade your program to ArduinoJson version 6))

#define StaticJsonBuffer ARDUINOJSON_DEPRECATION_ERROR(StaticJsonBuffer, class)
#define DynamicJsonBuffer ARDUINOJSON_DEPRECATION_ERROR(DynamicJsonBuffer, class)
#define JsonBuffer ARDUINOJSON_DEPRECATION_ERROR(JsonBuffer, class)
#define RawJson ARDUINOJSON_DEPRECATION_ERROR(RawJson, function)

#define ARDUINOJSON_NAMESPACE _Pragma ("GCC warning \"ARDUINOJSON_NAMESPACE is deprecated, use ArduinoJson instead\"") ArduinoJson

#endif

// clang-format on

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// DEPRECATED: use JsonDocument instead
template <size_t N>
class ARDUINOJSON_DEPRECATED("use JsonDocument instead") StaticJsonDocument
    : public JsonDocument {
 public:
  using JsonDocument::JsonDocument;

  size_t capacity() const {
    return N;
  }
};

// DEPRECATED: use JsonDocument instead
class ARDUINOJSON_DEPRECATED("use JsonDocument instead") DynamicJsonDocument
    : public JsonDocument {
 public:
  DynamicJsonDocument(size_t capacity) : _capacity(capacity) {}

  size_t capacity() const {
    return _capacity;
  }

  void garbageCollect() {}

 private:
  size_t _capacity;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
