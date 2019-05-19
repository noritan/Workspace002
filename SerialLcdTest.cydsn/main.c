/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2CM_Start();
    I2C_LCD_Start();
    
    {
        uint32 i;
        for (i = 0; i < 10; i++) {
            I2C_LCD_PrintString("UNKO! ");
        }
    }

    for(;;) {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
