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

/* Main function to create a sliding LED effect. */
void walkingLed() {

    NeoPixel_Clear();

    for (int column = 0; column < 5; column++) {
        for (int row = 0; row < 5; row++) {
            
            /*********** Clears the matrix ***********/
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    matriz[i][j][0] = 0;
                    matriz[i][j][1] = 0;
                    matriz[i][j][2] = 0;
                }
            }
            /******************************************/

            // Lights up the LED at the current position
            matriz[row][column][0] = 0;
            matriz[row][column][1] = 200;
            matriz[row][column][2] = 0;

            // Draws the matrix
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    int position = NeoPixel_GetIndex(i, j);
                    NeoPixel_SetLED(position, matriz[i][j][0], matriz[i][j][1], matriz[i][j][2]);
                }
            }

            NeoPixel_Write();
            sleep_ms(50); // Waits 50ms before lighting the next LED
        }
    }
}

/*************************** Push-Buttons Examples ***************************/

#endif /*EXAMPLES_H*/