#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//WIFI DATA
#define   SSID            "yourwifi"
#define   PASSWORD        "yourpass"
// PINS
#define   LUZ1_PIN        D6
#define   LUZ2_PIN        D5
#define   LUZ3_PIN        D2
#define   LUZ4_PIN        D1
//MQTT DATA
#define   AIO_SERVER      "io.adafruit.com"
#define   AIO_SERVERPORT  1883                   // use 8883 for SSL
#define   AIO_USERNAME    "yourusername"
#define   AIO_KEY         "yourkey"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe luz1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/luz1");
Adafruit_MQTT_Subscribe luz2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/luz2");
Adafruit_MQTT_Subscribe luz3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/luz3");
Adafruit_MQTT_Subscribe luz4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/luz4");
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/on-off-google");
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // Setup MQTT subscription
  //mqtt.subscribe(&onoffbutton);
  mqtt.subscribe(&luz1);
  mqtt.subscribe(&luz2);
  mqtt.subscribe(&luz3);
  mqtt.subscribe(&luz4);

  // PINS
  pinMode(LUZ1_PIN, OUTPUT);
  pinMode(LUZ2_PIN, OUTPUT);
  pinMode(LUZ3_PIN, OUTPUT);
  pinMode(LUZ4_PIN, OUTPUT);
}

void loop() {
  // Do Connect
  MQTT_connect();

  // Do Checks Programing
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &luz1) {
      Serial.print(F("luz1: "));
      Serial.println((char *)luz1.lastread);
      if (!strcmp((char*) luz1.lastread, "ON")){
        digitalWrite(LUZ1_PIN, HIGH);
      }else{
        digitalWrite(LUZ1_PIN, LOW);
      }
    }else if(subscription == &luz2){
      Serial.print(F("luz2: "));
      Serial.println((char *)luz2.lastread);
      if (!strcmp((char*) luz2.lastread, "ON")){
        digitalWrite(LUZ2_PIN, HIGH);
      }else{
        digitalWrite(LUZ2_PIN, LOW);
      }
    }else if(subscription == &luz3){
      Serial.print(F("luz3: "));
      Serial.println((char *)luz3.lastread);
      if (!strcmp((char*) luz3.lastread, "ON")){
        digitalWrite(LUZ3_PIN, HIGH);
      }else{
        digitalWrite(LUZ3_PIN, LOW);
      }
    }else if(subscription == &luz4){
      Serial.print(F("luz4: "));
      Serial.println((char *)luz4.lastread);
      if (!strcmp((char*) luz4.lastread, "ON")){
        digitalWrite(LUZ4_PIN, HIGH);
      }else{
        digitalWrite(LUZ4_PIN, LOW);
      }
    }
  }

}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}