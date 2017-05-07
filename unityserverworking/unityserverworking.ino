#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
const char* ssid = "Tvb";
const char* password = "12345678";
 
WiFiServer server(80);
WiFiClient client;
 


const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int minVal=265;
int maxVal=402;

double x;
double y;
double z;

  int xAng;
    int yAng; 
    int zAng; 
    
 
void setup() {
   Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  
  Serial.begin(115200);
 
  // Set our pins

  WiFi.begin(ssid,password);
  Serial.println("Connecting");
 
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
 
  Serial.print("Connected to "); 
  Serial.println(ssid);
  
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP());
 
  // Start the TCP server
  server.begin();
}
 
void loop() {
  // Listen for connecting clients
  client = server.available();
  if (client){
    Serial.println("Client connected");
    delay(100);
    while (client.connected()){
      //MPU6050 data
       Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  
    xAng = map(AcX,minVal,maxVal,-90,90);
    yAng = map(AcY,minVal,maxVal,-90,90);
    zAng = map(AcZ,minVal,maxVal,-90,90);

       x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
       y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
       z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

       

        // Send the distance to the client, along with a break to separate our messages
        client.println(x);
       
         client.println(y);
 
         client.println(z);
 
        // Delay before the next reading
        delay(10);
    }

    Serial.println("Client diconnected");
    client.stop();
  }
 } 
