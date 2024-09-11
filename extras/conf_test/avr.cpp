#include <ArduinoJson.h>

static_assert(ARDUINOJSON_ENABLE_PROGMEM == 1, "ARDUINOJSON_ENABLE_PROGMEM");

static_assert(ARDUINOJSON_USE_LONG_LONG == 0, "ARDUINOJSON_USE_LONG_LONG");

static_assert(ARDUINOJSON_SLOT_ID_SIZE == 1, "ARDUINOJSON_SLOT_ID_SIZE");

static_assert(ARDUINOJSON_POOL_CAPACITY == 16, "ARDUINOJSON_POOL_CAPACITY");

static_assert(ARDUINOJSON_LITTLE_ENDIAN == 1, "ARDUINOJSON_LITTLE_ENDIAN");

static_assert(ARDUINOJSON_USE_DOUBLE == 0, "ARDUINOJSON_USE_DOUBLE");

static_assert(ArduinoJson::detail::ResourceManager::slotSize == 6, "slot size");

void setup() {}
void loop() {}
