#pragma once
#ifndef ASSIGN_UTILS_HPP
#define ASSIGN_UTILS_HPP

#include <Arduino.h>
#include "types.hpp"

namespace SetUtils
{
    template <typename T>
    Types::SemaphoreMutexData<T> initMutexData(T initial)
    {
        SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
        if (mutex == nullptr)
        {
            ESP_LOGE("initMutexData", "Failed to create mutex!");
        }
        return {
            .value = initial,
            .xMutex = mutex};
    }

    template <typename T>
    void setMutexData(Types::SemaphoreMutexData<T> &data, const T &newValue)
    {
        if (xSemaphoreTake(data.xMutex, portMAX_DELAY) == pdTRUE)
        {
            data.value = newValue;
            xSemaphoreGive(data.xMutex);
        }
    }

    template <typename T>
    Types::EspNowMessage createEspNowMessage(String header, const T &content)
    {
        const String _content(content);
        Types::EspNowMessage message;

        strncpy(message.header, header.c_str(), sizeof(message.header) - 1);
        message.header[sizeof(message.header) - 1] = '\0';

        strncpy(message.content, _content.c_str(), sizeof(message.content) - 1);
        message.content[sizeof(message.content) - 1] = '\0';

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

namespace GetUtils
{
    template <typename T, typename F>
    void getMutexData(Types::SemaphoreMutexData<T> &data, F fn)
    {
        if (xSemaphoreTake(data.xMutex, portMAX_DELAY) == pdTRUE)
        {
            fn(data.value);
            xSemaphoreGive(data.xMutex);
        }
    }

}

#endif