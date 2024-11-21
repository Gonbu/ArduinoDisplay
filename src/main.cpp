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
MQTTManager mqttManager(espClient, mqttServer, mqttPort, mqttClientId, mqttTopic, matrix);

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

void loop()
{
  handleConnections();
  mqttManager.loop();

  // Si aucune animation n’est en cours, afficher l’heure
  if (!matrix.isAnimationInProgress())
  {
    if (!mqttManager.getMessageQueue().empty() || mqttManager.getIsPriorityMessageInProgress())
    {
      // Si des messages MQTT sont en attente ou en cours, traiter la queue
      mqttManager.processQueue();
    }
    else
    {
      // Sinon, afficher les données par défaut
      static unsigned long lastUpdate = 0;
      unsigned long now = millis();

      // Actualisation toutes les 5 secondes
      if (now - lastUpdate >= 5000)
      {
        String temp = readSensorData();
        String hum = readHumidity();
        String currentTime = timeManager.getCurrentTime();
        String message = currentTime + ", " + temp + " C, " + hum + " %";

        Serial.println("🕒 Affichage des données par défaut : " + message);

        matrix.startAnimation(message);
        lastUpdate = now;
      }
    }
  }
  else
  {
    matrix.stepAnimation();
  }
}
