#pragma once
#ifndef COMMUNICATE_HPP
#define COMMUNICATE_HPP

#include <esp_now.h>
#include "esp_log.h"
#include "types.hpp"
#include "utils/assign.hpp"
#include "utils/stringUtils.hpp"
#include "vector"
struct CommunicateResponse
{
  String header;
  std::vector<String> content;
};

using Message = Types::SemaphoreMutexData<CommunicateResponse>;
using SendStatus = Types::SemaphoreMutexData<bool>;

class Communicate
{
private:
  const char *TAG = "COMMUNICATE";
  static Communicate *instance; // Static pointer to the current instance
  uint8_t peerMac[6];

  Message receiveMsg;
  SendStatus isSuccess;
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
  bool send(String header, const std::vector<String> &data);
  bool isSentSuccess();
  std::vector<String> getReceiveMsg();
  CommunicateResponse getResponse();
};

#endif
