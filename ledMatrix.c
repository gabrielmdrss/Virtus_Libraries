#include "inc/ledMatrix.h"

/* 
 * Global 5x5 matrix storing RGB values for each position. 
 * Each position has [R, G, B].
 */
int matriz[5][5][3] = {
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

LED leds[LED_COUNT];    // Buffer containing the LEDs to be written to the strip.
PIO np_pio;             // PIO instance used for driving the LED strip.
uint sm;                // State machine index used on the specified PIO.

/* Assigns an RGB color to a specific LED at the given index. */
void NeoPixel_SetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

/* 
 * Writes the data from the LED buffer to the LED strip.
 * Sends the R, G, and B values of each LED via the configured PIO.
 */
void NeoPixel_Write() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

/* 
 * Clears all LEDs in the buffer by setting them to zero, 
 * then updates the strip to turn them off.
 */
void NeoPixel_Clear() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        NeoPixel_SetLED(i, 0, 0, 0);
    }
    NeoPixel_Write();
}

/* 
 * Initializes the PIO for controlling the LED matrix 
 * at the specified pin, and clears all pixels immediately.
 */
void NeoPixel_Init(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);

    /* Configure frequency and start the PIO program. */
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    /* Turn off all LEDs so the strip starts blank. */
    NeoPixel_Clear();
}

/* 
 * Converts the (x, y) coordinates from the 5x5 matrix 
 * into the corresponding index in the LED buffer.
 */
int NeoPixel_GetIndex(int x, int y) {
    if (y % 2 == 0) {
        /* Even rows go left to right. */
        return y * 5 + x;
    } else {
        /* Odd rows go right to left. */
        return y * 5 + (4 - x);
    }
}