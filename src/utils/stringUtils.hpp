#pragma once
#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace StringUtils
{
    std::vector<String> splitString(const String &data, char delimiter);
}

#endif