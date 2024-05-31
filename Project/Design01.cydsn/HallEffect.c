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

#include "HallEffect.h"

/* Array of functions to make accessing the pins in a for loop easier */
void (*pinWriteArr[COLUMNS])(uint8) = {Col0_Write, Col1_Write, Col2_Write, Col3_Write};

/* Initialize the keyboard */
void keyboardInit(void){
    //Put the configured trigger heights into an array for indexing
    const float triggerHeight[ROWS][COLUMNS] = TRIGGER_HEIGHT;
    
    #ifdef RAPID_TRIGGER
    //Put the configured rapid trigger distances into an array for indexing
    const float rapidDistance[ROWS][COLUMNS] = RAPID_TRIGGER;
    #endif
    
    #ifdef CALIBRATE_SWITCHES
    //Track if a calibration has been done this boot
    uint8 calibrationDoneThisInit = FALSE;
    const uint8 calibrationKey[2] = {(CALIBRATION_KEY[ROW_INDEX] - ASCII_OFFSET), 
                                     (CALIBRATION_KEY[COLUMN_INDEX] - ASCII_OFFSET)};
    #endif
    
    uint8 row, col;
    //One row holds 16 bytes in the EEPROM
    uint8 rowData[EEPROM_ROW_SIZE];

    //Check for warnings and errors, function doesn't return in case of errors
    //Function parameters depend on which features have been enabled
    checkErrors(triggerHeight
    #ifdef RAPID_TRIGGER
               ,rapidDistance 
    #endif
    #ifdef CALIBRATE_SWITCHES
               ,calibrationKey
    #endif
    );  //End of checkErrors function call
    
    #ifdef CALIBRATE_SWITCHES
    //Call calibration function every time if EEPROM is empty
    if(!EEPROM_ReadByte(0)){
        calibrateSwitches();
        calibrationDoneThisInit = TRUE;
    }else{
        //Call calibration function if calibration button is pressed
        sprintf(Transmitbuffer, "Press key [%i/%i] to start calibration\r\n", 
                calibrationKey[ROW_INDEX], calibrationKey[COLUMN_INDEX]);
        //Output to UART
        UART_PutString(Transmitbuffer);
        //Output to the display
        display_clear();
        display_update();
        //Set cursor to readable position - for the first ~40 lines only every second one works
        //If a working display is used, the offset can be set to 0 in config.h
        gfx_setCursor(0, DISPLAY_OFFSET);
        gfx_setTextSize(1);
        gfx_println(Transmitbuffer);
        display_update();
        
        //Read the designated calibration key, as defined in config.h
        AMux_Select(calibrationKey[ROW_INDEX]);
        (*pinWriteArr[calibrationKey[COLUMN_INDEX]])(HIGH);
        uint16 count = 0;
        //CALIBRATION_START_COUNT is defined as half the ADC sample rate, this loop always lasts ~500ms
        while(count < CALIBRATION_START_COUNT){
            if(ADC_Read16() < DEFAULT_TRIGGER_VALUE){
                uint8 completed = FALSE;
                calibrationDoneThisInit = TRUE;
                //Only start calibrating after the calibration key has been released
                //Display message to release calibration key
                UART_PutString("Release the key to start calibrating\r\n");
                //Output to the display
                display_clear();
                display_update();
                //Set cursor to readable position - for the first ~40 lines only every second one works
                //If a working display is used, the offset can be set to 0 in config.h
                gfx_setCursor(0, DISPLAY_OFFSET);
                gfx_setTextSize(1);
                gfx_println("Release the key to start calibration\r\n");
                display_update();
                while(!completed){
                    //Wait until key is released again
                    if(ADC_Read16() > DEFAULT_TRIGGER_VALUE){
                        completed = TRUE;
                        //Delay to make sure the key is released completely before function is called
                        CyDelay(KEYUP_DELAY);
                    }
                }
                //Deactivate row and column
                AMux_Disconnect(calibrationKey[ROW_INDEX]);
                (*pinWriteArr[calibrationKey[COLUMN_INDEX]])(LOW);
                //Call the calibration function
                calibrateSwitches();
                //Exit the loop to avoid retriggering calibration on this boot
                break;
            }
            count++;
        }
        //Deactivate the column pin
        (*pinWriteArr[calibrationKey[COLUMN_INDEX]])(LOW);
    }
     
    //Save new min and max values if calibration happened this boot
    //A row in eeprom saves 16 bytes, which is enough to store the min and max values of 4 switches (one row by default)
    if(calibrationDoneThisInit){
        for(row = 0; row < ROWS; row++){
            for(col = 0; col < COLUMNS; col++){
                //A conversion from uint16 to two uint8 is needed
                //Get the first 8 bits MSB first
                rowData[col * COLUMNS] = (uint8)(matrix[row][col].min >> 8);
                //Get the remaining 8 bits, 255 = 11111111
                rowData[col * COLUMNS + 1] = (uint8)(matrix[row][col].min & 255);
                //Same for the max value
                rowData[col * COLUMNS + 2] = (uint8)(matrix[row][col].max >> 8);
                rowData[col * COLUMNS + 3] = (uint8)(matrix[row][col].max & 255);
            }
            //Write data one row at a time to save on write cycles
            EEPROM_Write(rowData, row);
        }
        
    //Get min and max from EEPROM otherwise
    }else{
        uint16 eeprom_min, eeprom_max;
        for(row = 0; row < ROWS; row++){
            for(col = 0; col < COLUMNS; col++){
                //EEPROM saves in bytes, so a conversion from two bytes to one uint16 is needed
                //Grab the first 8 bits
                eeprom_min = EEPROM_ReadByte(row * ROWS*COLUMNS + col * COLUMNS) << 8;
                //Shift the first 8 bits to the left to make room for the second byte
                //Grab the second byte that is part of the min value
                eeprom_min |= EEPROM_ReadByte(row * ROWS*COLUMNS + col * COLUMNS + 1);
                //Do the same for the max value
                eeprom_max = EEPROM_ReadByte(row * ROWS*COLUMNS + col * COLUMNS + 2) << 8;
                eeprom_max |= EEPROM_ReadByte(row * ROWS*COLUMNS + col * COLUMNS + 3);
                //Save the read values to the switch struct
                matrix[row][col].min = eeprom_min;
                matrix[row][col].max = eeprom_max;
            }
        }
    }
    #endif
        
    //Set the individual trigger heights per switch
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLUMNS; col++){
            //Check if key has been disabled in config.h
            if(triggerHeight[row][col] != 0){
                #ifdef CALIBRATE_SWITCHES
                //Convert mm value to adc value at initialization to avoid conversions each matrix scan
                matrix[row][col].trigger_height = convertToVal(triggerHeight[row][col], matrix[row][col].min, 
                                                               matrix[row][col].max, MODE_CONVERT_HEIGHT);
                #ifdef RAPID_TRIGGER
                matrix[row][col].rapid_distance = convertToVal(rapidDistance[row][col], matrix[row][col].min, 
                                                               matrix[row][col].max, MODE_CONVERT_DISTANCE);
                #endif
                    
                #else
                //Convert mm value to adc value at initialization to avoid conversions each matrix scan
                matrix[row][col].trigger_height = convertToVal(triggerHeight[row][col], MODE_CONVERT_HEIGHT);
                #ifdef RAPID_TRIGGER
                rapidTriggerDistance = convertToVal(RAPID_TRIGGER, MODE_CONVERT_DISTANCE);
                #endif
                #endif
            }else{
                //Set the trigger height to 0 if key is disabled
                matrix[row][col].trigger_height = 0;
            }
            //Reset switch pressed status
            matrix[row][col].pressed = FALSE;
            
            //If Rapid Trigger is enabled, reset low value
            #ifdef RAPID_TRIGGER
            matrix[row][col].lowest_val = matrix[row][col].trigger_height;
            #endif
        }
    }
}

