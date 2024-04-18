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

#include "project.h"
#include "config.h"

struct switches {
    uint16 max;
    uint16 min;
} matrix[COLUMNS][ROWS];

uint16 calibrateSwitches(uint16 matrix[]){
    /* Read value at highest point */
    for(uint8 i = 0; i <= COLUMNS; i++){
        for(uint8 j = 0; j <= ROWS; j++){
            matrix[i][j].max = 
        }
    }
    
}

/* [] END OF FILE */
