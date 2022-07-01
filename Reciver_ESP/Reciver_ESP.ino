
#include <ESP8266WiFi.h>
#include <espnow.h>

#define buzzer D5


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {

  int ringNo;
  bool bellStatus;

} struct_message;

// Create a struct_message called myData
struct_message myData;



// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  Serial.print("Int: ");
  Serial.println(myData.ringNo);

  Serial.print("Bool: ");
  Serial.println(myData.bellStatus);
  Serial.println();

  if (myData.ringNo == 1 || myData.ringNo == 2 || myData.ringNo == 3) {
    digitalWrite(buzzer, myData.bellStatus );
    Serial.println(myData.bellStatus);
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}
