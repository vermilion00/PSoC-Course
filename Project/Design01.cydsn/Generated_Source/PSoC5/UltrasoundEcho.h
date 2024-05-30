/*******************************************************************************
* File Name: UltrasoundEcho.h  
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

#if !defined(CY_PINS_UltrasoundEcho_H) /* Pins UltrasoundEcho_H */
#define CY_PINS_UltrasoundEcho_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "UltrasoundEcho_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 UltrasoundEcho__PORT == 15 && ((UltrasoundEcho__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    UltrasoundEcho_Write(uint8 value);
void    UltrasoundEcho_SetDriveMode(uint8 mode);
uint8   UltrasoundEcho_ReadDataReg(void);
uint8   UltrasoundEcho_Read(void);
void    UltrasoundEcho_SetInterruptMode(uint16 position, uint16 mode);
uint8   UltrasoundEcho_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the UltrasoundEcho_SetDriveMode() function.
     *  @{
     */
        #define UltrasoundEcho_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define UltrasoundEcho_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define UltrasoundEcho_DM_RES_UP          PIN_DM_RES_UP
        #define UltrasoundEcho_DM_RES_DWN         PIN_DM_RES_DWN
        #define UltrasoundEcho_DM_OD_LO           PIN_DM_OD_LO
        #define UltrasoundEcho_DM_OD_HI           PIN_DM_OD_HI
        #define UltrasoundEcho_DM_STRONG          PIN_DM_STRONG
        #define UltrasoundEcho_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define UltrasoundEcho_MASK               UltrasoundEcho__MASK
#define UltrasoundEcho_SHIFT              UltrasoundEcho__SHIFT
#define UltrasoundEcho_WIDTH              1u

/* Interrupt constants */
#if defined(UltrasoundEcho__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in UltrasoundEcho_SetInterruptMode() function.
     *  @{
     */
        #define UltrasoundEcho_INTR_NONE      (uint16)(0x0000u)
        #define UltrasoundEcho_INTR_RISING    (uint16)(0x0001u)
        #define UltrasoundEcho_INTR_FALLING   (uint16)(0x0002u)
        #define UltrasoundEcho_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define UltrasoundEcho_INTR_MASK      (0x01u) 
#endif /* (UltrasoundEcho__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define UltrasoundEcho_PS                     (* (reg8 *) UltrasoundEcho__PS)
/* Data Register */
#define UltrasoundEcho_DR                     (* (reg8 *) UltrasoundEcho__DR)
/* Port Number */
#define UltrasoundEcho_PRT_NUM                (* (reg8 *) UltrasoundEcho__PRT) 
/* Connect to Analog Globals */                                                  
#define UltrasoundEcho_AG                     (* (reg8 *) UltrasoundEcho__AG)                       
/* Analog MUX bux enable */
#define UltrasoundEcho_AMUX                   (* (reg8 *) UltrasoundEcho__AMUX) 
/* Bidirectional Enable */                                                        
#define UltrasoundEcho_BIE                    (* (reg8 *) UltrasoundEcho__BIE)
/* Bit-mask for Aliased Register Access */
#define UltrasoundEcho_BIT_MASK               (* (reg8 *) UltrasoundEcho__BIT_MASK)
/* Bypass Enable */
#define UltrasoundEcho_BYP                    (* (reg8 *) UltrasoundEcho__BYP)
/* Port wide control signals */                                                   
#define UltrasoundEcho_CTL                    (* (reg8 *) UltrasoundEcho__CTL)
/* Drive Modes */
#define UltrasoundEcho_DM0                    (* (reg8 *) UltrasoundEcho__DM0) 
#define UltrasoundEcho_DM1                    (* (reg8 *) UltrasoundEcho__DM1)
#define UltrasoundEcho_DM2                    (* (reg8 *) UltrasoundEcho__DM2) 
/* Input Buffer Disable Override */
#define UltrasoundEcho_INP_DIS                (* (reg8 *) UltrasoundEcho__INP_DIS)
/* LCD Common or Segment Drive */
#define UltrasoundEcho_LCD_COM_SEG            (* (reg8 *) UltrasoundEcho__LCD_COM_SEG)
/* Enable Segment LCD */
#define UltrasoundEcho_LCD_EN                 (* (reg8 *) UltrasoundEcho__LCD_EN)
/* Slew Rate Control */
#define UltrasoundEcho_SLW                    (* (reg8 *) UltrasoundEcho__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define UltrasoundEcho_PRTDSI__CAPS_SEL       (* (reg8 *) UltrasoundEcho__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define UltrasoundEcho_PRTDSI__DBL_SYNC_IN    (* (reg8 *) UltrasoundEcho__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define UltrasoundEcho_PRTDSI__OE_SEL0        (* (reg8 *) UltrasoundEcho__PRTDSI__OE_SEL0) 
#define UltrasoundEcho_PRTDSI__OE_SEL1        (* (reg8 *) UltrasoundEcho__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define UltrasoundEcho_PRTDSI__OUT_SEL0       (* (reg8 *) UltrasoundEcho__PRTDSI__OUT_SEL0) 
#define UltrasoundEcho_PRTDSI__OUT_SEL1       (* (reg8 *) UltrasoundEcho__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define UltrasoundEcho_PRTDSI__SYNC_OUT       (* (reg8 *) UltrasoundEcho__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(UltrasoundEcho__SIO_CFG)
    #define UltrasoundEcho_SIO_HYST_EN        (* (reg8 *) UltrasoundEcho__SIO_HYST_EN)
    #define UltrasoundEcho_SIO_REG_HIFREQ     (* (reg8 *) UltrasoundEcho__SIO_REG_HIFREQ)
    #define UltrasoundEcho_SIO_CFG            (* (reg8 *) UltrasoundEcho__SIO_CFG)
    #define UltrasoundEcho_SIO_DIFF           (* (reg8 *) UltrasoundEcho__SIO_DIFF)
#endif /* (UltrasoundEcho__SIO_CFG) */

/* Interrupt Registers */
#if defined(UltrasoundEcho__INTSTAT)
    #define UltrasoundEcho_INTSTAT            (* (reg8 *) UltrasoundEcho__INTSTAT)
    #define UltrasoundEcho_SNAP               (* (reg8 *) UltrasoundEcho__SNAP)
    
	#define UltrasoundEcho_0_INTTYPE_REG 		(* (reg8 *) UltrasoundEcho__0__INTTYPE)
#endif /* (UltrasoundEcho__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_UltrasoundEcho_H */


/* [] END OF FILE */
