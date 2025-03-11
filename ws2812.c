/**
 * MIT License
 * 
 * Copyright (c) 2024 My Techno Talent
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file ws2812.c
 *
 * @brief Struct inits and functions for the ws2812 driver.
 * 
 * This contains the struct initializations and functions for the ws2812 driver 
 * for use with the Raspberry Pi Pico microcontroller board.
 *
 * @author Kevin Thomas
 * @date   04/11/2023
 */

#include "inc/ws2812.h"

// Declaração global para armazenar as cores dos 25 LEDs (5x5)
static uint32_t led_matrix[25] = {0};

PIO pio_ws = pio0;       // Aqui sim inicializa
uint sm_ws = 0;          // E aqui também

ws2812_config_t ws2812_config = {
    .pin = WS2812
};

// Função auxiliar para limitar o valor entre min e max.
static int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// Função auxiliar para montar a cor em formato 0xG R B, 
// onde os LEDs esperam os dados em ordem GRB.
// Trata valores acima de 255, limitando-os.
static uint32_t build_color(led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Ordem GRB: o byte mais significativo é o verde
    return ((uint32_t)green << 16) | ((uint32_t)red << 8) | ((uint32_t)blue);
}


void ws2812_init(ws2812_config_t *ws2812_config) {
    // Carrega e configura programa PIO (ws2818b_program vem de ws2818b.pio.h)
    uint offset = pio_add_program(pio_ws, &ws2818b_program);
    sm_ws = pio_claim_unused_sm(pio_ws, true);
    ws2818b_program_init(pio_ws, sm_ws, offset, ws2812_config->pin, 800000.f);

    // Limpa o array e envia zeros
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = 0;
    }
    ws2812_show(ws2812_config);
}

// Agora, em vez de bit-banging, set_pixel apenas gravará no buffer
// se você quiser realmente controlar um LED isolado (opcional).
void ws2812_set_pixel(ws2812_config_t *ws2812_config, uint32_t color) {
    led_matrix[0] = color; // Exemplo: trocar apenas o índice 0
}

// Define a mesma cor para todos os LEDs (sem loops de GPIO).
void ws2812_set_all(ws2812_config_t *ws2812_config, led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    uint32_t color = build_color(red, green, blue);
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = color;
    }
}

// Ajusta um LED específico (x,y) no buffer, respeitando o snake wiring.
void ws2812_set_pixel_xy(ws2812_config_t *ws2812_config, int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Limita x e y no intervalo [0,4]
    x = clamp(x, 0, 4);
    y = clamp(y, 0, 4);
    int index;
    if (y % 2 == 0) {
        index = y * 5 + (4 - x);
    } else {
        index = y * 5 + x;
    }
    led_matrix[index] = build_color(red, green, blue);
}

// Envia o buffer usando o FIFO da PIO, em formato GRB.
void ws2812_show(ws2812_config_t *ws2812_config) {
    for (int i = 0; i < 25; i++) {
        uint32_t c = led_matrix[i];
        uint8_t g = (c >> 16) & 0xFF;
        uint8_t r = (c >>  8) & 0xFF;
        uint8_t b =  c        & 0xFF;
        pio_sm_put_blocking(pio_ws, sm_ws, g);
        pio_sm_put_blocking(pio_ws, sm_ws, r);
        pio_sm_put_blocking(pio_ws, sm_ws, b);
    }
    // Pequeno atraso pós-transmissão
    sleep_us(300);
}
