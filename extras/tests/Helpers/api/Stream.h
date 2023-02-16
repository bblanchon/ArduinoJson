// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

// Reproduces Arduino's Stream class
class Stream  // : public Print
{
 public:
  virtual ~Stream() {}
  virtual int read() = 0;
  virtual size_t readBytes(char *buffer, size_t length) = 0;
};
