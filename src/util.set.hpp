#pragma once
#ifndef SET_UTILS_HPP
#define SET_UTILS_HPP

#include <Arduino.h>
#include "types.hpp"
#include <UUID.h>

namespace SetUtils
{
    extern UUID uuid; // Khai báo extern nếu dùng trong nhiều file

    inline String generateUUID()
    {
        uuid.seed(millis());       // Seed bằng thời gian hiện tại
        uuid.generate();           // Tạo UUID mới
        return uuid.toCharArray(); // Chuyển UUID thành chuỗi
    }

    template <typename T>
    Types::EspNowMessage<T> createEspNowMessage(const T &content)
    {
        Types::EspNowMessage<T> message;
        message.id = generateUUID();
        message.content = content;
        return message;
    }

    template <typename T>
    Types::HttpResponse<T> createHttpResponse(
        T data,
        bool success = false,
        String message = "Something went wrong!")
    {
        Types::HttpResponse<T> res;
        res.success = success;
        res.message = message;
        res.data = data;
        return res;
    }
}

#endif