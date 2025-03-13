#include <stdio.h>
#include "inc/button.h"
#include <string.h>

ButtonState button_a_state = {BUTTON_A, 0, true, 0, 0};
ButtonState button_b_state = {BUTTON_B, 0, true, 0, 0};

volatile uint32_t last_press_time = 0;
volatile uint32_t last_release_time = 0;
volatile uint32_t first_release_time = 0;
volatile ButtonEvent event = IDLE;
volatile uint8_t click_counter = 0;

// Função auxiliar para obter o estado do botão pelo nome
ButtonState* get_button_state(char* button_name) {
    if (strcmp(button_name, "BUTTON_A") == 0) return &button_a_state;
    if (strcmp(button_name, "BUTTON_B") == 0) return &button_b_state;
    return NULL;
}

//Função de Inicialização
void init_button(char* button_name) {
    ButtonState* button_state = get_button_state(button_name);
    if (button_state != NULL) {
        gpio_init(button_state->gpio);
        gpio_set_dir(button_state->gpio, GPIO_IN);
        gpio_pull_up(button_state->gpio);
    }
}

// Função de detecção com debounce
bool debounce_button(ButtonState* button_state) {
    bool current_state = gpio_get(button_state->gpio);
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_state != button_state->last_state) {
        button_state->last_time = current_time;
    }

    if ((current_time - button_state->last_time) > DEBOUNCE) {
        if (current_state != button_state->last_state) {
            button_state->last_state = current_state;
            return current_state;
        }
    }

    return button_state->last_state;
}

// Função para detectar pressionamento longo
bool long_press_button(ButtonState* button_state, uint32_t duration_ms) {
    bool button_state_value = !gpio_get(button_state->gpio);  // Assumindo pull-up, portanto invertido

    if (button_state_value) {
        if (button_state->press_time == 0) {
            button_state->press_time = to_ms_since_boot(get_absolute_time());
        } else if (to_ms_since_boot(get_absolute_time()) - button_state->press_time > duration_ms && !button_state->long_press_detected) {
            button_state->long_press_detected = true; // Marcar que o pressionamento longo foi detectado
            return true; // Pressionamento longo detectado
        }
    } else {
        button_state->press_time = 0;  // Resetar tempo quando botão é solto
        button_state->long_press_detected = false; // Resetar a marcação quando o botão é solto
    }

    return false;
}

// Função para detectar clique duplo
bool double_click_button(ButtonState* button_state, uint32_t interval_ms) {
    bool button_state_value = !gpio_get(button_state->gpio);

    if (button_state_value) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        if ((current_time - button_state->last_click_time) <= interval_ms) {
            button_state->last_click_time = 0; // Resetar após duplo clique detectado
            return true;
        }
        button_state->last_click_time = current_time;
    }

    return false;
}

void init_button_IT(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
    gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, handle_button_IT);
}

void handle_button_IT(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (events & GPIO_IRQ_EDGE_FALL) { // Borda de descida
        last_press_time = current_time;
        if(last_press_time-last_release_time<=DEBOUNCE){
            return;
        }
    } else if (events & GPIO_IRQ_EDGE_RISE) { // Borda de subida
        uint32_t press_duration = current_time - last_press_time;
        if(press_duration <= DEBOUNCE){
            return;
        }

        
        else if (press_duration >= LONG_PRESS_TIME_MS) {
            event = LONG_PRESS;
            click_counter = 0;
        }else{
            click_counter++;
            first_release_time = last_release_time;
            last_release_time = current_time;
        }
    }
}

void process_button_state(void (*event_handler)(ButtonEvent)) {
    uint32_t btwn_edge_time = last_press_time-first_release_time;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if(click_counter == 2 && (btwn_edge_time>DEBOUNCE) && (btwn_edge_time <= DOUBLE_CLICK_TIME_MS)){
         event = DOUBLE_CLICK;
         click_counter = 0;
        } else if(click_counter = 1 && current_time-last_press_time>DOUBLE_CLICK_TIME_MS){
            printf("Single Click\n");
            event = SINGLE_CLICK;
            click_counter = 0;
        }
    if (event != IDLE) {
        event_handler(event); // Chama a função de tratamento do evento
        event = IDLE;         // Redefine o estado para IDLE após executar a ação
    }
}

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