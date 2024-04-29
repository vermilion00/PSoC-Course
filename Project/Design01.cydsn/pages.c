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

/* Contains the output logic */

#include "pages.h"

volatile uint8 page = 0;
uint16 servoVal = 0;

void setPage(uint8 action){
    //Use keys to switch display modes, e.g. boxcar averages or different sensors
    //Since only keydown events are important, only check for those
    if(page != action){
        page = action;
    }
}

void updatePage(uint8 page){
    switch(page){
        case 0:
        //Dashboard
        
        break;
        case 1:
        //Hall sensor as bar
        break;
    }
}

/* [] END OF FILE */
