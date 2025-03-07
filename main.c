#include <stdio.h>
#include "pico/stdlib.h"
#include "examples.h"

int main()
{
    stdio_init_all();

    JoystickState joystick = {0};   // Instantiate the zeroed structure
    Joystick_Init();                // Initializes the ADC for the Joystick
    NeoPixel_Init(IN_MATRIX);       // Initializes the PIO machine on IN_MATRIX PIN (7)

    while (1) {

    // ---------------------- Testes do Joystick ---------------------- //
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- Testes do LedMatrix --------------------- //
        ledDeslizante();
    }

    return 0;
}
