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
static uint32_t build_color(int red, int green, int blue) {
    red   = clamp(red,   0, 255);
    green = clamp(green, 0, 255);
    blue  = clamp(blue,  0, 255);
    // Ordem GRB: byte mais significativo = Green, seguido por Red e Blue.
    return ((uint32_t)green << 16) | ((uint32_t)red << 8) | ((uint32_t)blue);
}

void ws2812_init(ws2812_config_t *ws2812_config) {
    gpio_init(ws2812_config->pin);
    gpio_set_dir(ws2812_config->pin, 1);

    // Reset: Força o pino em nível baixo por tempo suficiente (>50µs)
    gpio_put(ws2812_config->pin, 0);
    sleep_us(300);  // 300µs para garantir

    // Inicializa a matriz com todos os LEDs apagados
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = build_color(0, 0, 0);
    }
    
    // Envia os dados zerados IMEDIATAMENTE após o reset
    ws2812_show(ws2812_config);
    sleep_us(500); // Garante tempo para processamento
}

void ws2812_set_pixel(ws2812_config_t *ws2812_config, uint32_t color) {
    // Envia os 24 bits iniciando pelo bit mais significativo (MSB)
    for (int bit = 23; bit >= 0; bit--) {
        if (color & (1 << bit)) {
            // Envia um "1"
            for (volatile int i = 0; i <= 10; i++) {
                gpio_put(ws2812_config->pin, 1);
            }
            for (volatile int i = 0; i <= 2; i++) {
                gpio_put(ws2812_config->pin, 0);
            }
        } else {
            // Envia um "0"
            for (volatile int i = 0; i <= 3; i++) {
                gpio_put(ws2812_config->pin, 1);
            }
            for (volatile int i = 0; i <= 10; i++) {
                gpio_put(ws2812_config->pin, 0);
            }
        }
    }
}

// Define a mesma cor para todos os LEDs da matriz, recebendo vermelho, verde e azul separadamente.
void ws2812_set_all(ws2812_config_t *ws2812_config, uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = build_color(red, green, blue);
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = color;
    }
}

// Define a cor de um LED específico na posição (x,y) com os valores vermelho, verde e azul.
// Os valores de x e y serão limitados para o intervalo [0, 4].
void ws2812_set_pixel_xy(ws2812_config_t *ws2812_config, int x, int y, int red, int green, int blue) {
    // Limita x e y para o intervalo [0,4]
    x = clamp(x, 0, 4);
    y = clamp(y, 0, 4);
    
    int index;
    // Mapeamento para snake wiring considerando:
    // - As fileiras são consideradas a partir do fundo (y = 0) para o topo (y = 4).
    // - Para fileiras pares (y par): 
    //     o LED inferior esquerdo (x=0) deve mapear para o último da fileira, 
    //     ou seja, índice = y*5 + (4 - x).
    // - Para fileiras ímpares (y ímpar):
    //     a numeração é direta: índice = y*5 + x.
    if (y % 2 == 0) {
        index = y * 5 + (4 - x);
    } else {
        index = y * 5 + x;
    }
    
    led_matrix[index] = build_color(red, green, blue);
}

// Envia os dados de toda a matriz para os LEDs.
void ws2812_show(ws2812_config_t *ws2812_config) {
    for (int i = 0; i < 25; i++) {
        ws2812_set_pixel(ws2812_config, led_matrix[i]);
    }
}