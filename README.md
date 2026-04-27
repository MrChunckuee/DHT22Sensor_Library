# DHT22 Sensor Library
A library for interfacing **DHT22 (AM2302)** temperature and humidity sensors with Microchip PIC microcontrollers. This library handles the timing required by the 1-Wire protocol and provides data through a structure, including automated string formatting for easy display.

## Features
* **Data Structure:** Uses `DHT22_Sensor_t` to group status, numeric values, and pre-formatted strings.
* **Dual Output:** Provides readings in `float` for calculations and `uint8_t` arrays (strings) for LCD or UART output.
* **Data Integrity:** Integrated 40-bit checksum validation for reliable data acquisition.

## Hardware Connection
For optimal performance, ensure a pull-up resistor (4.7kΩ - 10kΩ) is placed between VCC and the Data pin.

| DHT22 Pin | Function | PIC18 Connection (Example) |
| :--- | :--- | :--- |
| **1** | VCC | 3.3V - 5V |
| **2** | DATA | RB0 (Configurable) |
| **3** | NC | Not Connected |
| **4** | GND | Ground |

## Usage Example

```c
#include <xc.h>
#include "dht22.h"

void main(void) {
    // Create an instance of the sensor structure
    DHT22_Sensor_t mySensor;
    
    // Initialize system peripherals (Oscillator, Ports, etc.)
    SYSTEM_Initialize(); 
    
    while(1) {
        // Check if the reading was successful using the struct status
         if(DHT22_Read(&mySensor)){ 
            // Use numeric values for logic
            if(mySensor.temperature > 30.0) {
                // High temperature logic...
            }
            
            // Use pre-formatted strings for LCD or UART
            // mySensor.str_temperature      -> e.g., " 25.4"
            // mySensor.str_relativeHumidity -> e.g., " 55.2"
        }
        
        // DHT22 requires at least 2 seconds between readings
        __delay_ms(2000); 
    }
}
```

## Project Structure
* `src/`: Core library files (`dht22.c`, `dht22.h`).
* `examples/`: Project examples for MPLAB X.
* `LICENSE`: MIT License.
* `CHANGELOG`: History of updates and bug fixes.

## Documentation & Tutorial
For a detailed implementation explanation and step-by-step guide, you can review the following examples:
* [PIC18F25K22 & XC8: Uso del sensor de temperatura y humedad relativa DHT22/AM2302 - Mostrar datos en una LCD16x2](https://mrchunckuee.blogspot.com/2026/03/pic18f25k22-xc8-uso-del-sensor-de.html)
* [PIC18F25K22 & XC8: Uso de bluetooth FSC-BT986 - Enviar datos del sensor DHT22 a un monitor serial](https://mrchunckuee.blogspot.com/2026/04/pic18f25k22-xc8-uso-de-bluetooth-fsc.html)
