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

/* Keyboard configuration 
   The parameters of the keyboard can be changed by changing the relevant defines and compiling and flashing the new firmware */

/* Multiplexer inputs need to be equal in amount to rows */
#define ROWS 4
/* Output pins need to be equal in amount to columns */
#define COLUMNS 4

/* How much the ADC value has to change to count */
//TODO: Use value at appropriate points
#define HYSTERESIS 2

/* Set the trigger height of each individual switch in 0.1 mm steps.
   Set the height to 0 to disable that switch. */
//TODO: Allow for 0.05 mm increments?
#define TRIGGER_HEIGHT {{ 1.0, 1.0, 1.0, 1.0 }, \
                        { 1.0, 1.0, 1.0, 1.0 }, \
                        { 1.0, 1.0, 1.0, 1.0 }, \
                        { 1.0, 1.0, 1.0, 1.0 }};

//Not in use
/*char keymap[ROWS][COLUMNS] = {{ 'Q', 'W', 'E', 'R' },
                              { 'A', 'S', 'D', 'F' },
                              { 'Y', 'X', 'C', 'V' },
                              { '1', '2', '3', '4' }};*/

/* Optional keyboard features, can be disabled by commenting the relevant define */
/* Sets the distance the switch needs to be lifted before it can activate again, in 0.1 mm steps */
#define RAPID_TRIGGER 0.1
/* Allows for keyboard calibration to happen */
#define CALIBRATE_SWITCHES
#ifdef CALIBRATE_SWITCHES
/* Start calibration if the key at this matrix location is held at startup, e.g. "00" is the top left switch.
   First digit is the row, second is the column, 0-indexed. */
#define CALIBRATION_KEY "33"
#endif
/* Enables optional UART Output */
#define UART

/* [] END OF FILE */
