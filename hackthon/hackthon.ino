

#include <DHT.h>
#include <DHT_U.h>
#include "ThingSpeak.h"
#include "WiFi.h"
#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
#define DEVICE_ID 3
#define DEVICE_NAME "Message esp"
#define TOKEN "~1178448_8jYyFbAvGj0LRyAhVE2aCYlu"

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

float humidity;
float temp;
long T;
float distanceCM ;
#define threshold 15


#define DHTPIN 21
const int trigger = 5;
const int echo = 18;
const int LED1=22;
const int LED2=23;
const int LED3=15;

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


char ssid[] = "ashutosh";  //SSID
char pass[] = "ashutosh";  //Password
WiFiClient client;

char* stringToSend="Some String to send";
uint16_t dataSize = 50;//size of double

uint16_t pos = 0;
uint8_t *data = (uint8_t*)malloc(dataSize );

unsigned long myChannelField = 1652667; //Channel ID
const int ChannelField1 = 1; //for Temperature
const int ChannelField2 = 2; //for  Humidity
const int ChannelField3 = 3; // for Distance
const char * myWriteAPIKey = "XQXB3M17TQ497T0Z"; //Your Write API Key

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
void setup() {
 Serial.begin(115200);
start_wifi();
 ThingSpeak.begin(client);
 dht.begin();
 pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
 pinMode(LED1, OUTPUT);
 pinMode(LED2, OUTPUT);
 pinMode(LED3, OUTPUT);
 remoteMe.setConnector(new RemoteMeSocketConnector());
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
  
}
void loop() { 

  remoteMe.loop();

 humidity = dht.readHumidity();  //To read Humidity
 temp = dht.readTemperature();    //To read Temperature
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
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
  if (distanceCM >=8 && distanceCM <= 17) {
   Serial.print("\nfull\n");
   digitalWrite(LED1,HIGH);
   RemoteMeMessagesUtils::putString(data, pos, stringToSend);
RemoteMeMessagesUtils::putDouble(data, pos, -1.2345);
remoteMe.sendPushNotificationMessage(4,"smart garbage","Bin is Full!!","","","");
remoteMe.sendUserMessage(DEVICE_ID , data, dataSize );
  }
  if (distanceCM >= 18  && distanceCM <= 29) {
    Serial.print("\nHalf\n");
    digitalWrite(LED2,HIGH);
       RemoteMeMessagesUtils::putString(data, pos, stringToSend);
RemoteMeMessagesUtils::putDouble(data, pos, -1.2345);
remoteMe.sendPushNotificationMessage(4,"smart garbage","Bin is nHalf!!","","","");
remoteMe.sendUserMessage(DEVICE_ID , data, dataSize );
  }
  if (distanceCM >=30  && distanceCM <= 42) {
    Serial.print("empty\n");
    digitalWrite(LED3,HIGH);
       RemoteMeMessagesUtils::putString(data, pos, stringToSend);
RemoteMeMessagesUtils::putDouble(data, pos, -1.2345);
remoteMe.sendPushNotificationMessage(4,"smart garbage","Bin is empty!!","","","");
remoteMe.sendUserMessage(DEVICE_ID , data, dataSize );
  }  
    if (distanceCM > 43) {
    Serial.print("Adujust your module\n");
       RemoteMeMessagesUtils::putString(data, pos, stringToSend);
RemoteMeMessagesUtils::putDouble(data, pos, -1.2345);
remoteMe.sendPushNotificationMessage(4,"smart garbage","Module might be misplaced!!","","","");
remoteMe.sendUserMessage(DEVICE_ID , data, dataSize );
  }  
  if(temp>= threshold){
    Serial.println("Temperature Threshold Reach!!");
RemoteMeMessagesUtils::putString(data, pos, stringToSend);
RemoteMeMessagesUtils::putDouble(data, pos, -1.2345);
remoteMe.sendPushNotificationMessage(4,"smart garbage","Temperature Threshold Reach!! Please Take Some Action","","","");
remoteMe.sendUserMessage(DEVICE_ID , data, dataSize );
  }
  ThingSpeak.writeField(myChannelField,ChannelField2,humidity, myWriteAPIKey);
  ThingSpeak.writeField(myChannelField,ChannelField1, temp, myWriteAPIKey);
  ThingSpeak.writeField(myChannelField,ChannelField3,distanceCM, myWriteAPIKey); 
delay(3000);
}
