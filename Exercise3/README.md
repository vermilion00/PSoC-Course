# Exercise 3
# Contents of main.c:

/*
* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 10.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog output of a temperature sensor and
*              outputs the average of the results over a period of 1 second
*              in mV and °C over UART in the JSON format.
*
* ===============================================================================
*
*
*******************************************************************************
* Copyright (2018-2020), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/

#include <project.h>
#include "stdio.h"
#include <stdlib.h>

/* Project Defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  64
#define SAMPLE_RATE 10000   //10000 Samples per second
#define CONVERSION_RATE 10 //10 mV per 1°C
#define TEN_CONSTANT 10 //Constant for modulo operation

volatile uint8 gADC_ISR = FALSE;    //ADC ISR flag


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs following functions:
*  1: Starts the ADC and UART components.
*  2: Checks for ADC end of conversion.  Stores latest result in output
*     if conversion complete.
*  3: Checks for UART input.
*     On 'C' or 'c' received: transmits the last sample via the UART.
*     On 'S' or 's' received: continuously transmits samples as they are completed.
*     On 'X' or 'x' received: stops continuously transmitting samples.
*     On 'E' or 'e' received: transmits a dummy byte of data.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    /* Variable to store ADC result */
    uint32 Output;
    /* Variable to store UART received character */
    uint8 Ch;
    /* Variable used to send emulated data */
    uint8 EmulatedData;
    /* Flags used to store transmit data commands */
    uint8 ContinuouslySendData;
    uint8 SendSingleByte;
    uint8 SendEmulatedData;
    /* Transmit Buffer */
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
    
    /* Start the components */
    ADC_DelSig_1_Start();
    UART_1_Start();
    
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;
    EmulatedData = 0;
    
    /* Start the ADC conversion */
    ADC_DelSig_1_StartConvert();
    
    /* Send message to verify COM port is connected properly */
    UART_1_PutString("COM Port Open\r\n");
    
    CyGlobalIntEnable;
    
    uint16 sample_count = 0; //Counts the amount of samples
    uint32 sample_sum = 0; //Stores the sum of the sample values
    uint16 average_value = 0; //Stores the average value of the sample value sum
    
    for(;;)
    {        
        /* Non-blocking call to get the latest data recieved  */
        Ch = UART_1_GetChar();
        
        /* Set flags based on UART command */
        switch(Ch)
        {
            case 0:
                /* No new data was recieved */
                break;
            case 'C':
            case 'c':
                SendSingleByte = TRUE;
                break;
            case 'S':
            case 's':
                ContinuouslySendData = TRUE;
                break;
            case 'X':
            case 'x':
                ContinuouslySendData = FALSE;
                break;
            case 'E':
            case 'e':
                SendEmulatedData = TRUE;
                break;
            default:
                /* Place error handling code here */
                break;    
        }
        if(gADC_ISR){   //Check if the ADC ISR flag is set
            gADC_ISR = FALSE;   //Reset the ISR flag
            sample_count++;     //Increment the sample counter
            sample_sum += ADC_DelSig_1_CountsTo_mVolts(ADC_DelSig_1_GetResult16());   //Add the sample values together
            if(sample_count >= SAMPLE_RATE){ //Check if the sample threshhold has been reached
                average_value = sample_sum / SAMPLE_RATE; //Calculate the average sampled value
                sprintf(TransmitBuffer, "{\r\n\t\"Avg ADC Value\": \"%1u mV\",\r\n", average_value);    //Convert the sampled value to a string
                UART_1_PutString(TransmitBuffer);   //Send the output string via UART
                sprintf(TransmitBuffer, "\t\"Temperature\": \"%i.%i C\"\r\n}\r\n", average_value / TEN_CONSTANT, average_value % TEN_CONSTANT);   //Convert the output value to a string
                UART_1_PutString(TransmitBuffer);   //Send the output string via UART
                sample_count = 0;   //Reset the sample count
                sample_sum = 0;   //Reset the sample sum value
            }
        }
        
        
            
        
        /* Check to see if an ADC conversion has completed */
        if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS))
        {
            /* Use the GetResult16 API to get an 8 bit unsigned result in
             * single ended mode.  The API CountsTo_mVolts is then used
             * to convert the ADC counts into mV before transmitting via
             * the UART.  See the datasheet API description for more
             * details */
            Output = ADC_DelSig_1_CountsTo_mVolts(ADC_DelSig_1_GetResult16());
            
            /* Send data based on last UART command */
            if(SendSingleByte || ContinuouslySendData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer, "Sample: %lu mV\r\n", Output);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer);
                /* Reset the send once flag */
                SendSingleByte = FALSE;
            }
            else if(SendEmulatedData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer, "Emulated Data: %x \r\n", EmulatedData);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer);
                EmulatedData++;
                /* Reset the send once flag */
                SendEmulatedData = FALSE;   
            }
        }
    }
}

