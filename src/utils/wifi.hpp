#pragma once
#ifndef WIFI_UTIL_HPP
#define WIFI_UTIL_HPP

#include <Arduino.h>
#include <WiFi.h>
#include "esp_log.h"

namespace WifiUtil
{
    static const char *TAG = "WIFI_UTIL";

    // Initialize WiFi in station mode (for ESP-NOW), optionally connect to a WiFi network with optional static IP config
    inline bool initWifi(
        const char *ssid = nullptr,
        const char *password = nullptr,
        bool waitForConnect = false,
        IPAddress local_ip = INADDR_NONE,
        IPAddress gateway = INADDR_NONE,
        IPAddress subnet = INADDR_NONE,
        IPAddress dns1 = INADDR_NONE,
        IPAddress dns2 = INADDR_NONE)
    {
        WiFi.disconnect(true); // Reset WiFi
        WiFi.mode(WIFI_STA);

        if (ssid && password)
        {
            // Only configure static IP if all required fields are provided
            if (local_ip != INADDR_NONE && gateway != INADDR_NONE && subnet != INADDR_NONE)
            {
                if (!WiFi.config(local_ip, gateway, subnet, dns1 != INADDR_NONE ? dns1 : gateway, dns2))
                {
                    ESP_LOGE(TAG, "Failed to configure static IP");
                    return false;
                }
                ESP_LOGI(TAG, "Static IP configured: %s", local_ip.toString().c_str());
            }

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
