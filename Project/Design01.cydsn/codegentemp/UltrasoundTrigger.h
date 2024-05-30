/*******************************************************************************
* File Name: UltrasoundTrigger.h  
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

#if !defined(CY_PINS_UltrasoundTrigger_H) /* Pins UltrasoundTrigger_H */
#define CY_PINS_UltrasoundTrigger_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "UltrasoundTrigger_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 UltrasoundTrigger__PORT == 15 && ((UltrasoundTrigger__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    UltrasoundTrigger_Write(uint8 value);
void    UltrasoundTrigger_SetDriveMode(uint8 mode);
uint8   UltrasoundTrigger_ReadDataReg(void);
uint8   UltrasoundTrigger_Read(void);
void    UltrasoundTrigger_SetInterruptMode(uint16 position, uint16 mode);
uint8   UltrasoundTrigger_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the UltrasoundTrigger_SetDriveMode() function.
     *  @{
     */
        #define UltrasoundTrigger_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define UltrasoundTrigger_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define UltrasoundTrigger_DM_RES_UP          PIN_DM_RES_UP
        #define UltrasoundTrigger_DM_RES_DWN         PIN_DM_RES_DWN
        #define UltrasoundTrigger_DM_OD_LO           PIN_DM_OD_LO
        #define UltrasoundTrigger_DM_OD_HI           PIN_DM_OD_HI
        #define UltrasoundTrigger_DM_STRONG          PIN_DM_STRONG
        #define UltrasoundTrigger_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define UltrasoundTrigger_MASK               UltrasoundTrigger__MASK
#define UltrasoundTrigger_SHIFT              UltrasoundTrigger__SHIFT
#define UltrasoundTrigger_WIDTH              1u

/* Interrupt constants */
#if defined(UltrasoundTrigger__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in UltrasoundTrigger_SetInterruptMode() function.
     *  @{
     */
        #define UltrasoundTrigger_INTR_NONE      (uint16)(0x0000u)
        #define UltrasoundTrigger_INTR_RISING    (uint16)(0x0001u)
        #define UltrasoundTrigger_INTR_FALLING   (uint16)(0x0002u)
        #define UltrasoundTrigger_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define UltrasoundTrigger_INTR_MASK      (0x01u) 
#endif /* (UltrasoundTrigger__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define UltrasoundTrigger_PS                     (* (reg8 *) UltrasoundTrigger__PS)
/* Data Register */
#define UltrasoundTrigger_DR                     (* (reg8 *) UltrasoundTrigger__DR)
/* Port Number */
#define UltrasoundTrigger_PRT_NUM                (* (reg8 *) UltrasoundTrigger__PRT) 
/* Connect to Analog Globals */                                                  
#define UltrasoundTrigger_AG                     (* (reg8 *) UltrasoundTrigger__AG)                       
/* Analog MUX bux enable */
#define UltrasoundTrigger_AMUX                   (* (reg8 *) UltrasoundTrigger__AMUX) 
/* Bidirectional Enable */                                                        
#define UltrasoundTrigger_BIE                    (* (reg8 *) UltrasoundTrigger__BIE)
/* Bit-mask for Aliased Register Access */
#define UltrasoundTrigger_BIT_MASK               (* (reg8 *) UltrasoundTrigger__BIT_MASK)
/* Bypass Enable */
#define UltrasoundTrigger_BYP                    (* (reg8 *) UltrasoundTrigger__BYP)
/* Port wide control signals */                                                   
#define UltrasoundTrigger_CTL                    (* (reg8 *) UltrasoundTrigger__CTL)
/* Drive Modes */
#define UltrasoundTrigger_DM0                    (* (reg8 *) UltrasoundTrigger__DM0) 
#define UltrasoundTrigger_DM1                    (* (reg8 *) UltrasoundTrigger__DM1)
#define UltrasoundTrigger_DM2                    (* (reg8 *) UltrasoundTrigger__DM2) 
/* Input Buffer Disable Override */
#define UltrasoundTrigger_INP_DIS                (* (reg8 *) UltrasoundTrigger__INP_DIS)
/* LCD Common or Segment Drive */
#define UltrasoundTrigger_LCD_COM_SEG            (* (reg8 *) UltrasoundTrigger__LCD_COM_SEG)
/* Enable Segment LCD */
#define UltrasoundTrigger_LCD_EN                 (* (reg8 *) UltrasoundTrigger__LCD_EN)
/* Slew Rate Control */
#define UltrasoundTrigger_SLW                    (* (reg8 *) UltrasoundTrigger__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define UltrasoundTrigger_PRTDSI__CAPS_SEL       (* (reg8 *) UltrasoundTrigger__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define UltrasoundTrigger_PRTDSI__DBL_SYNC_IN    (* (reg8 *) UltrasoundTrigger__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define UltrasoundTrigger_PRTDSI__OE_SEL0        (* (reg8 *) UltrasoundTrigger__PRTDSI__OE_SEL0) 
#define UltrasoundTrigger_PRTDSI__OE_SEL1        (* (reg8 *) UltrasoundTrigger__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define UltrasoundTrigger_PRTDSI__OUT_SEL0       (* (reg8 *) UltrasoundTrigger__PRTDSI__OUT_SEL0) 
#define UltrasoundTrigger_PRTDSI__OUT_SEL1       (* (reg8 *) UltrasoundTrigger__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define UltrasoundTrigger_PRTDSI__SYNC_OUT       (* (reg8 *) UltrasoundTrigger__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(UltrasoundTrigger__SIO_CFG)
    #define UltrasoundTrigger_SIO_HYST_EN        (* (reg8 *) UltrasoundTrigger__SIO_HYST_EN)
    #define UltrasoundTrigger_SIO_REG_HIFREQ     (* (reg8 *) UltrasoundTrigger__SIO_REG_HIFREQ)
    #define UltrasoundTrigger_SIO_CFG            (* (reg8 *) UltrasoundTrigger__SIO_CFG)
    #define UltrasoundTrigger_SIO_DIFF           (* (reg8 *) UltrasoundTrigger__SIO_DIFF)
#endif /* (UltrasoundTrigger__SIO_CFG) */

/* Interrupt Registers */
#if defined(UltrasoundTrigger__INTSTAT)
    #define UltrasoundTrigger_INTSTAT            (* (reg8 *) UltrasoundTrigger__INTSTAT)
    #define UltrasoundTrigger_SNAP               (* (reg8 *) UltrasoundTrigger__SNAP)
    
	#define UltrasoundTrigger_0_INTTYPE_REG 		(* (reg8 *) UltrasoundTrigger__0__INTTYPE)
#endif /* (UltrasoundTrigger__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_UltrasoundTrigger_H */


/* [] END OF FILE */
