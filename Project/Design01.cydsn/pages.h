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

#include "sensors.h"

#define TRANSMIT_BUFFER_SIZE 64

/* Define the various pages shown on the display */

void setPage(uint8 action);
void updatePage(uint8 page);
void updatePage0(void);

//TODO: Check if this needs to be volatile
volatile uint8 page;
uint16 servoVal;

char Transmitbuffer[TRANSMIT_BUFFER_SIZE];

/* [] END OF FILE */
