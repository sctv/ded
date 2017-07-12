//THIS ONE W_I_P!!!!!
#define I2C_ADDRESS 0x3C

//Wire and MCP23017 declarations
#include <Wire.h> 
#include "Adafruit_MCP23017.h" //inserted (4,5) to wire.begin();
Adafruit_MCP23017 mcp;

// SSD1306
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire oled;

//mqtt and http declarations
#include <WiFi.h>
#include <PubSubClient.h>
/*
const char* ssid = "Makerspace";
const char* password = "donotbeonfire";
*/
const char* ssid = "deggerspot";
const char* password = "eyxt0256";
const char* mqttServer = "192.168.43.243";
//const char* mqttServer = "raspberrypi.lan";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
 WiFiClient espClient;
PubSubClient client(espClient);

//global variables
int timer = 250; 
int state = 2; //for the GPIOAB register  
int state1 = 0;
int inputs = 12; // the number of inputs you want 
String pubString;
char regState[16];
String binaryState;
char* states[] = {
"happy", 
"sad", 
"inbetween", 
"out for a walk", 
"fancy a call",
"social media free",
"just quiet",
"r. need some help",
"skype me",
"sleeping",
"in a meeting",
"in a good place"
};  
void setup() {
  Serial.begin(115200);
  delay(10);  

//Initialise Display
Wire.begin(); 
oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();
  oled.print("Hello world!");
  Serial.print("Hello world");
  
//mqtt setup
  WiFi.begin(ssid, password);
  Serial.println(ssid);
 Serial.println("Connecting to WiFi..");
 oled.clear();
 oled.print("Connecting to WiFi");
 Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("Connected to the WiFi network");
 oled.clear();
 oled.println("Connected to the WiFi network");
 oled.println(WiFi.localIP());
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
client.publish("Feeds/status", "Hello from ESP32");
mcp.begin();      // use default address 0 
pinMode(18, INPUT_PULLUP);//switch on 18 
pinMode(2, OUTPUT);// use the p2 LED as debugging
pinMode(2, LOW);
//set up mcp pins for input and set pullups high   
for (int thisPin = 0; thisPin < 16; thisPin++) {
   mcp.pinMode(thisPin, INPUT);
   mcp.pullUp(thisPin, HIGH);
   }    
}

void loop() {

//Serial.println("in main loop");
client.loop();
//Serial.println("in main loop, just past the client.loop");
state=mcp.readGPIOAB();

if (state != state1) {
switch(state){
case 65534 : {oled.clear(); oled.println(states[0]); break; }
case 65533 : {oled.clear(); oled.println(states[1]); break; }
case 65531 : {oled.clear(); oled.println(states[2]); break; }
case 65527 : {oled.clear(); oled.println(states[3]); break; }
case 65519 : {oled.clear(); oled.println(states[4]); break; }
case 65503 : {oled.clear(); oled.println(states[5]); break; }
case 65471 : {oled.clear(); oled.println(states[6]); break; }
case 65407 : {oled.clear(); oled.println(states[7]); break; }
case 65279 : {oled.clear(); oled.println(states[8]); break; }
case 65023 : {oled.clear(); oled.println(states[9]); break; }
case 64511 : {oled.clear(); oled.println(states[10]); break; }
case 63487 : {oled.clear(); oled.println(states[11]); break; }

default: /*{oled.clear(); oled.println("something has gone wrong");*/ break;}
delay(50);
}
state1 = state;
  int sensorVal = digitalRead(18);
  if (sensorVal == LOW) {
    digitalWrite(2, HIGH);
   state=mcp.readGPIOAB(); // read the register
  pubString = String(state, BIN); //convert register to BIN and String
 pubString.toCharArray(regState, pubString.length()+1);
 client.publish("Feeds/state", regState);
 oled.print("Sending status...");
  Serial.println(regState);
  Serial.println(state);
 
  switch(state){
case 65534 : {  client.publish("Feeds/status"," switch 1 "); break; }
case 65533 : {  client.publish("Feeds/status"," switch 2 "); break; }
case 65531 : {  client.publish("Feeds/status"," switch 3 "); break; }
case 65527 : {  client.publish("Feeds/status"," switch 4 "); break; }
case 65519 : {  client.publish("Feeds/status"," switch 5 "); break; }
case 65503 : {  client.publish("Feeds/status"," switch 6 "); break; }
case 65471 : {  client.publish("Feeds/status"," switch 7 "); break; }
case 65407 : {  client.publish("Feeds/status"," switch 8 "); break; }
case 65279 : {  client.publish("Feeds/status"," switch 9 "); break; }
case 65023 : {  client.publish("Feeds/status"," switch 10 "); break; }
case 64511 : {  client.publish("Feeds/status"," switch 11 "); break; }
case 63487 : {  client.publish("Feeds/status"," switch 12 "); break; }
case 61439 : {  client.publish("Feeds/status"," switch 13 "); break; }
case 57343 : {  client.publish("Feeds/status"," switch 14 "); break; }
case 49151 : {  client.publish("Feeds/status"," switch 15 "); break; }
case 32767 : {  client.publish("Feeds/status"," switch 16 "); break; }
      
    default: 
      Serial.println("default");
    break;
    }
    Serial.println("1 seconds delay"); 
    delay(1000);
    
    oled.print("Sent");
    digitalWrite(2, LOW);
  }
  
}
