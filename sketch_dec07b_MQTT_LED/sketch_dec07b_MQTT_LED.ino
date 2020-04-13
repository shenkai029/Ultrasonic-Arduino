/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <math.h>

// Update these with values suitable for your network.

const char* ssid = "LynxRob";
const char* password = "dsh94538xmm";
const char* mqtt_server = "192.168.1.146";

WiFiClient espClient;
PubSubClient client(espClient);
//long lastMsg = 0;
//char msg[50];
int value = 0;
bool enable = LOW; 

void setup() {
  pinMode(D0, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D2, OUTPUT);     // Initialize the Red LED pin as an output
  pinMode(D3, OUTPUT);     // Initialize the Yellow LED pin as an output
  pinMode(D4, OUTPUT);     // Initialize the Green LED pin as an output
  pinMode(D5, INPUT);      // Initialize the Photo-resistor pin as an input
  digitalWrite(D0, HIGH);  // Turn off led from initialization
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    value += (payload[i]-'0') * pow(10, length - 1 - i); 
  }
  Serial.println();
  Serial.print("Message length : ");
  Serial.print(length);
  //Serial.print((int)payload[0]);
  Serial.println();
  Serial.print("Value: ");
  Serial.print(value);
  Serial.println();

  enable = digitalRead(D5);
  if (enable != HIGH)
  {
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);   
    return;
  }

//  // Switch on the LED if an 1 was received as first character
//  if ((char)payload[0] == '1') {
//    digitalWrite(D0, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    // but actually the LED is on; this is because
//    // it is active low on the ESP-01)
//  } else {
//    digitalWrite(D0, HIGH);  // Turn the LED off by making the voltage HIGH
//  }

  //Switch between Red and Green when distance hit limit setting
  if (value <= 20 && value > 8) {
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    } else if (value <= 8) {
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    } else {
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
      }

   value = 0;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "username", "password")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("ultraSonic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //verify code running by using D0 LED
  digitalWrite(D0, HIGH);
  delay(25);
  digitalWrite(D0, LOW);
  delay(25);
}
