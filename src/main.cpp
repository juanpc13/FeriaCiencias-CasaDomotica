#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//WIFI DATA
#define   SSID            "CLARO1_8D831B"
#define   PASSWORD        "606s2wxuxM"
//MQTT DATA
#define   AIO_SERVER      "io.adafruit.com"
#define   AIO_SERVERPORT  1883                   // use 8883 for SSL
#define   AIO_USERNAME    "juanpc13"
#define   AIO_KEY         "aio_wATA73m3GOukqE0hqoeAY2VXHJAe"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/on-off-google");
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
  mqtt.subscribe(&onoffbutton);

  // PINS
  pinMode(D4, OUTPUT);
}

void loop() {
  // Do Connect
  MQTT_connect();

  // Do Checks Programing
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      if (strcmp((char*) onoffbutton.lastread, "ON")){
        digitalWrite(D4, HIGH);
      }else{
        digitalWrite(D4, LOW);
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