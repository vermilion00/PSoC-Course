/*******************************************************************************
* File Name: Col3.h  
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

#if !defined(CY_PINS_Col3_H) /* Pins Col3_H */
#define CY_PINS_Col3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Col3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Col3__PORT == 15 && ((Col3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Col3_Write(uint8 value);
void    Col3_SetDriveMode(uint8 mode);
uint8   Col3_ReadDataReg(void);
uint8   Col3_Read(void);
void    Col3_SetInterruptMode(uint16 position, uint16 mode);
uint8   Col3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Col3_SetDriveMode() function.
     *  @{
     */
        #define Col3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Col3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Col3_DM_RES_UP          PIN_DM_RES_UP
        #define Col3_DM_RES_DWN         PIN_DM_RES_DWN
        #define Col3_DM_OD_LO           PIN_DM_OD_LO
        #define Col3_DM_OD_HI           PIN_DM_OD_HI
        #define Col3_DM_STRONG          PIN_DM_STRONG
        #define Col3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Col3_MASK               Col3__MASK
#define Col3_SHIFT              Col3__SHIFT
#define Col3_WIDTH              1u

/* Interrupt constants */
#if defined(Col3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Col3_SetInterruptMode() function.
     *  @{
     */
        #define Col3_INTR_NONE      (uint16)(0x0000u)
        #define Col3_INTR_RISING    (uint16)(0x0001u)
        #define Col3_INTR_FALLING   (uint16)(0x0002u)
        #define Col3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Col3_INTR_MASK      (0x01u) 
#endif /* (Col3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Col3_PS                     (* (reg8 *) Col3__PS)
/* Data Register */
#define Col3_DR                     (* (reg8 *) Col3__DR)
/* Port Number */
#define Col3_PRT_NUM                (* (reg8 *) Col3__PRT) 
/* Connect to Analog Globals */                                                  
#define Col3_AG                     (* (reg8 *) Col3__AG)                       
/* Analog MUX bux enable */
#define Col3_AMUX                   (* (reg8 *) Col3__AMUX) 
/* Bidirectional Enable */                                                        
#define Col3_BIE                    (* (reg8 *) Col3__BIE)
/* Bit-mask for Aliased Register Access */
#define Col3_BIT_MASK               (* (reg8 *) Col3__BIT_MASK)
/* Bypass Enable */
#define Col3_BYP                    (* (reg8 *) Col3__BYP)
/* Port wide control signals */                                                   
#define Col3_CTL                    (* (reg8 *) Col3__CTL)
/* Drive Modes */
#define Col3_DM0                    (* (reg8 *) Col3__DM0) 
#define Col3_DM1                    (* (reg8 *) Col3__DM1)
#define Col3_DM2                    (* (reg8 *) Col3__DM2) 
/* Input Buffer Disable Override */
#define Col3_INP_DIS                (* (reg8 *) Col3__INP_DIS)
/* LCD Common or Segment Drive */
#define Col3_LCD_COM_SEG            (* (reg8 *) Col3__LCD_COM_SEG)
/* Enable Segment LCD */
#define Col3_LCD_EN                 (* (reg8 *) Col3__LCD_EN)
/* Slew Rate Control */
#define Col3_SLW                    (* (reg8 *) Col3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Col3_PRTDSI__CAPS_SEL       (* (reg8 *) Col3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Col3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Col3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Col3_PRTDSI__OE_SEL0        (* (reg8 *) Col3__PRTDSI__OE_SEL0) 
#define Col3_PRTDSI__OE_SEL1        (* (reg8 *) Col3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Col3_PRTDSI__OUT_SEL0       (* (reg8 *) Col3__PRTDSI__OUT_SEL0) 
#define Col3_PRTDSI__OUT_SEL1       (* (reg8 *) Col3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Col3_PRTDSI__SYNC_OUT       (* (reg8 *) Col3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Col3__SIO_CFG)
    #define Col3_SIO_HYST_EN        (* (reg8 *) Col3__SIO_HYST_EN)
    #define Col3_SIO_REG_HIFREQ     (* (reg8 *) Col3__SIO_REG_HIFREQ)
    #define Col3_SIO_CFG            (* (reg8 *) Col3__SIO_CFG)
    #define Col3_SIO_DIFF           (* (reg8 *) Col3__SIO_DIFF)
#endif /* (Col3__SIO_CFG) */

/* Interrupt Registers */
#if defined(Col3__INTSTAT)
    #define Col3_INTSTAT            (* (reg8 *) Col3__INTSTAT)
    #define Col3_SNAP               (* (reg8 *) Col3__SNAP)
    
	#define Col3_0_INTTYPE_REG 		(* (reg8 *) Col3__0__INTTYPE)
#endif /* (Col3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Col3_H */


/* [] END OF FILE */
