# Exercise 5:
# Contents of main.c:

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



# Contents of OneWire.c:

/* ===============================================================================
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
*/

#include "OneWire.h"

int ResetBus(){ //Reset the communication bus at the start of communications
    int returnValue = 0;
    onewire_pin_Write(LOW); //Write low to the communication pin
    CyDelayUs(WAIT_H);  //Wait 480 us
    onewire_pin_Write(HIGH);    //Write high to the communication pin
    CyDelayUs(WAIT_I);  //Wait 70 us
    returnValue = onewire_pin_Read() ^ 0x01;    //Read the pin status
    CyDelayUs(WAIT_J);  //Wait 410 us
    return(returnValue);    //return the pin status
}

void WriteBit(int message){ //Write a single bit to the slave
    onewire_pin_Write(LOW);     //Write low to the communication pin
    if(message == HIGH){    //Check if writing a 1 or a 0
        CyDelayUs(WAIT_A); //Wait 6 us
        onewire_pin_Write(HIGH);    //Write high
        CyDelayUs(WAIT_B);  //Wait 64 us
    }else{     		    
        CyDelayUs(WAIT_C); //Wait 60 us
        onewire_pin_Write(HIGH);    //Write high
        CyDelayUs(WAIT_D);  //Wait 10 us
    }
}

void WriteByte(int message){ //Write a full byte to the slave
    for (uint8 shiftcount = 0; shiftcount <= 7; shiftcount++){  //Works by sending 8 single bits
        WriteBit((message >> shiftcount) & 0x01);       //Shift the message bits once to the right after sending each bit
    }
}

int ReadBit(){  //Read a single bit
    onewire_pin_Write(LOW);    // Write low to the communication pin
    CyDelayUs(WAIT_A);  //Wait 6 us
    onewire_pin_Write(HIGH);    //Write high to read slave response
    CyDelayUs(WAIT_E);  //Wait 9 us
    int resultValue = onewire_pin_Read();   //Read the pin state
    CyDelayUs(WAIT_F);  //Wait 55 us
 	return(resultValue);    //return the pin state
}

int ReadByte(){     //Read a full byte
	uint8 IncomingByte = 0, shiftcount = 0;
	for(shiftcount = 0; shiftcount <= 7; shiftcount++){ //Works by reading 8 single bits
        IncomingByte >>= 1; //Shift the received message 1 bit to the right
        if(ReadBit()){  //If received bit is a 1, set the MSB
            IncomingByte |= 0x80;   //0x80 = 10000000
        }
    }
    return(IncomingByte);   //Return the read byte
}

/* [] END OF FILE */



# Contents of OneWire.h:

/* ===============================================================================
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
*/

#include <project.h>

//Define the Bus speed (STANDARD/OVERDRIVE)
#define STANDARD
//#define OVERDRIVE

#define LOW 0
#define HIGH 1
#define FALSE 0
#define TRUE 1

/* Define Commands */

#define SKIP_ROM 0xCC
#define TEMP_CONVERSION 0x44
#define READ_SCRATCH 0xBE

/* Wait times for Standard mode */
#ifdef STANDARD
    #define WAIT_A 6
    #define WAIT_B 64
    #define WAIT_C 60
    #define WAIT_D 10
    #define WAIT_E 9
    #define WAIT_F 55
    #define WAIT_G 0
    #define WAIT_H 480
    #define WAIT_I 70
    #define WAIT_J 410
    #define WAIT_K 750
#endif

/* Wait times for overdrive mode */
#ifdef OVERDRIVE
    #define WAIT_A 1.5
    #define WAIT_B 7.5
    #define WAIT_C 7.5
    #define WAIT_D 2.5
    #define WAIT_E 0.75
    #define WAIT_F 7
    #define WAIT_G 2.5
    #define WAIT_H 70
    #define WAIT_I 8.5
    #define WAIT_J 40
#endif

/* Function definitions */

int ReadBit();
int ReadByte();

void WriteBit(int message);
void WriteByte(int message);

int ResetBus();

/* [] END OF FILE */


# Contents of ADC_DelSig_1_INT.c:

/* ===============================================================================
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

#define FALSE 0
#define TRUE 1

extern volatile uint8 gADC_ISR; //global flag for the ADC ISR

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
        gADC_ISR = TRUE;    //Set the ISR flag

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
