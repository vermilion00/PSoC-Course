/*
* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 11.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog outputs of several temperature
*              sensors using I2C and SPI and output the converted temperature
*              value over UART. Keep in mind that even though it wasn't required,
*              the LM35 is wired to both the internal and the external ADC.
*              The temperature (INT) is displaying the value provided by the
*              internal ADC, for debugging purposes.
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
#define FALSE  0    //Fake boolean operations
#define TRUE   1    //Fake boolean operations
#define TRANSMIT_BUFFER_SIZE  64    //8 bytes transmit buffer
#define RECEIVE_BUFFER_SIZE 8   //One byte receive buffer
#define DUMMY_SIZE 2    //Number of dummy bytes to transmit
#define SAMPLE_RATE 10000   //10000 Samples per second
#define VALUE_CORRECTION 37000 //Correct the SPI value
#define TEN_CONSTANT 10 //Constant for modulo operation
#define ADDRESS_TC74 0x4D   //I2C address of the TC74
#define TEMPERATURE_COMMAND 0x00    //Command value to read temperature

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
    uint16 TransmitDummy[2] = {0,1};    //Dummy array to receive SPI communications
    uint8 ReceiveI2CVal = 0;    //Stores the received I2C value
    uint16 ReceiveSPIVal = 0;   //Stores the received SPI value
    uint8 status = 0;   //Stores I2C error messages
    uint32 SpiAdcSum = 0;   //Stores the sum of the SPI ADC values
    
    /* Start the components */
    ADC_DelSig_1_Start();
    UART_1_Start();
    I2C_1_Start();
    SPIM_1_Start();
    cs_pin_Write(1);    //Deactivate the chip select line
    
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
    uint16 average_value = 0; //Stores the average value of the sample sum
    uint16 average_value_spi = 0;   //Stores the average value of the SPI sample sum
    
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
            
            /*Get SPI ADC value*/
            SPIM_1_ClearRxBuffer(); //Clear the Rx buffer
            SPIM_1_ClearTxBuffer(); //Clear the Tx buffer
            SPIM_1_ClearFIFO(); //Clear FIFO
            cs_pin_Write(0);    //Activate the Slave
            SPIM_1_PutArray(TransmitDummy, 2);  //Transmit 16 dummy bits to start communication
            ReceiveSPIVal = ADC_DelSig_1_CountsTo_mVolts(SPIM_1_ReadRxData()) - VALUE_CORRECTION;  //Read the SPI data, convert it to mV and correct an offset
            cs_pin_Write(1);    //Deactivate the Slave
            SpiAdcSum += ReceiveSPIVal; //Add the received SPI samples together
            
            if(sample_count >= SAMPLE_RATE){ //Check if the sample threshhold has been reached
                average_value = sample_sum / SAMPLE_RATE; //Calculate the average sampled value
                average_value_spi = SpiAdcSum / SAMPLE_RATE;    //Calculate the average sampled SPI value
                sprintf(TransmitBuffer, "{\r\n\t\"Avg ADC Value\": \"%1u mV\",\r\n", average_value);    //Convert the sampled value to a string
                UART_1_PutString(TransmitBuffer);   //Output the average adc value in mV
                sprintf(TransmitBuffer, "\t\"Temperature (Int)\": \"%i.%i C\",\r\n", average_value / TEN_CONSTANT, average_value % TEN_CONSTANT);   //Convert the output to a string
                UART_1_PutString(TransmitBuffer);   //Output the Temperature using the internal ADC
                sample_count = 0;   //Reset the sample count
                sample_sum = 0;   //Reset the sample sum value
                
                //Get I2C Sensor temperature
                I2C_1_MasterClearStatus();  //Clear the status of the I2C Master
                I2C_1_MasterSendStart(ADDRESS_TC74, I2C_1_WRITE_XFER_MODE); //Send the start message to the I2C slave in write mode
                status = I2C_1_MasterWriteByte(TEMPERATURE_COMMAND);    //Send the temperature command
                //status = I2C_1_MasterWriteBuf(ADDRESS_TC74, TEMPERATURE_COMMAND, I2C_1_WRITE_XFER_MODE, I2C_1_MODE_COMPLETE_XFER);
                I2C_1_MasterSendRestart(ADDRESS_TC74, I2C_1_READ_XFER_MODE);    //Restart the communication in read mode
                ReceiveI2CVal= I2C_1_MasterReadByte(I2C_1_NAK_DATA);    //Read the temperature data
                I2C_1_MasterSendStop(); //Send the stop message to cut communications
                sprintf(TransmitBuffer, "\t\"Temperature (I2C)\": \"%i C\",\r\n", ReceiveI2CVal);   //Convert the I2C temperature value to a string
                UART_1_PutString(TransmitBuffer);   //Output the I2C temperature
                
                sprintf(TransmitBuffer, "\t\"Temperature (SPI): \"%i.%i C\"\r\n}\r\n", average_value_spi / 10, average_value % 10); //Convert the SPI temperature value to a string
                UART_1_PutString(TransmitBuffer);   //Output the SPI temperature
                SpiAdcSum = 0;  //Reset the SPI value sum
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
