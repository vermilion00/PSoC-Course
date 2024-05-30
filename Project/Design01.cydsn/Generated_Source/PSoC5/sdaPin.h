/*******************************************************************************
* File Name: sdaPin.h  
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

#if !defined(CY_PINS_sdaPin_H) /* Pins sdaPin_H */
#define CY_PINS_sdaPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "sdaPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 sdaPin__PORT == 15 && ((sdaPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    sdaPin_Write(uint8 value);
void    sdaPin_SetDriveMode(uint8 mode);
uint8   sdaPin_ReadDataReg(void);
uint8   sdaPin_Read(void);
void    sdaPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   sdaPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the sdaPin_SetDriveMode() function.
     *  @{
     */
        #define sdaPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define sdaPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define sdaPin_DM_RES_UP          PIN_DM_RES_UP
        #define sdaPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define sdaPin_DM_OD_LO           PIN_DM_OD_LO
        #define sdaPin_DM_OD_HI           PIN_DM_OD_HI
        #define sdaPin_DM_STRONG          PIN_DM_STRONG
        #define sdaPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define sdaPin_MASK               sdaPin__MASK
#define sdaPin_SHIFT              sdaPin__SHIFT
#define sdaPin_WIDTH              1u

/* Interrupt constants */
#if defined(sdaPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in sdaPin_SetInterruptMode() function.
     *  @{
     */
        #define sdaPin_INTR_NONE      (uint16)(0x0000u)
        #define sdaPin_INTR_RISING    (uint16)(0x0001u)
        #define sdaPin_INTR_FALLING   (uint16)(0x0002u)
        #define sdaPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define sdaPin_INTR_MASK      (0x01u) 
#endif /* (sdaPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define sdaPin_PS                     (* (reg8 *) sdaPin__PS)
/* Data Register */
#define sdaPin_DR                     (* (reg8 *) sdaPin__DR)
/* Port Number */
#define sdaPin_PRT_NUM                (* (reg8 *) sdaPin__PRT) 
/* Connect to Analog Globals */                                                  
#define sdaPin_AG                     (* (reg8 *) sdaPin__AG)                       
/* Analog MUX bux enable */
#define sdaPin_AMUX                   (* (reg8 *) sdaPin__AMUX) 
/* Bidirectional Enable */                                                        
#define sdaPin_BIE                    (* (reg8 *) sdaPin__BIE)
/* Bit-mask for Aliased Register Access */
#define sdaPin_BIT_MASK               (* (reg8 *) sdaPin__BIT_MASK)
/* Bypass Enable */
#define sdaPin_BYP                    (* (reg8 *) sdaPin__BYP)
/* Port wide control signals */                                                   
#define sdaPin_CTL                    (* (reg8 *) sdaPin__CTL)
/* Drive Modes */
#define sdaPin_DM0                    (* (reg8 *) sdaPin__DM0) 
#define sdaPin_DM1                    (* (reg8 *) sdaPin__DM1)
#define sdaPin_DM2                    (* (reg8 *) sdaPin__DM2) 
/* Input Buffer Disable Override */
#define sdaPin_INP_DIS                (* (reg8 *) sdaPin__INP_DIS)
/* LCD Common or Segment Drive */
#define sdaPin_LCD_COM_SEG            (* (reg8 *) sdaPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define sdaPin_LCD_EN                 (* (reg8 *) sdaPin__LCD_EN)
/* Slew Rate Control */
#define sdaPin_SLW                    (* (reg8 *) sdaPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define sdaPin_PRTDSI__CAPS_SEL       (* (reg8 *) sdaPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define sdaPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) sdaPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define sdaPin_PRTDSI__OE_SEL0        (* (reg8 *) sdaPin__PRTDSI__OE_SEL0) 
#define sdaPin_PRTDSI__OE_SEL1        (* (reg8 *) sdaPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define sdaPin_PRTDSI__OUT_SEL0       (* (reg8 *) sdaPin__PRTDSI__OUT_SEL0) 
#define sdaPin_PRTDSI__OUT_SEL1       (* (reg8 *) sdaPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define sdaPin_PRTDSI__SYNC_OUT       (* (reg8 *) sdaPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(sdaPin__SIO_CFG)
    #define sdaPin_SIO_HYST_EN        (* (reg8 *) sdaPin__SIO_HYST_EN)
    #define sdaPin_SIO_REG_HIFREQ     (* (reg8 *) sdaPin__SIO_REG_HIFREQ)
    #define sdaPin_SIO_CFG            (* (reg8 *) sdaPin__SIO_CFG)
    #define sdaPin_SIO_DIFF           (* (reg8 *) sdaPin__SIO_DIFF)
#endif /* (sdaPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(sdaPin__INTSTAT)
    #define sdaPin_INTSTAT            (* (reg8 *) sdaPin__INTSTAT)
    #define sdaPin_SNAP               (* (reg8 *) sdaPin__SNAP)
    
	#define sdaPin_0_INTTYPE_REG 		(* (reg8 *) sdaPin__0__INTTYPE)
#endif /* (sdaPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_sdaPin_H */


/* [] END OF FILE */
