/*******************************************************************************
* File Name: CtlReg_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "CtlReg.h"

/* Check for removal by optimization */
#if !defined(CtlReg_Sync_ctrl_reg__REMOVED)

static CtlReg_BACKUP_STRUCT  CtlReg_backup = {0u};

    
/*******************************************************************************
* Function Name: CtlReg_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CtlReg_SaveConfig(void) 
{
    CtlReg_backup.controlState = CtlReg_Control;
}


/*******************************************************************************
* Function Name: CtlReg_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void CtlReg_RestoreConfig(void) 
{
     CtlReg_Control = CtlReg_backup.controlState;
}


/*******************************************************************************
* Function Name: CtlReg_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CtlReg_Sleep(void) 
{
    CtlReg_SaveConfig();
}


/*******************************************************************************
* Function Name: CtlReg_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CtlReg_Wakeup(void)  
{
    CtlReg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
