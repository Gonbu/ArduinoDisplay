#include "MicManager.h"

// Constructeur
MicManager::MicManager(int pin, float threshold) : micPin(pin), threshold(threshold) {}

// Initialisation
void MicManager::begin() {
    pinMode(micPin, INPUT);
}

// Lire le signal brut
int MicManager::readRaw() {
    return analogRead(micPin); // Lecture du signal analogique
}

// Détecter si le son dépasse le seuil
bool MicManager::detectSound() {
    return readRaw() > (threshold * 4095); // Normalisation sur 12 bits
}

// Ajuster le seuil
void MicManager::setThreshold(float newThreshold) {
    threshold = newThreshold;
}
