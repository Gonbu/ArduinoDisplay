#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <MD_Parola.h>
#include <Arduino.h>
#include <SPI.h>

// Configuration matérielle
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 21

class LEDMatrix
{
public:
    LEDMatrix();
    void initialize();
    void startAnimation(String message);
    bool stepAnimation();
    bool isAnimationInProgress() const;

private:
    MD_Parola Display;
    bool animationInProgress = false;
};

#endif // LEDMATRIX_H
