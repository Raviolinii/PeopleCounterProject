#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define SOUND_VELOCITY 0.034

const int triggerPin1 = 4;
const int echoPin1 = 5;

const int triggerPin2 = 13;
const int echoPin2 = 12;

const char inTriggerMessage[] = "In = true";
const char inResolveMessage[] = "Someone Exited";

const char outTriggerMessage[] = "Out = true";
const char outResolveMessage[] = "Someone Entered";

long duration1;
long duration2;
float distance1;
float distance2;
float lastDistance1;
float lastDistance2;

float prevDist1 = 200;
float prevDist2 = 200;

bool in;
bool out;

Ticker resetTicker;

const char* ssid = "ssid";
const char* password = "passs";
const char* mqttServer = "ip"; 
const char* mqttUser = "name";
const char* mqttPass= "pass";

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(115200);
    Serial.print("Serial");
    pinMode(triggerPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

    pinMode(triggerPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    in = false;
    out = false;

    prevDist1 = 200;
    prevDist2 = 200;

    SetupWifi();
    client.setServer(mqttServer, 1883);
}

void loop()
{
  if (!client.connected())
  {
    Reconnect();
  }

  distance1 = GetDistance(triggerPin1, echoPin1);
  MovementCheck(distance1, prevDist1, &in, &out, inTriggerMessage, inResolveMessage, false);
  prevDist1 = distance1;
  // Serial.print("Distance1 (cm): ");
  // Serial.println(distance1);

  distance2 = GetDistance(triggerPin2, echoPin2);
  MovementCheck(distance2, prevDist2, &out, &in, outTriggerMessage, outResolveMessage, true);
  prevDist2 = distance2;
  // Serial.print("Distance2 (cm): ");
  // Serial.println(distance2);
}

float GetDistance(int triggerPin, int echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * SOUND_VELOCITY / 2;
} 

void MovementCheck(float distance, float prevDist, bool* currentSensorBool, bool* oppositeSensorBool, const char* triggerMessage, const char* resolveMessage, bool entered)
{
  if(distance > 40 || *currentSensorBool || prevDist <= 40)
  {
    delay(50);
    return;
  }

  if(!*oppositeSensorBool)
  {
    Serial.println(triggerMessage);
    *currentSensorBool = true;
    resetTicker.attach(2, OnTick);
  }
  else
  {
    Serial.println(resolveMessage);
    *oppositeSensorBool = false;
    resetTicker.detach();
    digitalWrite(LED_BUILTIN, LOW);
    char json[200];
    PopulateJsonMessage(entered, json, sizeof(json));
    client.publish("Counter", json);
    Serial.println(entered);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(50);
}

void OnTick()
{
  in = false;
  out = false;
  resetTicker.detach();
  Serial.println("Reset");
}

void SetupWifi() 
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void Reconnect() 
{
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Client1", mqttUser, mqttPass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void PopulateJsonMessage(bool entered, char* json, size_t jsonSize)
{
  StaticJsonDocument<200> message;
  message["Entered"] = entered;
  serializeJson(message, json, jsonSize);
}