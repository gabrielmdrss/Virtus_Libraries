#include <stdio.h>
#include "pico/stdlib.h"
#include "Buttons_Lib.h"

// Definições para o LED RGB nos GPIOs corretos
#define LED_RGB_RED 13
#define LED_RGB_GREEN 11
#define LED_RGB_BLUE 12

// Função auxiliar para fazer o LED piscar
void blink_led(uint gpio, uint32_t delay_ms, int times) {
    for (int i = 0; i < times; i++) {
        gpio_put(gpio, true);  // Liga o LED
        sleep_ms(delay_ms);
        gpio_put(gpio, false); // Desliga o LED
        sleep_ms(delay_ms);
    }
}

void event_function(ButtonEvent event){
    if(event == SINGLE_CLICK){
        blink_led(12, 100, 5);
    }else if(event = DOUBLE_CLICK){
        blink_led(11, 100, 5);
    }else if(event == LONG_PRESS){
        blink_led(13, 100, 5);
    }
}

int main() {
    // Inicialização geral
    stdio_init_all();

    // Inicialização dos pinos dos LEDs
    gpio_init(LED_RGB_RED);
    gpio_set_dir(LED_RGB_RED, GPIO_OUT);

    gpio_init(LED_RGB_GREEN);
    gpio_set_dir(LED_RGB_GREEN, GPIO_OUT);

    gpio_init(LED_RGB_BLUE);
    gpio_set_dir(LED_RGB_BLUE, GPIO_OUT);

    // Inicialização dos botões
    init_button_IT(BUTTON_A);

    while (true) {
        
        process_button_state(event_function);
        sleep_ms(10);
    }

    return 0;
}