/* [] END OF FILE */


# Contents of ADC_DelSig_1_INT.c:
# Only the interrupt flag is set here

/*
* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 10.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog output of a temperature sensor and
*              outputs the average of the results over a period of 1 second
*              in mV and °C over UART in the JSON format.
*
* ===============================================================================
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_DelSig_1.h"
#include "cyapicallbacks.h"


/*******************************************************************************
* Custom Declarations and Variables
* - add user include files, prototypes and variables between the following
*   #START and #END tags
*******************************************************************************/
/* `#START ADC_SYS_VAR`  */
#define TRUE 1
#define FALSE 0

volatile extern uint8 gADC_ISR;

/* `#END`  */

#if(ADC_DelSig_1_IRQ_REMOVE == 0u)


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR1
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR1)
    {
        #ifdef ADC_DelSig_1_ISR1_ENTRY_CALLBACK
            ADC_DelSig_1_ISR1_EntryCallback();
        #endif /* ADC_DelSig_1_ISR1_ENTRY_CALLBACK */
        

        /**************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR1`  */
        gADC_ISR = TRUE;    //Set the flag for the ADC interrupt

        /* `#END`  */
        
        /* Stop the conversion if conversion mode is single sample and resolution
        *  is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG1_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG1_CONV_MODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *  resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */

        #ifdef ADC_DelSig_1_ISR1_EXIT_CALLBACK
            ADC_DelSig_1_ISR1_ExitCallback();
        #endif /* ADC_DelSig_1_ISR1_EXIT_CALLBACK */
    }


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR2
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR2)
    {
        #ifdef ADC_DelSig_1_ISR2_ENTRY_CALLBACK
            ADC_DelSig_1_ISR2_EntryCallback();
        #endif /* ADC_DelSig_1_ISR2_ENTRY_CALLBACK */
        
        
        /***************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR2`  */

        /* `#END`  */

        /* Stop the conversion conversion mode is single sample and resolution
        *  is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG2_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG2_CONVMODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *   resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */

        #ifdef ADC_DelSig_1_ISR2_EXIT_CALLBACK
            ADC_DelSig_1_ISR2_ExitCallback();
        #endif /* ADC_DelSig_1_ISR2_EXIT_CALLBACK */
    }


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR3
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR3)
    {
        #ifdef ADC_DelSig_1_ISR3_ENTRY_CALLBACK
            ADC_DelSig_1_ISR3_EntryCallback();
        #endif /* ADC_DelSig_1_ISR3_ENTRY_CALLBACK */        

        
        /***************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR3`  */

        /* `#END`  */
        
        /* Stop the conversion if conversion mode is set to single sample and
        *  resolution is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG3_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG3_CONVMODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *  resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */

        #ifdef ADC_DelSig_1_ISR3_EXIT_CALLBACK
            ADC_DelSig_1_ISR3_ExitCallback();
        #endif /* ADC_DelSig_1_ISR3_EXIT_CALLBACK */        
    }


    /*****************************************************************************
    * Function Name: ADC_DelSig_1_ISR4
    ******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *
    *****************************************************************************/
    CY_ISR( ADC_DelSig_1_ISR4)
    {
        #ifdef ADC_DelSig_1_ISR4_ENTRY_CALLBACK
            ADC_DelSig_1_ISR4_EntryCallback();
        #endif /* ADC_DelSig_1_ISR4_ENTRY_CALLBACK */            

        
        /***************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        **************************************************************************/
        /* `#START MAIN_ADC_ISR4`  */

        /* `#END`  */

        /* Stop the conversion if conversion mode is set to single sample and
        *  resolution is above 16 bits.
        */
        #if(ADC_DelSig_1_CFG4_RESOLUTION > 16 && \
            ADC_DelSig_1_CFG4_CONVMODE == ADC_DelSig_1_MODE_SINGLE_SAMPLE)
            ADC_DelSig_1_StopConvert();
            /* Software flag for checking conversion complete or not. Will be used when
            *  resolution is above 16 bits and conversion mode is single sample 
			*/
            ADC_DelSig_1_convDone = ADC_DelSig_1_DEC_CONV_DONE;
        #endif /* Single sample conversion mode with resolution above 16 bits */
        
        #ifdef ADC_DelSig_1_ISR4_EXIT_CALLBACK
            ADC_DelSig_1_ISR4_ExitCallback();
        #endif /* ADC_DelSig_1_ISR4_EXIT_CALLBACK */            
    }

#endif   /* End ADC_DelSig_1_IRQ_REMOVE */


/* [] END OF FILE */
