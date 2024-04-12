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

#include "OneWire.h"

int ResetBus(){
    int returnValue = 0;
    onewire_pin_Write(LOW);
    CyDelayUs(WAIT_H);
    onewire_pin_Write(HIGH);
    CyDelayUs(WAIT_I);
    returnValue = onewire_pin_Read() ^ 0x01;
    CyDelayUs(WAIT_J);
    return(returnValue);
}

void WriteBit(int message){
    onewire_pin_Write(LOW);       
    if(message == HIGH){
        CyDelayUs(WAIT_A); //1 write
        onewire_pin_Write(HIGH);
        CyDelayUs(WAIT_B);
    }else{     		    
        CyDelayUs(WAIT_C); //0 write
        onewire_pin_Write(HIGH);
        CyDelayUs(WAIT_D);
    }
}

void WriteByte(int message){ 
    for (uint8 shiftcount = 0; shiftcount <= 7; shiftcount++) {
        WriteBit((message >> shiftcount) & 0x01);
    }
}

int ReadBit(){ 
    onewire_pin_Write(LOW);    // signal want to read
    CyDelayUs(WAIT_A); 
    onewire_pin_Write(HIGH);    // send pin high so can read device response on line 
    CyDelayUs(WAIT_E);
    int resultValue = onewire_pin_Read();
    CyDelayUs(WAIT_F);
 	return(resultValue);
}

int ReadByte(){
	uint8 IncomingByte = 0, shiftcount = 0;
	for(shiftcount = 0; shiftcount <= 7; shiftcount++){
        IncomingByte >>= 1;
        if(ReadBit()){
            IncomingByte |= 0x80;
        }
    }
    return(IncomingByte);
}

/* [] END OF FILE */
