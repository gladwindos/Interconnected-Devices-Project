#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial esp(D2, D3);

//REMEMBER TO CHANGE WIFI ARGS
const char* ssid = "VM8585145";
const char* password = "fntw3ycGcvx7";

const char* mqttServer = "m23.cloudmqtt.com";
const int mqttPort = 18368;
const char* mqttUser = "tqvjzgfc";
const char* mqttPassword = "kgKYC5nzmFUP";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wifi...");
  }

  Serial.println("Connected to the Wifi network");

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  esp.begin(115200);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);

}

void loop() {
  client.loop();

  String str = "";
  char c;

  while (esp.available()) {
    c = esp.read();
    str.concat(c);
  }

  if (str != "") {

    char temp[5];
    char lightLvl[5];

    str.substring(0,2).toCharArray(temp, 5);
    str.substring(3,5).toCharArray(lightLvl, 5);
    
    Serial.print("Temp: ");
    Serial.println(temp);

    Serial.print("Light: ");
    Serial.println(lightLvl);

    client.publish("project/temp", temp);
    client.publish("project/light", lightLvl);
  }

  

  delay(50);
}
