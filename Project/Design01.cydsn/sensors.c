/*
 * ======================================================================================
 *
 * Names: Luca Spengler (1399655), Fabian Fritz (1394906)
 * Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
 * Date: 30.05.2024
 * Course ID: TX00DB04
 * Description: This project reads the analog outputs of a hall effect sensor matrix
 *              to switch between several output modes, then reads the inputs of
 *              hall effect, temperature and ultrasound sensors and displays them
 *              in different ways based on the output mode. The various output modes
 *              are described in the pages.h file.
 *
 *              The keyboard is arranged in a 4 by 4 matrix, with a multiplexer to
 *              connect the rows to the internal ADC. It features per-key calibration
 *              as well as individually configurable trigger heights and rapid trigger
 *              distances on a per-key basis. Calibration can be started by holding the
 *              key defined in config.h on boot. 
 *
 *              The ssd1306 files come from a separate source and can be found here:
 *              https://github.com/NRDaza/Oled-Psoc5/tree/master
 * 
 * ======================================================================================
 */

#include "sensors.h"

//Tracks the ultrasound sensor reading state
uint8 usReadInProgress = FALSE;
//External timer isr flag
extern volatile uint8 gTimerIsrFlag;

/* Get the sensor data for the relevant page */
void getSensorData(uint8 page){
    CtlReg_Write(HIGH);
    switch(page){
        case 0: //Dashboard
        //Get data from all sensors, update ~ 1Hz
        getHallEffectData(0, 0);
        getTemperatureData();
        getUltrasoundData(MODE_DISTANCE);
        break;
        
        case 1: //Hall Effect sensor
        //Read Hall effect data from switch "0/1"
        getHallEffectData(0, 1);
        //Reset timer and timer isr flag. Control register resets itself and doesn't need to be driven low
        CtlReg_Write(HIGH);
        gTimerIsrFlag = FALSE;
        break;
        
        case 2: //Hall Effect sensor
        //Read Hall Effect data from switch "0/2"
        getHallEffectData(0, 2);
        //Reset timer and timer isr flag
        CtlReg_Write(HIGH);
        gTimerIsrFlag = FALSE;
        break;
        
        case 3: //Hall Effect sensor
        //Read data from switch "0/3"
        getHallEffectData(0, 3);
        //Reset timer and timer isr flag
        CtlReg_Write(HIGH);
        gTimerIsrFlag = FALSE;
        break;
        
        case 4: //Temperature sensor
        case 5: 
        case 6: 
        case 7:
        //Read the temperature data
        getTemperatureData();
        //Reset timer and timer isr flag
        CtlReg_Write(HIGH);
        gTimerIsrFlag = FALSE;
        break;
        
        case 8: //Ultrasound sensor
        //Get only the time value
        getUltrasoundData(MODE_TIME);
        break;
        
        case 9: //Ultrasound sensor
        //Convert time value to distance
        getUltrasoundData(MODE_DISTANCE);
        break;
        
        case 10: //Ultrasound sensor
        //Gett the current temperature for accurate distance measurements
        getTemperatureData();
        //Convert time value to distance, adjusted for the air temperature
        getUltrasoundData(MODE_ADJUSTED_DISTANCE);
        break;
        
        case 11: //Ultrasound sensor
        //Gett the current temperature for accurate distance measurements
        getTemperatureData();
        //Convert time value to distance, adjusted for the air temperature
        getUltrasoundData(MODE_ADJUSTED_DISTANCE);
        break;
    }
}

/* Get the value of how low the key to activate the screen is pressed */
void getHallEffectData(uint8 row, uint8 col){
    //Set row and col appropriately
    AMux_Select(row);
    (*pinWriteArr[col])(HIGH);
    CyDelayUs(SCAN_DELAY);
    //Read the switch data and save it
    sensor.HallEffect = ADC_Read16();
    //Save the position of the read switch
    sensor.position[ROW_INDEX] = row;
    sensor.position[COLUMN_INDEX] = col;
    //Deactivate the column pin
    (*pinWriteArr[col])(LOW);
}

/* Get the temperature sensor data */
void getTemperatureData(void){
    //Select the temperature configuration of the ADC and restart the ADC
    ADC_SelectConfiguration(TEMPERATURE_CONFIG, ADC_RESTART);
    //Select the Multiplexer channel for the sensor
    AMux_Select(TEMPERATURE_SENSOR_CHANNEL);
    CyDelayUs(SCAN_DELAY);
    //Read the sensor temperature, convert value to mV
    sensor.Temperature = ADC_CountsTo_mVolts(ADC_Read16());
    //Set the configuration back to the hall effect config since the switches will be scanned next
    ADC_SelectConfiguration(HALLEFFECT_CONFIG, ADC_RESTART);
}

/* Get the ultrasound sensor data */
void getUltrasoundData(uint8 mode){
    //The rising edge of the echo signal starts the timer automatically
    //Since a read can take up to 200 ms, don't block thread while reading the signal
    if(!usReadInProgress){
        //Set the Trigger pin low for 10 us to start scan
        UltrasoundTrigger_Write(LOW);
        CyDelayUs(US_TRIGGER_LENGTH);
        //Reset trigger pin
        UltrasoundTrigger_Write(HIGH);
        usReadInProgress = TRUE;
    }else if(gTimerIsrFlag){
        //Reset the Timer isr flag and read in progress flag
        gTimerIsrFlag = FALSE;
        usReadInProgress = FALSE;
        //Read the captured timer value
        sensor.TimerVal = Timer_ReadCapture();
        //Reset timer after reading to be able to start next capture
        //Since the timer has no reset function, set the control register at the hardware reset pin instead
        CtlReg_Write(HIGH);
        //Calculate the elapsed time in microseconds
        sensor.UltrasoundTime = (TIMER_PERIOD_VAL - sensor.TimerVal) / TIMER_FREQUENCY_US;
        
        if(mode == MODE_DISTANCE){
            //Convert the elapsed time to a distance, assuming 20°C air temperature
            //Multiply by 0.5 since the two-way-trip is measured, but the one-way-trip is needed
            sensor.UltrasoundDistance = 0.5 * SOUND_SPEED_CM_US * sensor.UltrasoundTime * US_CONVERSION;
        }else if(mode == MODE_ADJUSTED_DISTANCE){
            //Calculate the actual speed of sound based on the temperature
            //Divide by ten since the temperature is saved as ten times the actual value
            double actualSpeed = BASE_SOUND_SPEED_CM_US + (SOUND_SPEED_TEMP_CONSTANT * sensor.Temperature / TEMP_FACTOR);
            //Multiply by 0.5 since the two-way-trip is measured, but the one-way-trip is needed
            sensor.ActualDistance = 0.5 * actualSpeed * sensor.UltrasoundTime * US_CONVERSION;
        }
    }
}



/* [] END OF FILE */
