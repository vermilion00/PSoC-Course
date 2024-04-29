/*******************************************************************************
* File Name: keyRow0.h  
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

#if !defined(CY_PINS_keyRow0_H) /* Pins keyRow0_H */
#define CY_PINS_keyRow0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "keyRow0_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 keyRow0__PORT == 15 && ((keyRow0__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    keyRow0_Write(uint8 value);
void    keyRow0_SetDriveMode(uint8 mode);
uint8   keyRow0_ReadDataReg(void);
uint8   keyRow0_Read(void);
void    keyRow0_SetInterruptMode(uint16 position, uint16 mode);
uint8   keyRow0_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the keyRow0_SetDriveMode() function.
     *  @{
     */
        #define keyRow0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define keyRow0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define keyRow0_DM_RES_UP          PIN_DM_RES_UP
        #define keyRow0_DM_RES_DWN         PIN_DM_RES_DWN
        #define keyRow0_DM_OD_LO           PIN_DM_OD_LO
        #define keyRow0_DM_OD_HI           PIN_DM_OD_HI
        #define keyRow0_DM_STRONG          PIN_DM_STRONG
        #define keyRow0_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define keyRow0_MASK               keyRow0__MASK
#define keyRow0_SHIFT              keyRow0__SHIFT
#define keyRow0_WIDTH              1u

/* Interrupt constants */
#if defined(keyRow0__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in keyRow0_SetInterruptMode() function.
     *  @{
     */
        #define keyRow0_INTR_NONE      (uint16)(0x0000u)
        #define keyRow0_INTR_RISING    (uint16)(0x0001u)
        #define keyRow0_INTR_FALLING   (uint16)(0x0002u)
        #define keyRow0_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define keyRow0_INTR_MASK      (0x01u) 
#endif /* (keyRow0__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define keyRow0_PS                     (* (reg8 *) keyRow0__PS)
/* Data Register */
#define keyRow0_DR                     (* (reg8 *) keyRow0__DR)
/* Port Number */
#define keyRow0_PRT_NUM                (* (reg8 *) keyRow0__PRT) 
/* Connect to Analog Globals */                                                  
#define keyRow0_AG                     (* (reg8 *) keyRow0__AG)                       
/* Analog MUX bux enable */
#define keyRow0_AMUX                   (* (reg8 *) keyRow0__AMUX) 
/* Bidirectional Enable */                                                        
#define keyRow0_BIE                    (* (reg8 *) keyRow0__BIE)
/* Bit-mask for Aliased Register Access */
#define keyRow0_BIT_MASK               (* (reg8 *) keyRow0__BIT_MASK)
/* Bypass Enable */
#define keyRow0_BYP                    (* (reg8 *) keyRow0__BYP)
/* Port wide control signals */                                                   
#define keyRow0_CTL                    (* (reg8 *) keyRow0__CTL)
/* Drive Modes */
#define keyRow0_DM0                    (* (reg8 *) keyRow0__DM0) 
#define keyRow0_DM1                    (* (reg8 *) keyRow0__DM1)
#define keyRow0_DM2                    (* (reg8 *) keyRow0__DM2) 
/* Input Buffer Disable Override */
#define keyRow0_INP_DIS                (* (reg8 *) keyRow0__INP_DIS)
/* LCD Common or Segment Drive */
#define keyRow0_LCD_COM_SEG            (* (reg8 *) keyRow0__LCD_COM_SEG)
/* Enable Segment LCD */
#define keyRow0_LCD_EN                 (* (reg8 *) keyRow0__LCD_EN)
/* Slew Rate Control */
#define keyRow0_SLW                    (* (reg8 *) keyRow0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define keyRow0_PRTDSI__CAPS_SEL       (* (reg8 *) keyRow0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define keyRow0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) keyRow0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define keyRow0_PRTDSI__OE_SEL0        (* (reg8 *) keyRow0__PRTDSI__OE_SEL0) 
#define keyRow0_PRTDSI__OE_SEL1        (* (reg8 *) keyRow0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define keyRow0_PRTDSI__OUT_SEL0       (* (reg8 *) keyRow0__PRTDSI__OUT_SEL0) 
#define keyRow0_PRTDSI__OUT_SEL1       (* (reg8 *) keyRow0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define keyRow0_PRTDSI__SYNC_OUT       (* (reg8 *) keyRow0__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(keyRow0__SIO_CFG)
    #define keyRow0_SIO_HYST_EN        (* (reg8 *) keyRow0__SIO_HYST_EN)
    #define keyRow0_SIO_REG_HIFREQ     (* (reg8 *) keyRow0__SIO_REG_HIFREQ)
    #define keyRow0_SIO_CFG            (* (reg8 *) keyRow0__SIO_CFG)
    #define keyRow0_SIO_DIFF           (* (reg8 *) keyRow0__SIO_DIFF)
#endif /* (keyRow0__SIO_CFG) */

/* Interrupt Registers */
#if defined(keyRow0__INTSTAT)
    #define keyRow0_INTSTAT            (* (reg8 *) keyRow0__INTSTAT)
    #define keyRow0_SNAP               (* (reg8 *) keyRow0__SNAP)
    
	#define keyRow0_0_INTTYPE_REG 		(* (reg8 *) keyRow0__0__INTTYPE)
#endif /* (keyRow0__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_keyRow0_H */


/* [] END OF FILE */
