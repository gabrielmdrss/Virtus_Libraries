#include "inc/joystick.h"
#include "hardware/adc.h"

/* Initializes the joystick and configures the ADC channels. */
void Joystick_Init(void) {
    adc_init();
    adc_gpio_init(26); // ADC pin for X-axis
    adc_gpio_init(27); // ADC pin for Y-axis
}

/* Reads the raw joystick values from ADC. */
void Joystick_Read(JoystickState *js) {
    // Select input channel 0 (X)
    adc_select_input(0);
    js->x_raw = adc_read();

    // Select input channel 1 (Y)
    adc_select_input(1);
    js->y_raw = adc_read();
}

/* Applies a dead zone around the midpoint (2048). */
int16_t Joystick_ApplyDeadZone(int16_t value) {
    if (value > (2048 - DEADZONE_THRESHOLD) && value < (2048 + DEADZONE_THRESHOLD)) {
        return 2048; // Centers the value within the threshold
    }
    return value;
}

/* Uses a moving average filter to smooth out noise. */
int16_t Joystick_MovingAverageFilter(int16_t new_sample, int16_t *buffer, uint8_t size) {
    static uint8_t index = 0;
    int32_t sum = 0;

    // Put the new sample into the current buffer position
    buffer[index] = new_sample;
    index = (index + 1) % size; // Wrap around the buffer

    // Calculate the sum of all samples in the buffer
    for (uint8_t i = 0; i < size; i++) {
        sum += buffer[i];
    }
    return sum / size; // Return the average
}

/* Applies a low-pass filter to reduce abrupt variations. */
int16_t Joystick_LowPassFilter(int16_t new_sample, int16_t prev_value) {
    return (int16_t)(LOWPASS_ALPHA * new_sample + (1 - LOWPASS_ALPHA) * prev_value);
}

/* Uses a median filter to remove extreme outliers. */
int16_t Joystick_MedianFilter(int16_t *buffer, uint8_t size) {
    int16_t temp[size];

    // Copy the original buffer
    for (uint8_t i = 0; i < size; i++) {
        temp[i] = buffer[i];
    }

    // Sort the copied values
    for (uint8_t i = 0; i < size - 1; i++) {
        for (uint8_t j = i + 1; j < size; j++) {
            if (temp[i] > temp[j]) {
                int16_t temp_val = temp[i];
                temp[i] = temp[j];
                temp[j] = temp_val;
            }
        }
    }

    return temp[size / 2]; // Returns the median value
}

/* Applies all filters (dead zone, moving average, and low-pass) to the joystick readings. */
void Joystick_ApplyFilters(JoystickState *js) {
    // Dead zone
    js->x_filtered = Joystick_ApplyDeadZone(js->x_raw);
    js->y_filtered = Joystick_ApplyDeadZone(js->y_raw);

    // Moving average
    js->x_filtered = Joystick_MovingAverageFilter(js->x_filtered, js->x_buffer, MOVING_AVG_WINDOW);
    js->y_filtered = Joystick_MovingAverageFilter(js->y_filtered, js->y_buffer, MOVING_AVG_WINDOW);

    // Low-pass
    js->x_filtered = Joystick_LowPassFilter(js->x_filtered, js->prev_x);
    js->y_filtered = Joystick_LowPassFilter(js->y_filtered, js->prev_y);

    // Update previous values
    js->prev_x = js->x_filtered;
    js->prev_y = js->y_filtered;
}