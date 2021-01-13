#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_ENABLE_ALIGNMENT == 1") {
  using namespace ARDUINOJSON_NAMESPACE;

  const size_t N = sizeof(void*);

  SECTION("isAligned()") {
    CHECK(isAligned(0) == true);
    CHECK(isAligned(1) == false);
    CHECK(isAligned(N) == true);
    CHECK(isAligned(N + 1) == false);
    CHECK(isAligned(2 * N) == true);
    CHECK(isAligned(2 * N + 1) == false);
  }

  SECTION("addPadding()") {
    CHECK(addPadding(0) == 0);
    CHECK(addPadding(1) == N);
    CHECK(addPadding(N) == N);
    CHECK(addPadding(N + 1) == 2 * N);
  }

  SECTION("AddPadding<>") {
    const size_t a = AddPadding<0>::value;
    CHECK(a == 0);

    const size_t b = AddPadding<1>::value;
    CHECK(b == N);

    const size_t c = AddPadding<N>::value;
    CHECK(c == N);

    const size_t d = AddPadding<N + 1>::value;
    CHECK(d == 2 * N);
  }
}
