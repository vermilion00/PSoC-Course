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

//row 0 is Hall effect, col 0 is dashboard, display value as val in col 1 und as bar in col 2, col 3 outputs to servo
//row 1 is Temperature, no other modes are used since the output is very stable
//row 2 is Ultrasound measurement, 0 is time value, 1 is distance, 2 and 3 are distance with temperature adjustment
//row 3 is currently unused

/* Min and Max servo PWM compare values */
#define SERVO_MIN_VALUE 1000
#define SERVO_MAX_VALUE 2000
#define SERVO_RANGE SERVO_MIN_VALUE - SERVO_MAX_VALUE

/* Smooth servo movement options, larger step size makes it move faster */
#define STEP_SIZE 10

/* Sets the I2C address of the display, can be 0x3C or 0x3D */
#define DISPLAY_ADDRESS 0x3C

/* Sets the display specifications */
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

/* Sets the page based on the pressed key */
void setPage(uint8 action);
/* Updates the previously set page based on new sensor information */
void updatePage(uint8 page);
/* Updates the servo based on new sensor information */
void updateServo(void);

volatile uint8 gPage;
uint16 servoVal;
uint16 servoPosition;

/* [] END OF FILE */
