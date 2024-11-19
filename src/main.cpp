#include <LEDmatrix.h>
#include <tempSensor.h>
#include <ConnectionManager.h>
#include <TimeManager.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <MQTTManager.h>

LEDMatrix matrix;
TempSensor sensor(17, DHT11);
ConnectionManager connectionManager("Testiot", "itiswhatitis");
TimeManager timeManager("pool.ntp.org", 3600, 3600);

// MQTT Configuration
const char *mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char *mqttTopic = "test/topic/wiwilo";
const char *mqttClientId = "BillieClient7755";

// Création d'instances MQTT
WiFiClient espClient;
MQTTManager mqttManager(espClient, mqttServer, mqttPort, mqttClientId, mqttTopic);

// Function to handle Wi-Fi connection
void connectWiFi()
{
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("AMCDeviceAP", "zazawiwinono"))
  {
    Serial.println("⚠️ Wi-Fi Auto-Connect failed, restarting...");
    delay(3000);
    ESP.restart();
  }
}

void setup()
{
  Serial.begin(115200);

  // Initializations
  matrix.initialize();
  sensor.initialize();
  connectionManager.connect();
  timeManager.setupTime();

  // Setup Wi-Fi and MQTT connections
  connectWiFi();
  mqttManager.initialize();
  mqttManager.connect();
}

// Function to handle reconnection logic for Wi-Fi and MQTT
void handleConnections()
{
  if (!connectionManager.isConnected())
  {
    connectionManager.connect();
  }

  if (!mqttManager.isConnected())
  {
    mqttManager.connect();
  }
}

// Function to read temperature data
String readSensorData()
{
  sensor.readSensor();
  return String(sensor.getTemperature(false));
}

// Function to read humidity data
String readHumidity()
{
  return String(sensor.getHumidity());
}

// Function to update the LED matrix display
void displayData(String currentTime, String temp, String hum)
{
  matrix.update(currentTime + ", " + temp + " C");
  matrix.update(currentTime + ", " + hum + " %");
}

// Function to publish the data to MQTT
void publishMQTTMessage(String temp, String hum, String currentTime)
{
  String payload = "Temp: " + temp + " C, Hum: " + hum + " %, Time: " + currentTime;
  mqttManager.publishMessage(payload);
}

void loop()
{
  handleConnections();

  // Read sensor data
  String temp = readSensorData();
  String hum = readHumidity();
  String currentTime = timeManager.getCurrentTime();

  // Display on LED matrix
  displayData(currentTime, temp, hum);

  // Publish message to MQTT
  publishMQTTMessage(temp, hum, currentTime);

  mqttManager.loop();

  delay(1000); // Adjust this delay as needed
}