/* Get switch values and set switch states, runs every loop */
void scanMatrix(void){
    uint8 row, col;
    for(row = 0; row < ROWS; row++){
        //Select the correct Mux channel to read the desired input
        AMux_Select(row);
        for(col = 0; col < COLUMNS; col++){
            //Check if switch location is used, ignore if it isn't
            if(matrix[row][col].trigger_height > 0){
                //Activate the relevant column pin
                (*pinWriteArr[col])(HIGH);
                CyDelayUs(SCAN_DELAY);
                uint16 val = ADC_Read16();
                //Read row pin to check if switch is activated, save state in 2D array
                //The read value is lower the further the switch is pressed
                if(val < (matrix[row][col].trigger_height - HYSTERESIS)){
                    //Rapid trigger logic, only compiled when needed
                    #ifdef RAPID_TRIGGER
                    //Set the lowest value if needed
                    if(val < (matrix[row][col].lowest_val - HYSTERESIS)){
                        matrix[row][col].lowest_val = val;
                        matrix[row][col].pressed = TRUE;
                    //Reset the switch pressed status if it's been lifted past the rapid trigger distance
                    }else if((val - matrix[row][col].lowest_val - HYSTERESIS) > matrix[row][col].rapid_distance){
                        matrix[row][col].pressed = FALSE;
                        //Set the rapid trigger activation threshold
                        matrix[row][col].lowest_val = val - matrix[row][col].rapid_distance;
                    }
                    #else
                    //Set switch state if pressed
                    matrix[row][col].pressed = TRUE;
                    #endif
                }else{
                    //Reset switch state if not pressed
                    matrix[row][col].pressed = FALSE;
                }
                //Deactivate the column pin at end of column scan to avoid ghosting issues
                (*pinWriteArr[col])(LOW);
            }
        }
    }
}

