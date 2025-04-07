#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP

#include <Arduino.h>

namespace Types
{
  template <typename T>
  struct SemaphoreMutexData
  {
    T value;
    SemaphoreHandle_t xMutex;
  };

  template <typename T>
  struct SemaphoreQueueData
  {
    T send;
    T receive;
    SemaphoreHandle_t xQueue;
  };

  struct VectorElements
  {
    float x = 0;
    float y = 0;
    float z = 0;

    float resultant() const
    {
      return sqrt(x * x + y * y + z * z);
    }
  };

  template <typename T>
  struct HttpResponse
  {
    bool success;
    String message;
    T data;
  };

  struct EspNowMessage
  {
    char id[36];
    char content[250];
  };

}

#endif