#include <ESP8266WiFi.h>
#include <espnow.h>
#include <IRremote.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int buttonPin = D5;
const int ledPin = 2;

//realy status
bool buttonState = LOW; //Define integer to remember the toggle state for relay 1
bool preState = 0;

typedef struct struct_message {

  int ringNo;
  bool bellStatus;

} struct_message;

// Create a struct_message called myData
struct_message myData;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(buttonPin , INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
   buttonState = digitalRead(buttonPin);

   if(buttonState == HIGH && preState == 0)
   {
    myData.ringNo = 1;
    digitalWrite(ledPin, HIGH);
    myData.bellStatus = buttonState;
    Serial.println(myData.bellStatus);
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    preState = 1;
    delay(100);
   }
   if(buttonState == LOW && preState==1 ){
    myData.ringNo = 1;
    digitalWrite(ledPin, LOW);
    myData.bellStatus = buttonState;
    Serial.println(myData.bellStatus);
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    preState = 0;
    }
   }
