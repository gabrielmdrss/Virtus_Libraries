#include <stdio.h>
#include "pico/stdlib.h"
#include "examples.h"

int main()
{
    stdio_init_all();

    JoystickState joystick = {0};   // Instantiate the zeroed structure
    Joystick_Init();                // Initializes the ADC for the Joystick
    // NeoPixel_Init(IN_MATRIX);       // Initializes the PIO machine on IN_MATRIX PIN (7)

    // NeoPixel_FillAll(0, 0, 0);

    const uint8_t DELAY = 70;

    ws2812_init(&ws2812_config);

    // Envia dados zerados duas vezes para garantir
    ws2812_set_all(&ws2812_config, 0, 0, 0);
    ws2812_show(&ws2812_config);
    sleep_ms(10);
    ws2812_set_all(&ws2812_config, 0, 0, 0);
    ws2812_show(&ws2812_config);
    sleep_ms(10);

    while (1) {

    // ---------------------- Testes do Joystick ---------------------- //
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- Testes do LedMatrix --------------------- //
        //walkingLed();
        //Example_BrightnessRamp();

        for (int i=0; i<5; i++)
        {
            for (int j=0; j<5; j++)
            {
                ws2812_set_pixel_xy(&ws2812_config, j, i, 50, 0, 0);
                ws2812_show(&ws2812_config);
                sleep_ms(DELAY);
                ws2812_set_all(&ws2812_config, 0, 0, 0);
            }
            
        }

    }

    return 0;
}
