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

uint8   duty = 0u;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2CM_Start();
    LCD_Driver_Start();
    LED_Driver_Start();

    for(;;)
    {
        /* Place your application code here. */
        LED_Driver_WritePWM(
            duty,
            duty,
            duty,
            duty
        );
        CyDelay(10);
        duty++;
    }
}

/* [] END OF FILE */
