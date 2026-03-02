/*******************************************************************************
 *
 *                  DHT22 Sensor Library
 *
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sánchez Ramírez
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 * 
 *                   ... C H A N G E L O G ...
 *******************************************************************************
 * Rev.     v1.0.0
 * Author:  Pedro Sanchez Ramirez  
 * Email:   mrchunckuee.electronics@gmail.com     
 * Blog:    https://mrchunckuee.blogspot.com/   
 * Date         Comment
 * 01/03/2026   - Created and tested the library on PIC18F25K22.
 ********************************************************************************/

#include <stdio.h>
#include "dht22.h"
#include "main.h"

/*******************************************************************************
 * Function:        void DHT22_Initialize(void)
 * Description:     Inicializa pin para DHT22
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void DHT22_Initialize(void){
    DHT22_Sensor_SetDigitalOutput();	
    DHT22_Sensor_SetHigh();        	
}

/*******************************************************************************
 * Function:        uint8_t DHT22_Read(DHT22_Sensor_t *sensor)
 * Description:     Funcion para leer datos del sensor.
 * Precondition:    None
 * Parameters:      sensor = Puntero a la estructura DHT22_Sensor_t
 * Return Values:   uint8_t 1 si la lectura es exitosa, 0 si falla.
 * Remarks:         Lee temperatura y humedad relativa del DHT22 con validacion 
 *                  de rangos y errores.
 ******************************************************************************/
bool DHT22_Read(DHT22_Sensor_t *sensor){
    uint8_t bits[5] = {0, 0, 0, 0, 0};
    uint8_t i, j;
    uint16_t timeout;

    // Inicio (Start Signal)
    DHT22_Sensor_SetDigitalOutput();
    DHT22_Sensor_SetLow();
    __delay_ms(18);
    
    DHT22_Sensor_SetHigh();
    __delay_us(30);
    DHT22_Sensor_SetDigitalInput();
    
    // Handshake con Timeouts
    timeout = 1000;
    while(DHT22_Sensor_GetValue()) 
        if(--timeout == 0){ sensor->status = DHT22_ERROR_TIMEOUT; return 0;}
    
    timeout = 1000;
    while(!DHT22_Sensor_GetValue()) 
        if(--timeout == 0){ sensor->status = DHT22_ERROR_TIMEOUT; return 0;}
    
    timeout = 1000;
    while(DHT22_Sensor_GetValue()) 
        if(--timeout == 0){ sensor->status = DHT22_ERROR_TIMEOUT; return 0;}

    // Lectura de datos (40 bits)
    for(j = 0; j < 5; j++){
        uint8_t result = 0;
        for(i = 0; i < 8; i++){
            timeout = 2000;
            while (!DHT22_Sensor_GetValue()) 
                if(--timeout == 0) { sensor->status = DHT22_ERROR_TIMEOUT; return 0; }

            __delay_us(40); 

            if(DHT22_Sensor_GetValue()){
                result |= (1 << (7 - i));
                timeout = 2000;
                while(DHT22_Sensor_GetValue()) 
                    if(--timeout == 0){ sensor->status = DHT22_ERROR_TIMEOUT; return 0;}
            }
        }
        bits[j] = result;
    }

    // Verificacion de Checksum 
    if (bits[4] != ((bits[0] + bits[1] + bits[2] + bits[3]) & 0xFF)) {
        sensor->status = DHT22_ERROR_CHECKSUM;
        return 0;
    }

    // Conversion y Rango
    uint16_t rawH = (bits[0] << 8) | bits[1];
    uint16_t rawT = (bits[2] << 8) | bits[3];

    float h = (float)rawH / 10.0;
    float t = (rawT & 0x8000) ? (float)(rawT & 0x7FFF) / -10.0 : (float)rawT / 10.0;

    if (h < 0.0 || h > 100.0 || t < -40.0 || t > 80.0) {
        sensor->status = DHT22_ERROR_RANGE;
        return 0;
    }

    // Exito - almacenar datos
    sensor->temperature = t;
    sensor->relativeHumidity = h;
    sensor->status = DHT22_OK;
    
    // Si no se ocupa, se puede comentar
    sprintf((char*)sensor->str_temperature, "%2.1f", (double)t);
    sprintf((char*)sensor->str_relativeHumidity, "%2.1f", (double)h);

    return 1;
}