/*******************************************************************************
* File Name: sclPin.h  
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

#if !defined(CY_PINS_sclPin_H) /* Pins sclPin_H */
#define CY_PINS_sclPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "sclPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 sclPin__PORT == 15 && ((sclPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    sclPin_Write(uint8 value);
void    sclPin_SetDriveMode(uint8 mode);
uint8   sclPin_ReadDataReg(void);
uint8   sclPin_Read(void);
void    sclPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   sclPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the sclPin_SetDriveMode() function.
     *  @{
     */
        #define sclPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define sclPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define sclPin_DM_RES_UP          PIN_DM_RES_UP
        #define sclPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define sclPin_DM_OD_LO           PIN_DM_OD_LO
        #define sclPin_DM_OD_HI           PIN_DM_OD_HI
        #define sclPin_DM_STRONG          PIN_DM_STRONG
        #define sclPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define sclPin_MASK               sclPin__MASK
#define sclPin_SHIFT              sclPin__SHIFT
#define sclPin_WIDTH              1u

/* Interrupt constants */
#if defined(sclPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in sclPin_SetInterruptMode() function.
     *  @{
     */
        #define sclPin_INTR_NONE      (uint16)(0x0000u)
        #define sclPin_INTR_RISING    (uint16)(0x0001u)
        #define sclPin_INTR_FALLING   (uint16)(0x0002u)
        #define sclPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define sclPin_INTR_MASK      (0x01u) 
#endif /* (sclPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define sclPin_PS                     (* (reg8 *) sclPin__PS)
/* Data Register */
#define sclPin_DR                     (* (reg8 *) sclPin__DR)
/* Port Number */
#define sclPin_PRT_NUM                (* (reg8 *) sclPin__PRT) 
/* Connect to Analog Globals */                                                  
#define sclPin_AG                     (* (reg8 *) sclPin__AG)                       
/* Analog MUX bux enable */
#define sclPin_AMUX                   (* (reg8 *) sclPin__AMUX) 
/* Bidirectional Enable */                                                        
#define sclPin_BIE                    (* (reg8 *) sclPin__BIE)
/* Bit-mask for Aliased Register Access */
#define sclPin_BIT_MASK               (* (reg8 *) sclPin__BIT_MASK)
/* Bypass Enable */
#define sclPin_BYP                    (* (reg8 *) sclPin__BYP)
/* Port wide control signals */                                                   
#define sclPin_CTL                    (* (reg8 *) sclPin__CTL)
/* Drive Modes */
#define sclPin_DM0                    (* (reg8 *) sclPin__DM0) 
#define sclPin_DM1                    (* (reg8 *) sclPin__DM1)
#define sclPin_DM2                    (* (reg8 *) sclPin__DM2) 
/* Input Buffer Disable Override */
#define sclPin_INP_DIS                (* (reg8 *) sclPin__INP_DIS)
/* LCD Common or Segment Drive */
#define sclPin_LCD_COM_SEG            (* (reg8 *) sclPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define sclPin_LCD_EN                 (* (reg8 *) sclPin__LCD_EN)
/* Slew Rate Control */
#define sclPin_SLW                    (* (reg8 *) sclPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define sclPin_PRTDSI__CAPS_SEL       (* (reg8 *) sclPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define sclPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) sclPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define sclPin_PRTDSI__OE_SEL0        (* (reg8 *) sclPin__PRTDSI__OE_SEL0) 
#define sclPin_PRTDSI__OE_SEL1        (* (reg8 *) sclPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define sclPin_PRTDSI__OUT_SEL0       (* (reg8 *) sclPin__PRTDSI__OUT_SEL0) 
#define sclPin_PRTDSI__OUT_SEL1       (* (reg8 *) sclPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define sclPin_PRTDSI__SYNC_OUT       (* (reg8 *) sclPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(sclPin__SIO_CFG)
    #define sclPin_SIO_HYST_EN        (* (reg8 *) sclPin__SIO_HYST_EN)
    #define sclPin_SIO_REG_HIFREQ     (* (reg8 *) sclPin__SIO_REG_HIFREQ)
    #define sclPin_SIO_CFG            (* (reg8 *) sclPin__SIO_CFG)
    #define sclPin_SIO_DIFF           (* (reg8 *) sclPin__SIO_DIFF)
#endif /* (sclPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(sclPin__INTSTAT)
    #define sclPin_INTSTAT            (* (reg8 *) sclPin__INTSTAT)
    #define sclPin_SNAP               (* (reg8 *) sclPin__SNAP)
    
	#define sclPin_0_INTTYPE_REG 		(* (reg8 *) sclPin__0__INTTYPE)
#endif /* (sclPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_sclPin_H */


/* [] END OF FILE */
