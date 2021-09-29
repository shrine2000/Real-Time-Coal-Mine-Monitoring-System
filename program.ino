#include <DHT.h>  // Including library for dht
#include <ESP8266WiFi.h>
String apiKey = "YOUR_API_KEY";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "honor 7C_E048";     // replace with your wifi ssid and wpa2 key
const char *pass =  "123456789";
const char* server = "api.thingspeak.com";
#define DHTPIN 14//pin where the dht11 is connected
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHT11);
WiFiClient client;
void setup() 
{
pinMode(12,OUTPUT);  
Serial.begin(115200);
delay(10);
dht.begin();
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
}
 
void loop() 
{
float a = analogRead(A0);
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)||isnan(a)) 
{
Serial.println("Failed to read from  sensor!");
return;
}
if (client.connect(server,80))//   "184.106.153.149" or api.thingspeak.com
{                   
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "&field3=";
postStr += String(a/1023*100);
postStr += "\r\n\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
 
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius, Humidity: ");
Serial.print(h);
Serial.print("Gas Level: ");
Serial.println(a/1023*100);
Serial.println("%. Send to Thingspeak.");

if(a/1023*100 >= 60)
{
  digitalWrite(12,HIGH);
}
else
{
  digitalWrite(12,LOW);
     }
}
client.stop();
Serial.println("Waiting...");
  
// thingspeak needs minimum 15 sec delay between updates
delay(1000);
}