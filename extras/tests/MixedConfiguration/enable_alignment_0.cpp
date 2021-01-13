#define ARDUINOJSON_NAMESPACE ArduinoJson_NoAlignment
#define ARDUINOJSON_ENABLE_ALIGNMENT 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_ENABLE_ALIGNMENT == 0") {
  using namespace ARDUINOJSON_NAMESPACE;

  const size_t N = sizeof(void*);

  SECTION("isAligned()") {
    CHECK(isAligned(0) == true);
    CHECK(isAligned(1) == true);
    CHECK(isAligned(N) == true);
    CHECK(isAligned(N + 1) == true);
    CHECK(isAligned(2 * N) == true);
    CHECK(isAligned(2 * N + 1) == true);
  }

  SECTION("addPadding()") {
    CHECK(addPadding(0) == 0);
    CHECK(addPadding(1) == 1);
    CHECK(addPadding(N) == N);
    CHECK(addPadding(N + 1) == N + 1);
  }

  SECTION("AddPadding<>") {
    const size_t a = AddPadding<0>::value;
    CHECK(a == 0);

    const size_t b = AddPadding<1>::value;
    CHECK(b == 1);

    const size_t c = AddPadding<N>::value;
    CHECK(c == N);

    const size_t d = AddPadding<N + 1>::value;
    CHECK(d == N + 1);
  }
}
