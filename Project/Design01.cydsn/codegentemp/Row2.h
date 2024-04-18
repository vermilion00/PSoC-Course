/*******************************************************************************
* File Name: Row2.h  
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

#if !defined(CY_PINS_Row2_H) /* Pins Row2_H */
#define CY_PINS_Row2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Row2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Row2__PORT == 15 && ((Row2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Row2_Write(uint8 value);
void    Row2_SetDriveMode(uint8 mode);
uint8   Row2_ReadDataReg(void);
uint8   Row2_Read(void);
void    Row2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Row2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Row2_SetDriveMode() function.
     *  @{
     */
        #define Row2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Row2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Row2_DM_RES_UP          PIN_DM_RES_UP
        #define Row2_DM_RES_DWN         PIN_DM_RES_DWN
        #define Row2_DM_OD_LO           PIN_DM_OD_LO
        #define Row2_DM_OD_HI           PIN_DM_OD_HI
        #define Row2_DM_STRONG          PIN_DM_STRONG
        #define Row2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Row2_MASK               Row2__MASK
#define Row2_SHIFT              Row2__SHIFT
#define Row2_WIDTH              1u

/* Interrupt constants */
#if defined(Row2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Row2_SetInterruptMode() function.
     *  @{
     */
        #define Row2_INTR_NONE      (uint16)(0x0000u)
        #define Row2_INTR_RISING    (uint16)(0x0001u)
        #define Row2_INTR_FALLING   (uint16)(0x0002u)
        #define Row2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Row2_INTR_MASK      (0x01u) 
#endif /* (Row2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Row2_PS                     (* (reg8 *) Row2__PS)
/* Data Register */
#define Row2_DR                     (* (reg8 *) Row2__DR)
/* Port Number */
#define Row2_PRT_NUM                (* (reg8 *) Row2__PRT) 
/* Connect to Analog Globals */                                                  
#define Row2_AG                     (* (reg8 *) Row2__AG)                       
/* Analog MUX bux enable */
#define Row2_AMUX                   (* (reg8 *) Row2__AMUX) 
/* Bidirectional Enable */                                                        
#define Row2_BIE                    (* (reg8 *) Row2__BIE)
/* Bit-mask for Aliased Register Access */
#define Row2_BIT_MASK               (* (reg8 *) Row2__BIT_MASK)
/* Bypass Enable */
#define Row2_BYP                    (* (reg8 *) Row2__BYP)
/* Port wide control signals */                                                   
#define Row2_CTL                    (* (reg8 *) Row2__CTL)
/* Drive Modes */
#define Row2_DM0                    (* (reg8 *) Row2__DM0) 
#define Row2_DM1                    (* (reg8 *) Row2__DM1)
#define Row2_DM2                    (* (reg8 *) Row2__DM2) 
/* Input Buffer Disable Override */
#define Row2_INP_DIS                (* (reg8 *) Row2__INP_DIS)
/* LCD Common or Segment Drive */
#define Row2_LCD_COM_SEG            (* (reg8 *) Row2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Row2_LCD_EN                 (* (reg8 *) Row2__LCD_EN)
/* Slew Rate Control */
#define Row2_SLW                    (* (reg8 *) Row2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Row2_PRTDSI__CAPS_SEL       (* (reg8 *) Row2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Row2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Row2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Row2_PRTDSI__OE_SEL0        (* (reg8 *) Row2__PRTDSI__OE_SEL0) 
#define Row2_PRTDSI__OE_SEL1        (* (reg8 *) Row2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Row2_PRTDSI__OUT_SEL0       (* (reg8 *) Row2__PRTDSI__OUT_SEL0) 
#define Row2_PRTDSI__OUT_SEL1       (* (reg8 *) Row2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Row2_PRTDSI__SYNC_OUT       (* (reg8 *) Row2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Row2__SIO_CFG)
    #define Row2_SIO_HYST_EN        (* (reg8 *) Row2__SIO_HYST_EN)
    #define Row2_SIO_REG_HIFREQ     (* (reg8 *) Row2__SIO_REG_HIFREQ)
    #define Row2_SIO_CFG            (* (reg8 *) Row2__SIO_CFG)
    #define Row2_SIO_DIFF           (* (reg8 *) Row2__SIO_DIFF)
#endif /* (Row2__SIO_CFG) */

/* Interrupt Registers */
#if defined(Row2__INTSTAT)
    #define Row2_INTSTAT            (* (reg8 *) Row2__INTSTAT)
    #define Row2_SNAP               (* (reg8 *) Row2__SNAP)
    
	#define Row2_0_INTTYPE_REG 		(* (reg8 *) Row2__0__INTTYPE)
#endif /* (Row2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Row2_H */


/* [] END OF FILE */
