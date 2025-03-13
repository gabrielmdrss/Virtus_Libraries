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


 /*  Inicialização do botão para teste  */
//gpio_init(6);
//gpio_set_dir(6, GPIO_IN);
//gpio_pull_up(6);

// Definições para o LED RGB nos GPIOs corretos
#define LED_RGB_RED 13
#define LED_RGB_GREEN 11
#define LED_RGB_BLUE 12

// Inicialização dos pinos dos LEDs
gpio_init(LED_RGB_RED);
gpio_set_dir(LED_RGB_RED, GPIO_OUT);
//gpio_put(LED_RGB_RED, false);

gpio_init(LED_RGB_GREEN);
gpio_set_dir(LED_RGB_GREEN, GPIO_OUT);
//gpio_put(LED_RGB_GREEN, false);

gpio_init(LED_RGB_BLUE);
gpio_set_dir(LED_RGB_BLUE, GPIO_OUT);
//gpio_put(LED_RGB_BLUE, false);
    
// Inicialização dos botões
init_button_IT(5);
init_button_IT(6);

    while (1) {

    // --------------------- PushButton's Example --------------------- //

    process_button_state(event_function);
    sleep_ms(10);

    // ---------------------- Joystick's Example ---------------------- //
        
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- LedMatrix's Example ---------------------- //

        //Test_LedMatrix_Wipe();
        //Test_LedMatrix_MovingSquare();
        //Test_LedMatrix_Spiral();

    // ---------------- Tests integrating all the libs ---------------- //

        //Test_joystick_LedMatrixControl(&joystick);
        Test_joystick_LedMatrixColorToggle(&joystick);

    // ----------------------------- LOG ------------------------------ //

        //log_values(&joystick);
        event = IDLE;

    }

    return 0;
}
