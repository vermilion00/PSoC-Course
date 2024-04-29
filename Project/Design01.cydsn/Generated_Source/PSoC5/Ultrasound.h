/*******************************************************************************
* File Name: Ultrasound.h  
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

#if !defined(CY_PINS_Ultrasound_H) /* Pins Ultrasound_H */
#define CY_PINS_Ultrasound_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Ultrasound_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Ultrasound__PORT == 15 && ((Ultrasound__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Ultrasound_Write(uint8 value);
void    Ultrasound_SetDriveMode(uint8 mode);
uint8   Ultrasound_ReadDataReg(void);
uint8   Ultrasound_Read(void);
void    Ultrasound_SetInterruptMode(uint16 position, uint16 mode);
uint8   Ultrasound_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Ultrasound_SetDriveMode() function.
     *  @{
     */
        #define Ultrasound_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Ultrasound_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Ultrasound_DM_RES_UP          PIN_DM_RES_UP
        #define Ultrasound_DM_RES_DWN         PIN_DM_RES_DWN
        #define Ultrasound_DM_OD_LO           PIN_DM_OD_LO
        #define Ultrasound_DM_OD_HI           PIN_DM_OD_HI
        #define Ultrasound_DM_STRONG          PIN_DM_STRONG
        #define Ultrasound_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Ultrasound_MASK               Ultrasound__MASK
#define Ultrasound_SHIFT              Ultrasound__SHIFT
#define Ultrasound_WIDTH              1u

/* Interrupt constants */
#if defined(Ultrasound__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Ultrasound_SetInterruptMode() function.
     *  @{
     */
        #define Ultrasound_INTR_NONE      (uint16)(0x0000u)
        #define Ultrasound_INTR_RISING    (uint16)(0x0001u)
        #define Ultrasound_INTR_FALLING   (uint16)(0x0002u)
        #define Ultrasound_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Ultrasound_INTR_MASK      (0x01u) 
#endif /* (Ultrasound__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Ultrasound_PS                     (* (reg8 *) Ultrasound__PS)
/* Data Register */
#define Ultrasound_DR                     (* (reg8 *) Ultrasound__DR)
/* Port Number */
#define Ultrasound_PRT_NUM                (* (reg8 *) Ultrasound__PRT) 
/* Connect to Analog Globals */                                                  
#define Ultrasound_AG                     (* (reg8 *) Ultrasound__AG)                       
/* Analog MUX bux enable */
#define Ultrasound_AMUX                   (* (reg8 *) Ultrasound__AMUX) 
/* Bidirectional Enable */                                                        
#define Ultrasound_BIE                    (* (reg8 *) Ultrasound__BIE)
/* Bit-mask for Aliased Register Access */
#define Ultrasound_BIT_MASK               (* (reg8 *) Ultrasound__BIT_MASK)
/* Bypass Enable */
#define Ultrasound_BYP                    (* (reg8 *) Ultrasound__BYP)
/* Port wide control signals */                                                   
#define Ultrasound_CTL                    (* (reg8 *) Ultrasound__CTL)
/* Drive Modes */
#define Ultrasound_DM0                    (* (reg8 *) Ultrasound__DM0) 
#define Ultrasound_DM1                    (* (reg8 *) Ultrasound__DM1)
#define Ultrasound_DM2                    (* (reg8 *) Ultrasound__DM2) 
/* Input Buffer Disable Override */
#define Ultrasound_INP_DIS                (* (reg8 *) Ultrasound__INP_DIS)
/* LCD Common or Segment Drive */
#define Ultrasound_LCD_COM_SEG            (* (reg8 *) Ultrasound__LCD_COM_SEG)
/* Enable Segment LCD */
#define Ultrasound_LCD_EN                 (* (reg8 *) Ultrasound__LCD_EN)
/* Slew Rate Control */
#define Ultrasound_SLW                    (* (reg8 *) Ultrasound__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Ultrasound_PRTDSI__CAPS_SEL       (* (reg8 *) Ultrasound__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Ultrasound_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Ultrasound__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Ultrasound_PRTDSI__OE_SEL0        (* (reg8 *) Ultrasound__PRTDSI__OE_SEL0) 
#define Ultrasound_PRTDSI__OE_SEL1        (* (reg8 *) Ultrasound__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Ultrasound_PRTDSI__OUT_SEL0       (* (reg8 *) Ultrasound__PRTDSI__OUT_SEL0) 
#define Ultrasound_PRTDSI__OUT_SEL1       (* (reg8 *) Ultrasound__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Ultrasound_PRTDSI__SYNC_OUT       (* (reg8 *) Ultrasound__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Ultrasound__SIO_CFG)
    #define Ultrasound_SIO_HYST_EN        (* (reg8 *) Ultrasound__SIO_HYST_EN)
    #define Ultrasound_SIO_REG_HIFREQ     (* (reg8 *) Ultrasound__SIO_REG_HIFREQ)
    #define Ultrasound_SIO_CFG            (* (reg8 *) Ultrasound__SIO_CFG)
    #define Ultrasound_SIO_DIFF           (* (reg8 *) Ultrasound__SIO_DIFF)
#endif /* (Ultrasound__SIO_CFG) */

/* Interrupt Registers */
#if defined(Ultrasound__INTSTAT)
    #define Ultrasound_INTSTAT            (* (reg8 *) Ultrasound__INTSTAT)
    #define Ultrasound_SNAP               (* (reg8 *) Ultrasound__SNAP)
    
	#define Ultrasound_0_INTTYPE_REG 		(* (reg8 *) Ultrasound__0__INTTYPE)
#endif /* (Ultrasound__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Ultrasound_H */


/* [] END OF FILE */
