#ifndef MICMANAGER_H
#define MICMANAGER_H

#include <Arduino.h>

class MicManager {
private:
    int micPin;       // Pin analogique pour le KY-038
    float threshold;  // Seuil pour détecter un son significatif (en option)

public:
    // Constructeur
    MicManager(int pin, float threshold = 0.5);

    // Initialisation
    void begin();

    // Lire le signal brut (analogique)
    int readRaw();

    // Détecter si le son dépasse le seuil
    bool detectSound();

    // Ajuster le seuil
    void setThreshold(float newThreshold);
};

#endif // MICMANAGER_H
