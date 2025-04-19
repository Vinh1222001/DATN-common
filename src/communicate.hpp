#pragma once
#ifndef COMMUNICATE_HPP
#define COMMUNICATE_HPP

#include <esp_now.h>
#include "esp_log.h"
#include "types.hpp"
#include "utils/set.hpp"
#include "vector"

using Message = Types::SemaphoreMutexData<String>;
class Communicate
{
private:
  static Communicate *instance; // Static pointer to the current instance
  uint8_t peerMac[6];

  Message receiveMsg;
  // Static callbacks
  static void onDataSentStatic(const uint8_t *mac_addr, esp_now_send_status_t status);
  static void onDataRecvStatic(const uint8_t *mac, const uint8_t *incomingData, int len);

  // Instance methods to call from static
  void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

public:
  Communicate(const uint8_t mac[6]);
  ~Communicate();

  bool begin();
  bool send(const std::vector<String> &data);
  String getReceiveMsg();
};

#endif
