#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP

#include <Arduino.h>

template <typename T>
struct SemaphoreMutexData
{
  T value;
  SemaphoreHandle_t xMutex;
};

template <typename T>
struct SemaphoreQueueData
{
  T sendData;
  T recieveDate;
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
#endif