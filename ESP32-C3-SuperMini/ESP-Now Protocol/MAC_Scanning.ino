/*
INPUT: ESP32-C3-SuperMini plug in by USB Type-C
OUTPUT: Print out the MAC Address
  MAC 1: Địa chỉ MAC của board này là: 58:8C:81:B1:16:AC
  MAC 2: Địa chỉ MAC của board này là: 58:8C:81:B1:FD:44
*/

#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA); // Bật chế độ Wi-Fi Station
  Serial.println("Đang đọc địa chỉ MAC...");
}

void loop() {
  Serial.print("Địa chỉ MAC của board này là: ");
  Serial.println(WiFi.macAddress());
  delay(3000);
}
