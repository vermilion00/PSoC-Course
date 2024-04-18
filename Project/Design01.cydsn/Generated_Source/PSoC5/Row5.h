/*******************************************************************************
* File Name: Row5.h  
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

#if !defined(CY_PINS_Row5_H) /* Pins Row5_H */
#define CY_PINS_Row5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Row5_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Row5__PORT == 15 && ((Row5__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Row5_Write(uint8 value);
void    Row5_SetDriveMode(uint8 mode);
uint8   Row5_ReadDataReg(void);
uint8   Row5_Read(void);
void    Row5_SetInterruptMode(uint16 position, uint16 mode);
uint8   Row5_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Row5_SetDriveMode() function.
     *  @{
     */
        #define Row5_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Row5_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Row5_DM_RES_UP          PIN_DM_RES_UP
        #define Row5_DM_RES_DWN         PIN_DM_RES_DWN
        #define Row5_DM_OD_LO           PIN_DM_OD_LO
        #define Row5_DM_OD_HI           PIN_DM_OD_HI
        #define Row5_DM_STRONG          PIN_DM_STRONG
        #define Row5_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Row5_MASK               Row5__MASK
#define Row5_SHIFT              Row5__SHIFT
#define Row5_WIDTH              1u

/* Interrupt constants */
#if defined(Row5__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Row5_SetInterruptMode() function.
     *  @{
     */
        #define Row5_INTR_NONE      (uint16)(0x0000u)
        #define Row5_INTR_RISING    (uint16)(0x0001u)
        #define Row5_INTR_FALLING   (uint16)(0x0002u)
        #define Row5_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Row5_INTR_MASK      (0x01u) 
#endif /* (Row5__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Row5_PS                     (* (reg8 *) Row5__PS)
/* Data Register */
#define Row5_DR                     (* (reg8 *) Row5__DR)
/* Port Number */
#define Row5_PRT_NUM                (* (reg8 *) Row5__PRT) 
/* Connect to Analog Globals */                                                  
#define Row5_AG                     (* (reg8 *) Row5__AG)                       
/* Analog MUX bux enable */
#define Row5_AMUX                   (* (reg8 *) Row5__AMUX) 
/* Bidirectional Enable */                                                        
#define Row5_BIE                    (* (reg8 *) Row5__BIE)
/* Bit-mask for Aliased Register Access */
#define Row5_BIT_MASK               (* (reg8 *) Row5__BIT_MASK)
/* Bypass Enable */
#define Row5_BYP                    (* (reg8 *) Row5__BYP)
/* Port wide control signals */                                                   
#define Row5_CTL                    (* (reg8 *) Row5__CTL)
/* Drive Modes */
#define Row5_DM0                    (* (reg8 *) Row5__DM0) 
#define Row5_DM1                    (* (reg8 *) Row5__DM1)
#define Row5_DM2                    (* (reg8 *) Row5__DM2) 
/* Input Buffer Disable Override */
#define Row5_INP_DIS                (* (reg8 *) Row5__INP_DIS)
/* LCD Common or Segment Drive */
#define Row5_LCD_COM_SEG            (* (reg8 *) Row5__LCD_COM_SEG)
/* Enable Segment LCD */
#define Row5_LCD_EN                 (* (reg8 *) Row5__LCD_EN)
/* Slew Rate Control */
#define Row5_SLW                    (* (reg8 *) Row5__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Row5_PRTDSI__CAPS_SEL       (* (reg8 *) Row5__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Row5_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Row5__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Row5_PRTDSI__OE_SEL0        (* (reg8 *) Row5__PRTDSI__OE_SEL0) 
#define Row5_PRTDSI__OE_SEL1        (* (reg8 *) Row5__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Row5_PRTDSI__OUT_SEL0       (* (reg8 *) Row5__PRTDSI__OUT_SEL0) 
#define Row5_PRTDSI__OUT_SEL1       (* (reg8 *) Row5__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Row5_PRTDSI__SYNC_OUT       (* (reg8 *) Row5__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Row5__SIO_CFG)
    #define Row5_SIO_HYST_EN        (* (reg8 *) Row5__SIO_HYST_EN)
    #define Row5_SIO_REG_HIFREQ     (* (reg8 *) Row5__SIO_REG_HIFREQ)
    #define Row5_SIO_CFG            (* (reg8 *) Row5__SIO_CFG)
    #define Row5_SIO_DIFF           (* (reg8 *) Row5__SIO_DIFF)
#endif /* (Row5__SIO_CFG) */

/* Interrupt Registers */
#if defined(Row5__INTSTAT)
    #define Row5_INTSTAT            (* (reg8 *) Row5__INTSTAT)
    #define Row5_SNAP               (* (reg8 *) Row5__SNAP)
    
	#define Row5_0_INTTYPE_REG 		(* (reg8 *) Row5__0__INTTYPE)
#endif /* (Row5__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Row5_H */


/* [] END OF FILE */
