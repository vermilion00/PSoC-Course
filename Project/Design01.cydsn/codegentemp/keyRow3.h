/*******************************************************************************
* File Name: keyRow3.h  
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

#if !defined(CY_PINS_keyRow3_H) /* Pins keyRow3_H */
#define CY_PINS_keyRow3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "keyRow3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 keyRow3__PORT == 15 && ((keyRow3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    keyRow3_Write(uint8 value);
void    keyRow3_SetDriveMode(uint8 mode);
uint8   keyRow3_ReadDataReg(void);
uint8   keyRow3_Read(void);
void    keyRow3_SetInterruptMode(uint16 position, uint16 mode);
uint8   keyRow3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the keyRow3_SetDriveMode() function.
     *  @{
     */
        #define keyRow3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define keyRow3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define keyRow3_DM_RES_UP          PIN_DM_RES_UP
        #define keyRow3_DM_RES_DWN         PIN_DM_RES_DWN
        #define keyRow3_DM_OD_LO           PIN_DM_OD_LO
        #define keyRow3_DM_OD_HI           PIN_DM_OD_HI
        #define keyRow3_DM_STRONG          PIN_DM_STRONG
        #define keyRow3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define keyRow3_MASK               keyRow3__MASK
#define keyRow3_SHIFT              keyRow3__SHIFT
#define keyRow3_WIDTH              1u

/* Interrupt constants */
#if defined(keyRow3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in keyRow3_SetInterruptMode() function.
     *  @{
     */
        #define keyRow3_INTR_NONE      (uint16)(0x0000u)
        #define keyRow3_INTR_RISING    (uint16)(0x0001u)
        #define keyRow3_INTR_FALLING   (uint16)(0x0002u)
        #define keyRow3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define keyRow3_INTR_MASK      (0x01u) 
#endif /* (keyRow3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define keyRow3_PS                     (* (reg8 *) keyRow3__PS)
/* Data Register */
#define keyRow3_DR                     (* (reg8 *) keyRow3__DR)
/* Port Number */
#define keyRow3_PRT_NUM                (* (reg8 *) keyRow3__PRT) 
/* Connect to Analog Globals */                                                  
#define keyRow3_AG                     (* (reg8 *) keyRow3__AG)                       
/* Analog MUX bux enable */
#define keyRow3_AMUX                   (* (reg8 *) keyRow3__AMUX) 
/* Bidirectional Enable */                                                        
#define keyRow3_BIE                    (* (reg8 *) keyRow3__BIE)
/* Bit-mask for Aliased Register Access */
#define keyRow3_BIT_MASK               (* (reg8 *) keyRow3__BIT_MASK)
/* Bypass Enable */
#define keyRow3_BYP                    (* (reg8 *) keyRow3__BYP)
/* Port wide control signals */                                                   
#define keyRow3_CTL                    (* (reg8 *) keyRow3__CTL)
/* Drive Modes */
#define keyRow3_DM0                    (* (reg8 *) keyRow3__DM0) 
#define keyRow3_DM1                    (* (reg8 *) keyRow3__DM1)
#define keyRow3_DM2                    (* (reg8 *) keyRow3__DM2) 
/* Input Buffer Disable Override */
#define keyRow3_INP_DIS                (* (reg8 *) keyRow3__INP_DIS)
/* LCD Common or Segment Drive */
#define keyRow3_LCD_COM_SEG            (* (reg8 *) keyRow3__LCD_COM_SEG)
/* Enable Segment LCD */
#define keyRow3_LCD_EN                 (* (reg8 *) keyRow3__LCD_EN)
/* Slew Rate Control */
#define keyRow3_SLW                    (* (reg8 *) keyRow3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define keyRow3_PRTDSI__CAPS_SEL       (* (reg8 *) keyRow3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define keyRow3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) keyRow3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define keyRow3_PRTDSI__OE_SEL0        (* (reg8 *) keyRow3__PRTDSI__OE_SEL0) 
#define keyRow3_PRTDSI__OE_SEL1        (* (reg8 *) keyRow3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define keyRow3_PRTDSI__OUT_SEL0       (* (reg8 *) keyRow3__PRTDSI__OUT_SEL0) 
#define keyRow3_PRTDSI__OUT_SEL1       (* (reg8 *) keyRow3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define keyRow3_PRTDSI__SYNC_OUT       (* (reg8 *) keyRow3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(keyRow3__SIO_CFG)
    #define keyRow3_SIO_HYST_EN        (* (reg8 *) keyRow3__SIO_HYST_EN)
    #define keyRow3_SIO_REG_HIFREQ     (* (reg8 *) keyRow3__SIO_REG_HIFREQ)
    #define keyRow3_SIO_CFG            (* (reg8 *) keyRow3__SIO_CFG)
    #define keyRow3_SIO_DIFF           (* (reg8 *) keyRow3__SIO_DIFF)
#endif /* (keyRow3__SIO_CFG) */

/* Interrupt Registers */
#if defined(keyRow3__INTSTAT)
    #define keyRow3_INTSTAT            (* (reg8 *) keyRow3__INTSTAT)
    #define keyRow3_SNAP               (* (reg8 *) keyRow3__SNAP)
    
	#define keyRow3_0_INTTYPE_REG 		(* (reg8 *) keyRow3__0__INTTYPE)
#endif /* (keyRow3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_keyRow3_H */


/* [] END OF FILE */
