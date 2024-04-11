/*******************************************************************************
* File Name: ledOut.h  
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

#if !defined(CY_PINS_ledOut_H) /* Pins ledOut_H */
#define CY_PINS_ledOut_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ledOut_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ledOut__PORT == 15 && ((ledOut__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ledOut_Write(uint8 value);
void    ledOut_SetDriveMode(uint8 mode);
uint8   ledOut_ReadDataReg(void);
uint8   ledOut_Read(void);
void    ledOut_SetInterruptMode(uint16 position, uint16 mode);
uint8   ledOut_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ledOut_SetDriveMode() function.
     *  @{
     */
        #define ledOut_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ledOut_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ledOut_DM_RES_UP          PIN_DM_RES_UP
        #define ledOut_DM_RES_DWN         PIN_DM_RES_DWN
        #define ledOut_DM_OD_LO           PIN_DM_OD_LO
        #define ledOut_DM_OD_HI           PIN_DM_OD_HI
        #define ledOut_DM_STRONG          PIN_DM_STRONG
        #define ledOut_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ledOut_MASK               ledOut__MASK
#define ledOut_SHIFT              ledOut__SHIFT
#define ledOut_WIDTH              1u

/* Interrupt constants */
#if defined(ledOut__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ledOut_SetInterruptMode() function.
     *  @{
     */
        #define ledOut_INTR_NONE      (uint16)(0x0000u)
        #define ledOut_INTR_RISING    (uint16)(0x0001u)
        #define ledOut_INTR_FALLING   (uint16)(0x0002u)
        #define ledOut_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ledOut_INTR_MASK      (0x01u) 
#endif /* (ledOut__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ledOut_PS                     (* (reg8 *) ledOut__PS)
/* Data Register */
#define ledOut_DR                     (* (reg8 *) ledOut__DR)
/* Port Number */
#define ledOut_PRT_NUM                (* (reg8 *) ledOut__PRT) 
/* Connect to Analog Globals */                                                  
#define ledOut_AG                     (* (reg8 *) ledOut__AG)                       
/* Analog MUX bux enable */
#define ledOut_AMUX                   (* (reg8 *) ledOut__AMUX) 
/* Bidirectional Enable */                                                        
#define ledOut_BIE                    (* (reg8 *) ledOut__BIE)
/* Bit-mask for Aliased Register Access */
#define ledOut_BIT_MASK               (* (reg8 *) ledOut__BIT_MASK)
/* Bypass Enable */
#define ledOut_BYP                    (* (reg8 *) ledOut__BYP)
/* Port wide control signals */                                                   
#define ledOut_CTL                    (* (reg8 *) ledOut__CTL)
/* Drive Modes */
#define ledOut_DM0                    (* (reg8 *) ledOut__DM0) 
#define ledOut_DM1                    (* (reg8 *) ledOut__DM1)
#define ledOut_DM2                    (* (reg8 *) ledOut__DM2) 
/* Input Buffer Disable Override */
#define ledOut_INP_DIS                (* (reg8 *) ledOut__INP_DIS)
/* LCD Common or Segment Drive */
#define ledOut_LCD_COM_SEG            (* (reg8 *) ledOut__LCD_COM_SEG)
/* Enable Segment LCD */
#define ledOut_LCD_EN                 (* (reg8 *) ledOut__LCD_EN)
/* Slew Rate Control */
#define ledOut_SLW                    (* (reg8 *) ledOut__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ledOut_PRTDSI__CAPS_SEL       (* (reg8 *) ledOut__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ledOut_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ledOut__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ledOut_PRTDSI__OE_SEL0        (* (reg8 *) ledOut__PRTDSI__OE_SEL0) 
#define ledOut_PRTDSI__OE_SEL1        (* (reg8 *) ledOut__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ledOut_PRTDSI__OUT_SEL0       (* (reg8 *) ledOut__PRTDSI__OUT_SEL0) 
#define ledOut_PRTDSI__OUT_SEL1       (* (reg8 *) ledOut__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ledOut_PRTDSI__SYNC_OUT       (* (reg8 *) ledOut__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ledOut__SIO_CFG)
    #define ledOut_SIO_HYST_EN        (* (reg8 *) ledOut__SIO_HYST_EN)
    #define ledOut_SIO_REG_HIFREQ     (* (reg8 *) ledOut__SIO_REG_HIFREQ)
    #define ledOut_SIO_CFG            (* (reg8 *) ledOut__SIO_CFG)
    #define ledOut_SIO_DIFF           (* (reg8 *) ledOut__SIO_DIFF)
#endif /* (ledOut__SIO_CFG) */

/* Interrupt Registers */
#if defined(ledOut__INTSTAT)
    #define ledOut_INTSTAT            (* (reg8 *) ledOut__INTSTAT)
    #define ledOut_SNAP               (* (reg8 *) ledOut__SNAP)
    
	#define ledOut_0_INTTYPE_REG 		(* (reg8 *) ledOut__0__INTTYPE)
#endif /* (ledOut__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ledOut_H */


/* [] END OF FILE */
