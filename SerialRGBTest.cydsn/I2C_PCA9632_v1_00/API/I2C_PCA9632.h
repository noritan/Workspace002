//**********************************************************************
//
//  File Name: `$INSTANCE_NAME`.h
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//
//  Description:
//    This header file contains registers and constants associated
//    with the I2C PCA9632 component.
//
//**********************************************************************
//  Copyright noritan.org, 2019
//  All Rights Reserved
//  UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF noritan.org
//**********************************************************************

#if !defined(CY_I2C_PCA9632_`$INSTANCE_NAME`_H)
#define CY_I2C_PCA9632_`$INSTANCE_NAME`_H

#include "cytypes.h"
#include "cyfitter.h"

#include "`$I2cMasterInstanceName`.h"

#if defined(`$I2cMasterInstanceName`_SCB_MODE)
    #include "`$I2cMasterInstanceName`_I2C.h"
#endif /* `$I2cMasterInstanceName`_SCB_MODE */

//======================================================================
//  Global Cariables
//======================================================================
extern uint8 `$INSTANCE_NAME`_initVar;

//======================================================================
//  Function Prototypes
//======================================================================
void `$INSTANCE_NAME`_Init(void);
void `$INSTANCE_NAME`_Enable(void);
void `$INSTANCE_NAME`_Disable(void);
void `$INSTANCE_NAME`_Start(void);
void `$INSTANCE_NAME`_Stop(void);

#endif /* CY_I2C_PCA9632_`$INSTANCE_NAME`_H */

/* [] END OF FILE */
