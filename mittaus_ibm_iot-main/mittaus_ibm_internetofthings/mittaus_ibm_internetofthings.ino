/***************************************************

 ****************************************************/

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> 

/************************* WiFi Access Point *********************************/

const char* ssid = "IOTLABRA";
const char* password = "iotlabra2020";

/************************* mqtt setup *********************************/

char server[] = "k2936e.messaging.internetofthings.ibmcloud.com";
char pubTopic[] = "iot-2/evt/status1/fmt/json";
char authMethod[] = "use-token-auth";
char clientId[] = "d:k2936e:ESP32:esp_Team4";
char token[] = "pPNXujHx-yYCzD7IQQ";

/************ Global State ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient wifiClient;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client
PubSubClient client(server, 1883, NULL, wifiClient);


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
   Serial.println(ssid);

  WiFi.begin(ssid, password);
  delay(2000);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  if (!client.connected()) {
      Serial.print("Reconnecting client to ");
      Serial.println(server);
      while (!client.connect(clientId, authMethod, token)) {
          Serial.print(".");
          delay(500);
      }
      Serial.println("Bluemix connected");
  }
}

uint32_t x=0;
long lastMsg = 0;
long now = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive
  client.loop();

  now = millis();

  if (now - lastMsg > 3000) {
    lastMsg = now;
    String payload = "{\"d\":{\"name\":\"Team4 esp\"";
              payload += ",\"value\":";
              payload += x;
              payload += "}}";
    x++;

    Serial.print("Sending payload: ");
    Serial.println(payload);

    if (client.publish(pubTopic, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    } else {
      Serial.println("Publish failed");
    }

    
  }
}
