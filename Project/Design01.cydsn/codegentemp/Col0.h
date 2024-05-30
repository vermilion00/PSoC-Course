/*******************************************************************************
* File Name: Col0.h  
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

#if !defined(CY_PINS_Col0_H) /* Pins Col0_H */
#define CY_PINS_Col0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Col0_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Col0__PORT == 15 && ((Col0__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Col0_Write(uint8 value);
void    Col0_SetDriveMode(uint8 mode);
uint8   Col0_ReadDataReg(void);
uint8   Col0_Read(void);
void    Col0_SetInterruptMode(uint16 position, uint16 mode);
uint8   Col0_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Col0_SetDriveMode() function.
     *  @{
     */
        #define Col0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Col0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Col0_DM_RES_UP          PIN_DM_RES_UP
        #define Col0_DM_RES_DWN         PIN_DM_RES_DWN
        #define Col0_DM_OD_LO           PIN_DM_OD_LO
        #define Col0_DM_OD_HI           PIN_DM_OD_HI
        #define Col0_DM_STRONG          PIN_DM_STRONG
        #define Col0_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Col0_MASK               Col0__MASK
#define Col0_SHIFT              Col0__SHIFT
#define Col0_WIDTH              1u

/* Interrupt constants */
#if defined(Col0__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Col0_SetInterruptMode() function.
     *  @{
     */
        #define Col0_INTR_NONE      (uint16)(0x0000u)
        #define Col0_INTR_RISING    (uint16)(0x0001u)
        #define Col0_INTR_FALLING   (uint16)(0x0002u)
        #define Col0_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Col0_INTR_MASK      (0x01u) 
#endif /* (Col0__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Col0_PS                     (* (reg8 *) Col0__PS)
/* Data Register */
#define Col0_DR                     (* (reg8 *) Col0__DR)
/* Port Number */
#define Col0_PRT_NUM                (* (reg8 *) Col0__PRT) 
/* Connect to Analog Globals */                                                  
#define Col0_AG                     (* (reg8 *) Col0__AG)                       
/* Analog MUX bux enable */
#define Col0_AMUX                   (* (reg8 *) Col0__AMUX) 
/* Bidirectional Enable */                                                        
#define Col0_BIE                    (* (reg8 *) Col0__BIE)
/* Bit-mask for Aliased Register Access */
#define Col0_BIT_MASK               (* (reg8 *) Col0__BIT_MASK)
/* Bypass Enable */
#define Col0_BYP                    (* (reg8 *) Col0__BYP)
/* Port wide control signals */                                                   
#define Col0_CTL                    (* (reg8 *) Col0__CTL)
/* Drive Modes */
#define Col0_DM0                    (* (reg8 *) Col0__DM0) 
#define Col0_DM1                    (* (reg8 *) Col0__DM1)
#define Col0_DM2                    (* (reg8 *) Col0__DM2) 
/* Input Buffer Disable Override */
#define Col0_INP_DIS                (* (reg8 *) Col0__INP_DIS)
/* LCD Common or Segment Drive */
#define Col0_LCD_COM_SEG            (* (reg8 *) Col0__LCD_COM_SEG)
/* Enable Segment LCD */
#define Col0_LCD_EN                 (* (reg8 *) Col0__LCD_EN)
/* Slew Rate Control */
#define Col0_SLW                    (* (reg8 *) Col0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Col0_PRTDSI__CAPS_SEL       (* (reg8 *) Col0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Col0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Col0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Col0_PRTDSI__OE_SEL0        (* (reg8 *) Col0__PRTDSI__OE_SEL0) 
#define Col0_PRTDSI__OE_SEL1        (* (reg8 *) Col0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Col0_PRTDSI__OUT_SEL0       (* (reg8 *) Col0__PRTDSI__OUT_SEL0) 
#define Col0_PRTDSI__OUT_SEL1       (* (reg8 *) Col0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Col0_PRTDSI__SYNC_OUT       (* (reg8 *) Col0__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Col0__SIO_CFG)
    #define Col0_SIO_HYST_EN        (* (reg8 *) Col0__SIO_HYST_EN)
    #define Col0_SIO_REG_HIFREQ     (* (reg8 *) Col0__SIO_REG_HIFREQ)
    #define Col0_SIO_CFG            (* (reg8 *) Col0__SIO_CFG)
    #define Col0_SIO_DIFF           (* (reg8 *) Col0__SIO_DIFF)
#endif /* (Col0__SIO_CFG) */

/* Interrupt Registers */
#if defined(Col0__INTSTAT)
    #define Col0_INTSTAT            (* (reg8 *) Col0__INTSTAT)
    #define Col0_SNAP               (* (reg8 *) Col0__SNAP)
    
	#define Col0_0_INTTYPE_REG 		(* (reg8 *) Col0__0__INTTYPE)
#endif /* (Col0__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Col0_H */


/* [] END OF FILE */
