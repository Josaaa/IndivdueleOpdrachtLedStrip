#include <FastLED.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h> 
#include "WiFi.h"

#define NumOfLed 30
#define PinData 12
#define PinClock 14

CRGB leds[NumOfLed];
//testcode
const char *ssid = "2";
const char *password = "12345678";

const char* mqtt_broker   = "51.254.217.43";
const char* mqtt_topic    = "/Apa102/ledstrip/josa";
const char* mqtt_username = "emon";
const char* mqtt_password = "uw2ELjAKrEUwqgLT";

int currentColor; 
int lightState =0;

WiFiClient wifiClient;
PubSubClient mqttClient("", 0, wifiClient);

void setup() {
  // put your setup code here, to run once: 
  currentColor = 0;  
    delay(2000);
    LEDS.addLeds<APA102,PinData, PinClock,RGB>(leds,NumOfLed);
  Serial.begin(115200);

    Serial.println(" Init wifi connection" );
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  WiFi.begin(ssid, password); //connect to wifi
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connection established");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
    if( !mqttClient.connected() ) {
      mqtt_connect();
    } else {
      mqttClient.loop();
    }
     switch(lightState){
    case 0: 
     redLed();
     break; 
   case 1: 
    greenLed();
     break;
   case 2: 
     blueLed();
     break;  
   case 3: 
      discoLed();
      break;
   case 4: 
    fireEffect(); 
    break;
   case 5: 
    colorLoop();
    break;
   case 6:
    christmasLights();
    break; 
   case 7:
    oceanSprarkle();
    break;
   case 8:
    randomSparkels();
    break;
   case 9:
    forestLed();
    break; 
   case 10:
    flitsLed();
    break; 
   case 11:
    redLedForLed();
    break; 
   case 12:
    sprintLed();
    break;
  }     
    delay(10);
    if (WiFi.status() == WL_CONNECTED) {   
      delay(20);        
  } else { 
    Serial.println("Geen WiFi verbinding !");
    delay(1000);
  }
}

void redLed(){
  for(int i = 0; i < NumOfLed; i++){
     leds[i] = CRGB(0,0,255);
  }
  FastLED.show();
}

void greenLed(){
   for(int i = 0; i < NumOfLed; i++){
     leds[i] = CRGB(0,255,0);
  }
  FastLED.show();
}

void blueLed(){
  for(int i = 0; i < NumOfLed; i++){
     leds[i] = CRGB(255,0,0);
  }
  FastLED.show();
}

void discoLed(){
  static int hue = 0;

  for(int i = 0; i < NumOfLed; i++) {
    leds[i] = CHSV(hue++, 255, 255); 
    FastLED.show(); 
    fadeall();
    delay(10);
  }

  for(int i = (NumOfLed)-1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
}

void sprintLed(){
  static int hue = 0;
  
  for(int i = 0; i < NumOfLed; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
    blackall();
    delay(10);
  }
  
  for(int i = (NumOfLed)-1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    blackall();
    delay(10);
  }
}

void blackall() { 
  for(int i = 0; i < NumOfLed; i++) { 
  leds[i] =  CRGB::Black ; 
  } 
  }


void fadeall(){
  for(int i = 0; i < NumOfLed; i++) { 
   leds[i].nscale8(250);
  } 
  
}

void fireEffect(){
  Serial.println("Fire");
  int i = (rand() % ((NumOfLed-1)-0 +1))+0; 
  int hueFire = (rand() % ( 160-150 +1))+150; 
  Serial.println(hueFire); 
  leds[i] = CHSV(hueFire, 255, 255);
  FastLED.show(); 
  fadeall(); 
  delay(1); 
  
}



void colorLoop(){

  switch(currentColor){
    case 0: 
     for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(0,0,255);
      FastLED.show();
      fadeall(); 
      
      delay(10); 
     }
     currentColor++; 
     break; 
   case 1: 
    for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(0,255,0);
      FastLED.show();
      fadeall(); 
      
      delay(10); 
     }
     currentColor++; 
     break;
   case 2: 
   for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(255,0,0);
      FastLED.show();
      fadeall(); 
      
      delay(10); 
     }
     currentColor = 0;
     break;     
  } 
}

