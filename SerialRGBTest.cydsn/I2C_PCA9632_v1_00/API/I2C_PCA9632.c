//**********************************************************************
//
//  File Name: `$INSTANCE_NAME`.c
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//
//  Description:
//    This file provides source code for the I2C PCA9632 component's API.
//
//**********************************************************************
//  Copyright noritan.org, 2019
//  All Rights Reserved
//  UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF noritan.org
//**********************************************************************

//======================================================================
//  Header file inclusion
//======================================================================
#include "`$INSTANCE_NAME`.h"

//======================================================================
//  Variable declaration
//======================================================================
uint8 `$INSTANCE_NAME`_initVar = 0u;

//======================================================================
//  Function Name: `$INSTANCE_NAME`_Init
//======================================================================
//
//  Summary:
//    Performs initialization required for the component's normal work.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Init(void) {
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Enable
//======================================================================
//
//  Summary:
//    Turns on the LED.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Enable(void) {
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Disable
//======================================================================
//
//  Summary:
//    Turns off the LED.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Disable(void) {
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Start
//======================================================================
//
//  Summary:
//   This function initializes the LED hardware module as follows:
//     Initialize the I2C bus if it hasn't already been
//
//  Parameters:
//    `$INSTANCE_NAME`_initVar - global variable.
//
//  Return:
//    `$INSTANCE_NAME`_initVar - global variable.
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Start(void) {
    // If not initialized then perform initialization
    if (!`$INSTANCE_NAME`_initVar) {
        `$INSTANCE_NAME`_Init();
        `$INSTANCE_NAME`_initVar = 1u;
    }

    // Turn on the LED
    `$INSTANCE_NAME`_Enable();
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Stop
//======================================================================
//
//  Summary:
//    Turns off the LED but does not stop the I2C Master component.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Stop(void) {
    `$INSTANCE_NAME`_Disable();
}


/* [] END OF FILE */
