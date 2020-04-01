#include <FastLED.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h> 
#include "WiFi.h"


//testcode
const char *ssid = "2";
const char *password = "12345678";

const char* mqtt_broker   = "51.254.217.43";
const char* mqtt_topic    = "/Apa102/ledstrip/josa";
const char* mqtt_username = "emon";
const char* mqtt_password = "uw2ELjAKrEUwqgLT";

const int buttonPin12 = 12; 

WiFiClient wifiClient;
PubSubClient mqttClient("", 0, wifiClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(buttonPin12, INPUT); 

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

    

    if(digitalRead(buttonPin12)){
      Serial.println("Pressed"); 
      mqtt_publish(
        "Pressed" // id String
      );
    }

  
    // Publish payload is BTN is pressed

    if (WiFi.status() == WL_CONNECTED) {
    
      delay(200);
    
    
  } else { 
    Serial.println("Geen WiFi verbinding !");
    delay(1000);
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
  Serial.println();
}
