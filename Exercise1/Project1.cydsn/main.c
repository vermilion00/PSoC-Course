/* =======================================================================
 *
 * Names: Luca Spengler (1399655), Fabian Fritz (1394906)
 * Emails: luca.spengler2@stud.fra-uas.de, fabian.fritz@stud.fra-uas.de
 * Date: 08.04.2024
 * Course ID: TX00DB04
 * Description: While a button is pressed, an LED will blink. This program
 *              counts the number of blinks and transmits the number over
 *              UART when an incoming transmission is received.
 *
 * =======================================================================
*/
#include "project.h"
#include <stdio.h>

#define none 0 //For use with PutCRLF, to only send CRLF without additional bits

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start(); //Initialize the UART Interface
    UART_1_PutString("Luca Spengler"); //Send name
    UART_1_PutCRLF(none);  //Send line break and carriage return
    uint8 counter = 0;  //Initialize blink counter to zero
    char convstr[4];    //Initialize an empty array to convert the string later
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
   
    for(;;) //Infinite loop 
    {
        /* Place your application code here. */
        if(!Button_Read()){ //Check if button is pressed, active low
            LedOut_Write(1);    //Turn the LED on
            counter++;  //Increment the counter
            CyDelay(200);   //Wait 0.2 s
            LedOut_Write(0);    //Turn the LED off
            CyDelay(800);   //Wait 0.8 s
        }
        if(UART_1_ReadRxStatus() == UART_1_RX_STS_FIFO_NOTEMPTY){   //Check if UART is receiving packets
            UART_1_ClearRxBuffer(); //Clear UART input buffer
            sprintf(convstr, "%i", counter);    //Convert counter to string
            UART_1_PutString(convstr);  //Output converted counter value via UART
            UART_1_PutCRLF(none);  //Send line break and carriage return
        }
    }
}

/* [] END OF FILE */
