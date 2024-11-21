#include "LEDmatrix.h"

LEDMatrix::LEDMatrix() : Display(HARDWARE_TYPE, CS_PIN, MAX_DEVICES) {}

void LEDMatrix::initialize()
{
    Display.begin();
    Display.setIntensity(0);
    Display.displayClear();
}

void LEDMatrix::startAnimation(String message)
{
    if (message.length() > 0)
    {
        char *msg = new char[message.length() + 1];
        strcpy(msg, message.c_str());

        if (animationInProgress)
        {
            Display.displayClear();
            Display.displayReset();
        }

        Display.displayText(msg, PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        animationInProgress = true;
    }
}

bool LEDMatrix::stepAnimation()
{
    if (animationInProgress)
    {
        if (Display.displayAnimate())
        {
            animationInProgress = false;
        }
    }
    return animationInProgress;
}

bool LEDMatrix::isAnimationInProgress() const
{
    return animationInProgress;
}
