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

/* Contains the output logic */

#include "pages.h"

/* Global variables */
volatile uint8 gPage = 0;

/* Local variables */
uint16 currentServoVal = SERVO_MAX_VALUE;
uint16 servoPosition = SERVO_MAX_VALUE;

void setPage(uint8 action){
    //Use keys to switch display modes, e.g. boxcar averages or different sensors
    if(gPage != action){
        gPage = action;
    }
}

void updatePage(uint8 page){
    uint8 row = sensor.position[ROW_INDEX];
    uint8 col = sensor.position[COLUMN_INDEX];
    uint8 barWidth;
    switch(page){
        case 0: //Dashboard
        //Compute output information
        sprintf(Transmitbuffer, "Hall Effect: %i,\r\nTemperature: %i.%i C,\r\nUltrasound: %lu.%lu cm\r\n", 
                sensor.HallEffect, sensor.Temperature / TEMP_FACTOR, sensor.Temperature % TEMP_FACTOR, 
                sensor.UltrasoundDistance / US_CONVERSION, sensor.UltrasoundDistance % US_CONVERSION);
        break;
        
        case 1: //Hall sensor as value
        sprintf(Transmitbuffer, "Hall Effect: %i\r\n", sensor.HallEffect);
        break;
        
        case 2: //Hall sensor as bar
        sprintf(Transmitbuffer, "Hall Effect: %i\r\n", sensor.HallEffect);
        
        display_clear();
        #ifdef CALIBRATE_SWITCHES
        barWidth = ((matrix[row][col].max - matrix[row][col].min) / DISPLAY_WIDTH) * sensor.HallEffect;
        #else
        barWidth = SWITCH_RANGE / DISPLAY_WIDTH * sensor.HallEffect;
        #endif
        gfx_fillRect(0, DISPLAY_OFFSET, barWidth, DISPLAY_HEIGHT - DISPLAY_OFFSET, WHITE);
        display_update();
        break;
        
        case 3: //Hall sensor as bar and servo output
        //Write the new servo position value
        updateServo();
        sprintf(Transmitbuffer, "Hall Effect: %i\r\nServo value: %i\r\n", sensor.HallEffect, servoPosition);
        break;
        
        case 4: //Temperature as value
        sprintf(Transmitbuffer, "Temperature: %i.%i C\r\n", sensor.Temperature / TEMP_FACTOR, 
                sensor. Temperature % TEMP_FACTOR);
        break;
        
        case 5: //Temperature as value
        sprintf(Transmitbuffer, "Temperature: %i.%i C\r\n", sensor.Temperature / TEMP_FACTOR, 
                sensor. Temperature % TEMP_FACTOR);
        break;
        
        case 6: //Temperature as value
        sprintf(Transmitbuffer, "Temperature: %i.%i C\r\n", sensor.Temperature / TEMP_FACTOR, 
                sensor. Temperature % TEMP_FACTOR);
        break;
        
        case 7: //Temperature as value
        sprintf(Transmitbuffer, "Temperature: %i.%i C\r\n", sensor.Temperature / TEMP_FACTOR, 
                sensor. Temperature % TEMP_FACTOR);
        break;
        
        case 8: //Ultrasound as time value
        sprintf(Transmitbuffer, "Ultrasound time: %lu us\r\n", sensor.UltrasoundTime);
        break;
        
        case 9: //Ultrasound as approximated distance
        sprintf(Transmitbuffer, "Ultrasound distance: %lu.%lu cm\r\n", sensor.UltrasoundDistance / US_CONVERSION,
                sensor.UltrasoundDistance % US_CONVERSION);
        break;
        
        case 10: //Ultrasound as actual distance
        sprintf(Transmitbuffer, "Actual distance: %lu.%lu cm\r\n", sensor.ActualDistance / US_CONVERSION,
                sensor.ActualDistance % US_CONVERSION);
        break;
        
        case 11: //Ultrasound as actual distance
        sprintf(Transmitbuffer, "Actual distance: %lu.%lu cm\r\n", sensor.ActualDistance / US_CONVERSION,
                sensor.ActualDistance % US_CONVERSION);
        break;
    }
    //Output over UART
    UART_PutString(Transmitbuffer);
    //Output to display, page 2 has its special output
    if(gPage != 2){
        display_clear();
        gfx_setCursor(0, DISPLAY_OFFSET);
        gfx_println(Transmitbuffer);
        display_update();
    }
}

/* Updates the servo position based on har far the switch is pressed */
void updateServo(void){
    //Copy struct values to local variables for readability
    uint8 row = sensor.position[ROW_INDEX];
    uint8 col = sensor.position[COLUMN_INDEX];
    uint16 val = sensor.HallEffect;
    
    #ifdef CALIBRATE_SWITCHES
    //Clamp the hall effect value just in case
    if(val > matrix[row][col].max){
        val = matrix[row][col].max;
    }else if(val < matrix[row][col].min){
        val = matrix[row][col].min;
    }
    
    //Calculate the servo position
    servoPosition = (((val - matrix[row][col].min) * SERVO_RANGE) / 
                      (matrix[row][col].max - matrix[row][col].min)) + SERVO_MIN_VALUE;
    #else
    //Clamp the hall effect value just in case
    if(val > (SWITCH_RANGE + CONVERSION_OFFSET)){
        val = SWITCH_RANGE + CONVERSION_OFFSET;
    }else if(val < CONVERSION_OFFSET){
        val = CONVERSION_OFFSET;
    }
    
    //Calculate the servo position
    servoPosition = (((val - CONVERSION_OFFSET) * SERVO_RANGE) / SWITCH_RANGE) + SERVO_MIN_VALUE;
    #endif
    
    //Clamp the servo position to make sure it stays in range
    if(servoPosition > SERVO_MAX_VALUE){
        servoPosition = SERVO_MAX_VALUE;
    }else if(servoPosition < SERVO_MIN_VALUE){
        servoPosition = SERVO_MIN_VALUE;
    }
    
    //If SMOOTH_SERVO is defined in config.h, the servo output will be slower but smoother
    #ifdef SMOOTH_SERVO
    //Increase the PWM value incrementally when the desired value is larger than the current value
    if(currentServoVal < (servoPosition - STEP_SIZE)){
        //Increment the servo value
        currentServoVal += STEP_SIZE;
        //Write the value to the PWM
        PWM_WriteCompare(currentServoVal);
    //Decrease the PWM value if it is smaller than the current value
    }else if(currentServoVal > (servoPosition + STEP_SIZE)){
        //Decrement the servo value
        currentServoVal -= STEP_SIZE;
        //Write the value to the PWM
        PWM_WriteCompare(currentServoVal);
    }
    #else
    //Write the pwm value directly
    PWM_WriteCompare(servoPosition);
    #endif
}

/* [] END OF FILE */
