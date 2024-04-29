/*******************************************************************************
* File Name: keyRow1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_keyRow1_H) /* Pins keyRow1_H */
#define CY_PINS_keyRow1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "keyRow1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 keyRow1__PORT == 15 && ((keyRow1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    keyRow1_Write(uint8 value);
void    keyRow1_SetDriveMode(uint8 mode);
uint8   keyRow1_ReadDataReg(void);
uint8   keyRow1_Read(void);
void    keyRow1_SetInterruptMode(uint16 position, uint16 mode);
uint8   keyRow1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the keyRow1_SetDriveMode() function.
     *  @{
     */
        #define keyRow1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define keyRow1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define keyRow1_DM_RES_UP          PIN_DM_RES_UP
        #define keyRow1_DM_RES_DWN         PIN_DM_RES_DWN
        #define keyRow1_DM_OD_LO           PIN_DM_OD_LO
        #define keyRow1_DM_OD_HI           PIN_DM_OD_HI
        #define keyRow1_DM_STRONG          PIN_DM_STRONG
        #define keyRow1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define keyRow1_MASK               keyRow1__MASK
#define keyRow1_SHIFT              keyRow1__SHIFT
#define keyRow1_WIDTH              1u

/* Interrupt constants */
#if defined(keyRow1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in keyRow1_SetInterruptMode() function.
     *  @{
     */
        #define keyRow1_INTR_NONE      (uint16)(0x0000u)
        #define keyRow1_INTR_RISING    (uint16)(0x0001u)
        #define keyRow1_INTR_FALLING   (uint16)(0x0002u)
        #define keyRow1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define keyRow1_INTR_MASK      (0x01u) 
#endif /* (keyRow1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define keyRow1_PS                     (* (reg8 *) keyRow1__PS)
/* Data Register */
#define keyRow1_DR                     (* (reg8 *) keyRow1__DR)
/* Port Number */
#define keyRow1_PRT_NUM                (* (reg8 *) keyRow1__PRT) 
/* Connect to Analog Globals */                                                  
#define keyRow1_AG                     (* (reg8 *) keyRow1__AG)                       
/* Analog MUX bux enable */
#define keyRow1_AMUX                   (* (reg8 *) keyRow1__AMUX) 
/* Bidirectional Enable */                                                        
#define keyRow1_BIE                    (* (reg8 *) keyRow1__BIE)
/* Bit-mask for Aliased Register Access */
#define keyRow1_BIT_MASK               (* (reg8 *) keyRow1__BIT_MASK)
/* Bypass Enable */
#define keyRow1_BYP                    (* (reg8 *) keyRow1__BYP)
/* Port wide control signals */                                                   
#define keyRow1_CTL                    (* (reg8 *) keyRow1__CTL)
/* Drive Modes */
#define keyRow1_DM0                    (* (reg8 *) keyRow1__DM0) 
#define keyRow1_DM1                    (* (reg8 *) keyRow1__DM1)
#define keyRow1_DM2                    (* (reg8 *) keyRow1__DM2) 
/* Input Buffer Disable Override */
#define keyRow1_INP_DIS                (* (reg8 *) keyRow1__INP_DIS)
/* LCD Common or Segment Drive */
#define keyRow1_LCD_COM_SEG            (* (reg8 *) keyRow1__LCD_COM_SEG)
/* Enable Segment LCD */
#define keyRow1_LCD_EN                 (* (reg8 *) keyRow1__LCD_EN)
/* Slew Rate Control */
#define keyRow1_SLW                    (* (reg8 *) keyRow1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define keyRow1_PRTDSI__CAPS_SEL       (* (reg8 *) keyRow1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define keyRow1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) keyRow1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define keyRow1_PRTDSI__OE_SEL0        (* (reg8 *) keyRow1__PRTDSI__OE_SEL0) 
#define keyRow1_PRTDSI__OE_SEL1        (* (reg8 *) keyRow1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define keyRow1_PRTDSI__OUT_SEL0       (* (reg8 *) keyRow1__PRTDSI__OUT_SEL0) 
#define keyRow1_PRTDSI__OUT_SEL1       (* (reg8 *) keyRow1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define keyRow1_PRTDSI__SYNC_OUT       (* (reg8 *) keyRow1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(keyRow1__SIO_CFG)
    #define keyRow1_SIO_HYST_EN        (* (reg8 *) keyRow1__SIO_HYST_EN)
    #define keyRow1_SIO_REG_HIFREQ     (* (reg8 *) keyRow1__SIO_REG_HIFREQ)
    #define keyRow1_SIO_CFG            (* (reg8 *) keyRow1__SIO_CFG)
    #define keyRow1_SIO_DIFF           (* (reg8 *) keyRow1__SIO_DIFF)
#endif /* (keyRow1__SIO_CFG) */

/* Interrupt Registers */
#if defined(keyRow1__INTSTAT)
    #define keyRow1_INTSTAT            (* (reg8 *) keyRow1__INTSTAT)
    #define keyRow1_SNAP               (* (reg8 *) keyRow1__SNAP)
    
	#define keyRow1_0_INTTYPE_REG 		(* (reg8 *) keyRow1__0__INTTYPE)
#endif /* (keyRow1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_keyRow1_H */


/* [] END OF FILE */
