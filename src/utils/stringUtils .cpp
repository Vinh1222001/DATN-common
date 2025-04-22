#include "stringUtils.hpp"

namespace StringUtils
{
    std::vector<String> splitString(const String &data, char delimiter)
    {
        std::vector<String> result;
        int startIndex = 0;
        int endIndex = 0;

        while ((endIndex = data.indexOf(delimiter, startIndex)) != -1)
        {
            result.push_back(data.substring(startIndex, endIndex));
            startIndex = endIndex + 1;
        }

        // Thêm phần cuối cùng
        if (startIndex < data.length())
        {
            result.push_back(data.substring(startIndex));
        }

        return result;
    }
}
