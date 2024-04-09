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
#include "project.h"
#include <stdbool.h>
#include "rx_isr.h"

#define STEP_SIZE 1 //How large the change in pulse_width is per loop in microseconds
#define STEP_LENGTH 1 //How long the time between steps is, in milliseconds

int pulse_width[6] = {1000, 1200, 1400, 1600, 1800, 2000}; //in microseconds
uint8 position = 0; //Track current position
bool gRX_ISR = false; //RX Interrupt Flag

int main(void){
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start(); //Initialize the UART Interface
    rx_isr_Start(); //Start RX Interrupt
    rx_isr_ClearPending();
    PWM_1_Start(); //Initialize PWM 
    int current_pulse_width = 1000;

    for(;;)
    {
        if(gRX_ISR){
            if(position > 4){
                position = 0;
            }else{
                position++;
            }
            gRX_ISR = false;
        }
        if(pulse_width[position] > current_pulse_width){
            current_pulse_width += STEP_SIZE;
            PWM_1_WriteCompare(current_pulse_width);
            ledPin_Write(1);
            CyDelay(STEP_LENGTH);
        }else if(pulse_width[position] < current_pulse_width){
            current_pulse_width -= STEP_SIZE;
            PWM_1_WriteCompare(current_pulse_width);
            ledPin_Write(1);
            CyDelay(STEP_LENGTH);
        }else{
            ledPin_Write(0);
        }
    }
}

/* [] END OF FILE */
