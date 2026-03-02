/* 
 * File:   dht22.h
 * Author: Pedro
 *
 * Created on 2 de marzo de 2026, 01:47 PM
 */

#ifndef DHT22_H
#define	DHT22_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <xc.h>
    #include <stdbool.h>

    // Definiciones paes el pin del sensor
    #define DHT22_Sensor_TRIS               TRISCbits.TRISC0
    #define DHT22_Sensor_LAT                LATCbits.LATC0
    #define DHT22_Sensor_GetValue()         PORTCbits.RC0
    #define DHT22_Sensor_SetDigitalInput()  do { DHT22_Sensor_TRIS = 1; } while(0) 
    #define DHT22_Sensor_SetDigitalOutput() do { DHT22_Sensor_TRIS = 0; } while(0) 
    #define DHT22_Sensor_SetHigh()          do { DHT22_Sensor_LAT = 1; } while(0)
    #define DHT22_Sensor_SetLow()           do { DHT22_Sensor_LAT = 0; } while(0)

    typedef enum{
        DHT22_ERROR_TIMEOUT = 0,    // El sensor no responde o se desconectó
        DHT22_OK = 1,               // Lectura exitosa
        DHT22_ERROR_CHECKSUM = 2,   // Datos corruptos por ruido
        DHT22_ERROR_RANGE = 3       // Valores fuera de límites (-40 a 80 C)
    }DHT22_Status_t;

    typedef struct{
        DHT22_Status_t status;
        float temperature;
        float relativeHumidity;
        uint8_t  str_temperature[7];
        uint8_t  str_relativeHumidity[6];
    }DHT22_Sensor_t;


    void DHT22_Initialize(void);
    bool DHT22_Read(DHT22_Sensor_t *sensor);


#ifdef	__cplusplus
}
#endif

#endif	/* DHT22_H */

