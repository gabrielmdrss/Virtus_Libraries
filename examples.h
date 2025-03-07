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

// Função principal para criar o efeito de LED deslizante.
void ledDeslizante() {

    NeoPixel_Clear();

    for (int coluna = 0; coluna < 5; coluna++) {
        for (int linha = 0; linha < 5; linha++) {
            
            
            /*********** Limpa a matriz ***********/

            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    matriz[i][j][0] = 0;
                    matriz[i][j][1] = 0;
                    matriz[i][j][2] = 0;
                }
            }

            /**************************************/

            // Acende o LED na posição atual
            matriz[linha][coluna][0] = 0;
            matriz[linha][coluna][1] = 200;
            matriz[linha][coluna][2] = 0;

            // Desenha a matriz
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    int posicao = NeoPixel_GetIndex(i, j);
                    NeoPixel_SetLED(posicao, matriz[i][j][0], matriz[i][j][1], matriz[i][j][2]);
                }
            }

            NeoPixel_Write();
            sleep_ms(50); // Espera 200ms antes de acender o próximo LED
        }
    }
}

/*************************** Push-Buttons Examples ***************************/

#endif /*EXAMPLES_H*/