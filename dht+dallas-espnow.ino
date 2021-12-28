#include <DallasTemperature.h>
#include <OneWire.h>
OneWire oneWire(14);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>
#define DHTPin 13
#define DHTType DHT22
DHT dht(DHTPin, DHTType);
float t,h;

uint8_t broadcastAddress[] = {0x9C, 0x9c, 0x1F, 0xC2, 0x8A, 0x78};  //-- uPesy

typedef struct struct_message {
  float a;
  float b;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) 
{
  Serial.print("Last Packet Send Status: ");
}
 
void setup() {
  Serial.begin(115200);
  delay(100);
  sensors.begin();
  dht.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  sensors.requestTemperatures();
  myData.a = sensors.getTempCByIndex(0);
  delay(10);
  myData.b = dht.readHumidity();
  delay(10);
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  delay(2000);
}
