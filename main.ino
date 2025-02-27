#define BLYNK_TEMPLATE_ID "----------"
#define BLYNK_TEMPLATE_NAME "--------"
#define BLYNK_AUTH_TOKEN "---------------"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "-------";
const char* password = "-----------";

const char* mqtt_server = "----------------"; 
const int mqtt_port = 8883;              
const char* mqtt_user = "-----------";         
const char* mqtt_password = "-------";     

WiFiClientSecure espClient;  
PubSubClient client(espClient);

#define DHTPIN 18              
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

BlynkTimer timer;

#define LED_PIN 19 

const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
------------------------
"-----END CERTIFICATE-----\n";

void sendSensorData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  sensors_event_t event;
  accel.getEvent(&event);
  float vibrationX = event.acceleration.x;
  float vibrationY = event.acceleration.y;
  float vibrationZ = event.acceleration.z;

  char tempStr[8], humStr[8], vibXStr[8], vibYStr[8], vibZStr[8];
  dtostrf(temp, 1, 2, tempStr);
  dtostrf(hum, 1, 2, humStr);
  dtostrf(vibrationX, 1, 2, vibXStr);
  dtostrf(vibrationY, 1, 2, vibYStr);
  dtostrf(vibrationZ, 1, 2, vibZStr);

  client.publish("iot/temperature", tempStr);
  client.publish("iot/humidity", humStr);
  client.publish("iot/vibrationX", vibXStr);
  client.publish("iot/vibrationY", vibYStr);
  client.publish("iot/vibrationZ", vibZStr);

  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, hum);
  Blynk.virtualWrite(V3, vibrationX);
  Blynk.virtualWrite(V4, vibrationY);
  Blynk.virtualWrite(V5, vibrationZ);

  Serial.print("Temperature: "); Serial.println(temp);
  Serial.print("Humidity: "); Serial.println(hum);
}

BLYNK_WRITE(V6) {  
  int switchState = param.asInt();
  if (switchState == 1) {
    digitalWrite(LED_PIN, HIGH);
    Serial.print("LED ON");
    client.publish("iot/led", "ON");  
  } else {
    digitalWrite(LED_PIN, LOW); 
    Serial.print("LED OFF"); 
    client.publish("iot/led", "OFF");    
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);  // Configure LED pin as output
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  espClient.setCACert(ca_cert);
  client.setServer(mqtt_server, mqtt_port);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  dht.begin();
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  reconnect();

  timer.setInterval(2000L, sendSensorData);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Blynk.run();
  timer.run();
}