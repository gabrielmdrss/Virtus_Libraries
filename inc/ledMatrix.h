/******************************************************************************
 * @file    LEDMATRIX.H
 * @brief   Header file containing functions for controlling the LED Matrix.
 *
 * @authors Gabriel Domingos de Medeiros, Jezreal Pereira Filgueiras
 * @date    March 2025
 * @version 1.0.0
 *
 * @note    This file declares the functions required to set LED colors, clear
 *          the buffer, write data to the LED strip, initialize the PIO machine,
 *          and convert matrix positions into buffer indices.
 *
 *****************************************************************************/

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Library generated by the .pio file during compilation.
#include "ws2818b.pio.h"

#define LED_COUNT 25    // Total number of LEDs in the matrix.
#define IN_MATRIX 7     // Pin used to send data to the LED matrix.

/**
 * @brief Structure defining each LED's color.
 */
typedef struct {
    uint8_t R; ///< Red component
    uint8_t G; ///< Green component 
    uint8_t B; ///< Blue component
} LED;

extern int matriz[5][5][3]; // Global 5x5 matrix storing RGB values for each position.
extern LED leds[LED_COUNT]; // Buffer containing the LEDs to be written to the strip.
extern PIO np_pio;          // PIO instance used for driving the LED strip.
extern uint sm;             // State machine index used on the specified PIO.

/**
 * @brief Assigns an RGB color to a specific LED index in the buffer.
 * @param index LED index in the buffer.
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 */
void NeoPixel_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

/**
 * @brief Sends the LED buffer data to the strip.
 */
void NeoPixel_Write();

/**
 * @brief Clears all LEDs, turning them off in the buffer and updating the strip.
 */
void NeoPixel_Clear();

/**
 * @brief Initializes the PIO for controlling the LED matrix at a specified pin.
 * @param pin The GPIO pin used to communicate with the LED strip.
 */
void NeoPixel_Init(uint pin);

/**
 * @brief Converts matrix coordinates (x, y) into the corresponding buffer index.
 * @param x Row of the matrix.
 * @param y Column of the matrix.
 * @return The buffer index that corresponds to the (x, y) position.
 */
int NeoPixel_GetIndex(int x, int y);

#endif /*LEDMATRIX_H*/