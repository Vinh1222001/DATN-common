#pragma once
#ifndef COMPARE_UTILS_HPP
#define COMPARE_UTILS_HPP

#include <Arduino.h>

namespace CompareUtils
{
  template <typename T>
  bool isInConstraint(T value, T from, T to)
  {
    return value >= from && value <= to;
  }
}

#endif