void oceanSprarkle(){
  blueLed(); 
  for(int i =0; i < 6; i ++){
    int ranLed = (rand() % ((NumOfLed-1)-0 +1))+0; 
    leds[ranLed] = CRGB(255,255,255);
    delay(20); 
    FastLED.show(); 
  }
}

void christmasLights(){
  int greenRed = (rand() % ( 2-1 +1))+2; 
  Serial.println(greenRed);
  int randomLight = (rand() % ((NumOfLed-1)-0 +1))+0;
  if(greenRed == 2){
    leds[randomLight] = CRGB(0,255,0); 
    FastLED.show(); 
  }else{
    leds[randomLight] = CRGB(0,0,255); 
    FastLED.show(); 
  }
  delay(10); 
  fadeall(); 
}

void randomSparkels(){
  int i = (rand() % ((NumOfLed-1)-0 +1))+0; 
  int randomLed = (rand() % ( 255-0 +1))+0; 
  leds[i] = CHSV(randomLed, randomLed, randomLed);
  FastLED.show(); 
  fadeall(); 
  delay(1); 
}

void forestLed(){
  int i = (rand() % ((NumOfLed-1)-0 +1))+0; 
  int hueForest = (rand() % ( 110-75 +1))+75;  
  leds[i] = CHSV(hueForest, 255, 255);
  FastLED.show(); 
  fadeall(); 
  delay(1); 
}

void flitsLed(){
  switch(currentColor){
    case 0: 
     for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(0,0,255);
      FastLED.show();
      blackall();      
     }
     currentColor++; 
     break; 
   case 1: 
    for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(0,255,0);
      FastLED.show();
      blackall();      
     }
     currentColor++; 
     break;
   case 2: 
   for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(255,0,0);
      FastLED.show();
      blackall();      
     }
     currentColor = 0;
     break;     
  } 
  delay(50); 
}

void redLedForLed(){
   for(int i = 0; i < NumOfLed; i++){
      leds[i] = CRGB(0,0,255);
      FastLED.show();
      delay(30); 
      blackall();      
     }
}


void mqtt_connect() 
/* 
short:      Connect to MQTT server UNSECURE
inputs:        
outputs: 
notes:         
Version :   DMK, Initial code
*******************************************************************/
{  
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(mqtt_broker, 1883);

  // Connect with unique id
  String clientId = "TI14-";
  clientId += String(random(0xffff), HEX);
  clientId += '-';
  clientId += String((uint32_t)ESP.getEfuseMac(), HEX);
  
  if(mqttClient.connect( clientId.c_str(), mqtt_username, mqtt_password )){

    // Subscribe to topic
    mqttClient.subscribe(mqtt_topic);

    // Setup callback
    mqttClient.setCallback(mqtt_callback);
    Serial.printf("%s: Connected to %s:%d\n", __FUNCTION__, mqtt_broker, 1883);
  } else {    
    Serial.printf("%s: Connection ERROR (%s:%d)\n", __FUNCTION__, mqtt_broker, 1883);
    delay(2000);
  }
}

/******************************************************************/
void mqtt_publish(String id)
/* 
short:      Pulish on MQTT topic (UNSECURE)
inputs:     mascotte id, latitude from gps, longitude from gps
outputs: 
notes:         
Version :   1.0, Initial code
*******************************************************************/
{
  DynamicJsonDocument jsonDocument(1024);

  JsonObject info = jsonDocument.createNestedObject("Led");
  info["id"] = id;


  char json[1024];
  serializeJson(jsonDocument, json);
  Serial.printf("%s\n", json);
  mqttClient.publish(mqtt_topic, json);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
/* 
short:    MQTT callback. Elke publish op subscibed topic wordt hier
          afgehandeld
inputs:   'topic' waarop gepublished is
          'payload' bevat de published datablock
          'length' is de lengte van het payload array
outputs: 
notes:    In deze callback wordt gebruikt gemaakt van JSON parser
Version:  DMK, Initial code
*******************************************************************/
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    
  }
  if(lightState < 12){
    lightState ++; 
  }else{
    lightState = 0; 
  }
  
  
  
  Serial.println();
}
