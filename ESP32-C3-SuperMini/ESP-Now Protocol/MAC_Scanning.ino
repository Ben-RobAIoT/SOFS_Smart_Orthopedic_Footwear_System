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
