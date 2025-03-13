/******************************************************************************
 * @file    examples.h
 * @brief   Demonstration header file with test examples of the libraries.
 *
 * @authors 
 * @date    March 2025
 * @version 1.0.0
 *
 * @note    This file contains simple functions that demonstrate how to use
 *          the About LED Matrix, Push-Buttons and Joystick Libraries.
 ******************************************************************************/

#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/joystick.h"
#include "inc/ledMatrix.h"

#define ADC_UPPER_THRESHOLD_X 2900 	// Limite superior do ADC no eixo X direita
#define ADC_LOWER_THRESHOLD_X 1300  // Limite inferior do ADC no eixo X esquerda
#define ADC_UPPER_THRESHOLD_Y 2800 	// Limite superior do ADC no eixo Y cima
#define ADC_LOWER_THRESHOLD_Y 1300 	// Limite inferior do ADC no eixo Y baixo

/***************************** Joystick Examples *****************************/

/* Tests the joystick reading and applies all filters. */
void Test_joystick_Complete(JoystickState *js){
    
    // Read the joystick values
    Joystick_Read(js);
        
    // Apply all configured filters
    Joystick_ApplyFilters(js);

    printf("Bruto: X=%d, Y=%d || Filtrado: X=%d, Y=%d\n",
        js->x_raw, js->y_raw,
        js->x_filtered, js->y_filtered);
    sleep_ms(50);
}


/* Tests only the dead zone filter for the joystick. */
void Test_joystick_DeadZone(JoystickState *js) {

    // Read the joystick values
    Joystick_Read(js);

    // Apply only the dead zone filter
    js->x_filtered = Joystick_ApplyDeadZone(js->x_raw);
    js->y_filtered = Joystick_ApplyDeadZone(js->y_raw);

    printf("Bruto: X=%d, Y=%d || Filtrado: X=%d, Y=%d  || DEADZONE=%d\n",
        js->x_raw, js->y_raw,
        js->x_filtered, js->y_filtered, DEADZONE_THRESHOLD);
    sleep_ms(50);
}


/* Tests only the low-pass filter for the joystick. */
void Test_joystick_LowpassFilter(JoystickState *js) {

    // Read the joystick values
    Joystick_Read(js);

    // Apply only the low-pass filter
    js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->prev_x);
    js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->prev_y);

    printf("Bruto: X=%d, Y=%d || Filtrado: X=%d, Y=%d  || ALPHA=%.1f\n",
        js->x_raw, js->y_raw,
        js->x_filtered, js->y_filtered, LOWPASS_ALPHA);

    // Update previous values
    js->prev_x = js->x_filtered;
    js->prev_y = js->y_filtered;
    sleep_ms(50);
}

/**************************** LED Matrix Examples ****************************/

/* Lights each LED sequentially across the matrix ("wipe" effect). */
void Test_LedMatrix_Wipe(void) {
    // Clear the matrix first.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(100);

    // Iterate through each coordinate in the 5x5 matrix.
    for (int index = 0; index < 25; index++) {
        // Compute x and y from the linear index.
        int x = index % 5;
        int y = index / 5;
        
        // Set the current LED to red at maximum intensity.
        LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_OFF);
        LedMatrix_Update();
        sleep_ms(200);
    }
    
    sleep_ms(500);
    // Clear the matrix after the wipe.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
}

/* Animates a 2x2 square moving across the LED matrix. */
void Test_LedMatrix_MovingSquare(void) {
    // Traverse rows and columns to animate the square.
    // The square will move from top-left (0,0) to bottom-right (3,3).
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            LedMatrix_Clear();
            // Fill a 2x2 rectangle starting at (x,y) with a blend of intensities.
            LedMatrix_FillRectangle(x, y, x+1, y+1, LED_WEAK, LED_WEAK, LED_WEAK);
            LedMatrix_Update();
            sleep_ms(300);
        }
    }
    
    // Clear the matrix after the animation.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
}

/**
 * @brief Animates a spiral effect on the LED matrix.
 */
