#include <stdio.h>
#include "pico/stdlib.h"
#include "examples.h"



int main()
{
    stdio_init_all();

    JoystickState joystick = {0}; // Instancia a estrutura zerada
    Joystick_Init();

    while (1) {

        Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);
    }

    return 0;
}
