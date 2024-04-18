/*******************************************************************************
* File Name: Col2.h  
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

#if !defined(CY_PINS_Col2_H) /* Pins Col2_H */
#define CY_PINS_Col2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Col2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Col2__PORT == 15 && ((Col2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Col2_Write(uint8 value);
void    Col2_SetDriveMode(uint8 mode);
uint8   Col2_ReadDataReg(void);
uint8   Col2_Read(void);
void    Col2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Col2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Col2_SetDriveMode() function.
     *  @{
     */
        #define Col2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Col2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Col2_DM_RES_UP          PIN_DM_RES_UP
        #define Col2_DM_RES_DWN         PIN_DM_RES_DWN
        #define Col2_DM_OD_LO           PIN_DM_OD_LO
        #define Col2_DM_OD_HI           PIN_DM_OD_HI
        #define Col2_DM_STRONG          PIN_DM_STRONG
        #define Col2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Col2_MASK               Col2__MASK
#define Col2_SHIFT              Col2__SHIFT
#define Col2_WIDTH              1u

/* Interrupt constants */
#if defined(Col2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Col2_SetInterruptMode() function.
     *  @{
     */
        #define Col2_INTR_NONE      (uint16)(0x0000u)
        #define Col2_INTR_RISING    (uint16)(0x0001u)
        #define Col2_INTR_FALLING   (uint16)(0x0002u)
        #define Col2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Col2_INTR_MASK      (0x01u) 
#endif /* (Col2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Col2_PS                     (* (reg8 *) Col2__PS)
/* Data Register */
#define Col2_DR                     (* (reg8 *) Col2__DR)
/* Port Number */
#define Col2_PRT_NUM                (* (reg8 *) Col2__PRT) 
/* Connect to Analog Globals */                                                  
#define Col2_AG                     (* (reg8 *) Col2__AG)                       
/* Analog MUX bux enable */
#define Col2_AMUX                   (* (reg8 *) Col2__AMUX) 
/* Bidirectional Enable */                                                        
#define Col2_BIE                    (* (reg8 *) Col2__BIE)
/* Bit-mask for Aliased Register Access */
#define Col2_BIT_MASK               (* (reg8 *) Col2__BIT_MASK)
/* Bypass Enable */
#define Col2_BYP                    (* (reg8 *) Col2__BYP)
/* Port wide control signals */                                                   
#define Col2_CTL                    (* (reg8 *) Col2__CTL)
/* Drive Modes */
#define Col2_DM0                    (* (reg8 *) Col2__DM0) 
#define Col2_DM1                    (* (reg8 *) Col2__DM1)
#define Col2_DM2                    (* (reg8 *) Col2__DM2) 
/* Input Buffer Disable Override */
#define Col2_INP_DIS                (* (reg8 *) Col2__INP_DIS)
/* LCD Common or Segment Drive */
#define Col2_LCD_COM_SEG            (* (reg8 *) Col2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Col2_LCD_EN                 (* (reg8 *) Col2__LCD_EN)
/* Slew Rate Control */
#define Col2_SLW                    (* (reg8 *) Col2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Col2_PRTDSI__CAPS_SEL       (* (reg8 *) Col2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Col2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Col2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Col2_PRTDSI__OE_SEL0        (* (reg8 *) Col2__PRTDSI__OE_SEL0) 
#define Col2_PRTDSI__OE_SEL1        (* (reg8 *) Col2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Col2_PRTDSI__OUT_SEL0       (* (reg8 *) Col2__PRTDSI__OUT_SEL0) 
#define Col2_PRTDSI__OUT_SEL1       (* (reg8 *) Col2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Col2_PRTDSI__SYNC_OUT       (* (reg8 *) Col2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Col2__SIO_CFG)
    #define Col2_SIO_HYST_EN        (* (reg8 *) Col2__SIO_HYST_EN)
    #define Col2_SIO_REG_HIFREQ     (* (reg8 *) Col2__SIO_REG_HIFREQ)
    #define Col2_SIO_CFG            (* (reg8 *) Col2__SIO_CFG)
    #define Col2_SIO_DIFF           (* (reg8 *) Col2__SIO_DIFF)
#endif /* (Col2__SIO_CFG) */

/* Interrupt Registers */
#if defined(Col2__INTSTAT)
    #define Col2_INTSTAT            (* (reg8 *) Col2__INTSTAT)
    #define Col2_SNAP               (* (reg8 *) Col2__SNAP)
    
	#define Col2_0_INTTYPE_REG 		(* (reg8 *) Col2__0__INTTYPE)
#endif /* (Col2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Col2_H */


/* [] END OF FILE */
