#include <WiFi.h>
#include <esp_now.h>

// Mother Box MAC address
uint8_t motherMac[] = {0x30, 0xED, 0xA0, 0xA8, 0xBD, 0xD8};

// Peer info
esp_now_peer_info_t peerInfo;

// Send callback (ESP-IDF v5)
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("📤 Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  // Init Wi-Fi in Station mode
  WiFi.mode(WIFI_STA);
  Serial.print("Edge Box MAC: ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  // Add Mother as peer
  memcpy(peerInfo.peer_addr, motherMac, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add Mother Box as peer");
    return;
  }
}

void loop() {
  // Example message (replace with FARMEDGE sensor data later)
  String message = "Hello from Edge Box!";
  esp_err_t result = esp_now_send(motherMac, (uint8_t *)message.c_str(), message.length());

  if (result == ESP_OK) {
    Serial.println("📨 Message sent");
  } else {
    Serial.println("❌ Error sending message");
  }

  delay(2000); // send every 2 seconds
}