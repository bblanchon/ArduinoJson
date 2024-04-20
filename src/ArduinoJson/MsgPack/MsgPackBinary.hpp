#pragma once

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class MsgPackBinary {
 public:
  explicit MsgPackBinary(const void* c, size_t size) : data_(c), size_(size) {}
  const void* data() const {
    return data_;
  }

  size_t size() const {
    return size_;
  }

 private:
  const void* data_;
  size_t size_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
