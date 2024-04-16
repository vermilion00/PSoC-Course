#Exercise 4
#Contents of main.c:

/*
* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 11.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog outputs of several temperature
*              sensors using I2C and SPI and output the converted temperature
*              value over UART.
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
                sprintf(TransmitBuffer, "\t\"Temperature (Int)\": \"%i.%i \",\r\n", average_value / TEN_CONSTANT, average_value % TEN_CONSTANT);   //Convert the output to a string
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


#Contents of ADC_DelSig_1_INT.c:
#Only the interrupt flag is set in this file

/* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 11.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog outputs of several temperature
*              sensors using I2C and SPI and output the converted temperature
*              value over UART.
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


#Contents of SPIM_1.c:
#Only the type of some functions has been changed from uint8 to uint16

/* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 11.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog outputs of several temperature
*              sensors using I2C and SPI and output the converted temperature
*              value over UART.
*
* ===============================================================================
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPIM_1_PVT.h"

#if(SPIM_1_TX_SOFTWARE_BUF_ENABLED)
    volatile uint16 SPIM_1_txBuffer[SPIM_1_TX_BUFFER_SIZE];
    volatile uint8 SPIM_1_txBufferFull;
    volatile uint8 SPIM_1_txBufferRead;
    volatile uint8 SPIM_1_txBufferWrite;
#endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

#if(SPIM_1_RX_SOFTWARE_BUF_ENABLED)
    volatile uint16 SPIM_1_rxBuffer[SPIM_1_RX_BUFFER_SIZE];
    volatile uint8 SPIM_1_rxBufferFull;
    volatile uint8 SPIM_1_rxBufferRead;
    volatile uint8 SPIM_1_rxBufferWrite;
#endif /* (SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

uint8 SPIM_1_initVar = 0u;

volatile uint8 SPIM_1_swStatusTx;
volatile uint8 SPIM_1_swStatusRx;


/*******************************************************************************
* Function Name: SPIM_1_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_1_Init(void) 
{
    /* Initialize the Bit counter */
    SPIM_1_COUNTER_PERIOD_REG = SPIM_1_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != SPIM_1_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (SPIM_1_TX_ISR_NUMBER);
        CyIntSetPriority     (SPIM_1_TX_ISR_NUMBER,  SPIM_1_TX_ISR_PRIORITY);
        (void) CyIntSetVector(SPIM_1_TX_ISR_NUMBER, &SPIM_1_TX_ISR);
    #endif /* (0u != SPIM_1_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != SPIM_1_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (SPIM_1_RX_ISR_NUMBER);
        CyIntSetPriority     (SPIM_1_RX_ISR_NUMBER,  SPIM_1_RX_ISR_PRIORITY);
        (void) CyIntSetVector(SPIM_1_RX_ISR_NUMBER, &SPIM_1_RX_ISR);
    #endif /* (0u != SPIM_1_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    SPIM_1_ClearFIFO();

    #if(SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        SPIM_1_rxBufferFull  = 0u;
        SPIM_1_rxBufferRead  = 0u;
        SPIM_1_rxBufferWrite = 0u;
    #endif /* (SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        SPIM_1_txBufferFull  = 0u;
        SPIM_1_txBufferRead  = 0u;
        SPIM_1_txBufferWrite = 0u;
    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    (void) SPIM_1_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) SPIM_1_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    SPIM_1_TX_STATUS_MASK_REG = SPIM_1_TX_INIT_INTERRUPTS_MASK;
    SPIM_1_RX_STATUS_MASK_REG = SPIM_1_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: SPIM_1_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPIM_1_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SPIM_1_COUNTER_CONTROL_REG |= SPIM_1_CNTR_ENABLE;
    SPIM_1_TX_STATUS_ACTL_REG  |= SPIM_1_INT_ENABLE;
    SPIM_1_RX_STATUS_ACTL_REG  |= SPIM_1_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != SPIM_1_INTERNAL_CLOCK)
        SPIM_1_IntClock_Enable();
    #endif /* (0u != SPIM_1_INTERNAL_CLOCK) */

    SPIM_1_EnableTxInt();
    SPIM_1_EnableRxInt();
}


