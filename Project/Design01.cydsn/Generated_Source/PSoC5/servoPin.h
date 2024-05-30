/*******************************************************************************
* File Name: servoPin.h  
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

#if !defined(CY_PINS_servoPin_H) /* Pins servoPin_H */
#define CY_PINS_servoPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "servoPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 servoPin__PORT == 15 && ((servoPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    servoPin_Write(uint8 value);
void    servoPin_SetDriveMode(uint8 mode);
uint8   servoPin_ReadDataReg(void);
uint8   servoPin_Read(void);
void    servoPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   servoPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the servoPin_SetDriveMode() function.
     *  @{
     */
        #define servoPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define servoPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define servoPin_DM_RES_UP          PIN_DM_RES_UP
        #define servoPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define servoPin_DM_OD_LO           PIN_DM_OD_LO
        #define servoPin_DM_OD_HI           PIN_DM_OD_HI
        #define servoPin_DM_STRONG          PIN_DM_STRONG
        #define servoPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define servoPin_MASK               servoPin__MASK
#define servoPin_SHIFT              servoPin__SHIFT
#define servoPin_WIDTH              1u

/* Interrupt constants */
#if defined(servoPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in servoPin_SetInterruptMode() function.
     *  @{
     */
        #define servoPin_INTR_NONE      (uint16)(0x0000u)
        #define servoPin_INTR_RISING    (uint16)(0x0001u)
        #define servoPin_INTR_FALLING   (uint16)(0x0002u)
        #define servoPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define servoPin_INTR_MASK      (0x01u) 
#endif /* (servoPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define servoPin_PS                     (* (reg8 *) servoPin__PS)
/* Data Register */
#define servoPin_DR                     (* (reg8 *) servoPin__DR)
/* Port Number */
#define servoPin_PRT_NUM                (* (reg8 *) servoPin__PRT) 
/* Connect to Analog Globals */                                                  
#define servoPin_AG                     (* (reg8 *) servoPin__AG)                       
/* Analog MUX bux enable */
#define servoPin_AMUX                   (* (reg8 *) servoPin__AMUX) 
/* Bidirectional Enable */                                                        
#define servoPin_BIE                    (* (reg8 *) servoPin__BIE)
/* Bit-mask for Aliased Register Access */
#define servoPin_BIT_MASK               (* (reg8 *) servoPin__BIT_MASK)
/* Bypass Enable */
#define servoPin_BYP                    (* (reg8 *) servoPin__BYP)
/* Port wide control signals */                                                   
#define servoPin_CTL                    (* (reg8 *) servoPin__CTL)
/* Drive Modes */
#define servoPin_DM0                    (* (reg8 *) servoPin__DM0) 
#define servoPin_DM1                    (* (reg8 *) servoPin__DM1)
#define servoPin_DM2                    (* (reg8 *) servoPin__DM2) 
/* Input Buffer Disable Override */
#define servoPin_INP_DIS                (* (reg8 *) servoPin__INP_DIS)
/* LCD Common or Segment Drive */
#define servoPin_LCD_COM_SEG            (* (reg8 *) servoPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define servoPin_LCD_EN                 (* (reg8 *) servoPin__LCD_EN)
/* Slew Rate Control */
#define servoPin_SLW                    (* (reg8 *) servoPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define servoPin_PRTDSI__CAPS_SEL       (* (reg8 *) servoPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define servoPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) servoPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define servoPin_PRTDSI__OE_SEL0        (* (reg8 *) servoPin__PRTDSI__OE_SEL0) 
#define servoPin_PRTDSI__OE_SEL1        (* (reg8 *) servoPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define servoPin_PRTDSI__OUT_SEL0       (* (reg8 *) servoPin__PRTDSI__OUT_SEL0) 
#define servoPin_PRTDSI__OUT_SEL1       (* (reg8 *) servoPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define servoPin_PRTDSI__SYNC_OUT       (* (reg8 *) servoPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(servoPin__SIO_CFG)
    #define servoPin_SIO_HYST_EN        (* (reg8 *) servoPin__SIO_HYST_EN)
    #define servoPin_SIO_REG_HIFREQ     (* (reg8 *) servoPin__SIO_REG_HIFREQ)
    #define servoPin_SIO_CFG            (* (reg8 *) servoPin__SIO_CFG)
    #define servoPin_SIO_DIFF           (* (reg8 *) servoPin__SIO_DIFF)
#endif /* (servoPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(servoPin__INTSTAT)
    #define servoPin_INTSTAT            (* (reg8 *) servoPin__INTSTAT)
    #define servoPin_SNAP               (* (reg8 *) servoPin__SNAP)
    
	#define servoPin_0_INTTYPE_REG 		(* (reg8 *) servoPin__0__INTTYPE)
#endif /* (servoPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_servoPin_H */


/* [] END OF FILE */
