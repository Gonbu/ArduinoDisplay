#include <Arduino.h>
#include <HWCDC.h>
#include <time.h>

class TimeManager
{
private:
    const char *ntpServer;
    long gmtOffset_sec;
    int daylightOffset_sec;

public:
    TimeManager(const char *ntpServer, long gmtOffset_sec, int daylightOffset_sec)
    {
        this->ntpServer = ntpServer;
        this->gmtOffset_sec = gmtOffset_sec;
        this->daylightOffset_sec = daylightOffset_sec;
    }

    void setupTime()
    {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        Serial.println("Synchronisation de l'heure...");

        // Attendre que l'heure soit synchronisée
        struct tm timeinfo;
        for (int i = 0; i < 10; i++)
        { // Réessaye pendant 10 secondes
            if (getLocalTime(&timeinfo))
            {
                Serial.println("Heure synchronisée !");
                return;
            }
            Serial.println("En attente de la synchronisation...");
            delay(1000);
        }
        Serial.println("Échec de la synchronisation de l'heure.");
    }

    String getCurrentTime()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Impossible d'obtenir l'heure.");
            return "00:00";
        }
        char timeString[6];
        strftime(timeString, sizeof(timeString), "%H:%M", &timeinfo);
        return String(timeString);
    }
};
