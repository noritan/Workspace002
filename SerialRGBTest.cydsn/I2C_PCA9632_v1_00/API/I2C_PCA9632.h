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
//  Macros
//======================================================================
#define `$INSTANCE_NAME`_MODE1_SLEEP (0x10)  
#define `$INSTANCE_NAME`_MODE1_ENABLE (0x00)  
#define `$INSTANCE_NAME`_MODE1_DISABLE (`$INSTANCE_NAME`_MODE1_SLEEP)  
#define `$INSTANCE_NAME`_LEDOUT_ENABLE (0xAA)  
#define `$INSTANCE_NAME`_LEDOUT_DISABLE (0x00)  

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
void `$INSTANCE_NAME`_WriteMode1(uint8 mode1);
void `$INSTANCE_NAME`_WriteLedout(uint8 ledout);
void `$INSTANCE_NAME`_WritePWM(uint8 pwm0, uint8 pwm1, uint8 pwm2, uint8 pwm3);

#endif /* CY_I2C_PCA9632_`$INSTANCE_NAME`_H */

/* [] END OF FILE */
