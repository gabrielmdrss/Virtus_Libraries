# Libraries for Raspberry Pi Pico W

This repository contains libraries for the Raspberry Pi Pico W, including:

- **Joystick** (reading analog values and applying filters)  
- **LED Matrix** (controlling a 5x5 RGB LED matrix using PIO)  
- **Examples** (in examples.h, showing how to utilize both features)  

---

## Structure

```txt
.
├── inc
│   ├── joystick.h      # Joystick definitions (filters, dead zone, etc.)
│   ├── ledMatrix.h     # LED Matrix definitions
│   └── ws2818b.pio.h   # Auto-generated header from the PIO code
├── joystick.c          # Joystick function implementations
├── ledMatrix.c         # LED Matrix function implementations
├── examples.h          # Example functions demonstrating Joystick & LED usage
├── main.c              # Main entry point testing these libraries
├── ws2818b.pio         # PIO code for generating WS2812/WS2818 signals
└── CMakeLists.txt      # CMake build configuration
```

## Joystick

The joystick library reads raw ADC values and applies filters such as:

- **Dead zone**
- **Moving average**
- **Low-pass**
- **Median**

### Usage:

1. Include `joystick.h`.
2. Call `Joystick_Init()` before reading values.
3. Use `Joystick_Read(js)` to load raw data into the `JoystickState`.
4. Apply filters with `Joystick_ApplyFilters(js)` or individually (e.g., `Joystick_ApplyDeadZone(value)`).

## LED Matrix

The LED matrix library controls a **5x5 RGB LED grid**:

- `ledMatrix.h` declares `NeoPixel_SetLED()`, `NeoPixel_Write()`, `NeoPixel_Clear()`, and `NeoPixel_Init()`.
- `ledMatrix.c` contains logic for updating the matrix and `NeoPixel_GetIndex()` to convert `(x, y)` coordinates into a linear buffer index.

### Usage:

1. Include `ledMatrix.h`.
2. Call `NeoPixel_Init(pin)` to set up the PIO at the chosen GPIO pin.
3. Set a specific LED color with `NeoPixel_SetLED(index, r, g, b)`.
4. Send data to the strip with `NeoPixel_Write()`.

## Examples

In `examples.h`, there are sample functions showing how to integrate the joystick and LED matrix:

- Joystick tests (like `Test_joystick_Complete`)
- A sliding LED function (`walkingLed`) that animates the matrix column by column.

## main.c

The main file initializes the joystick and can call LED matrix operations. You can modify it to test the example functions or create your own.

## Build and Run

1. Install and configure the Pico SDK.
2. Generate the build using CMake (e.g., out-of-source build in a `build` folder).
3. Use Ninja or another tool to compile, then upload the generated `.uf2` to the Pico W.

### Example steps on a command line:

```sh
mkdir build && cd build
cmake ..
ninja
```

When done, drag and drop the `libraries.uf2` onto the `RPI-RP2` drive or use `picotool` to load the firmware.

## Notes

- The `ws2818b.pio` file is **PIO assembly** for WS281x LED protocols.
- Global variables (e.g., `matriz`, `leds`, `np_pio`, `sm`) are declared as `extern` in `ledMatrix.h` and defined in `ledMatrix.c`. This prevents multiple definitions during linking.
- Extend or customize the joystick and LED functionalities by editing `joystick.c` or `ledMatrix.c`.

Feel free to adapt or improve these libraries to suit your Raspberry Pi Pico W projects.

