// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License
//
// clang-format off

#include <ArduinoJson/Namespace.hpp>

#ifdef ARDUINOJSON_SLOT_OFFSET_SIZE
#error ARDUINOJSON_SLOT_OFFSET_SIZE has been removed, use ARDUINOJSON_SLOT_ID_SIZE instead
#endif

#ifdef ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
#warning "ARDUINOJSON_ENABLE_STRING_DEDUPLICATION has been removed, string deduplication is now always enabled"
#endif

#ifdef __GNUC__

#define ARDUINOJSON_PRAGMA(x) _Pragma(#x)

#define ARDUINOJSON_COMPILE_ERROR(msg) ARDUINOJSON_PRAGMA(GCC error msg)

#define ARDUINOJSON_STRINGIFY(S) #S

#define ARDUINOJSON_DEPRECATION_ERROR(X, Y) \
  ARDUINOJSON_COMPILE_ERROR(ARDUINOJSON_STRINGIFY(X is a Y from ArduinoJson 5. Please see https:/\/arduinojson.org/v7/upgrade-from-v5/ to learn how to upgrade to ArduinoJson 7))

#define StaticJsonBuffer ARDUINOJSON_DEPRECATION_ERROR(StaticJsonBuffer, class)
#define DynamicJsonBuffer ARDUINOJSON_DEPRECATION_ERROR(DynamicJsonBuffer, class)
#define JsonBuffer ARDUINOJSON_DEPRECATION_ERROR(JsonBuffer, class)
#define RawJson ARDUINOJSON_DEPRECATION_ERROR(RawJson, function)

#define ARDUINOJSON_NAMESPACE _Pragma ("GCC warning \"ARDUINOJSON_NAMESPACE is deprecated, use ArduinoJson instead\"") ArduinoJson

// DEPRECATED: you don't need to compute the size anymore
#define JSON_ARRAY_SIZE(N) _Pragma ("GCC warning \"JSON_ARRAY_SIZE is deprecated, you don't need to compute the size anymore\"") (ArduinoJson::detail::sizeofArray(N))

// DEPRECATED: you don't need to compute the size anymore
#define JSON_OBJECT_SIZE(N) _Pragma ("GCC warning \"JSON_OBJECT_SIZE is deprecated, you don't need to compute the size anymore\"") (ArduinoJson::detail::sizeofObject(N))

// DEPRECATED: you don't need to compute the size anymore
#define JSON_STRING_SIZE(N) _Pragma ("GCC warning \"JSON_STRING_SIZE is deprecated, you don't need to compute the size anymore\"") (N+1)

#else

// DEPRECATED: you don't need to compute the size anymore
#define JSON_ARRAY_SIZE(N) (ArduinoJson::detail::sizeofArray(N))

// DEPRECATED: you don't need to compute the size anymore
#define JSON_OBJECT_SIZE(N) (ArduinoJson::detail::sizeofObject(N))

// DEPRECATED: you don't need to compute the size anymore
#define JSON_STRING_SIZE(N) (N+1)

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

namespace detail {
template <typename TAllocator>
class AllocatorAdapter : public Allocator {
 public:
  AllocatorAdapter(const AllocatorAdapter&) = delete;
  AllocatorAdapter& operator=(const AllocatorAdapter&) = delete;

  void* allocate(size_t size) override {
    return _allocator.allocate(size);
  }

  void deallocate(void* ptr) override {
    _allocator.deallocate(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) override {
    return _allocator.reallocate(ptr, new_size);
  }

  static Allocator* instance() {
    static AllocatorAdapter instance;
    return &instance;
  }

 private:
  AllocatorAdapter() = default;
  ~AllocatorAdapter() = default;

  TAllocator _allocator;
};
}  // namespace detail

// DEPRECATED: use JsonDocument instead
template <typename TAllocator>
class ARDUINOJSON_DEPRECATED("use JsonDocument instead") BasicJsonDocument
    : public JsonDocument {
 public:
  BasicJsonDocument(size_t capacity)
      : JsonDocument(detail::AllocatorAdapter<TAllocator>::instance()),
        _capacity(capacity) {}

  size_t capacity() const {
    return _capacity;
  }

  void garbageCollect() {}

 private:
  size_t _capacity;
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

inline JsonObject JsonArray::createNestedObject() const {
  return add<JsonObject>();
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
