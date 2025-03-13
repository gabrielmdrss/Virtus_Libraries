#include <stdio.h>
#include "pico/stdlib.h"
#include "examples.h"

int main()
{
    stdio_init_all();               // Initializes the UART for printing

    JoystickState joystick = {0};   // Instantiate the zeroed structure
    Joystick_Init(&joystick);       // Initializes the ADC for the Joystick
    LedMatrix_Init(&ws2812_config); // Initializes the LED Matrix

    LedMatrix_Clear();
    LedMatrix_Update();

    // Definições para o LED RGB nos GPIOs corretos
    #define LED_RGB_RED 13
    #define LED_RGB_GREEN 11
    #define LED_RGB_BLUE 12

    // Initializing LED pins
    gpio_init(LED_RGB_RED);
    gpio_set_dir(LED_RGB_RED, GPIO_OUT);
    //gpio_put(LED_RGB_RED, false);

    gpio_init(LED_RGB_GREEN);
    gpio_set_dir(LED_RGB_GREEN, GPIO_OUT);
    //gpio_put(LED_RGB_GREEN, false);

    gpio_init(LED_RGB_BLUE);
    gpio_set_dir(LED_RGB_BLUE, GPIO_OUT);
    //gpio_put(LED_RGB_BLUE, false);
        
    // Initializing buttons for use with the interrupt
    init_button_IT(5);
    init_button_IT(6);

    //test_Button = 1;    // Variable that will tell the test(1 = test interrupt button, 0 = test joystick/ledmatrix)
    test_Button = 0;

    while (1) {

    // --------------------- PushButton's Example --------------------- //

        process_button_state(event_function);

    // ---------------------- Joystick's Example ---------------------- //
        
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- LedMatrix's Example ---------------------- //

        //Test_LedMatrix_Wipe();
        //Test_LedMatrix_MovingSquare();
        //Test_LedMatrix_Spiral();

    // ---------------- Tests integrating all the libs ---------------- //

        Test_joystick_LedMatrixControl(&joystick);
        //Test_joystick_LedMatrixColorToggle(&joystick);

    // ----------------------------- LOG ------------------------------ //

        //log_values(&joystick);
        event = IDLE;

    }

    return 0;
}
