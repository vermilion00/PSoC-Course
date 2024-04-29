/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "HallEffect.h"

/* Defines the ADC channel that the tempersture sensor uses */
#define TEMPERATURE_SENSOR_CHANNEL ROWS
/* Defines the ADC channel that the ultrasound sensor uses */
#define ULTRASOUND_SENSOR_CHANNEL ROWS+1

void getSensorData(uint8 page);
void getHallEffectData(uint8 row, uint8 col);
void getTemperatureData(void);
void getUltrasoundData(void);

typedef struct value {
    uint8 HallEffect;
    uint16 Temperature;
    uint16 Ultrasound;
} Sensors;

Sensors sensor;

extern volatile uint8 page;

/* [] END OF FILE */
