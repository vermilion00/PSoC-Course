/* ========================================================
 *
 * The keyboard is arranged in a 4 by 4 matrix, with
 * a multiplexer to connect the rows to the internal ADC
 * in a column to row power direction.
 * 
 * ========================================================
*/

#include "pages.h"

int main(void){
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    /* Initialize struct */
    Matrix matrix;
    /* Initialize the PSoC functions */
    ADC_Start();
    ADC_AMux_Start();
    UART_Start();
    
    /* Initialize the keyboard */
    keyboardInit(&matrix);
    
    /* External variables used for function calls */
    extern volatile uint8 action;
    extern volatile uint8 page;

    for(;;){
        //TODO: Check how long a scan cycle takes by counting to 10000 with the ADC and incrementing a number each cycle, then outputting it
        /* Scan the keyboard switches for updates */
        scanMatrix(&matrix);
        /* Check the keys for keydown or keyup events */
        updateKeyEvents(&matrix);
        /* Set the new displayed page based on key events */
        setPage(action);
        /* Grabs new sensor data for the current page */
        getSensorData(page);
        /*  Updates data on set page */
        updatePage(page);
    }
}



/* [] END OF FILE */
