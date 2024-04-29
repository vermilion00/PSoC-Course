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

/* The ADC reads a lower value the further the switch is pressed */

#include "HallEffect.h"

/* Array of functions to make accessing the pins in a for loop easier */
void (*pinWriteArr[COLUMNS])(uint8) = {Col1_Write, Col2_Write, Col3_Write, Col4_Write};

//TODO: Check if define works
/* Initialize the keyboard */
void keyboardInit(Matrix *matrix){
    //TODO: Find out if its possible to use define directly
    float triggerHeight[ROWS][COLUMNS] = TRIGGER_HEIGHT;
    uint8 row, col;
    uint8 error = 0;
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLUMNS; col++){
            //Check if the trigger height set in config.h is within spec
            if(triggerHeight[row][col] > MAX_TRIGGER_HEIGHT){
                error = 1;
            }else if(triggerHeight[row][col] < MIN_TRIGGER_HEIGHT){
                error = 2;
            }else{
                //Set the individual trigger heights per switch
                matrix[row][col]->trigger_height = convertToVal(triggerHeight[row][col]);
                //Reset switch statuses
                matrix[row][col]->pressed = FALSE;
                
                //If Rapid Trigger is enabled, reset low value
                #ifdef RAPID_TRIGGER
                matrix[row][col]->lowestVal = matrix[row][col]->trigger_height;
                #endif
                
                //If calibration is enabled, call calibration function if calibrate button is pressed
                #ifdef CALIBRATE_SWITCHES
                //TODO: Check if indexing the defined string works
                //Convert char to uint8
                uint8 calibrationKey[2] = {(uint8)atoi(&CALIBRATION_KEY[0]), (uint8)atoi(&CALIBRATION_KEY[1])};
                //Read the designated calibration key, as defined in config.h
                ADC_AMux_Select(calibrationKey[0]);
                (*pinWriteArr[calibrationKey[1]])(HIGH);
                //Call the calibration function if the key is held at startup
                if(ADC_Read16() < matrix[calibrationKey[0]][calibrationKey[1]]->trigger_height){
                    uint8 completed = FALSE;
                    //Only start calibrating after the calibration key has been released
                    //Display message to release calibration key
                    while(!completed){
                        //Wait until key is released again
                        if(ADC_Read16() > matrix[calibrationKey[0]][calibrationKey[1]]->trigger_height){
                            completed = TRUE;
                        }
                    }
                    //Call the calibration function
                    calibrateSwitches(matrix);
                }
                //Deactivate row and column
                ADC_AMux_Disconnect(calibrationKey[0]);
                (*pinWriteArr[calibrationKey[1]])(LOW);
                #endif
            }
        }
    }
    #ifdef RAPID_TRIGGER
    //Check if the Rapid trigger distance is acceptable
    if(RAPID_TRIGGER > MAX_RAPID_TRIGGER_DISTANCE){
        error = 3;
    }else if(RAPID_TRIGGER < MIN_RAPID_TRIGGER_DISTANCE){
        error = 4;
    }else{
        //Convert mm value to adc value at initialization to avoid conversions each matrix scan
        rapidTriggerDistance = convertToVal(RAPID_TRIGGER);
    }
    #endif
    //Check if Error messages are being thrown
    //TODO: Check for more possible error messages
    if(error != 0){
        //Output "Error: Please change the following setting in config.h and flash the firmware again:"
        switch(error){
            case 1: //Trigger height is too high
            #ifdef UART
            UART_PutString("Error: Trigger height is too high!\r\nThe maximum height allowed is MAX_TRIGGER_HEIGHT.\r\n");    
            #endif
            break;
            case 2: //Trigger height is too low
            #ifdef UART
            UART_PutString("Error: Trigger height is too low!\r\nThe minimum height allowed is MIN_TRIGGER_HEIGHT.\r\n");    
            #endif
            break;
            #ifdef RAPID_TRIGGER
            case 3: //Rapid trigger distance is too high
            #ifdef UART
            UART_PutString("Error: Rapid trigger distance is too high!\r\nThe maximum distance allowed is MAX_RAPID_TRIGGER_DISTANCE.\r\n");    
            #endif
            break;
            case 4: //Rapid trigger distance is too low
            #ifdef UART
            UART_PutString("Error: Rapid trigger distance is too low!\r\nThe minimum distance allowed is MIN_RAPID_TRIGGER_DISTANCE.\r\n");
            #endif
            break;
            #endif
        }
        #ifdef UART
        UART_PutString("Change the appropriate setting in config.h and flash the firmware again.\r\n");
        #endif
        //Don't allow the program to continue in case of error
        while(TRUE){
            //Do nothing, message repeats every 5 seconds
            CyDelay(ERROR_WAIT_TIME);
        }
    }
}

