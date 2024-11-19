#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
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
    void update(String message);

private:
    MD_Parola Display;
};

#endif // LEDMATRIX_H
