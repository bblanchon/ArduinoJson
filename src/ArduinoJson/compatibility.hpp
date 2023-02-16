// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License
//
// clang-format off

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
