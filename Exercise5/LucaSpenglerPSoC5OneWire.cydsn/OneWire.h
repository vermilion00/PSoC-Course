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
