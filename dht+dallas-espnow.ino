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

<<<<<<< HEAD
uint8_t broadcastAddress[] = {0x9C, 0x9c, 0x1F, 0xC2, 0x8A, 0x78};  //-- uPesy
=======
uint8_t broadcastAddress[] = {0x9C, 0x9c, 0x1F, 0xC2, 0x8A, 0x78};  //-- uPesy
>>>>>>> 138bc4554bb3dc835a0de3c379000d4495290d02

typedef struct struct_message {
  float a;
  float b;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) 
{
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)
  {
    //Serial.println(Sent);
  }
  else
  {
    //Serial.println(Failed); 
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(100);
  //lcd.begin();
  delay(10);
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
  //t = dht.readTemperature();
  sensors.requestTemperatures();
  myData.a = sensors.getTempCByIndex(0);
  delay(10);
  h = dht.readHumidity();
  delay(10);

  //myData.a = t;
  myData.b = h;

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  //lcd.clear();
  delay(2000);
}
