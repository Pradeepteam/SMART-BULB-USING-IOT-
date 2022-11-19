#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define switch1        D0
#define switch2        D1
#define switch3        D2


#define WLAN_SSID       "project"
#define WLAN_PASS       "123456789"


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "pradeeppradeep"//"techiesms"
#define AIO_KEY         "aio_wkyY94QJ8ZQWYhMemDiPgOQDRZE0"//"912b30c900574034a653f41e2b4df838"//b2d639b609484333bfbcf9d2e216cf78,  3dff628baf014a98a9859d9957303bc6



// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light1");
Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light2");
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fan");


// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() 
{
  Serial.begin(9600);
  delay(10);
  pinMode(switch1, OUTPUT);
  digitalWrite(switch1,LOW);
  pinMode(switch2, OUTPUT);
  digitalWrite(switch2,LOW);
  pinMode(switch3, OUTPUT);
  digitalWrite(switch3,LOW);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&light1);
  mqtt.subscribe(&light2);
  mqtt.subscribe(&fan);
  
}

uint32_t x = 0;

void loop() 
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) 
  {
   
    if (subscription == &light1)
    {
      Serial.print(F("gate: "));
      Serial.println((char *)light1.lastread);
      int light_State1 = atoi((char *)light1.lastread);
      if(light_State1==1)
      {
        digitalWrite(switch1, light_State1);
  
       }
    else
    {
     digitalWrite(switch1, light_State1);
  
    }
    }

     if (subscription == &light2)
    {
      Serial.print(F("gate: "));
      Serial.println((char *)light2.lastread);
      int light_State2 = atoi((char *)light2.lastread);
      if(light_State2==1)  
      {
       digitalWrite(switch2, light_State2);
  
      }
    else
    {
     digitalWrite(switch2, light_State2);
  
    }
    }
    if (subscription == &fan) 
    {
      Serial.print(F("fan: "));
      Serial.println((char *)fan.lastread);
      int fan_State = atoi((char *)fan.lastread);
      if(fan_State==1)
      {
       digitalWrite(switch3, fan_State);
      
      }
    else
     {
      digitalWrite(switch3, fan_State);
  
     }
    }
   }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
 
}
