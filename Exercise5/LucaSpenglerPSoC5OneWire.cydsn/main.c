/*
* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 12.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog value of a temperature sensor
*              connected over OneWire and sends the output over UART using
*              the JSON format.
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
#include "OneWire.h"

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
    
    uint8 SampleTracker = 0;
    int scratch[2] = {0, 0};
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
        
        /*Counting and conversion logic*/
        if(gADC_ISR){   //Check if the ADC ISR flag is set
            gADC_ISR = FALSE;   //Reset the ISR flag
            sample_sum += ADC_DelSig_1_CountsTo_mVolts(ADC_DelSig_1_GetResult16());   //Add the sample values together
            
            if(sample_count == 0){      //Only initiate communication at the start of every second
                int returnValue = ResetBus();   //Reset the onewire slave device
                WriteByte(SKIP_ROM);    //Skip the address as  only one device is connected
                if(SampleTracker > 0){  //Conversion only happens every second iteration
                    onewire_pin_SetDriveMode(onewire_pin_DM_STRONG);    //Change the pin drive mode for the conversion
                    WriteByte(TEMP_CONVERSION); //Initiate the conversion
                    //CyDelay(WAIT_K);
                }else{    
                    WriteByte(READ_SCRATCH);    //Tell the Slave that you want the read the scratchboard to initiate data transfer
                    for(uint8 i = 0; i < 2; i++){   //Read two bytes
                        scratch[i] = ReadByte();    //Save the return value in an array
                    }
                }
            }
            
            sample_count++; //Increment the sample counter
                    
            if(sample_count >= SAMPLE_RATE){ //Check if the sample threshhold has been reached
                average_value = sample_sum / SAMPLE_RATE; //Calculate the average sampled value
                sprintf(TransmitBuffer, "{\r\n\t\"Avg ADC Value\": \"%1u mV\",\r\n", average_value);    //Convert the sampled value to a string
                UART_1_PutString(TransmitBuffer);   //Send the output string via UART
                sprintf(TransmitBuffer, "\t\"Temperature (Int)\": \"%i.%i C\",\r\n", average_value / TEN_CONSTANT, average_value % TEN_CONSTANT);   //Convert the value to a string
                UART_1_PutString(TransmitBuffer);   //Send the output string via UART
                sample_count = 0;   //Reset the sample count
                sample_sum = 0;   //Reset the sample sum value
                
                /* Calculate the OneWire temperature */
                SampleTracker = ~SampleTracker; //Invert the sample tracker, since it only gets polled every second time
                if(SampleTracker > 0){  //Check if a conversion has happened should be polled
                    onewire_pin_SetDriveMode(onewire_pin_DM_RES_UP);    //reset the drive mode
                }
                int msg = (scratch[1] << 8) | scratch[0];   //Convert two bytes to one int
                /* Inaccurate way of obtaining the fractional part, but the accuracy of the sensor is +- 2 C anyway */
                int msg_frac = ((msg & 15) * 10) / 16;  //Only the first four bits are needed, divide by 16 for the correct fraction
                int msg_int = msg >> 4; //The first 4 bits are the fractional part
                sprintf(TransmitBuffer, "\t\"Temperature (OneWire)\": = \"%i.%i C\"\r\n}\r\n", msg_int, msg_frac); //Convert the onewire temperature to a string
                UART_1_PutString(TransmitBuffer);   //Send the output string
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
