/* ===========================================================================
 *
 * Names: Luca Spengler (1399655), Fabian Fritz (1394906)
 * Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
 * Date: 09.04.2024
 * Course ID: TX00DB04
 * Description: Control the position of a servo motor via PWM. When the UART-
 *              Interface receives a signal, the position changes smoothly.
 *
 * ===========================================================================
*/
#include "project.h"
#include <stdbool.h>
#include "rx_isr.h"

#define STEP_SIZE 1 //How large the change in pulse_width is per loop in microseconds
#define STEP_LENGTH 1 //How long the time between steps is, in milliseconds

int pulse_width[6] = {1000, 1200, 1400, 1600, 1800, 2000}; //pulse width steps in microseconds
uint8 position = 0; //Track current position
bool gRX_ISR = false; //RX Interrupt Flag

int main(void){
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start(); //Initialize the UART Interface
    rx_isr_Start(); //Start RX Interrupt
    rx_isr_ClearPending();
    PWM_1_Start(); //Initialize PWM 
    int current_pulse_width = pulse_width[0]; //Set the inital pulse width

    for(;;){
        if(gRX_ISR){    //Check if the interrupt has been triggered
            if(position > 4){   //If the last position has been reached, set it to 0
                position = 0;
            }else{  //increment the position counter if it isn't the last one
                position++;
            }
            gRX_ISR = false;    //Reset the interrupt flag
        }
        if(current_pulse_width < pulse_width[position]){    //Check if the current pulse width is smaller than the wanted pulse width
            current_pulse_width += STEP_SIZE;   //Increment the pulse width gradually
            PWM_1_WriteCompare(current_pulse_width);    //Write the pulse width to the PWM
            ledPin_Write(1);    //Set the LED for debug purposes
            CyDelay(STEP_LENGTH);   //Add a small delay for smoother operation
        }else if(current_pulse_width > pulse_width[position]){  //Check if the current pulse width is larger than the wanted pulse width
            current_pulse_width -= STEP_SIZE;   //Decrement the pulse width gradually
            PWM_1_WriteCompare(current_pulse_width);    //Write the pulse width to the PWM
            ledPin_Write(1);    //Set the LED for debug purposes
            CyDelay(STEP_LENGTH);   //Add a small delay for smoother operation
        }else{
            ledPin_Write(0);    //Turn the debug LED off
        }
    }
}

/* [] END OF FILE */
