/*******************************************************************************
* File Name: ledPin.h  
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

#if !defined(CY_PINS_ledPin_H) /* Pins ledPin_H */
#define CY_PINS_ledPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ledPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ledPin__PORT == 15 && ((ledPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ledPin_Write(uint8 value);
void    ledPin_SetDriveMode(uint8 mode);
uint8   ledPin_ReadDataReg(void);
uint8   ledPin_Read(void);
void    ledPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   ledPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ledPin_SetDriveMode() function.
     *  @{
     */
        #define ledPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ledPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ledPin_DM_RES_UP          PIN_DM_RES_UP
        #define ledPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define ledPin_DM_OD_LO           PIN_DM_OD_LO
        #define ledPin_DM_OD_HI           PIN_DM_OD_HI
        #define ledPin_DM_STRONG          PIN_DM_STRONG
        #define ledPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ledPin_MASK               ledPin__MASK
#define ledPin_SHIFT              ledPin__SHIFT
#define ledPin_WIDTH              1u

/* Interrupt constants */
#if defined(ledPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ledPin_SetInterruptMode() function.
     *  @{
     */
        #define ledPin_INTR_NONE      (uint16)(0x0000u)
        #define ledPin_INTR_RISING    (uint16)(0x0001u)
        #define ledPin_INTR_FALLING   (uint16)(0x0002u)
        #define ledPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ledPin_INTR_MASK      (0x01u) 
#endif /* (ledPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ledPin_PS                     (* (reg8 *) ledPin__PS)
/* Data Register */
#define ledPin_DR                     (* (reg8 *) ledPin__DR)
/* Port Number */
#define ledPin_PRT_NUM                (* (reg8 *) ledPin__PRT) 
/* Connect to Analog Globals */                                                  
#define ledPin_AG                     (* (reg8 *) ledPin__AG)                       
/* Analog MUX bux enable */
#define ledPin_AMUX                   (* (reg8 *) ledPin__AMUX) 
/* Bidirectional Enable */                                                        
#define ledPin_BIE                    (* (reg8 *) ledPin__BIE)
/* Bit-mask for Aliased Register Access */
#define ledPin_BIT_MASK               (* (reg8 *) ledPin__BIT_MASK)
/* Bypass Enable */
#define ledPin_BYP                    (* (reg8 *) ledPin__BYP)
/* Port wide control signals */                                                   
#define ledPin_CTL                    (* (reg8 *) ledPin__CTL)
/* Drive Modes */
#define ledPin_DM0                    (* (reg8 *) ledPin__DM0) 
#define ledPin_DM1                    (* (reg8 *) ledPin__DM1)
#define ledPin_DM2                    (* (reg8 *) ledPin__DM2) 
/* Input Buffer Disable Override */
#define ledPin_INP_DIS                (* (reg8 *) ledPin__INP_DIS)
/* LCD Common or Segment Drive */
#define ledPin_LCD_COM_SEG            (* (reg8 *) ledPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define ledPin_LCD_EN                 (* (reg8 *) ledPin__LCD_EN)
/* Slew Rate Control */
#define ledPin_SLW                    (* (reg8 *) ledPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ledPin_PRTDSI__CAPS_SEL       (* (reg8 *) ledPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ledPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ledPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ledPin_PRTDSI__OE_SEL0        (* (reg8 *) ledPin__PRTDSI__OE_SEL0) 
#define ledPin_PRTDSI__OE_SEL1        (* (reg8 *) ledPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ledPin_PRTDSI__OUT_SEL0       (* (reg8 *) ledPin__PRTDSI__OUT_SEL0) 
#define ledPin_PRTDSI__OUT_SEL1       (* (reg8 *) ledPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ledPin_PRTDSI__SYNC_OUT       (* (reg8 *) ledPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ledPin__SIO_CFG)
    #define ledPin_SIO_HYST_EN        (* (reg8 *) ledPin__SIO_HYST_EN)
    #define ledPin_SIO_REG_HIFREQ     (* (reg8 *) ledPin__SIO_REG_HIFREQ)
    #define ledPin_SIO_CFG            (* (reg8 *) ledPin__SIO_CFG)
    #define ledPin_SIO_DIFF           (* (reg8 *) ledPin__SIO_DIFF)
#endif /* (ledPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(ledPin__INTSTAT)
    #define ledPin_INTSTAT            (* (reg8 *) ledPin__INTSTAT)
    #define ledPin_SNAP               (* (reg8 *) ledPin__SNAP)
    
	#define ledPin_0_INTTYPE_REG 		(* (reg8 *) ledPin__0__INTTYPE)
#endif /* (ledPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ledPin_H */


/* [] END OF FILE */
