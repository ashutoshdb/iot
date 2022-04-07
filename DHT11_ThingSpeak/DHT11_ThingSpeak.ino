#include "ThingSpeak.h"
#include "WiFi.h"
#include <Wire.h>  //Include Library for HTU21D
#include "SparkFunHTU21D.h"

//-------Enter your WiFi Details------//
char ssid[] = "your ssid";  //SSID
char pass[] = "your password";  //Password
//-----------------------------------//

#define HTU21DPIN 4  //Pin where the HTU21D is connected

float humidity;
float temp;
//HTU21D htu(HTU21DPIN, HTU21D);
WiFiClient client;

//-------------ThingSpeak Details-----------------//
unsigned long myChannelField = 1487450; //Channel ID
const int ChannelField1 = 1; //for Humidity
const int ChannelField2 = 2; //for Temperature
const char * myWriteAPIKey = "8OLYPMW0FCX9SIOK"; //Your Write API Key
//-----------------------------------------------//

//Create an instance of the object
HTU21D myHumidity;

void setup() {
  Serial.begin(115200);
  delay(10);
  myHumidity.begin();
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
  
  humidity = myHumidity.readHumidity();  //To read Humidity
  temp = myHumidity.readTemperature();   //To read Temperature
  /*if(isnan(humidity) || isnan(temp))
  {
    Serial.println("Faild to read from Sensor!\n");
    return;
  }*/
  
  /*Printing the result on Serial Terminal*/
  Serial.print("Time:");
  Serial.print(millis());
  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humidity, 1);
  Serial.print("%");
  Serial.println();
  
  delay(1000);

  //-------------Sending Data to ThingSpeak Cloud---------------//
  ThingSpeak.writeField(myChannelField,ChannelField1, humidity, myWriteAPIKey);
  ThingSpeak.writeField(myChannelField,ChannelField2, temp, myWriteAPIKey);
}
