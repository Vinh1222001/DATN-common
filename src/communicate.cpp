#include "communicate.hpp"

// ESP32 CAM MAC: 08:d1:f9:38:a8:ac

Communicate *Communicate::instance = nullptr;

Communicate::Communicate(const uint8_t mac[6])
{
  instance = this;
  memcpy(this->peerMac, mac, 6);
  this->receiveMsg.xMutex = xSemaphoreCreateMutex();
  this->begin();
}

Communicate::~Communicate() {}

bool Communicate::begin()
{
  if (esp_now_init() != ESP_OK)
  {
    ESP_LOGE(this->TAG, "ESP-NOW init failed");
    return false;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(peerMac))
  {
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
      ESP_LOGE(this->TAG, "Failed to add peer");
      return false;
    }
  }

  // Register both send and receive callbacks
  esp_now_register_send_cb(onDataSentStatic);
  esp_now_register_recv_cb(onDataRecvStatic);

  ESP_LOGI(this->TAG, "ESP-NOW init successful");
  return true;
}

bool Communicate::send(String header, const std::vector<String> &data)
{
  String combined;
  for (size_t i = 0; i < data.size(); ++i)
  {
    combined += data[i];
    if (i < data.size() - 1)
      combined += ","; // dùng dấu phẩy để phân tách
  }

  const Types::EspNowMessage msg = SetUtils::createEspNowMessage<String>(header, combined);

  ESP_LOGI(this->TAG, "Data send: Id:%s, header, %s, content: %s, size: %d", msg.id, msg.header, msg.content, sizeof(msg));
  esp_err_t result = esp_now_send(peerMac, reinterpret_cast<const uint8_t *>(&msg), sizeof(msg));
  if (result == ESP_OK)
  {
    ESP_LOGI(this->TAG, "Sent data successfully");
    return true;
  }
  else
  {
    ESP_LOGE(this->TAG, "Failed to send data: %d", result);
    return false;
  }
}

void Communicate::onDataSentStatic(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  if (instance)
    instance->onDataSent(mac_addr, status);
}

void Communicate::onDataRecvStatic(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  if (instance)
    instance->onDataRecv(mac, incomingData, len);
}

void Communicate::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  ESP_LOGI(this->TAG, "Data sent to %02X:%02X:%02X:%02X:%02X:%02X - Status: %s",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5],
           status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void Communicate::onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  ESP_LOGI(this->TAG, "Received data from %02X:%02X:%02X:%02X:%02X:%02X, length: %d",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], len);

  // Chuyển đổi dữ liệu thành Message
  const Types::EspNowMessage *packet = reinterpret_cast<const Types::EspNowMessage *>(incomingData);
  ESP_LOGI(this->TAG, "Data Received: Id: %s, Value: %s", packet->id, packet->content);
  // TODO: Bạn có thể thêm logic xử lý dữ liệu ở đây
  if (xSemaphoreTake(this->receiveMsg.xMutex, portMAX_DELAY) == pdTRUE)
  {
    this->receiveMsg.value.id = String(packet->id);
    this->receiveMsg.value.header = String(packet->header);
    this->receiveMsg.value.content = String(packet->content);
    xSemaphoreGive(this->receiveMsg.xMutex);
  }
}

String Communicate::getReceiveMsg()
{
  String msg;
  if (xSemaphoreTake(this->receiveMsg.xMutex, portMAX_DELAY) == pdTRUE)
  {
    if (this->receiveMsg.value.content.length() > 0)
    {
      msg = String(this->receiveMsg.value.content);
    }

    xSemaphoreGive(this->receiveMsg.xMutex);
  }

  return msg;
}

CommunicateResponse Communicate::getResponse()
{
  CommunicateResponse response;
  if (xSemaphoreTake(this->receiveMsg.xMutex, portMAX_DELAY) == pdTRUE)
  {
    response = this->receiveMsg.value;

    xSemaphoreGive(this->receiveMsg.xMutex);
  }

  return response;
}