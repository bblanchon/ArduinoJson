#include <ArduinoJson.h>

static_assert(ARDUINOJSON_ENABLE_PROGMEM == 1, "ARDUINOJSON_ENABLE_PROGMEM");

static_assert(ARDUINOJSON_USE_LONG_LONG == 0, "ARDUINOJSON_USE_LONG_LONG");

static_assert(ARDUINOJSON_SLOT_OFFSET_SIZE == 1,
              "ARDUINOJSON_SLOT_OFFSET_SIZE");

static_assert(ARDUINOJSON_LITTLE_ENDIAN == 1, "ARDUINOJSON_LITTLE_ENDIAN");

static_assert(ARDUINOJSON_USE_DOUBLE == 1, "ARDUINOJSON_USE_DOUBLE");

static_assert(sizeof(ArduinoJson::detail::VariantSlot) == 8,
              "sizeof(VariantSlot)");

void setup() {}
void loop() {}
