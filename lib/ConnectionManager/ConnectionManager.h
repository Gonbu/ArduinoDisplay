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
    }

    void connect()
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.print("Connexion Wi-Fi.");
            WiFi.begin(ssid, password);
            int attempts = maxAttempts;
            while (WiFi.status() != WL_CONNECTED && attempts > 0)
            {
                delay(1000);
                Serial.print(".");
                attempts--;
            }
            Serial.println();
            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("Connecté au Wi-Fi !");
            }
            else
            {
                Serial.println("Impossible de se connecter au Wi-Fi.");
                wifiManager.autoConnect("AMCDeviceAP", "zazawiwinono");
            }
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