/* Get switch values and set switch states, runs every loop */
void scanMatrix(Matrix *matrix){
    uint8 row, col;
    for(row = 0; row < ROWS; row++){
        //Set Mux pins to read the correct input
        ADC_AMux_Select(row);  
        for(col = 0; col < COLUMNS; col++){
            //Check if switch location is used, ignore if it isn't
            //TODO: Check if it's slower when ignoring
            if(matrix[row][col]->trigger_height != 0){
                //Activate the column pin
                (*pinWriteArr[col])(HIGH);
                //Read row pin to check if switch is activated, save state in 2D array
                //TODO: Save in 1D array instead
                uint16 val = ADC_Read16();
                //The read value is lower the further the switch is pressed
                if(val < (matrix[row][col]->trigger_height - HYSTERESIS)){
                    //Rapid trigger logic, only compiled when needed
                    #ifdef RAPID_TRIGGER
                    //TODO: Check if this works
                    //Set the lowest value if needed
                    if(val < (matrix[row][col]->lowestVal - HYSTERESIS)){
                        matrix[row][col]->lowestVal = val;
                        matrix[row][col]->pressed = TRUE;
                    //Reset the switch pressed status if it's been lifted past the rapid trigger distance
                    }else if((val - matrix[row][col]->lowestVal - HYSTERESIS) > rapidTriggerDistance){
                        matrix[row][col]->pressed = FALSE;
                        //Set the rapid trigger activation threshold
                        matrix[row][col]->lowestVal = val - rapidTriggerDistance;
                        //Reset the lowest value when switch is released
                        matrix[row][col]->lowestVal = matrix[row][col]->trigger_height;
                    }else{
                        //TODO: Check if necessary
                        //Set switch state if pressed
                        matrix[row][col]->pressed = TRUE;
                    }
                    #else
                    //Set switch state if pressed
                    matrix[row][col]->pressed = TRUE;
                    #endif
                }else{
                    //Reset switch state if not pressed
                    matrix[row][col]->pressed = FALSE;
                }
                //Deactivate the column pin at end of column scan to avoid ghosting issues
                (*pinWriteArr[col])(LOW);
            }
        }
    }
}

/* Check if key was pressed last scan */
//TODO: Check if global or local variable and array is more efficient
void updateKeyEvents(Matrix *matrix){
    uint8 prevMatrix[ROWS][COLUMNS];
    //Actionbuffer equal in length to the amount of keys
    uint8 actionBuffer[ROWS*COLUMNS];
    uint8 row, col;
    
    //Check if keydown or keyup event happened and set state accordingly
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLUMNS; col++){
            if(matrix[row][col]->pressed != prevMatrix[row][col]){
                //Add keydown event to action buffer
                actionBuffer[row*ROWS+col] = !prevMatrix[row][col];
                //Update the previous matrix
                prevMatrix[row][col] = !prevMatrix[row][col];
            }
        }
    }
    
    //Update the displayed page if necessary
    //Iterate backwards through the array to assume that the top left button is most likely the intended one
    for(uint8 i = (ROWS*COLUMNS-1); i >= 0; i--){
        if(actionBuffer[i]){
            //Set the action based on the key, action == key location
            action = i;
            //Reset the action buffer at scanned position
            actionBuffer[i] = 0;
        }
    }
}

uint16 convertToVal(float height){
    #ifdef CALIBRATE_SWITCHES
    //TODO: Adjust values
    //TODO: Decide if offset should be applied here
    return(height * CONVERSION_CONSTANT - CONVERSION_OFFSET);
    #else
    return(height * CONVERSION_CONSTANT - CONVERSION_OFFSET);
    #endif
}

//Not in use since only part of the switch range is detected
#ifdef CALIBRATE_SWITCHES
/* Get undepressed value of each switch and save it */
//TODO: Make sure calibration starts after calibration key is released
void calibrateSwitches(Matrix *matrix){
    uint8 row, col;
    for(col = 0; col < COLUMNS; col++){
        //Set column pin high
        (*pinWriteArr[col])(HIGH);
        for(row = 0; row < ROWS; row++){
            //Set Mux pins to read the correct input
            ADC_AMux_Select(row);
            //Read row pin and save value in 2d array
            //TODO: Apply offset, save as uint8?
            matrix[col][row]->max = ADC_Read16();
        }
        (*pinWriteArr[col])(LOW);
    }
    /* Read depressed value of each switch one by one */
    //TODO: Display OLED message with instructions
    uint8 val = 0;
    for(row = 0; row < ROWS; row++){
        ADC_AMux_Select(row);
        for(col = 0; col < COLUMNS; col++){
            (*pinWriteArr[col])(HIGH);
            //Display the key location here
            //TODO: Check if this works
            //Display message to press and release displayed key to continue
            uint8 complete = FALSE;
            while(!complete){
                while(ADC_Read16() < matrix[row][col]->trigger_height){
                    if(ADC_Read16() < matrix[row][col]->lowestVal){
                        matrix[row][col]->lowestVal = val;
                    }
                }
                //Set the minimum switch height and reset the lowest value
                if(matrix[row][col]->lowestVal < matrix[row][col]->trigger_height){
                    matrix[row][col]->min = matrix[row][col]->lowestVal;
                    matrix[row][col]->lowestVal = matrix[row][col]->trigger_height;
                    complete = TRUE;
                }
            }
            //Deactivate the column pin
            (*pinWriteArr[col])(LOW);
        }
    }
    //Output "Calibration complete, press any key to continue"
    //TODO: Check if this works
    uint8 pressed = FALSE;
    while(!pressed){
        scanMatrix(matrix);
        for(row = 0; row < ROWS; row++){
            for(col = 0; col < COLUMNS; col++){
                pressed = matrix[row][col]->pressed;
            }
        }
    }
}
#endif

