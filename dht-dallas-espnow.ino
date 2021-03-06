#include <DallasTemperature.h>
#include <OneWire.h>
OneWire oneWire(13);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>
#define DHTPin 14
#define DHTType DHT22
DHT dht(DHTPin, DHTType);
float t,h;

uint8_t broadcastAddress1[] = {0x9C, 0x9c, 0x1F, 0xC2, 0x8A, 0x78};    //-- uPesy
uint8_t broadcastAddress2[] = {0xc8, 0xc9, 0xa3, 0xd2, 0x5a, 0xc8}; //  nouveau devkit
uint8_t broadcastAddress3[] = {0x24, 0x6F, 0x28, 0xB1, 0x45, 0x48};  // TFT2.8 DEVKIT

typedef struct struct_message {
  float a;
  float b;
} struct_message;
struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  Serial.begin(115200);
  sensors.begin();
  dht.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  sensors.requestTemperatures();
  myData.a = sensors.getTempCByIndex(0);
  myData.b = dht.readHumidity();

  esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
  delay(2000);
}
