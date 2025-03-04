#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/joystick.h"



int main()
{
    stdio_init_all();

    JoystickState joystick = {0}; // Instancia a estrutura zerada
    Joystick_Init();

    while (1) {
        Joystick_Read(&joystick);
        Joystick_ApplyFilters(&joystick);

        printf("Bruto: X=%d, Y=%d | Filtrado: X=%d, Y=%d\n",
            joystick.x_raw, joystick.y_raw,
            joystick.x_filtered, joystick.y_filtered);
        sleep_ms(50);
    }

    return 0;
}
