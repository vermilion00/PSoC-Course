# Exercise 2
## Contents of main.c:

```

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

#define STEP_SIZE 1 //How large the change in pulse_width is per loop in microseconds
#define STEP_LENGTH 1 //How long the time between steps is, in milliseconds

uint16 pulse_width[6] = {1000, 1200, 1400, 1600, 1800, 2000}; //pulse width steps in microseconds
uint8 position = 0; //Track current position
bool gRX_ISR = false; //RX Interrupt Flag

int main(void){
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start(); //Initialize the UART Interface
    rx_isr_Start(); //Start RX Interrupt
    rx_isr_ClearPending();
    PWM_1_Start(); //Initialize PWM 
    uint16 current_pulse_width = pulse_width[0]; //Set the inital pulse width

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

```

## Contents of rx_isr.c:
### Only the interrupt flag is set here

```

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
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <rx_isr.h>
#include "cyapicallbacks.h"

#if !defined(rx_isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START rx_isr_intc` */
    #include <stdbool.h> 
    extern bool gRX_ISR; //Track UART Receive Interrupt as external flag
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: rx_isr_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_Start(void)
{
    /* For all we know the interrupt is active. */
    rx_isr_Disable();

    /* Set the ISR to point to the rx_isr Interrupt. */
    rx_isr_SetVector(&rx_isr_Interrupt);

    /* Set the priority. */
    rx_isr_SetPriority((uint8)rx_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    rx_isr_Enable();
}


/*******************************************************************************
* Function Name: rx_isr_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    rx_isr_Disable();

    /* Set the ISR to point to the rx_isr Interrupt. */
    rx_isr_SetVector(address);

    /* Set the priority. */
    rx_isr_SetPriority((uint8)rx_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    rx_isr_Enable();
}


/*******************************************************************************
* Function Name: rx_isr_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_Stop(void)
{
    /* Disable this interrupt. */
    rx_isr_Disable();

    /* Set the ISR to point to the passive one. */
    rx_isr_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: rx_isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for rx_isr.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(rx_isr_Interrupt)
{
    #ifdef rx_isr_INTERRUPT_INTERRUPT_CALLBACK
        rx_isr_Interrupt_InterruptCallback();
    #endif /* rx_isr_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START rx_isr_Interrupt` */
    gRX_ISR = true; //set interrupt flag
    
    /* `#END` */
}


/*******************************************************************************
* Function Name: rx_isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling rx_isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use rx_isr_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)rx_isr__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: rx_isr_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress rx_isr_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)rx_isr__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: rx_isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling rx_isr_Start or rx_isr_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after rx_isr_Start or rx_isr_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_SetPriority(uint8 priority)
{
    *rx_isr_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: rx_isr_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 rx_isr_GetPriority(void)
{
    uint8 priority;


    priority = *rx_isr_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: rx_isr_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_Enable(void)
{
    /* Enable the general interrupt. */
    *rx_isr_INTC_SET_EN = rx_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: rx_isr_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 rx_isr_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*rx_isr_INTC_SET_EN & (uint32)rx_isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: rx_isr_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_Disable(void)
{
    /* Disable the general interrupt. */
    *rx_isr_INTC_CLR_EN = rx_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: rx_isr_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void rx_isr_SetPending(void)
{
    *rx_isr_INTC_SET_PD = rx_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: rx_isr_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void rx_isr_ClearPending(void)
{
    *rx_isr_INTC_CLR_PD = rx_isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

```
