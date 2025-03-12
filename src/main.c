#include <stdio.h>
#include "pico/stdlib.h"
#include "examples.h"

int main()
{
    stdio_init_all();               // Initializes the UART for printing

    JoystickState joystick = {0};   // Instantiate the zeroed structure
    Joystick_Init();                // Initializes the ADC for the Joystick

    LedMatrix_Init(&ws2812_config); // Initializes the LED Matrix

    while (1) {

    // --------------------- PushButton's Example --------------------- //

        // Testes

    // ---------------------- Joystick's Example ---------------------- //
        
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- LedMatrix's Example ---------------------- //

        //Test_LedMatrix_Wipe();
        //Test_LedMatrix_MovingSquare();
        Test_LedMatrix_Spiral();

    }

    return 0;
}