//Unused since Trigger height is now a define
//TODO: Delete this if not needed
/* Initialize the keyboard */
/*void keyboardInit(Matrix *matrix, float triggerHeight[ROWS][COLUMNS]){
    uint8 row, col;
    uint8 error = 0;
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLUMNS; col++){
            //Check if the trigger height set in config.h is within spec
            if(triggerHeight[row][col] > MAX_TRIGGER_HEIGHT){
                error = 1;
            }else if(triggerHeight[row][col] < MIN_TRIGGER_HEIGHT){
                error = 2;
            }else{
                //Set the individual trigger heights per switch
                matrix[row][col]->trigger_height = convertToVal(triggerHeight[row][col]);
                //Reset switch statuses
                matrix[row][col]->pressed = FALSE;
                
                //If Rapid Trigger is enabled, reset low value
                #ifdef RAPID_TRIGGER
                matrix[row][col]->lowestVal = matrix[row][col]->trigger_height;
                #endif
                
                //If calibration is enabled, call calibration function if calibrate button is pressed
                #ifdef CALIBRATE_SWITCHES
                //TODO: Check if indexing the defined string works
                //Convert char to uint8
                uint8 calibrationKey[2] = {(uint8)atoi(&CALIBRATION_KEY[0]), (uint8)atoi(&CALIBRATION_KEY[1])};
                //Read the designated calibration key, as defined in config.h
                ADC_AMux_Select(calibrationKey[0]);
                (*pinWriteArr[calibrationKey[1]])(HIGH);
                //Call the calibration function if the key is pressed at startup
                if(ADC_Read16() < matrix[calibrationKey[0]][calibrationKey[1]]->trigger_height){
                    uint8 completed = FALSE;
                    //Only start calibrating after the calibration key has been released
                    //Display message to release calibration key
                    while(!completed){
                        //Wait until key is released again
                        if(ADC_Read16() > matrix[calibrationKey[0]][calibrationKey[1]]->trigger_height){
                            completed = TRUE;
                        }
                    }
                    calibrateSwitches(matrix);
                }
                //Deactivate row and column
                ADC_AMux_Disconnect(calibrationKey[0]);
                (*pinWriteArr[calibrationKey[1]])(LOW);
                #endif
            }
        }
    }
    #ifdef RAPID_TRIGGER
    //Check if the Rapid trigger distance is acceptable
    if(RAPID_TRIGGER > MAX_RAPID_TRIGGER_DISTANCE){
        error = 3;
    }else if(RAPID_TRIGGER < MIN_RAPID_TRIGGER_DISTANCE){
        error = 4;
    }else{
        //Convert mm value to adc value at initialization to avoid conversions each matrix scan
        rapidTriggerDistance = convertToVal(RAPID_TRIGGER);
    }
    #endif
    //Check if Error messages are being thrown
    //TODO: Check for more possible error messages
    if(error != 0){
        //Output "Error: Please change the following setting in config.h and flash the firmware again."
        switch(error){
            case 1:
                //Output "Trigger height is too high! The maximum height allowed is MAX_TRIGGER_HEIGHT."
                break;
            case 2:
                //Output "Trigger height is too low! The minimum height allowed is MIN_TRIGGER_HEIGHT."
                break;
            #ifdef RAPID_TRIGGER
            case 3:
                //Output "Rapid trigger distance is too high! The maximum distance allowed is MAX_RAPID_TRIGGER_DISTANCE."
                break;
            case 4:
                //Output "Rapid trigger distance is too low! The minimum distance allowed is MIN_RAPID_TRIGGER_DISTANCE."
                break;
            #endif
            //Don't allow the program to continue in case of error
            while(TRUE){
                //Do nothing
                CyDelay(1000);
            }
        }
    }
}*/

/* [] END OF FILE */
