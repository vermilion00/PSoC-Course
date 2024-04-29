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

#include <stdlib.h>
#include "project.h"
#include "config.h"

#define LOW 0
#define HIGH 1
#define FALSE 0
#define TRUE 1

#define ERROR_WAIT_TIME 5000

/* Hall effect values range from ~240 to 450 */
//TODO: Actually get the proper value, fix conversion function
//TODO: Work with integer instead
#define CONVERSION_CONSTANT 52.63
/* Sets the offset so that the reading is 0 on bottom-out */
//TODO: Measure correct offset
#define CONVERSION_OFFSET 200

//Boundary values for the trigger height and rapid trigger checks
#define MAX_TRIGGER_HEIGHT 4.0
#define MIN_TRIGGER_HEIGHT 0
#ifdef RAPID_TRIGGER
#define MAX_RAPID_TRIGGER_DISTANCE 3.9
#define MIN_RAPID_TRIGGER_DISTANCE 0.1
#endif

/* Struct represents a single switch, the switch matrix is saved as a 2D array of switch structs */
typedef struct switches {
    uint8 pos;
    uint8 pressed;
    uint16 trigger_height;
    #ifdef RAPID_TRIGGER
    uint16 lowestVal;
    #endif
    #ifdef CALIBRATE_SWITCHES
    #ifndef RAPID_TRIGGER
    uint16 lowestVal;
    #endif
    uint16 max;
    uint16 min;
    #endif
} Matrix[ROWS][COLUMNS];

/*Stores the current page to display */
volatile uint8 action;

/* Buffer for the UART output string */
char Transmitbuffer[64];

/* Array of functions to make accessing the pins in a for loop easier */
void (*pinWriteArr[COLUMNS])(uint8);

/* Rapid trigger distance converted to ADC value for higher accuracy */
#ifdef RAPID_TRIGGER
uint16 rapidTriggerDistance;
#endif

/* TODO: 
    -Add EEPROM support to save calibration data to EEPROM when enabled
    -Add checks to make sure rapid trigger and trigger height values are acceptible - Done
    -Check if delay after Mux switching is needed
    -Have page 0 be a dashboard of every sensor, updates slower
    -Poll each sensor or only the one one required for the page?
    -Convert 2D arrays to normal arrays
    -Have a button display a description of the current page
    -Save hall effect value as uint8 and subtract offset each cycle, or save as uint16 and subtract offset at init?
    -Make sure same offset is applied at every HE read
    -Check temp and ultrasound sensor pins*/
void keyboardInit(Matrix *matrix);
uint16 convertToVal(float height);
void scanMatrix(Matrix *matrix);
void updateKeyEvents(Matrix *matrix);
void updateLogic(uint8 action);
#ifdef CALIBRATE_SWITCHES
void calibrateSwitches(Matrix *matrix);
#endif

/* [] END OF FILE */
