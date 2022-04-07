#include <DHT.h>
#include <DHT_U.h>
#include "ThingSpeak.h"
#include "WiFi.h"
//#include <Wire.h>  //Include Library for HTU21D
//#include "SparkFunHTU21D.h"

//-------Enter your WiFi Details------//
char ssid[] = "ashutosh";  //SSID
char pass[] = "ashutosh";  //Password
//-----------------------------------//

const int trigger = 5;
const int echo = 18;
int count =0;
long T;
float distCM;
WiFiClient client;

//-------------ThingSpeak Details-----------------//
unsigned long myChannelField = 1644752; //Channel ID
const int ChannelField3 = 3; // for distance
const char * myWriteAPIKey = "KLIOGV7GYI7NX8I0"; //Your Write API Key
//-----------------------------------------------//

//Create an instance of the object
void setup() {
  Serial.begin(115200);
pinMode(trigger,OUTPUT);
pinMode(echo,INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() 
{
  //-----------------WiFi Setup-------------------//
  if(WiFi.status()!= WL_CONNECTED)
  {
    Serial.print("Attempting to Connect to SSID: ");
    Serial.println(ssid);
  }
  while(WiFi.status()!= WL_CONNECTED)
  {
    WiFi.begin(ssid,pass);
    Serial.print(".");
    delay(5000);
  }
  Serial.println("\nConnected.");
  //--------------------------------------------//
 digitalWrite(trigger,LOW);
 delay(1);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  T = pulseIn(echo, HIGH);
distCM = (T * 0.034)/2;
Serial.println("Distance in cm: ");
Serial.println(distCM);
if(distCM<=20){
  count++;
  Serial.println("object is passed and count is:");
  Serial.println(count);
}

  ThingSpeak.writeField(myChannelField,ChannelField3,distCM, myWriteAPIKey);
}
