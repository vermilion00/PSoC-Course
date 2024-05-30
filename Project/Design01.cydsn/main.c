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

/*  Matrix pin configuration:
 *  Row0 = Blue 2[0], Row1 = Purple 2[1], Row2 = Grey 2[2], Row3 = Green 2[3]
 *  Col0 = White 3[0], Col1 = Yellow 3[1], COl2 = Brown 3[2], Col3 = Orange 3[3]
 *  
 *  UART TX Pin = 12[7]
 *  
 *  Servo pin = 2[6]
 *  
 *  Ultrasound pins:
 *  Trigger = 0[1], Echo = 0[2]
 *  
 *  Temperature pin = 0[0]
 *  
 *  OLED display I2C pins:
 *  SCL = 12[0], SDA = 12[1]
 */

#include "pages.h"

/* Create a global instance of the switch struct matrix */
extern volatile Switch matrix[ROWS][COLUMNS];

extern volatile Sensor sensor;

int main(void){
    /* Enable global interrupts. */
    CyGlobalIntEnable; 

    /* Initialize the PSoC functions */
    ADC_Start();
    AMux_Start();
    EEPROM_Start();
    UART_Start();
    PWM_Start();
    Timer_Start();
    TimerISR_Start();
    I2C_Start();
    
    /* Initialize the display at boot */
    display_init(DISPLAY_ADDRESS); //Address can be 0x3C or 0x3D
    display_clear();
    display_update();
    gfx_setTextSize(1);
    gfx_setTextColor(WHITE);
    gfx_setTextBg(BLACK);
    gfx_setCursor(0,32);
    gfx_println("Initializing");
    display_update();
    
    /* External variables used for function calls */
    extern volatile uint8 action;
    extern volatile uint8 gPage;
      
    /* Initialize the keyboard at boot, only runs once */
    keyboardInit();

    for(;;){
        /* Scan the keyboard switches for updates */
        scanMatrix();
     
        /* Check the keys for keydown or keyup events */
        updateKeyEvents();
        
        /* Set the new displayed page based on key events */
        setPage(action);
        
        /* Grabs new sensor data for the current page */
        getSensorData(gPage);
        
        /*  Updates data on set page */
        updatePage(gPage);
    }
}



/* [] END OF FILE */
