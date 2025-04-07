#pragma once
#ifndef WIFI_UTIL_HPP
#define WIFI_UTIL_HPP

#include <Arduino.h>
#include <WiFi.h>
#include "esp_log.h"

namespace WifiUtil
{
    static const char *TAG = "WIFI_UTIL";

    // Initialize WiFi in station mode (for ESP-NOW), optionally connect to a WiFi network
    inline bool initWifi(const char *ssid = nullptr, const char *password = nullptr, bool waitForConnect = false)
    {
        WiFi.disconnect(true); // Reset WiFi
        WiFi.mode(WIFI_STA);

        if (ssid && password)
        {
            WiFi.begin(ssid, password);
            ESP_LOGI(TAG, "Connecting to WiFi SSID: %s", ssid);

            if (waitForConnect)
            {
                unsigned long startAttemptTime = millis();
                while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
                {
                    delay(100);
                }

                if (WiFi.status() != WL_CONNECTED)
                {
                    ESP_LOGE(TAG, "Failed to connect to WiFi");
                    return false;
                }

                ESP_LOGI(TAG, "Connected! IP address: %s", WiFi.localIP().toString().c_str());
            }
        }
        else
        {
            ESP_LOGI(TAG, "WiFi set to STA mode without connecting to a router.");
        }

        return true;
    }
}

#endif
