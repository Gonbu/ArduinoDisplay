#include <PubSubClient.h>
#include <LEDmatrix.h>
#include <queue>

class MQTTManager
{
private:
    PubSubClient mqttClient;
    LEDMatrix &LEDmatrix;
    std::queue<String> messageQueue;
    bool isPriorityMessageInProgress = false;
    const char *server;
    int port;
    const char *clientId;
    const char *topic;

public:
    MQTTManager(Client &espClient, const char *server, int port, const char *clientId, const char *topic, LEDMatrix &ledMatrix)
        : mqttClient(espClient), LEDmatrix(ledMatrix)
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
                // Configure le callback ici 👇
                mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length)
                                       { this->callback(topic, payload, length); });

                Serial.println("✅ Connecté à MQTT !");
                mqttClient.subscribe(topic);
                Serial.println("✅ Abonné.e au topic " + String(topic));
            }
            else
            {
                Serial.print("❌ Échec, rc=");
                Serial.print(mqttClient.state());
                Serial.println(" 🔄 Retente dans 5 secondes...");
                delay(5000);
            }
            this->loop();
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

    void enqueueMessage(const String &message)
    {
        messageQueue.push(message);
        Serial.println("📩 Message ajouté à la queue : " + message);
    }

    void processQueue()
    {
        if (!LEDmatrix.isAnimationInProgress() && !messageQueue.empty())
        {
            String nextMessage = messageQueue.front();
            messageQueue.pop();
            Serial.println("🚀 Affichage d'un message MQTT prioritaire : " + nextMessage);
            isPriorityMessageInProgress = true; // Un message MQTT est en cours
            LEDmatrix.startAnimation(nextMessage);
        }

        // Si la queue est vide et qu'aucune animation n'est en cours, réinitialise le flag
        if (messageQueue.empty() && !LEDmatrix.isAnimationInProgress())
        {
            isPriorityMessageInProgress = false;
        }
    }

    std::queue<String> getMessageQueue() const
    {
        return messageQueue;
    }

    bool getIsPriorityMessageInProgress() const
    {
        return isPriorityMessageInProgress;
    }

    void loop()
    {
        mqttClient.loop();
    }

    bool isConnected()
    {
        return mqttClient.connected();
    }

    void adminCommands(String command)
    {
        if (command == "reboot")
        {
            Serial.println("🔄 Redémarrage...");
            delay(1000);
            ESP.restart();
        }

        if (command == "clearHistory")
        {
            Serial.println("🧹 Historique effacé !");
        }
    }

    void callback(char *topic, byte *payload, unsigned int length)
    {
        String message = "";
        for (int i = 0; i < length; i++)
        {
            message += (char)payload[i];
        }
        enqueueMessage(message);
    }
};
