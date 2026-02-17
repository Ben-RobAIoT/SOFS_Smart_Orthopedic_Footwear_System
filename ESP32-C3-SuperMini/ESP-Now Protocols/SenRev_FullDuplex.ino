#include <esp_now.h>
#include <WiFi.h>

// Chân LED nội bộ của ESP32-C3 SuperMini (Bạn đã biết ở phần trước)
#define LED_BUILTIN 8 

// === QUAN TRỌNG: THAY ĐỔI ĐỊA CHỈ MAC CỦA BOARD BÊN KIA VÀO ĐÂY ===
// Ví dụ MAC là 34:85:18:XX:YY:ZZ thì viết là 0x34, 0x85, 0x18, 0xXX, 0xYY, 0xZZ
uint8_t peerAddress[] = {0x58, 0x8C, 0x81, 0xB1, 0x16, 0xAC}; 

typedef struct struct_message {
  char text[32];
  int counter;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;
volatile bool blinkLed = false;

// 1. SỬA LỖI GỬI: Tham số đầu tiên phải là const wifi_tx_info_t *
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("\r\n[TRẠNG THÁI GỬI] -> ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công! 🚀" : "Thất bại! ❌");
}

// 2. SỬA LỖI NHẬN: Tham số đầu tiên phải là const esp_now_recv_info_t *
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Lấy địa chỉ MAC của người gửi từ cấu trúc info
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           info->src_addr[0], info->src_addr[1], info->src_addr[2], 
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  Serial.printf("\n[NHẬN TIN từ %s] Nội dung: %s | Lần: %d", macStr, myData.text, myData.counter);
  
  blinkLed = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Tắt LED (SuperMini thường tích cực mức LOW)

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Lỗi khởi tạo ESP-NOW");
    return;
  }

  // Đăng ký Callback với cấu trúc hàm mới
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Lỗi: Không thêm được địa chỉ người nhận!");
    return;
  }
  
  Serial.println("Hệ thống sẵn sàng. Nhập tin nhắn để gửi...");
}

void loop() {
  // Xử lý chớp LED khi nhận tin
  if (blinkLed) {
    digitalWrite(LED_BUILTIN, LOW);  // Bật LED
    delay(150);
    digitalWrite(LED_BUILTIN, HIGH); // Tắt LED
    blinkLed = false;
  }

  // Đọc dữ liệu từ Serial Monitor để gửi đi
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      input.toCharArray(myData.text, 32);
      myData.counter++;
      
      esp_now_send(peerAddress, (uint8_t *) &myData, sizeof(myData));
    }
  }
}
