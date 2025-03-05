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

// Função para atribuir uma cor RGB a um LED.
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

// Função para escrever os dados do buffer nos LEDs.
void npWrite();

// Função para limpar o buffer de pixels.
void npClear();

// Função para inicializar a máquina PIO.
void npInit(uint pin);

// Função para converter a posição da matriz para um índice do vetor.
int getIndex(int x, int y);

#endif /*LEDMATRIX_H*/