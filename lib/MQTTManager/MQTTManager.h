#include <PubSubClient.h>

class MQTTManager
{
private:
    PubSubClient mqttClient;
    const char *server;
    int port;
    const char *clientId;
    const char *topic;

public:
    MQTTManager(Client &espClient, const char *server, int port, const char *clientId, const char *topic)
        : mqttClient(espClient)
    {
        this->server = server;
        this->port = port;
        this->clientId = clientId;
        this->topic = topic;
    }

    void initialize()
    {
        mqttClient.setServer(server, port);
    }

    void connect()
    {
        while (!mqttClient.connected())
        {
            Serial.println("🌀 Tentative de connexion à MQTT...");

            if (mqttClient.connect(clientId))
            {
                Serial.println("✅ Connecté à MQTT !");
                mqttClient.subscribe(topic);
            }
            else
            {
                Serial.print("❌ Échec, rc=");
                Serial.print(mqttClient.state());
                Serial.println(" 🔄 Retente dans 5 secondes...");
                delay(5000);
            }
        }
    }

    void publishMessage(const String &message)
    {
        if (mqttClient.connected())
        {
            if (mqttClient.publish(topic, message.c_str(), false))
            {
                Serial.print("✅ Message publié : ");
                Serial.println(message);
            }
            else
            {
                Serial.println("❌ Échec de la publication !");
            }
        }
        else
        {
            Serial.println("⚠️ Impossible de publier, MQTT non connecté.");
        }
    }

    void loop()
    {
        mqttClient.loop();
    }

    bool isConnected()
    {
        return mqttClient.connected();
    }
};