void Test_LedMatrix_Spiral(void) {
    // Ordem espiral para [x,y] na matriz 5x5.
    int spiral_order[25][2] = {
        {0,0}, {1,0}, {2,0}, {3,0}, {4,0},
        {4,1}, {4,2}, {4,3}, {4,4},
        {3,4}, {2,4}, {1,4}, {0,4},
        {0,3}, {0,2}, {0,1},
        {1,1}, {2,1}, {3,1},
        {3,2}, {3,3},
        {2,3}, {1,3},
        {1,2}, {2,2}
    };
    
    // Limpa a matriz.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
    
    // Acende LEDs na ordem espiral.
    for (int i = 0; i < 25; i++) {
        int x = spiral_order[i][0];
        int y = spiral_order[i][1];
        LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_WEAK);  // exemplo: roxo
        LedMatrix_Update();
        sleep_ms(150);
    }
    sleep_ms(500);

    // Turn off the LEDs one by one using the same spiral order.
    for (int i = 24; i > 0; i--) {
        int x = spiral_order[i][0];
        int y = spiral_order[i][1];
        LedMatrix_SetPixel(x, y, LED_OFF, LED_OFF, LED_OFF);  // Turn off the LED.
        LedMatrix_Update();
        sleep_ms(150);
    }
    
    sleep_ms(200);
}


/*************************** Push-Buttons Examples ***************************/

/*********************** Tests integrating all the libs **********************/


/* Moves an LED around the matrix based on joystick input. */
void Test_joystick_LedMatrixControl(JoystickState *js) {
    static int x = 2;
    static int y = 2;
    static bool ledOn[5][5] = {false};  // Tracks which LEDs remain lit

    // Read and filter joystick values
    Joystick_Read(js);
    js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
    js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

    // Adjust X position
    if (js->x_filtered > ADC_UPPER_THRESHOLD_X && x < 4) {
        x++;
    } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && x > 0) {
        x--;
    }

    // Adjust Y position
    if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && y < 4) {
        y++;
    } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && y > 0) {
        y--;
    }

    // Check if button is pressed (pull-up assumed, pressed reads 0)
    if (!gpio_get(5)) {
        ledOn[y][x] = true;  // Keep this LED lit permanently
        sleep_ms(200);       // Debounce
    }

    // Print current info
    printf("x=%d, y=%d\r\n", x, y);
    printf("filtered x=%d, y=%d\r\n", js->x_filtered, js->y_filtered);
    printf("raw x=%d, y=%d\r\n", js->x_raw, js->y_raw);

    // Clear the matrix
    LedMatrix_Clear();

    // Turn on any previously lit LEDs
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (ledOn[row][col]) {
                LedMatrix_SetPixel(col, row, LED_WEAK, LED_OFF, LED_OFF);
            }
        }
    }

    // Light the current position
    LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_OFF);

    // Update the LEDs and wait
    LedMatrix_Update();
    sleep_ms(60);
}

/* Move um LED com o joystick. Quando o botão é pressionado, muda a cor do LED. */
void Test_joystick_LedMatrixColorToggle(JoystickState *js) {
    static int x = 2, y = 2;
    static uint8_t colorIndex = 0;
    // Lista de cores de exemplo
    const led_intensity_t colors[3][3] = {
        {LED_WEAK, LED_OFF, LED_OFF   },    // Red
        {LED_OFF, LED_WEAK, LED_OFF   },    // Green
        {LED_OFF, LED_OFF, LED_WEAK}        // Blue
    };

    // Read and filter joystick values
    Joystick_Read(js);
    js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
    js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

     // Adjust X position
     if (js->x_filtered > ADC_UPPER_THRESHOLD_X && x < 4) {
        x++;
    } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && x > 0) {
        x--;
    }

    // Adjust Y position
    if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && y < 4) {
        y++;
    } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && y > 0) {
        y--;
    }

    // Check if button is pressed (pull-up assumed, pressed reads 0)
    if (!gpio_get(6)) {
        colorIndex = (colorIndex + 1) % 3;
        sleep_ms(200); // Debounce
    }

    // Clear the matrix and set the new pixel
    LedMatrix_Clear();
    LedMatrix_SetPixel(
        x, y,
        colors[colorIndex][0],
        colors[colorIndex][1],
        colors[colorIndex][2]
    );

    // Update the LEDs and wait
    LedMatrix_Update();
    sleep_ms(50);
}

#endif /*EXAMPLES_H*/