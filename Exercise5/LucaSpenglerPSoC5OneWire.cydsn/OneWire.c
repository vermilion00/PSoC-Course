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