//Not useful for this application, but an extended version including a buffer would be used in an actual keyboard
/* Check if key was pressed last scan */
void updateKeyEvents(void){
    uint8 row, col;
    //Check if keydown or keyup event happened and set state accordingly
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLUMNS; col++){
            if(matrix[row][col].pressed){
                action = row*ROWS+col;
                break;
            }
        }
    }
}

/* Convert the millimeter values to adc values */
#ifdef CALIBRATE_SWITCHES
//If calibration is enabled, each switch has a separate conversion constant
uint16 convertToVal(float height, uint16 min, uint16 max, uint8 mode){
    if(mode == MODE_CONVERT_HEIGHT){
        return((MAX_TRIGGER_HEIGHT - height) * ((max - min) / MAX_TRIGGER_HEIGHT) + min);
    }else{
        return(height * (max - min) / MAX_TRIGGER_HEIGHT);
    }
}
#else
//If calibration isn't enabled, constants are used instead
uint16 convertToVal(float height, uint8 mode){
    if(mode == MODE_CONVERT_HEIGHT){
        return((MAX_TRIGGER_HEIGHT - height) * CONVERSION_FACTOR + CONVERSION_OFFSET);
    }else{
        return(height * CONVERSION_FACTOR);
    }
}
#endif

#ifdef CALIBRATE_SWITCHES
/* Calibrate the switches to allow for accurate readings if the values vary */
void calibrateSwitches(void){
    uint8 row, col;
    uint16 val = 0;
    
    //Get the undepressed key values
    for(row = 0; row < ROWS; row++){
        //Set Mux channel to read the correct input
        AMux_Select(row);
        for(col = 0; col < COLUMNS; col++){
            //Set column pin high
            (*pinWriteArr[col])(HIGH);
            //Read row switch max value and save it in 2d array
            CyDelayUs(SCAN_DELAY);
            matrix[row][col].max = ADC_Read16();
            (*pinWriteArr[col])(LOW);
            //Set the lowest value of each switch to a known safe value initially
            matrix[row][col].lowest_val = DEFAULT_TRIGGER_VALUE;
        }
    }       
    
    //Read depressed value of each switch one by one
    UART_PutString("Press each key all the way down to complete the calibration\r\n");
    //Output to the display
    display_clear();
    display_update();
    //Set cursor to readable position - for the first ~40 lines only every second one works
    //If a working display is used, the offset can be set to 0 in config.h
    gfx_setCursor(0, DISPLAY_OFFSET);
    gfx_println("Press each key all the way down to complete the calibration\r\n");
    display_update();
    
    val = 0;
    for(row = 0; row < ROWS; row++){
        AMux_Select(row);
        for(col = 0; col < COLUMNS; col++){
            //Display the key location here
            sprintf(Transmitbuffer, "Press and release key [%i/%i]\r\n", row, col);
            UART_PutString(Transmitbuffer);
            //Output to the display
            display_clear();
            display_update();
            //Set cursor to readable position - for the first ~40 lines only every second one works
            //If a working display is used, the offset can be set to 0 in config.h
            gfx_setCursor(0, DISPLAY_OFFSET);
            gfx_println(Transmitbuffer);
            display_update();
            //Tracks if the lowest point has been reached
            uint8 lowestPointMeasured = FALSE;
            //Tracks if the calibration for the switch is complete
            uint8 complete = FALSE;
            //Pull the column pin high
            (*pinWriteArr[col])(HIGH);
            CyDelayUs(SCAN_DELAY);
            //Loop while the lowest point hasn't been reached
            while(!lowestPointMeasured){
                val = ADC_Read16();
                if(val < DEFAULT_TRIGGER_VALUE){
                    if(val < matrix[row][col].lowest_val){
                        matrix[row][col].lowest_val = val;
                        sprintf(Transmitbuffer, "lowest: %i\r\n", val);
                        UART_PutString(Transmitbuffer);
                        //Output to the display
                        display_clear();
                        display_update();
                        //Set cursor to readable position - for the first ~40 lines only every second one works
                        //If a working display is used, the offset can be set to 0 in config.h
                        gfx_setCursor(0, DISPLAY_OFFSET);
                        gfx_setTextSize(1);
                        gfx_println(Transmitbuffer);
                        display_update();
                    }
                }else if(matrix[row][col].lowest_val < (DEFAULT_TRIGGER_VALUE)){
                    lowestPointMeasured = TRUE;
                }
            }
            
            //Check if the switch has been released again
            if(!complete){
                //Set the minimum switch height and reset the lowest value
                if(matrix[row][col].lowest_val < (DEFAULT_TRIGGER_VALUE - HYSTERESIS)){
                    matrix[row][col].min = matrix[row][col].lowest_val;
                    //Switch calibration complete, go to next switch
                    complete = TRUE;
                    sprintf(Transmitbuffer, "min: %i,\r\nmax: %i\r\n", matrix[row][col].min, matrix[row][col].max);
                    UART_PutString(Transmitbuffer);
                    //Output to the display
                    display_clear();
                    display_update();
                    //Set cursor to readable position - for the first ~40 lines only every second one works
                    //If a working display is used, the offset can be set to 0 in config.h
                    gfx_setCursor(0, DISPLAY_OFFSET);
                    gfx_setTextSize(1);
                    gfx_println(Transmitbuffer);
                    display_update();
                }
                CyDelay(1);
            }
            //Deactivate the column pin
            (*pinWriteArr[col])(LOW);
        }
    }
}
#endif

