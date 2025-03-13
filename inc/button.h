/**
 * Virtus-CC
 * Universidade Federal de Campina Grande 
 * Biblioteca para implemenatação de funcionalidades para os Botões A e B da BitDogLab
 * Autores: Jezrael Pereira Filgueiras, Gabriel Domingos de Medeiros
 */
#ifndef BUTTONS_LIB_H
#define BUTTONS_LIB_H

#include "pico/stdlib.h"

// Definição de GPIOs para os botões
#define BUTTON_A 5
#define BUTTON_B 6

// Definição de atraso para debounce e clique duplo
#define DEBOUNCE 20
#define DOUBLE_CLICK_TIME_MS 300
#define LONG_PRESS_TIME_MS 1000

// Estrutura para armazenar o estado de cada botão
typedef struct {
    uint gpio;
    uint32_t last_time;
    bool last_state;
    uint32_t press_time;
    uint32_t last_click_time;
    bool long_press_detected;
} ButtonState;

typedef enum {
    IDLE,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_PRESS
} ButtonEvent;

// Funções básicas
ButtonState* get_button_state(char* button_name);
void init_button(char* button_name);
bool debounce_button(ButtonState* button_state);
bool long_press_button(ButtonState* button_state, uint32_t duration_ms);
bool double_click_button(ButtonState* button_state, uint32_t interval_ms);
void handle_button_IT(uint gpio, uint32_t events);
void init_button_IT(uint gpio);
void process_button_state(void (*event_handler)(ButtonEvent));

// Função auxiliar para fazer o LED piscar
void blink_led(uint gpio, uint32_t delay_ms, int times);

void event_function(ButtonEvent event);

#endif // BUTTONS_LIB_H