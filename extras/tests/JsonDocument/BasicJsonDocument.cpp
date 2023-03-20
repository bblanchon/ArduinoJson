// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdlib.h>  // malloc, free
#include <catch.hpp>
#include <sstream>
#include <utility>

class SpyingAllocator : public Allocator {
 public:
  virtual ~SpyingAllocator() {}

  void* allocate(size_t n) override {
    _log << "A" << n;
    return malloc(n);
  }

  void deallocate(void* p) override {
    _log << "F";
    free(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    _log << "R" << n;
    return realloc(ptr, n);
  }

  std::string log() const {
    return _log.str();
  }

 private:
  std::ostringstream _log;
};

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

TEST_CASE("BasicJsonDocument") {
  SpyingAllocator spyingAllocator;
  ControllableAllocator controllableAllocator;

  SECTION("Construct/Destruct") {
    { BasicJsonDocument doc(4096, &spyingAllocator); }
    REQUIRE(spyingAllocator.log() == "A4096F");
  }

  SECTION("Copy construct") {
    {
      BasicJsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      BasicJsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == "A4096A4096FF");
  }

  SECTION("Move construct") {
    {
      BasicJsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      BasicJsonDocument doc2(std::move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == "A4096F");
  }

  SECTION("Copy assign larger") {
    {
      BasicJsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      BasicJsonDocument doc2(8, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == "A4096A8FA4096FF");
  }

  SECTION("Copy assign smaller") {
    {
      BasicJsonDocument doc1(1024, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      BasicJsonDocument doc2(4096, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 1024);
    }
    REQUIRE(spyingAllocator.log() == "A1024A4096FA1024FF");
  }

  SECTION("Copy assign same size") {
    {
      BasicJsonDocument doc1(1024, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      BasicJsonDocument doc2(1024, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.capacity() == 1024);
    }
    REQUIRE(spyingAllocator.log() == "A1024A1024FF");
  }

  SECTION("Move assign") {
    {
      BasicJsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      BasicJsonDocument doc2(8, &spyingAllocator);

      doc2 = std::move(doc1);

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == "A4096A8FF");
  }

  SECTION("garbageCollect()") {
    BasicJsonDocument doc(4096, &controllableAllocator);

    SECTION("when allocation succeeds") {
      deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      doc.remove("blanket");

      bool result = doc.garbageCollect();

      REQUIRE(result == true);
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 8);
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
    }

    SECTION("when allocation fails") {
      deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      doc.remove("blanket");
      controllableAllocator.disable();

      bool result = doc.garbageCollect();

      REQUIRE(result == false);
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(2) + 16);
      REQUIRE(doc.capacity() == 4096);
      REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
    }
  }
}
