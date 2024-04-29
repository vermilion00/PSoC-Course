/*******************************************************************************
* File Name: Temperature.h  
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

#if !defined(CY_PINS_Temperature_H) /* Pins Temperature_H */
#define CY_PINS_Temperature_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Temperature_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Temperature__PORT == 15 && ((Temperature__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Temperature_Write(uint8 value);
void    Temperature_SetDriveMode(uint8 mode);
uint8   Temperature_ReadDataReg(void);
uint8   Temperature_Read(void);
void    Temperature_SetInterruptMode(uint16 position, uint16 mode);
uint8   Temperature_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Temperature_SetDriveMode() function.
     *  @{
     */
        #define Temperature_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Temperature_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Temperature_DM_RES_UP          PIN_DM_RES_UP
        #define Temperature_DM_RES_DWN         PIN_DM_RES_DWN
        #define Temperature_DM_OD_LO           PIN_DM_OD_LO
        #define Temperature_DM_OD_HI           PIN_DM_OD_HI
        #define Temperature_DM_STRONG          PIN_DM_STRONG
        #define Temperature_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Temperature_MASK               Temperature__MASK
#define Temperature_SHIFT              Temperature__SHIFT
#define Temperature_WIDTH              1u

/* Interrupt constants */
#if defined(Temperature__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Temperature_SetInterruptMode() function.
     *  @{
     */
        #define Temperature_INTR_NONE      (uint16)(0x0000u)
        #define Temperature_INTR_RISING    (uint16)(0x0001u)
        #define Temperature_INTR_FALLING   (uint16)(0x0002u)
        #define Temperature_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Temperature_INTR_MASK      (0x01u) 
#endif /* (Temperature__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Temperature_PS                     (* (reg8 *) Temperature__PS)
/* Data Register */
#define Temperature_DR                     (* (reg8 *) Temperature__DR)
/* Port Number */
#define Temperature_PRT_NUM                (* (reg8 *) Temperature__PRT) 
/* Connect to Analog Globals */                                                  
#define Temperature_AG                     (* (reg8 *) Temperature__AG)                       
/* Analog MUX bux enable */
#define Temperature_AMUX                   (* (reg8 *) Temperature__AMUX) 
/* Bidirectional Enable */                                                        
#define Temperature_BIE                    (* (reg8 *) Temperature__BIE)
/* Bit-mask for Aliased Register Access */
#define Temperature_BIT_MASK               (* (reg8 *) Temperature__BIT_MASK)
/* Bypass Enable */
#define Temperature_BYP                    (* (reg8 *) Temperature__BYP)
/* Port wide control signals */                                                   
#define Temperature_CTL                    (* (reg8 *) Temperature__CTL)
/* Drive Modes */
#define Temperature_DM0                    (* (reg8 *) Temperature__DM0) 
#define Temperature_DM1                    (* (reg8 *) Temperature__DM1)
#define Temperature_DM2                    (* (reg8 *) Temperature__DM2) 
/* Input Buffer Disable Override */
#define Temperature_INP_DIS                (* (reg8 *) Temperature__INP_DIS)
/* LCD Common or Segment Drive */
#define Temperature_LCD_COM_SEG            (* (reg8 *) Temperature__LCD_COM_SEG)
/* Enable Segment LCD */
#define Temperature_LCD_EN                 (* (reg8 *) Temperature__LCD_EN)
/* Slew Rate Control */
#define Temperature_SLW                    (* (reg8 *) Temperature__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Temperature_PRTDSI__CAPS_SEL       (* (reg8 *) Temperature__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Temperature_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Temperature__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Temperature_PRTDSI__OE_SEL0        (* (reg8 *) Temperature__PRTDSI__OE_SEL0) 
#define Temperature_PRTDSI__OE_SEL1        (* (reg8 *) Temperature__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Temperature_PRTDSI__OUT_SEL0       (* (reg8 *) Temperature__PRTDSI__OUT_SEL0) 
#define Temperature_PRTDSI__OUT_SEL1       (* (reg8 *) Temperature__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Temperature_PRTDSI__SYNC_OUT       (* (reg8 *) Temperature__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Temperature__SIO_CFG)
    #define Temperature_SIO_HYST_EN        (* (reg8 *) Temperature__SIO_HYST_EN)
    #define Temperature_SIO_REG_HIFREQ     (* (reg8 *) Temperature__SIO_REG_HIFREQ)
    #define Temperature_SIO_CFG            (* (reg8 *) Temperature__SIO_CFG)
    #define Temperature_SIO_DIFF           (* (reg8 *) Temperature__SIO_DIFF)
#endif /* (Temperature__SIO_CFG) */

/* Interrupt Registers */
#if defined(Temperature__INTSTAT)
    #define Temperature_INTSTAT            (* (reg8 *) Temperature__INTSTAT)
    #define Temperature_SNAP               (* (reg8 *) Temperature__SNAP)
    
	#define Temperature_0_INTTYPE_REG 		(* (reg8 *) Temperature__0__INTTYPE)
#endif /* (Temperature__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Temperature_H */


/* [] END OF FILE */
