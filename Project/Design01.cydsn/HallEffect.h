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

#include <stdlib.h>
#include <stdio.h>
#include "project.h"
#include "config.h"
#include "ssd1306.h"

/* Obvious utility defines */
#define LOW 0
#define HIGH 1
#define FALSE 0
#define TRUE 1

/* Delay between pulling the column high and reading the adc value, in us */
#define SCAN_DELAY 70

/* The calibration key array saves the row first and the index second */
#define ROW_INDEX 0
#define COLUMN_INDEX 1
/* How long the checkErrors function waits before repeating the message */
#define ERROR_WAIT_TIME 5000
/* Offset to properly convert chars to uint8 */
#define ASCII_OFFSET 48
/* How many counts the adc does before the calibration starts, ~= 500ms */
#define CALIBRATION_START_COUNT ADC_CFG1_SRATE / 2
/* How long the delay is to make sure the calibration key is released */
#define KEYUP_DELAY 100
/* How large the transmit buffer array is */
#define TRANSMIT_BUFFER_SIZE 64
/* How many bytes the EEPROM fits into a row */
#define EEPROM_ROW_SIZE 16
/* How many bytes of calibration data each switch needs to save */
#define SWITCH_DATA_SIZE 4

/* Millimeter to ADC value conversion modes */
#define MODE_CONVERT_HEIGHT 0
#define MODE_CONVERT_DISTANCE 1

/* A safe trigger value, gets replaced during calibration. 
   Needs to be set to a value each switch is guaranteed to hit for the calibration to work */
#define DEFAULT_TRIGGER_VALUE 2000

/* Disclaimer: These would be used instead of per-key min and max values if the read value across all keys is similar
   enough. However, calibration needs to be enabled for the project to work currently, so these values aren't used */
/* Hall effect values range from ~1400 to ~2800 */
/* Millimeter values get multiplied by this factor to get ADC values */
#define CONVERSION_FACTOR 300
/* Sets the minimum read value */
#define CONVERSION_OFFSET 1400
/* Sets the switch adc value range */
#define SWITCH_RANGE 1400

/* Apply factor to avoid outputting floats */
#define RESOLUTION_FACTOR 100
#define HEIGHT_FACTOR 10
#define TEMP_FACTOR 10

/* Boundary values for the trigger height and rapid trigger checks */
#define MAX_TRIGGER_HEIGHT 3.8
#define MIN_TRIGGER_HEIGHT RESOLUTION
#define MAX_RAPID_TRIGGER_DISTANCE MAX_TRIGGER_HEIGHT - RESOLUTION
#define MIN_RAPID_TRIGGER_DISTANCE RESOLUTION

/* Struct represents a single switch, the switch matrix is saved as a 2D array of switch structs */
typedef struct switches {
    uint8 pressed;
    uint16 trigger_height;
    #ifdef RAPID_TRIGGER
    uint16 lowest_val;
    uint16 rapid_distance;
    #endif
    #ifdef CALIBRATE_SWITCHES
    #ifndef RAPID_TRIGGER
    uint16 lowest_val;
    #endif
    uint16 max;
    uint16 min;
    #endif
} Switch;

volatile Switch matrix[ROWS][COLUMNS];

uint8 prevMatrix[ROWS][COLUMNS];
uint8 actionBuffer[ROWS*COLUMNS];

/*Stores the current page to display */
volatile uint8 action;

/* Buffer for the UART output string */
char Transmitbuffer[TRANSMIT_BUFFER_SIZE];

/* Array of functions to make accessing the pins in a for loop easier */
void (*pinWriteArr[COLUMNS])(uint8);

/* Initializes the keyboard, calls checkErrors and calibrateSwitches functions, only runs once */
void keyboardInit();
/* Scans the matrix for changes, runs every loop */
void scanMatrix();
/* Updates the key events, runs every loop */
void updateKeyEvents();

/* Checks the configuration for errors and warnings, only runs once */
void checkErrors(const float triggerHeight[ROWS][COLUMNS] 
#ifdef RAPID_TRIGGER
                ,const float rapidDistance[ROWS][COLUMNS]
#endif
#ifdef CALIBRATE_SWITCHES
                ,const uint8 calibrationKey[2]
#endif
);

#ifdef CALIBRATE_SWITCHES
/* Gets the min and max values of each switch for proper distance calculations, only runs once */
void calibrateSwitches(void);
/* Converts the defined mm values to adc values, only runs once */
uint16 convertToVal(float height, uint16 max, uint16 min, uint8 mode);
#else
uint16 convertToVal(float height, uint8 mode);
#endif

/* [] END OF FILE */
