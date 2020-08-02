/* 
 *  By Kai Shen
 *  Email shenkai029@gmail.com
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Set your MAC address and IP address here

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x6A, 0x0C };
IPAddress ip(192, 168, 2, 30);

// Mqtt server info
const char* mqtt_server = "192.168.2.10";

// Ethernet and MQTT related objects
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

//
int batSoc = 50;

void setup() 
{
  // Useful for debugging purposes
  Serial.begin(9600);

  // Start the ethernet connection
  Ethernet.begin(mac, ip);

  // Ethernet takes some time to boot
  delay(3000);   

  // Set the MQTT server to the server stated above 
  mqttClient.setServer(mqtt_server, 1883);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void publishMessage(char* topic, String payload)
{
  char pub_msg[20];
  payload.toCharArray(pub_msg, 20);
  // Attempt to publish a value to the topic "MakerIOTopic"
  if(mqttClient.publish(topic, pub_msg))
  {
    Serial.println("
    Published message: " + String(topic) + "/" + payload);
  }
  else
  {
    Serial.println("Could not send message :(");
  }
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
   
  // Attempt to connect
    if (mqttClient.connect("robotdebug", "username", "password")) 
    {
      Serial.println("connected");     
     } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
     }
  }
}

void loop() 
{
  if (!mqttClient.connected()) 
  {
    reconnect();
  }
  mqttClient.loop();

  if (batSoc > 100)
  {
    batSoc = 50;
    }
    
  publishMessage("batteryFake", "SOC:" + String(batSoc) + "%");
  batSoc++;

  //verify code running by using build-in LED
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(100);                     
  digitalWrite(LED_BUILTIN, LOW);   
  delay(100);
}