/*******************************************************************************
* Function Name: SPIM_1_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPIM_1_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_1_Start(void) 
{
    if(0u == SPIM_1_initVar)
    {
        SPIM_1_Init();
        SPIM_1_initVar = 1u;
    }

    SPIM_1_Enable();
}


/*******************************************************************************
* Function Name: SPIM_1_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void SPIM_1_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SPIM_1_TX_STATUS_ACTL_REG &= ((uint8) ~SPIM_1_INT_ENABLE);
    SPIM_1_RX_STATUS_ACTL_REG &= ((uint8) ~SPIM_1_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != SPIM_1_INTERNAL_CLOCK)
        SPIM_1_IntClock_Disable();
    #endif /* (0u != SPIM_1_INTERNAL_CLOCK) */

    SPIM_1_DisableTxInt();
    SPIM_1_DisableRxInt();
}


/*******************************************************************************
* Function Name: SPIM_1_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIM_1_EnableTxInt(void) 
{
    #if(0u != SPIM_1_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(SPIM_1_TX_ISR_NUMBER);
    #endif /* (0u != SPIM_1_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIM_1_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIM_1_EnableRxInt(void) 
{
    #if(0u != SPIM_1_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(SPIM_1_RX_ISR_NUMBER);
    #endif /* (0u != SPIM_1_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIM_1_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIM_1_DisableTxInt(void) 
{
    #if(0u != SPIM_1_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(SPIM_1_TX_ISR_NUMBER);
    #endif /* (0u != SPIM_1_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIM_1_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIM_1_DisableRxInt(void) 
{
    #if(0u != SPIM_1_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(SPIM_1_RX_ISR_NUMBER);
    #endif /* (0u != SPIM_1_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIM_1_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SPIM_1_SetTxInterruptMode(uint8 intSrc) 
{
    SPIM_1_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: SPIM_1_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SPIM_1_SetRxInterruptMode(uint8 intSrc) 
{
    SPIM_1_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: SPIM_1_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  SPIM_1_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIM_1_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SPIM_1_DisableTxInt();

        tmpStatus = SPIM_1_GET_STATUS_TX(SPIM_1_swStatusTx);
        SPIM_1_swStatusTx = 0u;

        SPIM_1_EnableTxInt();

    #else

        tmpStatus = SPIM_1_TX_STATUS_REG;

    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: SPIM_1_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  SPIM_1_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIM_1_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        SPIM_1_DisableRxInt();

        tmpStatus = SPIM_1_GET_STATUS_RX(SPIM_1_swStatusRx);
        SPIM_1_swStatusRx = 0u;

        SPIM_1_EnableRxInt();

    #else

        tmpStatus = SPIM_1_RX_STATUS_REG;

    #endif /* (SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: SPIM_1_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  SPIM_1_txBuffer[SPIM_1_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_1_WriteTxData(uint16 txData) 
{
    #if(SPIM_1_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = SPIM_1_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (SPIM_1_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == SPIM_1_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        SPIM_1_DisableTxInt();

        tempStatus = SPIM_1_GET_STATUS_TX(SPIM_1_swStatusTx);
        SPIM_1_swStatusTx = tempStatus;


        if((SPIM_1_txBufferRead == SPIM_1_txBufferWrite) &&
           (0u != (SPIM_1_swStatusTx & SPIM_1_STS_TX_FIFO_NOT_FULL)))
        {
            /* Put data element into the TX FIFO */
            CY_SET_REG16(SPIM_1_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            SPIM_1_txBufferWrite++;
            if(SPIM_1_txBufferWrite >= SPIM_1_TX_BUFFER_SIZE)
            {
                SPIM_1_txBufferWrite = 0u;
            }

            if(SPIM_1_txBufferWrite == SPIM_1_txBufferRead)
            {
                SPIM_1_txBufferRead++;
                if(SPIM_1_txBufferRead >= SPIM_1_TX_BUFFER_SIZE)
                {
                    SPIM_1_txBufferRead = 0u;
                }
                SPIM_1_txBufferFull = 1u;
            }

            SPIM_1_txBuffer[SPIM_1_txBufferWrite] = txData;

            SPIM_1_TX_STATUS_MASK_REG |= SPIM_1_STS_TX_FIFO_NOT_FULL;
        }

        SPIM_1_EnableTxInt();

    #else
        /* Wait until TX FIFO has a place */
        while(0u == (SPIM_1_TX_STATUS_REG & SPIM_1_STS_TX_FIFO_NOT_FULL))
        {
        }

        /* Put data element into the TX FIFO */
        CY_SET_REG16(SPIM_1_TXDATA_PTR, txData);

    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIM_1_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  SPIM_1_rxBuffer[SPIM_1_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint16 SPIM_1_ReadRxData(void) 
{
    uint16 rxData;

    #if(SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        SPIM_1_DisableRxInt();

        if(SPIM_1_rxBufferRead != SPIM_1_rxBufferWrite)
        {
            if(0u == SPIM_1_rxBufferFull)
            {
                SPIM_1_rxBufferRead++;
                if(SPIM_1_rxBufferRead >= SPIM_1_RX_BUFFER_SIZE)
                {
                    SPIM_1_rxBufferRead = 0u;
                }
            }
            else
            {
                SPIM_1_rxBufferFull = 0u;
            }
        }

        rxData = SPIM_1_rxBuffer[SPIM_1_rxBufferRead];

        SPIM_1_EnableRxInt();

    #else

        rxData = CY_GET_REG16(SPIM_1_RXDATA_PTR);

    #endif /* (SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: SPIM_1_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 SPIM_1_GetRxBufferSize(void) 
{
    uint8 size;

    #if(SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        SPIM_1_DisableRxInt();

        if(SPIM_1_rxBufferRead == SPIM_1_rxBufferWrite)
        {
            size = 0u;
        }
        else if(SPIM_1_rxBufferRead < SPIM_1_rxBufferWrite)
        {
            size = (SPIM_1_rxBufferWrite - SPIM_1_rxBufferRead);
        }
        else
        {
            size = (SPIM_1_RX_BUFFER_SIZE - SPIM_1_rxBufferRead) + SPIM_1_rxBufferWrite;
        }

        SPIM_1_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (SPIM_1_RX_STATUS_REG & SPIM_1_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: SPIM_1_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  SPIM_1_GetTxBufferSize(void) 
{
    uint8 size;

    #if(SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SPIM_1_DisableTxInt();

        if(SPIM_1_txBufferRead == SPIM_1_txBufferWrite)
        {
            size = 0u;
        }
        else if(SPIM_1_txBufferRead < SPIM_1_txBufferWrite)
        {
            size = (SPIM_1_txBufferWrite - SPIM_1_txBufferRead);
        }
        else
        {
            size = (SPIM_1_TX_BUFFER_SIZE - SPIM_1_txBufferRead) + SPIM_1_txBufferWrite;
        }

        SPIM_1_EnableTxInt();

    #else

        size = SPIM_1_TX_STATUS_REG;

        if(0u != (size & SPIM_1_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & SPIM_1_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = SPIM_1_FIFO_SIZE;
        }

    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: SPIM_1_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPIM_1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  SPIM_1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_1_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(SPIM_1_RX_STATUS_REG & SPIM_1_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG16(SPIM_1_RXDATA_PTR);
    }

    #if(SPIM_1_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        SPIM_1_DisableRxInt();

        SPIM_1_rxBufferFull  = 0u;
        SPIM_1_rxBufferRead  = 0u;
        SPIM_1_rxBufferWrite = 0u;

        SPIM_1_EnableRxInt();
    #endif /* (SPIM_1_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: SPIM_1_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPIM_1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  SPIM_1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_1_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    SPIM_1_AUX_CONTROL_DP0_REG |= ((uint8)  SPIM_1_TX_FIFO_CLR);
    SPIM_1_AUX_CONTROL_DP0_REG &= ((uint8) ~SPIM_1_TX_FIFO_CLR);

    #if(SPIM_1_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        SPIM_1_AUX_CONTROL_DP1_REG |= ((uint8)  SPIM_1_TX_FIFO_CLR);
        SPIM_1_AUX_CONTROL_DP1_REG &= ((uint8) ~SPIM_1_TX_FIFO_CLR);
    #endif /* (SPIM_1_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(SPIM_1_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        SPIM_1_DisableTxInt();

        SPIM_1_txBufferFull  = 0u;
        SPIM_1_txBufferRead  = 0u;
        SPIM_1_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        SPIM_1_TX_STATUS_MASK_REG &= ((uint8) ~SPIM_1_STS_TX_FIFO_NOT_FULL);

        SPIM_1_EnableTxInt();
    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != SPIM_1_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: SPIM_1_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void SPIM_1_TxEnable(void) 
    {
        SPIM_1_CONTROL_REG |= SPIM_1_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: SPIM_1_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void SPIM_1_TxDisable(void) 
    {
        SPIM_1_CONTROL_REG &= ((uint8) ~SPIM_1_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != SPIM_1_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: SPIM_1_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_1_PutArray(const uint16 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        SPIM_1_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: SPIM_1_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void SPIM_1_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(SPIM_1_RX_STATUS_REG & SPIM_1_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG16(SPIM_1_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    SPIM_1_AUX_CONTROL_DP0_REG |= ((uint8)  SPIM_1_TX_FIFO_CLR);
    SPIM_1_AUX_CONTROL_DP0_REG &= ((uint8) ~SPIM_1_TX_FIFO_CLR);

    #if(SPIM_1_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        SPIM_1_AUX_CONTROL_DP1_REG |= ((uint8)  SPIM_1_TX_FIFO_CLR);
        SPIM_1_AUX_CONTROL_DP1_REG &= ((uint8) ~SPIM_1_TX_FIFO_CLR);
    #endif /* (SPIM_1_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: SPIM_1_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIM_1_EnableInt(void) 
{
    SPIM_1_EnableRxInt();
    SPIM_1_EnableTxInt();
}


/*******************************************************************************
* Function Name: SPIM_1_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void SPIM_1_DisableInt(void) 
{
    SPIM_1_DisableTxInt();
    SPIM_1_DisableRxInt();
}


/*******************************************************************************
* Function Name: SPIM_1_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void SPIM_1_SetInterruptMode(uint8 intSrc) 
{
    SPIM_1_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~SPIM_1_STS_SPI_IDLE));
    SPIM_1_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: SPIM_1_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  SPIM_1_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 SPIM_1_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(SPIM_1_TX_SOFTWARE_BUF_ENABLED || SPIM_1_RX_SOFTWARE_BUF_ENABLED)

        SPIM_1_DisableInt();

        tmpStatus  = SPIM_1_GET_STATUS_RX(SPIM_1_swStatusRx);
        tmpStatus |= SPIM_1_GET_STATUS_TX(SPIM_1_swStatusTx);
        tmpStatus &= ((uint8) ~SPIM_1_STS_SPI_IDLE);

        SPIM_1_swStatusTx = 0u;
        SPIM_1_swStatusRx = 0u;

        SPIM_1_EnableInt();

    #else

        tmpStatus  = SPIM_1_RX_STATUS_REG;
        tmpStatus |= SPIM_1_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~SPIM_1_STS_SPI_IDLE);

    #endif /* (SPIM_1_TX_SOFTWARE_BUF_ENABLED || SPIM_1_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}

/* [] END OF FILE */





#Contents of SPIM_1.h:
#Only the type of some functions has been changed from uint8 to uint16

/* ===============================================================================
*
* Names: Luca Spengler (1399655), Fabian Fritz (1394906)
* Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
* Date: 11.04.2024
* Course ID: TX00DB04
* Description: This program reads the analog outputs of several temperature
*              sensors using I2C and SPI and output the converted temperature
*              value over UART.
*
* ===============================================================================
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_SPIM_1_H)
#define CY_SPIM_SPIM_1_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define SPIM_1_INTERNAL_CLOCK             (1u)

#if(0u != SPIM_1_INTERNAL_CLOCK)
    #include "SPIM_1_IntClock.h"
#endif /* (0u != SPIM_1_INTERNAL_CLOCK) */

#define SPIM_1_MODE                       (1u)
#define SPIM_1_DATA_WIDTH                 (15u)
#define SPIM_1_MODE_USE_ZERO              (1u)
#define SPIM_1_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define SPIM_1_TX_BUFFER_SIZE             (4u)
#define SPIM_1_RX_BUFFER_SIZE             (4u)
#define SPIM_1_INTERNAL_TX_INT_ENABLED    (1u)
#define SPIM_1_INTERNAL_RX_INT_ENABLED    (0u)

#define SPIM_1_SINGLE_REG_SIZE            (8u)
#define SPIM_1_USE_SECOND_DATAPATH        (SPIM_1_DATA_WIDTH > SPIM_1_SINGLE_REG_SIZE)

#define SPIM_1_FIFO_SIZE                  (4u)
#define SPIM_1_TX_SOFTWARE_BUF_ENABLED    ((0u != SPIM_1_INTERNAL_TX_INT_ENABLED) && \
                                                     (SPIM_1_TX_BUFFER_SIZE > SPIM_1_FIFO_SIZE))

#define SPIM_1_RX_SOFTWARE_BUF_ENABLED    ((0u != SPIM_1_INTERNAL_RX_INT_ENABLED) && \
                                                     (SPIM_1_RX_BUFFER_SIZE > SPIM_1_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} SPIM_1_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  SPIM_1_Init(void)                           ;
void  SPIM_1_Enable(void)                         ;
void  SPIM_1_Start(void)                          ;
void  SPIM_1_Stop(void)                           ;

void  SPIM_1_EnableTxInt(void)                    ;
void  SPIM_1_EnableRxInt(void)                    ;
void  SPIM_1_DisableTxInt(void)                   ;
void  SPIM_1_DisableRxInt(void)                   ;

void  SPIM_1_Sleep(void)                          ;
void  SPIM_1_Wakeup(void)                         ;
void  SPIM_1_SaveConfig(void)                     ;
void  SPIM_1_RestoreConfig(void)                  ;

void  SPIM_1_SetTxInterruptMode(uint8 intSrc)     ;
void  SPIM_1_SetRxInterruptMode(uint8 intSrc)     ;
uint8 SPIM_1_ReadTxStatus(void)                   ;
uint8 SPIM_1_ReadRxStatus(void)                   ;
void  SPIM_1_WriteTxData(uint16 txData)  \
                                                            ;
uint16 SPIM_1_ReadRxData(void) \
                                                            ;
uint8 SPIM_1_GetRxBufferSize(void)                ;
uint8 SPIM_1_GetTxBufferSize(void)                ;
void  SPIM_1_ClearRxBuffer(void)                  ;
void  SPIM_1_ClearTxBuffer(void)                  ;
void  SPIM_1_ClearFIFO(void)                              ;
void  SPIM_1_PutArray(const uint16 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != SPIM_1_BIDIRECTIONAL_MODE)
    void  SPIM_1_TxEnable(void)                   ;
    void  SPIM_1_TxDisable(void)                  ;
#endif /* (0u != SPIM_1_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(SPIM_1_TX_ISR);
CY_ISR_PROTO(SPIM_1_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 SPIM_1_initVar;


/***************************************
*           API Constants
***************************************/

#define SPIM_1_TX_ISR_NUMBER     ((uint8) (SPIM_1_TxInternalInterrupt__INTC_NUMBER))
#define SPIM_1_RX_ISR_NUMBER     ((uint8) (SPIM_1_RxInternalInterrupt__INTC_NUMBER))

#define SPIM_1_TX_ISR_PRIORITY   ((uint8) (SPIM_1_TxInternalInterrupt__INTC_PRIOR_NUM))
#define SPIM_1_RX_ISR_PRIORITY   ((uint8) (SPIM_1_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define SPIM_1_INT_ON_SPI_DONE    ((uint8) (0u   << SPIM_1_STS_SPI_DONE_SHIFT))
#define SPIM_1_INT_ON_TX_EMPTY    ((uint8) (0u   << SPIM_1_STS_TX_FIFO_EMPTY_SHIFT))
#define SPIM_1_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           SPIM_1_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPIM_1_INT_ON_BYTE_COMP   ((uint8) (1u  << SPIM_1_STS_BYTE_COMPLETE_SHIFT))
#define SPIM_1_INT_ON_SPI_IDLE    ((uint8) (0u   << SPIM_1_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define SPIM_1_INT_ON_TX_NOT_FULL_DEF ((SPIM_1_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (SPIM_1_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define SPIM_1_TX_INIT_INTERRUPTS_MASK    (SPIM_1_INT_ON_SPI_DONE  | \
                                                     SPIM_1_INT_ON_TX_EMPTY  | \
                                                     SPIM_1_INT_ON_TX_NOT_FULL_DEF | \
                                                     SPIM_1_INT_ON_BYTE_COMP | \
                                                     SPIM_1_INT_ON_SPI_IDLE)

#define SPIM_1_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          SPIM_1_STS_RX_FIFO_FULL_SHIFT))
#define SPIM_1_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          SPIM_1_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPIM_1_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          SPIM_1_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define SPIM_1_RX_INIT_INTERRUPTS_MASK    (SPIM_1_INT_ON_RX_FULL      | \
                                                     SPIM_1_INT_ON_RX_NOT_EMPTY | \
                                                     SPIM_1_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define SPIM_1_BITCTR_INIT            (((uint8) (SPIM_1_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define SPIM_1_TXDATA_REG (* (reg16 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F0_REG)
    #define SPIM_1_TXDATA_PTR (  (reg16 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F0_REG)
    #define SPIM_1_RXDATA_REG (* (reg16 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F1_REG)
    #define SPIM_1_RXDATA_PTR (  (reg16 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(SPIM_1_USE_SECOND_DATAPATH)
        #define SPIM_1_TXDATA_REG (* (reg16 *) \
                                          SPIM_1_BSPIM_sR16_Dp_u0__16BIT_F0_REG)
        #define SPIM_1_TXDATA_PTR (  (reg16 *) \
                                          SPIM_1_BSPIM_sR16_Dp_u0__16BIT_F0_REG)
        #define SPIM_1_RXDATA_REG (* (reg16 *) \
                                          SPIM_1_BSPIM_sR16_Dp_u0__16BIT_F1_REG)
        #define SPIM_1_RXDATA_PTR (  (reg16 *) \
                                          SPIM_1_BSPIM_sR16_Dp_u0__16BIT_F1_REG)
    #else
        #define SPIM_1_TXDATA_REG (* (reg8 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F0_REG)
        #define SPIM_1_TXDATA_PTR (  (reg8 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F0_REG)
        #define SPIM_1_RXDATA_REG (* (reg8 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F1_REG)
        #define SPIM_1_RXDATA_PTR (  (reg8 *) \
                                                SPIM_1_BSPIM_sR16_Dp_u0__F1_REG)
    #endif /* (SPIM_1_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define SPIM_1_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        SPIM_1_BSPIM_sR16_Dp_u0__DP_AUX_CTL_REG)
#define SPIM_1_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        SPIM_1_BSPIM_sR16_Dp_u0__DP_AUX_CTL_REG)

#if(SPIM_1_USE_SECOND_DATAPATH)
    #define SPIM_1_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        SPIM_1_BSPIM_sR16_Dp_u1__DP_AUX_CTL_REG)
    #define SPIM_1_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        SPIM_1_BSPIM_sR16_Dp_u1__DP_AUX_CTL_REG)
#endif /* (SPIM_1_USE_SECOND_DATAPATH) */

#define SPIM_1_COUNTER_PERIOD_REG     (* (reg8 *) SPIM_1_BSPIM_BitCounter__PERIOD_REG)
#define SPIM_1_COUNTER_PERIOD_PTR     (  (reg8 *) SPIM_1_BSPIM_BitCounter__PERIOD_REG)
#define SPIM_1_COUNTER_CONTROL_REG    (* (reg8 *) SPIM_1_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define SPIM_1_COUNTER_CONTROL_PTR    (  (reg8 *) SPIM_1_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define SPIM_1_TX_STATUS_REG          (* (reg8 *) SPIM_1_BSPIM_TxStsReg__STATUS_REG)
#define SPIM_1_TX_STATUS_PTR          (  (reg8 *) SPIM_1_BSPIM_TxStsReg__STATUS_REG)
#define SPIM_1_RX_STATUS_REG          (* (reg8 *) SPIM_1_BSPIM_RxStsReg__STATUS_REG)
#define SPIM_1_RX_STATUS_PTR          (  (reg8 *) SPIM_1_BSPIM_RxStsReg__STATUS_REG)

#define SPIM_1_CONTROL_REG            (* (reg8 *) \
                                      SPIM_1_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define SPIM_1_CONTROL_PTR            (  (reg8 *) \
                                      SPIM_1_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define SPIM_1_TX_STATUS_MASK_REG     (* (reg8 *) SPIM_1_BSPIM_TxStsReg__MASK_REG)
#define SPIM_1_TX_STATUS_MASK_PTR     (  (reg8 *) SPIM_1_BSPIM_TxStsReg__MASK_REG)
#define SPIM_1_RX_STATUS_MASK_REG     (* (reg8 *) SPIM_1_BSPIM_RxStsReg__MASK_REG)
#define SPIM_1_RX_STATUS_MASK_PTR     (  (reg8 *) SPIM_1_BSPIM_RxStsReg__MASK_REG)

#define SPIM_1_TX_STATUS_ACTL_REG     (* (reg8 *) SPIM_1_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPIM_1_TX_STATUS_ACTL_PTR     (  (reg8 *) SPIM_1_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define SPIM_1_RX_STATUS_ACTL_REG     (* (reg8 *) SPIM_1_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define SPIM_1_RX_STATUS_ACTL_PTR     (  (reg8 *) SPIM_1_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(SPIM_1_USE_SECOND_DATAPATH)
    #define SPIM_1_AUX_CONTROLDP1     (SPIM_1_AUX_CONTROL_DP1_REG)
#endif /* (SPIM_1_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define SPIM_1_STS_SPI_DONE_SHIFT             (0x00u)
#define SPIM_1_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define SPIM_1_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define SPIM_1_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define SPIM_1_STS_SPI_IDLE_SHIFT             (0x04u)
#define SPIM_1_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define SPIM_1_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define SPIM_1_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define SPIM_1_STS_SPI_DONE           ((uint8) (0x01u << SPIM_1_STS_SPI_DONE_SHIFT))
#define SPIM_1_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << SPIM_1_STS_TX_FIFO_EMPTY_SHIFT))
#define SPIM_1_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << SPIM_1_STS_TX_FIFO_NOT_FULL_SHIFT))
#define SPIM_1_STS_BYTE_COMPLETE      ((uint8) (0x01u << SPIM_1_STS_BYTE_COMPLETE_SHIFT))
#define SPIM_1_STS_SPI_IDLE           ((uint8) (0x01u << SPIM_1_STS_SPI_IDLE_SHIFT))
#define SPIM_1_STS_RX_FIFO_FULL       ((uint8) (0x01u << SPIM_1_STS_RX_FIFO_FULL_SHIFT))
#define SPIM_1_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << SPIM_1_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define SPIM_1_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << SPIM_1_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define SPIM_1_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define SPIM_1_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define SPIM_1_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define SPIM_1_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define SPIM_1_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define SPIM_1_FIFO_CLR       (SPIM_1_TX_FIFO_CLR | SPIM_1_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define SPIM_1_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define SPIM_1_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define SPIM_1_AUX_CTRL_FIFO0_CLR         (0x01u)
#define SPIM_1_AUX_CTRL_FIFO1_CLR         (0x02u)
#define SPIM_1_AUX_CTRL_FIFO0_LVL         (0x04u)
#define SPIM_1_AUX_CTRL_FIFO1_LVL         (0x08u)
#define SPIM_1_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define SPIM_1_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define SPIM_1_IS_ENABLED (0u != (SPIM_1_TX_STATUS_ACTL_REG & SPIM_1_INT_ENABLE))

/* Retuns TX status register */
#define SPIM_1_GET_STATUS_TX(swTxSts) ( (uint8)(SPIM_1_TX_STATUS_REG | \
                                                          ((swTxSts) & SPIM_1_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define SPIM_1_GET_STATUS_RX(swRxSts) ( (uint8)(SPIM_1_RX_STATUS_REG | \
                                                          ((swRxSts) & SPIM_1_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define SPIM_1_WriteByte   SPIM_1_WriteTxData
#define SPIM_1_ReadByte    SPIM_1_ReadRxData
void  SPIM_1_SetInterruptMode(uint8 intSrc)       ;
uint8 SPIM_1_ReadStatus(void)                     ;
void  SPIM_1_EnableInt(void)                      ;
void  SPIM_1_DisableInt(void)                     ;

#define SPIM_1_TXDATA                 (SPIM_1_TXDATA_REG)
#define SPIM_1_RXDATA                 (SPIM_1_RXDATA_REG)
#define SPIM_1_AUX_CONTROLDP0         (SPIM_1_AUX_CONTROL_DP0_REG)
#define SPIM_1_TXBUFFERREAD           (SPIM_1_txBufferRead)
#define SPIM_1_TXBUFFERWRITE          (SPIM_1_txBufferWrite)
#define SPIM_1_RXBUFFERREAD           (SPIM_1_rxBufferRead)
#define SPIM_1_RXBUFFERWRITE          (SPIM_1_rxBufferWrite)

#define SPIM_1_COUNTER_PERIOD         (SPIM_1_COUNTER_PERIOD_REG)
#define SPIM_1_COUNTER_CONTROL        (SPIM_1_COUNTER_CONTROL_REG)
#define SPIM_1_STATUS                 (SPIM_1_TX_STATUS_REG)
#define SPIM_1_CONTROL                (SPIM_1_CONTROL_REG)
#define SPIM_1_STATUS_MASK            (SPIM_1_TX_STATUS_MASK_REG)
#define SPIM_1_STATUS_ACTL            (SPIM_1_TX_STATUS_ACTL_REG)

#define SPIM_1_INIT_INTERRUPTS_MASK  (SPIM_1_INT_ON_SPI_DONE     | \
                                                SPIM_1_INT_ON_TX_EMPTY     | \
                                                SPIM_1_INT_ON_TX_NOT_FULL_DEF  | \
                                                SPIM_1_INT_ON_RX_FULL      | \
                                                SPIM_1_INT_ON_RX_NOT_EMPTY | \
                                                SPIM_1_INT_ON_RX_OVER      | \
                                                SPIM_1_INT_ON_BYTE_COMP)
                                                
#define SPIM_1_DataWidth                  (SPIM_1_DATA_WIDTH)
#define SPIM_1_InternalClockUsed          (SPIM_1_INTERNAL_CLOCK)
#define SPIM_1_InternalTxInterruptEnabled (SPIM_1_INTERNAL_TX_INT_ENABLED)
#define SPIM_1_InternalRxInterruptEnabled (SPIM_1_INTERNAL_RX_INT_ENABLED)
#define SPIM_1_ModeUseZero                (SPIM_1_MODE_USE_ZERO)
#define SPIM_1_BidirectionalMode          (SPIM_1_BIDIRECTIONAL_MODE)
#define SPIM_1_Mode                       (SPIM_1_MODE)
#define SPIM_1_DATAWIDHT                  (SPIM_1_DATA_WIDTH)
#define SPIM_1_InternalInterruptEnabled   (0u)

#define SPIM_1_TXBUFFERSIZE   (SPIM_1_TX_BUFFER_SIZE)
#define SPIM_1_RXBUFFERSIZE   (SPIM_1_RX_BUFFER_SIZE)

#define SPIM_1_TXBUFFER       SPIM_1_txBuffer
#define SPIM_1_RXBUFFER       SPIM_1_rxBuffer

#endif /* (CY_SPIM_SPIM_1_H) */


/* [] END OF FILE */