/* Check for warnings and errors in the configuration */
void checkErrors(const float triggerHeight[ROWS][COLUMNS] 
#ifdef RAPID_TRIGGER
                ,const float rapidDistance[ROWS][COLUMNS]
#endif
#ifdef CALIBRATE_SWITCHES
                ,const uint8 calibrationKey[2]
#endif
){
    uint8 row, col;
    int8 error = FALSE;
    
    //Check for configuration warnings and errors
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLUMNS; col++){
            //Warnings
            //Convert the float to a uint16 to be able to use the modulo operator
            //Throw a warning if the trigger height or rapid trigger distance is not set in 0.05 mm steps
            #ifdef RAPID_TRIGGER
            if((((uint16)(triggerHeight[row][col] * 100) % (uint16)(RESOLUTION * 100)) != 0)
            || (((uint16)(rapidDistance[row][col] * 100) % (uint16)(RESOLUTION * 100)) != 0)){
                error = -1;
            }
            #endif
            
            //Errors
            //Check if the trigger height set in config.h is within spec
            if(triggerHeight[row][col] > MAX_TRIGGER_HEIGHT){
                error = 1;
                break;
            }else if(triggerHeight[row][col] < MIN_TRIGGER_HEIGHT && triggerHeight[row][col] != 0){
                //Trigger height of 0 is allowed to disable the switch
                error = 2;
                break;
            }
            #ifdef RAPID_TRIGGER
            //Check if the rapid trigger distance is within spec
            else if(rapidDistance[row][col] > MAX_RAPID_TRIGGER_DISTANCE){
                error = 3;
                break;
            }else if(rapidDistance[row][col] < MIN_RAPID_TRIGGER_DISTANCE){
                error = 4;
                break;
            }
            #endif
        }
        //Break out of the loop if an error has been detected
        if(error > 0){
            break;
        }
    }
    
    #ifdef CALIBRATE_SWITCHES
    //Check if the calibration key is part of the switch matrix, matrix is 0-indexed
    if(calibrationKey[ROW_INDEX] > (ROWS - 1)){
        error = 5;
    }else if(calibrationKey[ROW_INDEX] < 0){
        error = 6;
    }else if(calibrationKey[COLUMN_INDEX] > (COLUMNS - 1)){
        error = 7;
    }else if(calibrationKey[COLUMN_INDEX] < 0){
        error = 8;
        
    //Check if calibration key is disabled
    }else if(triggerHeight[calibrationKey[ROW_INDEX]][calibrationKey[COLUMN_INDEX]] == 0){
        error = 9;
    //Check if EEPROM is large enough, each switch needs 4 bytes of memory for calibration data
    //Assumes that each possible switch position is used
    }else if(CYDEV_EE_SIZE < ROWS*COLUMNS*SWITCH_DATA_SIZE){
        error = 10;
    }
    #endif
    
    if(error != 0){
        //Convert defined constants to variables to use them in strings
        //Hacky way to get floats to work with sprintf without increasing heap size and including additional libraries
        const uint8 maxHeight = MAX_TRIGGER_HEIGHT * HEIGHT_FACTOR;
        const uint8 minHeight = MIN_TRIGGER_HEIGHT * HEIGHT_FACTOR;
        const uint8 maxDistance = MAX_RAPID_TRIGGER_DISTANCE * HEIGHT_FACTOR;
        const uint8 minDistance = MIN_RAPID_TRIGGER_DISTANCE * HEIGHT_FACTOR;
        const uint8 maxCalibrationKeyRow = ROWS - 1;
        const uint8 maxCalibrationKeyCol = COLUMNS - 1;
        const uint8 resolution = RESOLUTION * RESOLUTION_FACTOR;
        //Don't allow the program to continue in case of error
        while(error != 0){
            switch(error){
                case -1: //Step size is less than the RESOLUTION defined in config.h
                sprintf(Transmitbuffer, "Warning: Steps less than %u.%u mm are experimental and might cause issues.\r\n",
                        resolution / RESOLUTION_FACTOR, resolution % RESOLUTION_FACTOR);
                //Reset warning to allow program to continue
                error = 0;
                break;
                
                case 1: //Trigger height is too high
                sprintf(Transmitbuffer, "Error: Trigger height set for switch [%u/%u] is too high!\r\nThe maximum height allowed is %u.%u mm.\r\n",
                        row, col, maxHeight / HEIGHT_FACTOR, maxHeight % HEIGHT_FACTOR);   
                break;
                
                case 2: //Trigger height is too low
                sprintf(Transmitbuffer, "Error: Trigger heightset for switch [%u/%u] is too low!\r\nThe minimum height allowed is %u.%u mm.\r\n", 
                        row, col, minHeight / HEIGHT_FACTOR, minHeight % HEIGHT_FACTOR);    
                break;
                
                #ifdef RAPID_TRIGGER
                case 3: //Rapid trigger distance is too high
                sprintf(Transmitbuffer, "Error: Rapid trigger distance is too high!\r\nThe maximum distance allowed is %u.%u mm.\r\n", 
                        maxDistance / HEIGHT_FACTOR, maxDistance % HEIGHT_FACTOR);    
                break;
                
                case 4: //Rapid trigger distance is too low
                sprintf(Transmitbuffer, "Error: Rapid trigger distance is too low!\r\nThe minimum distance allowed is %u.%u mm.\r\n", 
                        minDistance / HEIGHT_FACTOR, minDistance % HEIGHT_FACTOR);
                break;
                #endif
                
                #ifdef CALIBRATE_SWITCHES
                case 5: //CALIBRATION_KEY row is higher than ROWS
                sprintf(Transmitbuffer, "Error: Calibration key row is too high!\r\nThe largest number allowed is %u.\r\n", 
                        maxCalibrationKeyRow);
                break;
                
                case 6: //CALIBRATION_KEY row is less than 0
                sprintf(Transmitbuffer, "Error: Calibration key row can't be less than 0!\r\n");
                break;
                
                case 7: //CALIBRATION_KEY column is higher COLUMNS
                sprintf(Transmitbuffer, "Error: Calibration key column is too high!\r\nThe largest number allowed is %u.\r\n", 
                        maxCalibrationKeyCol);
                break;
                
                case 8: //CALIBRATION_KEY column is less than 0
                sprintf(Transmitbuffer, "Error: Calibration key column can't be less than 0!\r\n");
                break;
                
                case 9: //CALIBRATION_KEY is disabled
                sprintf(Transmitbuffer, "Error: The calibration key must not be disabled!\r\n");
                break;
                
                case 10: //EEPROM not large enough to save all key data
                sprintf(Transmitbuffer, "Error: The EEPROM is not large enough to save the data of all keys!\r\nDisable calibration or reduce the amount of keys.\r\n");
                break;
                #endif
            }
            //Output over UART
            UART_PutString(Transmitbuffer);
            //Output to the display
            display_clear();
            display_update();
            gfx_setCursor(0,DISPLAY_OFFSET);
            gfx_setTextSize(1);
            gfx_println(Transmitbuffer);
            display_update();
            CyDelay(ERROR_WAIT_TIME / 2);
            
            if(error){
                sprintf(Transmitbuffer, "Change the appropriate setting in config.h and flash the firmware again.\r\n");
                //Output over UART
                UART_PutString(Transmitbuffer);
                //Output to the display
                display_clear();
                display_update();
                gfx_setCursor(0,DISPLAY_OFFSET);
                gfx_setTextSize(1);
                gfx_println(Transmitbuffer);
                display_update();
                //Do nothing, message repeats every 5 seconds by default
                CyDelay(ERROR_WAIT_TIME / 2);
            }else{
                //Shorter wait time for warnings since program continues afterwards
                CyDelay(ERROR_WAIT_TIME / 5);
            }
        }
    }
}

/* [] END OF FILE */
