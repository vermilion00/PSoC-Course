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

#include "sensors.h"

//row 0 is Hall effect, col 0 is dashboard, display value as bar in col 1 und as val in col 2, use col 3 to toggle servo output
//row 1 is Temperature, current Temp in row 0, graph of temp in row 1, 1 s average in row 2 and graph of 1 s avg in row 3
//row 2 is Ultrasound measurement
//TODO: Display a message of what's shown when switching pages for 1 s, allows to get value in meantime
//TODO: OR have a key show a description message when pressed

void getSensorData(uint8 page){
    switch(page){
        case 0: //Dashboard
        //Get data from all sensors, update ~ 1Hz
        getHallEffectData(0, 0);
        getTemperatureData();
        getUltrasoundData();
        break;
        case 1: //Hall Effect as value
        //Read Hall effect data from switch "01"
        getHallEffectData(0, 1);
        break;
        case 2: //Hall Effect as bar
        //Read Hall Effect data from switch "02"
        getHallEffectData(0, 2);
        break;
        case 3: //Hall Effect as bar and servo output
        //Read data from switch "03"
        getHallEffectData(0, 3);
        break;
    }
}

/* Get the value of how low the key to activate the screen is pressed */
void getHallEffectData(uint8 row, uint8 col){
    //Set row and col appropriately
    ADC_AMux_Select(row);
    (*pinWriteArr[col])(HIGH);
    //TODO: Check if delay is needed
    sensor.HallEffect = ADC_Read16() - CONVERSION_OFFSET;
    (*pinWriteArr[col])(LOW);
}

/* Get the temperature sensor data */
void getTemperatureData(void){
    //Select the Multiplexer channel for the sensor
    ADC_AMux_Select(TEMPERATURE_SENSOR_CHANNEL);
    //TODO: Check if delay is needed
    //Read the sensor temperature, convert value to mV
    sensor.Temperature = ADC_CountsTo_mVolts(ADC_Read16());
}

/* Get the ultrasound sensor data */
void getUltrasoundData(void){
    
}



/* [] END OF FILE */
