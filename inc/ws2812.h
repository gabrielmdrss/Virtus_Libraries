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
 * @file ws2812.h
 * 
 * @brief Constant struct and function prototypes for the ws2812 driver.
 * 
 * This contains the constants, struct and function prototypes for the ws2812 
 * driver for use with the Raspberry Pi Pico microcontroller board.
 *   
 * @author Kevin Thomas
 * @date   04/11/2024
*/

#ifndef _WS2812_H
#define _WS2812_H

#include "pico/stdlib.h"
#include "ws2818b.pio.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

/** 
 * @brief Defines ws2812 LED config value.
 * 
 * These config defines are used to target ws2812 LED pin.
 */
#define WS2812                           7 

/**
 * @brief Structure for WS2812 LED configuration.
 *
 * This structure defines the configuration for controlling WS2812 LEDs.
 */
typedef struct {
    uint8_t pin;
} ws2812_config_t;

/**
 * @brief Enumeration for LED intensity levels.
 *
 * This enumeration defines the intensity levels for controlling the brightness
 * of WS2812 LEDs.
 */
typedef enum {
    LED_OFF = 0,        // Desligado
    LED_WEAK = 4,       // Fraco
    LED_MEDIUM = 2,     // Intermediário
    LED_STRONG = 1,     // Forte
    LED_MAX = 3         // Muito forte
} led_intensity_t;

/**
 * @brief External declaration of WS2812 LED configuration.
 *
 * This declares an external variable `ws2812_config` of type `ws2812_config_t`,
 * which holds the configuration settings for controlling WS2812 LEDs.
 * The actual definition of this variable is expected to be provided in another 
 * source file.
 */
extern ws2812_config_t ws2812_config;

extern PIO pio_ws;
extern uint sm_ws;

/**
 * @brief Initializes WS2812 LED control.
 *
 * This function initializes the GPIO pin specified in the provided `ws2812_config`
 * for controlling WS2812 LEDs. It sets up the GPIO pin direction for output.
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @return None
 */
void ws2812_init(ws2812_config_t *ws2812_config);

/**
 * @brief Sets a pixel on a WS2812 LED strip with the specified color.
 *
 * This function sends color data to a WS2812 LED strip using a specified GPIO pin.
 * The color data is represented by the `set_color` parameter, where each bit
 * corresponds to a color component (e.g., Red, Green, Blue).
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @param  set_color     Color value to be transmitted to the WS2812 LED.
 * @return None
 */
void ws2812_set_pixel(ws2812_config_t *ws2812_config, uint32_t set_color);

// Define a mesma cor para todos os LEDs da matriz, recebendo R, G e B separadamente.
void ws2812_set_all(ws2812_config_t *ws2812_config, led_intensity_t red, led_intensity_t green, led_intensity_t blue);

// Define a cor de um LED específico (posição x,y onde x e y variam de 0 a 4)
// recebendo os valores de R, G e B separadamente.
void ws2812_set_pixel_xy(ws2812_config_t *ws2812_config, int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue);

// Envia os dados de toda a matriz para os LEDs.
void ws2812_show(ws2812_config_t *ws2812_config);

// Função auxiliar para limitar o valor entre min e max.
static int clamp(int val, int min, int max);

// Função auxiliar para montar a cor no formato 0xGRB, com tratamento para valores fora do intervalo [0,255].
static uint32_t build_color(led_intensity_t red, led_intensity_t green, led_intensity_t blue);

#endif /* _WS2812_H */
