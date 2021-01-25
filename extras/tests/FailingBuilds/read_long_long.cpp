// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h>

#if defined(__SIZEOF_LONG__) && __SIZEOF_LONG__ >= 8
#error This test requires sizeof(long) < 8
#endif

#if !ARDUINOJSON_HAS_LONG_LONG
#error This test requires C++11
#endif

ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(long long)
int main() {
  DynamicJsonDocument doc(1024);
  doc["dummy"].as<long long>();
}
