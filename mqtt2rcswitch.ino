#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <MQTT.h>

#include "config.h"

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
WiFiClient wifiClient;
PubSubClient mqtt_client( wifiClient );

int buttonState;
int lastButtonState = LOW;
int reading;

long lastDebounceTime = 0;
long debounceDelay = 200;

int connectState = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  char* type;
  char* code_a;
  char* code_b;
  char* action;

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  payload[length] = 0;
  type = strtok(reinterpret_cast<char*>(payload),",");
  code_a = strtok(0, ",");
  code_b = strtok(0, ",");
  action = strtok(0, ",");
  Serial.println(type);
  Serial.println(code_a);
  Serial.println(code_b);
  Serial.println(action);

  if ( type[0] == 'a' ) {
    Serial.println("Type A");
    if ( action[0] == '1' ) {
      mySwitch.switchOn(code_a, code_b);
      Serial.println("On");
    } else {
      mySwitch.switchOff(code_a, code_b);
    }
  } else if ( type[0] == 'd' ) {
    Serial.println("Type D");
    if ( action[0] == '1' ) {
      mySwitch.switchOn(code_a[0],atoi(code_b));
      Serial.println("On");
    } else {
      mySwitch.switchOff(code_a[0], atoi(code_b));
    }
  }
}

void setup() {
  pinMode(14, INPUT);
  lastButtonState = digitalRead(14);
  buttonState = digitalRead(14);
  Serial.begin(9600);
  Serial.println("");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WLAN ");
  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected");

  mqtt_client.setServer(mqtt_broker, 1883);
  mqtt_client.setCallback(callback);

  mySwitch.enableTransmit(4);
  mySwitch.setPulseLength(360);
}

void checkConnect() {
  uint32_t beginWait = millis();
  Serial.println("MQTT reconnecting.");
  while (millis() - beginWait < 1000) {
    delay(100);
    if ( WiFi.status() == WL_CONNECTED ) {
      connectState = 1;

      mqtt_client.loop();
      mqtt_client.disconnect();
      mqtt_client.loop();
      delay(100);
      mqtt_client.connect("SteckdosenControl01",mqtt_user, mqtt_pass);
      mqtt_client.loop();
      delay(100);

      if ( mqtt_client.connected() ) {
        Serial.println("connected");
        mqtt_client.subscribe(MQTT_TOPIC);
        return;
      }
    }
  }
  Serial.print(".");
}

void loop() {

  if ( WiFi.status() != WL_CONNECTED || connectState == 0 || mqtt_client.connected() != 1 ) {
    connectState = 0;
    checkConnect();
  } else {
    mqtt_client.loop();
  }
  delay(10);
}
