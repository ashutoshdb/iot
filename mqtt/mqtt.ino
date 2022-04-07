#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include "WiFi.h"

float humidity;
float temp;
long T;
float distanceCM ;
#define threshold 15

WiFiClient espclient;
const char* mqtt_server = "broker.mqtt-dashboard.com";
PubSubClient client(espclient);

#define DHTPIN 21
const int trigger = 5;
const int echo = 18;
const int LED1=22;
const int LED2=23;
const int LED3=24;

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "ashutosh";  //SSID
char pass[] = "ashutosh";  //Password

// unsigned long myChannelField = 1652667; //Channel ID
// const int ChannelField1 = 1; //for Temperature
// const int ChannelField2 = 2; //for  Humidity
// const int ChannelField3 = 3; // for Distance
// const char * myWriteAPIKey = "XQXB3M17TQ497T0Z"; //Your Write API Key

void start_wifi() {
delay(10);
Serial.println();
Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
     WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "81331e17-7339-4ec9-a852-cbe618f91b0e";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Feedback", "Reconnecting");
      // ... and resubscribe
      client.subscribe("Connection_Feedback");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
 Serial.begin(115200);
start_wifi();
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
//  ThingSpeak.begin(client);
 dht.begin();
 pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
}
void loop() { 

   if (!client.connected()) {
    reconnect();
  }
  client.loop();

 humidity = dht.readHumidity();  //To read Humidity
 temp = dht.readTemperature();   //To read Temperature
 if(isnan(humidity) || isnan(temp))
  {
    Serial.println("Faild to read from Sensor!\n");
    return;
  }
  digitalWrite(trigger, LOW);
  delay(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = (T * 0.034)/2;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);

  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print(" C");
  Serial.print(" Humidity:");
  Serial.print(humidity, 1);
  Serial.print("%");
  Serial.println();
  if (distanceCM >=8 && distanceCM <= 17) {
       client.publish("test","our bin is full !");
   Serial.print("\nfull\n");
   digitalWrite(LED1,HIGH);
  }
  if (distanceCM >= 18  && distanceCM <= 29) {
     client.publish("test","our bin is Half!");
    Serial.print("\nHalf\n");
    digitalWrite(LED2,HIGH);
  }
  if (distanceCM >=30  && distanceCM <= 42) {
      client.publish("test","our bin is empty!");
    Serial.print("empty\n");
    digitalWrite(LED3,HIGH);
  }  
  if(temp>= threshold){
    client.publish("test","Temp Reached!");
    Serial.println("Temperature Threshold Reach!! ");
  }

delay(10000);
}
