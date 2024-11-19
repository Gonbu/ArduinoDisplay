#include <WiFi.h>
#include <WiFiManager.h>

class ConnectionManager
{
private:
    const char *ssid;
    const char *password;
    WiFiClient espClient;
    WiFiManager wifiManager;
    const int maxAttempts = 10;

public:
    ConnectionManager(const char *ssid, const char *password)
        : ssid(ssid), password(password)
    {
        Serial.begin(115200);
    }

    void connect()
    {
        int networks = WiFi.scanNetworks();

        for (int i = 0; i < networks; i++)
        {
            String targetSSID = WiFi.SSID(i);
            Serial.print("Network: ");
            Serial.println(targetSSID);

            if (targetSSID == ssid)
            {
                Serial.println("Found matching target network");
                WiFi.begin(ssid, password);
                Serial.print("Connecting to target network");

                int attempts = maxAttempts;
                while (WiFi.status() != WL_CONNECTED && attempts > 0)
                {
                    delay(1000);
                    Serial.print(".");
                    attempts--;
                }
                Serial.println(".");

                if (WiFi.status() == WL_CONNECTED)
                {
                    Serial.println("Connected to target network");
                    return;
                }
                else
                {
                    Serial.println("Could not connect to target network");
                }
            }
        }

        Serial.println("Target network not found, trying autoconnect...");
        if (!wifiManager.autoConnect("AMCDeviceAP", "zazawiwinono"))
        {
            Serial.println("⚠️ Auto-Connection failed, restarting...");
            delay(3000);
            ESP.restart();
        }
    }

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    void checkConnection()
    {
        if (!isConnected())
        {
            Serial.println("WiFi connection lost. Attempting to reconnect...");
            connect();
        }
    }
};
