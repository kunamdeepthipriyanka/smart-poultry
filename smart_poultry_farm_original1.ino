
#include <Adafruit_Sensor.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Servo.h>
Servo myservo;
#define  DHTPIN D8
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
#define temp_led D7

const char* ssid = "MLRITM-PLACEMENT";
const char* password = "Mlritm@123";
String command;

#define ORG "q6w3k7"
#define DEVICE_TYPE "smart_poultry"
#define DEVICE_ID "1234"
#define TOKEN "12345678"
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() 
{ 
  myservo.attach(D4);
  Serial.begin(115200);
  dht.begin();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
 }
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(temp_led,OUTPUT);
  Serial.print("poultry farm");
  Serial.println("poultry farm");
  
}

void loop() 
{ 
  int pos;
   delay(2000); 
  //gas sensor start
  int analogSensor = analogRead(A0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  delay(500);
 if (analogSensor >800)
  {
   Serial.println("Please switch on the motor");
  }
else
  {
    Serial.println("Motor is in Off Condition");
    }
  delay(100);
  //gas sensor end
  
  //dht11 start
  float t = dht.readTemperature();
  Serial.println(t);
  delay(500);
if(t>35)
  {
   Serial.println("Temperature is HIGH");
   myservo.write(45);
   digitalWrite(temp_led, HIGH); //Red
   delay(200);
  }
else 
  {
   Serial.println("Temperature is LOW");
   digitalWrite(temp_led,  LOW);
   myservo.write(90);
  }
 PublishData(t, analogSensor);
}
void PublishData(float t, float analogSensor)
 {
  if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += t;
  payload+="," "\"analogSensor\":";
  payload += analogSensor;
  payload += "}}";
  Serial.print("Sending payload: ");
  Serial.println(payload);
if (client.publish(topic, (char*) payload.c_str()))
 {
 Serial.println("Publish ok");
 } 
else 
{
 Serial.println("Publish failed");
 }
 }

  
 
