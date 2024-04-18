/*******************************************************************************
* File Name: Col4.h  
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

#if !defined(CY_PINS_Col4_H) /* Pins Col4_H */
#define CY_PINS_Col4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Col4_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Col4__PORT == 15 && ((Col4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Col4_Write(uint8 value);
void    Col4_SetDriveMode(uint8 mode);
uint8   Col4_ReadDataReg(void);
uint8   Col4_Read(void);
void    Col4_SetInterruptMode(uint16 position, uint16 mode);
uint8   Col4_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Col4_SetDriveMode() function.
     *  @{
     */
        #define Col4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Col4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Col4_DM_RES_UP          PIN_DM_RES_UP
        #define Col4_DM_RES_DWN         PIN_DM_RES_DWN
        #define Col4_DM_OD_LO           PIN_DM_OD_LO
        #define Col4_DM_OD_HI           PIN_DM_OD_HI
        #define Col4_DM_STRONG          PIN_DM_STRONG
        #define Col4_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Col4_MASK               Col4__MASK
#define Col4_SHIFT              Col4__SHIFT
#define Col4_WIDTH              1u

/* Interrupt constants */
#if defined(Col4__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Col4_SetInterruptMode() function.
     *  @{
     */
        #define Col4_INTR_NONE      (uint16)(0x0000u)
        #define Col4_INTR_RISING    (uint16)(0x0001u)
        #define Col4_INTR_FALLING   (uint16)(0x0002u)
        #define Col4_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Col4_INTR_MASK      (0x01u) 
#endif /* (Col4__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Col4_PS                     (* (reg8 *) Col4__PS)
/* Data Register */
#define Col4_DR                     (* (reg8 *) Col4__DR)
/* Port Number */
#define Col4_PRT_NUM                (* (reg8 *) Col4__PRT) 
/* Connect to Analog Globals */                                                  
#define Col4_AG                     (* (reg8 *) Col4__AG)                       
/* Analog MUX bux enable */
#define Col4_AMUX                   (* (reg8 *) Col4__AMUX) 
/* Bidirectional Enable */                                                        
#define Col4_BIE                    (* (reg8 *) Col4__BIE)
/* Bit-mask for Aliased Register Access */
#define Col4_BIT_MASK               (* (reg8 *) Col4__BIT_MASK)
/* Bypass Enable */
#define Col4_BYP                    (* (reg8 *) Col4__BYP)
/* Port wide control signals */                                                   
#define Col4_CTL                    (* (reg8 *) Col4__CTL)
/* Drive Modes */
#define Col4_DM0                    (* (reg8 *) Col4__DM0) 
#define Col4_DM1                    (* (reg8 *) Col4__DM1)
#define Col4_DM2                    (* (reg8 *) Col4__DM2) 
/* Input Buffer Disable Override */
#define Col4_INP_DIS                (* (reg8 *) Col4__INP_DIS)
/* LCD Common or Segment Drive */
#define Col4_LCD_COM_SEG            (* (reg8 *) Col4__LCD_COM_SEG)
/* Enable Segment LCD */
#define Col4_LCD_EN                 (* (reg8 *) Col4__LCD_EN)
/* Slew Rate Control */
#define Col4_SLW                    (* (reg8 *) Col4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Col4_PRTDSI__CAPS_SEL       (* (reg8 *) Col4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Col4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Col4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Col4_PRTDSI__OE_SEL0        (* (reg8 *) Col4__PRTDSI__OE_SEL0) 
#define Col4_PRTDSI__OE_SEL1        (* (reg8 *) Col4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Col4_PRTDSI__OUT_SEL0       (* (reg8 *) Col4__PRTDSI__OUT_SEL0) 
#define Col4_PRTDSI__OUT_SEL1       (* (reg8 *) Col4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Col4_PRTDSI__SYNC_OUT       (* (reg8 *) Col4__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Col4__SIO_CFG)
    #define Col4_SIO_HYST_EN        (* (reg8 *) Col4__SIO_HYST_EN)
    #define Col4_SIO_REG_HIFREQ     (* (reg8 *) Col4__SIO_REG_HIFREQ)
    #define Col4_SIO_CFG            (* (reg8 *) Col4__SIO_CFG)
    #define Col4_SIO_DIFF           (* (reg8 *) Col4__SIO_DIFF)
#endif /* (Col4__SIO_CFG) */

/* Interrupt Registers */
#if defined(Col4__INTSTAT)
    #define Col4_INTSTAT            (* (reg8 *) Col4__INTSTAT)
    #define Col4_SNAP               (* (reg8 *) Col4__SNAP)
    
	#define Col4_0_INTTYPE_REG 		(* (reg8 *) Col4__0__INTTYPE)
#endif /* (Col4__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Col4_H */


/* [] END OF FILE */
