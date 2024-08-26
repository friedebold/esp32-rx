#include <Arduino.h>
#include <coms.h>

#include <WiFi.h>
#include <esp_now.h>

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0xA0, 0xA3, 0xB3, 0xAA, 0x6C, 0xA8};

struct ReceiveData
{
    Remote remote_data;
    PID pid_data;
};

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    ReceiveData receive_data;
    memcpy(&receive_data, incomingData, sizeof(receive_data));
    remote_data = receive_data.remote_data;
    pid_data = receive_data.pid_data;
}

void setup_wifi()
{
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);
    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
}