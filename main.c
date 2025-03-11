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
    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5);

    int cont = 1;

    while (1) {

    // ---------------------- Testes do Joystick ---------------------- //
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- Testes do LedMatrix --------------------- //
        //walkingLed();
        //Example_BrightnessRamp();

        // for (int i=0; i<5; i++)
        // {
        //     for (int j=0; j<5; j++)
        //     {
        //         ws2812_set_pixel_xy(&ws2812_config, j, i, 255, 0, 0);
        //         ws2812_show(&ws2812_config);
        //         sleep_ms(DELAY);
        //         ws2812_set_all(&ws2812_config, 0, 0, 0);
        //     }
            
        // }

        if(!gpio_get(5))
        {
            printf("Botão pressionado\n");
        }

        // ws2812_set_pixel_xy(&ws2812_config, 2, 2, 0, 4, 2);
        // ws2812_show(&ws2812_config);
        // printf("4\n");
        // sleep_ms(DELAY*5); 

        // ws2812_set_pixel_xy(&ws2812_config, 2, 2, 4, 4, 2);
        // ws2812_show(&ws2812_config);
        // printf("4\n");
        // sleep_ms(DELAY*5); 

        // ws2812_set_pixel_xy(&ws2812_config, 2, 2, 2, 4, 2);
        // ws2812_show(&ws2812_config);
        // printf("2");
        // sleep_ms(DELAY*5); 

        // ws2812_set_pixel_xy(&ws2812_config, 2, 2, 1, 4, 2);
        // ws2812_show(&ws2812_config);
        // printf("1\n");
        // sleep_ms(DELAY*5); 

        // ws2812_set_pixel_xy(&ws2812_config, 2, 2, 3, 4, 2);
        // ws2812_show(&ws2812_config);
        // printf("3");
        
        ws2812_set_pixel_xy(&ws2812_config, 2, 2, LED_MEDIUM, LED_OFF, LED_OFF);
        ws2812_show(&ws2812_config);
        sleep_ms(DELAY*5); 

        ws2812_set_pixel_xy(&ws2812_config, 2, 2, LED_OFF, LED_OFF, LED_OFF);
        ws2812_show(&ws2812_config);
        sleep_ms(DELAY*5);


    }

    return 0;
}
