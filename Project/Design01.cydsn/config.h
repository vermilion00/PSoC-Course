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

/* ======= Keyboard configuration =========
   The parameters of the keyboard can be changed by configuring the relevant defines 
   and compiling and flashing the new firmware */

/* Multiplexer inputs need to be equal in amount to rows */
#define ROWS 4
/* Output pins need to be equal in amount to columns */
#define COLUMNS 4

/* Larger values make the Switches less responsive but more resistant to jitter
   Make sure that the Resolution is set accordingly */
#define HYSTERESIS 7

/* Sets the minimum distance the sensor can reliably detect */
#define RESOLUTION 0.05

/* Set the trigger height of each individual switch in 0.05 mm steps.
   Set the height to 0 to disable that switch. */
#define TRIGGER_HEIGHT {{ 1.0, 0.3, 0.5, 1.0 }, \
                        { 2.5, 1.0, 0.5, 3.5 }, \
                        { 1.0, 1.0, 0.5, 1.0 }, \
                        { 1.0, 1.0, 0.5, 1.0 }};
                        
                   

/* Optional keyboard features, can be disabled by commenting the relevant define */
/* Sets the distance the switch needs to be lifted before it can activate again, in 0.05 mm steps */
#define RAPID_TRIGGER {{ 0.05, 0.1, 0.1, 0.1 }, \
                       { 2.5,  0.1, 0.1, 0.1 }, \
                       { 0.1,  0.1, 0.1, 0.1 }, \
                       { 0.1,  0.1, 0.1, 0.1 }};
                    
/* Allows for keyboard calibration to happen */
#define CALIBRATE_SWITCHES
/* Start calibration if the key at this matrix location is held at startup, e.g. "33" is the bottom right switch.
   First digit is the row, second digit is the column, 0-indexed. */
#define CALIBRATION_KEY "33"

/* If all display lines work fine, this should be set to 0. The first 40 lines of my display don't work */
#define DISPLAY_OFFSET 40

/* Smoothes out the servo movement but makes it less responsive */
//#define SMOOTH_SERVO

/* [] END OF FILE */
