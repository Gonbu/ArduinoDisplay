#include "LEDmatrix.h"

LEDMatrix::LEDMatrix() : Display(HARDWARE_TYPE, CS_PIN, MAX_DEVICES) {}

void LEDMatrix::initialize()
{
    Display.begin();
    Display.setIntensity(0);
    Display.displayClear();
}

void LEDMatrix::update(String message)
{
    if (message.length() > 0)
    {
        Display.displayText(message.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        while (!Display.displayAnimate())
        {
        }
    }
}
