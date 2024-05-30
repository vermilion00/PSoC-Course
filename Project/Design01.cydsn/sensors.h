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

/* Defines the ADC channel that the tempersture sensor uses */
#define TEMPERATURE_SENSOR_CHANNEL 4

/* Defines Halleffect sensor ADC configuration */
#define HALLEFFECT_CONFIG 1
/* Defnes Temperature sensor ADC configuration */
#define TEMPERATURE_CONFIG 2

/* Define the ultrasound measurement mode */
/* Only get the elapsed time */
#define MODE_TIME 0
/* Get the distance assuming 20°C air temperature */
#define MODE_DISTANCE 1
/* Get the actual distance adjusted for air temperature */
#define MODE_ADJUSTED_DISTANCE 2
/* Defines the ADC configuration change modes */
#define ADC_RESTART 1

/* Defines the Ultrasound trigger length in uS */
#define US_TRIGGER_LENGTH 10

/* Constant definitions */
/* Travel speed of sound in air, in cm/us, 20°C */
#define SOUND_SPEED_CM_US 0.03435
/* Travel speed of sound in air in cm/us, to be adjusted for temperature */
#define BASE_SOUND_SPEED_CM_US 0.03315
/* Speed of sound divided by temperature in cm/(us * °C) */
#define SOUND_SPEED_TEMP_CONSTANT 0.00006
/* Contant for ultrasound value uint32 conversion to avoid floats */
#define US_CONVERSION 10000
/* Constant Timer period value */
#define TIMER_PERIOD_VAL 7200000
/* Constant Timer clock frequency in 1/microsecond */
#define TIMER_FREQUENCY_US 24

/* Get the sensor data for the relevant page */
void getSensorData(uint8 page);
/* Get the value of how low the key to activate the screen is pressed */
void getHallEffectData(uint8 row, uint8 col);
/* Get the temperature sensor data */
void getTemperatureData(void);
/* Get the ultrasound sensor data */
void getUltrasoundData(uint8 mode);

typedef struct sensor {
    //Hall effect value in ADC counts
    uint16 HallEffect;
    //Position of the switch
    uint8 position[2];
    //Temperature is saved as ten times the actual value to avoid floats
    uint16 Temperature;
    //The measured echo time of the ultrasound sensor, in us
    uint32 UltrasoundTime;
    //The converted distance of the ultrasound sensor, in cm
    uint32 UltrasoundDistance;
    //The converted actual distance of the ultrasound sensor, in cm
    uint32 ActualDistance;
    //The captured timer value
    uint32 TimerVal;
} Sensor;

volatile Sensor sensor;

extern volatile uint8 gPage;
extern volatile uint8 gTimerIsrFlag;

/* [] END OF FILE */
