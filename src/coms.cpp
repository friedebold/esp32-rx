#include <Arduino.h>
#include <coms.h>

#include <WiFi.h>
#include <esp_now.h>

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x19, 0xE1, 0x1C};

struct ReceiveData
{
    Remote remote_data;
    PID pid;
};

struct SendData
{
    Battery battery;
    IMU imu;
    float global_thrust;
    Thrust thrust;
};

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    ReceiveData receive_data;
    memcpy(&receive_data, incomingData, sizeof(receive_data));
    remote_data = receive_data.remote_data;
    pid = receive_data.pid;
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

void send_data()
{
    SendData send_data = {battery, imu, global_thrust, thrust};
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&send_data, sizeof(send_data));
};