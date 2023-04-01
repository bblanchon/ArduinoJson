// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdlib.h>  // malloc, free
#include <catch.hpp>
#include <utility>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofObject;

class ControllableAllocator : public Allocator {
 public:
  ControllableAllocator() : _enabled(true) {}
  virtual ~ControllableAllocator() {}

  void* allocate(size_t n) override {
    return _enabled ? malloc(n) : 0;
  }

  void deallocate(void* p) override {
    free(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    return realloc(ptr, n);
  }

  void disable() {
    _enabled = false;
  }

 private:
  bool _enabled;
};

TEST_CASE("JsonDocument's allocator") {
  SpyingAllocator spyingAllocator;
  ControllableAllocator controllableAllocator;

  SECTION("Construct/Destruct") {
    { JsonDocument doc(4096, &spyingAllocator); }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate());
  }

  SECTION("Copy construct") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      JsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Deallocate());
  }

  SECTION("Move construct") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      JsonDocument doc2(std::move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate());
  }

  SECTION("Copy assign larger") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      JsonDocument doc2(8, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Allocate(8)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Deallocate());
  }

  SECTION("Copy assign smaller") {
    {
      JsonDocument doc1(1024, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      JsonDocument doc2(4096, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 1024);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Deallocate());
  }

  SECTION("Copy assign same size") {
    {
      JsonDocument doc1(1024, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      JsonDocument doc2(1024, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 1024);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Deallocate());
  }

  SECTION("Move assign") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      JsonDocument doc2(8, &spyingAllocator);

      doc2 = std::move(doc1);

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Allocate(8)
                                         << AllocatorLog::Deallocate()
                                         << AllocatorLog::Deallocate());
  }

  SECTION("garbageCollect()") {
    JsonDocument doc(4096, &controllableAllocator);

    SECTION("when allocation succeeds") {
      deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
      doc.remove("blanket");

      bool result = doc.garbageCollect();

      REQUIRE(result == true);
      REQUIRE(doc.memoryUsage() == sizeofObject(1) + 8);
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
    }

    SECTION("when allocation fails") {
      deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
      doc.remove("blanket");
      controllableAllocator.disable();

      bool result = doc.garbageCollect();

      REQUIRE(result == false);
      REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
    }
  }
}
